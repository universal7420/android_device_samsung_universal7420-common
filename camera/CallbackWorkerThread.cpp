/*
 * Copyright (C) 2019, The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Camera HAL "CallbackWorkerThread" workaround description
 *
 * The camera HAL of the Exynos7580 (A3, A5, ... 2016) reguralry deadlocks when using
 * most camera apps, this happens a lot when using Google Camera but does occur
 * occasionally in Snap.
 *
 * The issue was tracked down to the way that Samsung operates their HAL, all operations
 * are run in multiple threads and the different callbacks are executed from one of these
 * threads.
 *
 * The deadlocks occur when the camera client executes a function like cancel_auto_focus
 * or stop_preview and a callback from the HAL occurs at around the same time. The HAL
 * waits for the callback functions to return before they finish processing the client
 * calls and the client stops processing callbacks until their calling function completes.
 *
 * You end up in a state when both the HAL and the client are waiting for the other
 * process to finish and so end up with a deadlock of the HAL/Client which only a reboot
 * can cure.
 *
 * This worker thread offloads the actual callbacks to another thread which allows the
 * HAL to finish the client calls and avoid the deadlock scenario.
 *
 */

#define LOG_NDEBUG 0
#define LOG_TAG "Camera2WrapperCbThread"

#include "CallbackWorkerThread.h"
#include <iostream>
#include <cutils/log.h>

using namespace std;

#define MSG_EXIT_THREAD         1
#define MSG_EXECUTE_CALLBACK      2
#define MSG_UPDATE_CALLBACKS    3

struct ThreadMsg
{
    ThreadMsg(int i, const void* m, long long ts) { id = i; msg = m; CallerTS = ts; }
    int id;
    const void* msg;
    long long CallerTS;
};

CallbackWorkerThread::CallbackWorkerThread() : m_thread(0) {
}

CallbackWorkerThread::~CallbackWorkerThread() {
    ExitThread();
}

bool CallbackWorkerThread::CreateThread() {
    if (!m_thread)
        m_thread = new thread(&CallbackWorkerThread::Process, this);
    return true;
}

void CallbackWorkerThread::ExitThread() {
    if (!m_thread)
        return;

    /* Create the exit thread worker message */
    ThreadMsg* threadMsg = new ThreadMsg(MSG_EXIT_THREAD, 0, GetTimestamp());

    /* Add it to the message queue */
    {
        lock_guard<mutex> lock(m_mutex);
        m_queue.push(threadMsg);
        m_cv.notify_one();
    }

    /* Join the thread and then cleanup */
    m_thread->join();
    delete m_thread;
    m_thread = 0;
}

void CallbackWorkerThread::AddCallback(const WorkerMessage* data) {
    /* Assert that the thread exists */
    ALOG_ASSERT(m_thread != NULL);

    /* Create a new worker thread message from the data */
    ThreadMsg* threadMsg = new ThreadMsg(MSG_EXECUTE_CALLBACK, data, GetTimestamp());

    /* Add it to our worker queue and notify the worker */
    std::unique_lock<std::mutex> lk(m_mutex);
    m_queue.push(threadMsg);
    m_cv.notify_one();
}

void CallbackWorkerThread::SetCallbacks(const CallbackData* data) {
    /* Assert that the thread exists */
    ALOG_ASSERT(m_thread != NULL);

    /* Create a new worker thread message from the callback data */
    ThreadMsg* threadMsg = new ThreadMsg(MSG_UPDATE_CALLBACKS, data, GetTimestamp());

    /* Add it to our worker queue and notify the worker */
    std::unique_lock<std::mutex> lk(m_mutex);
    m_queue.push(threadMsg);
    m_cv.notify_one();
}

void CallbackWorkerThread::ClearCallbacks() {
    /* Assert that the thread exists */
    ALOG_ASSERT(m_thread != NULL);

    /* Lock the mutex and clear the message queue */
    std::unique_lock<std::mutex> lk(m_mutex);

    ALOGV("%s: Clearing %i messages", __FUNCTION__, m_queue.size());

    /* Whilst the queue is not empty */
    while (!m_queue.empty()) {
        /* Pop the message from the queue and delete the allocated data */
        ThreadMsg* msg = m_queue.front();
        m_queue.pop();
        delete msg;
    }

    m_cv.notify_one();
}

void CallbackWorkerThread::Process() {
    camera_notify_callback UserNotifyCb = NULL;
    camera_data_callback UserDataCb = NULL;

    while (1) {
        ThreadMsg* msg = 0;
        {
            /* Wait for a message to be added to the queue */
            std::unique_lock<std::mutex> lk(m_mutex);
            while (m_queue.empty())
                m_cv.wait(lk);

            if (m_queue.empty())
                continue;

            msg = m_queue.front();
            m_queue.pop();
        }

        switch (msg->id) {
            case MSG_EXECUTE_CALLBACK:
            {
                /* Assert that we have a valid message */
                ALOG_ASSERT(msg->msg != NULL);

                /* Cast the the ThreadMsg void* data back to a WorkerMessage* */
                const WorkerMessage* userData = static_cast<const WorkerMessage*>(msg->msg);

                /* If the callback is not stale (newer than 5mS) */
                if(GetTimestamp() - msg->CallerTS < 5) {
                    /* If the callback type is set to notifycb */
                    if(userData->CbType == CB_TYPE_NOTIFY) {
                        /* Execute the users notify callback if it is valid */
                        if(UserNotifyCb != NULL) {
                            ALOGV("%s: UserNotifyCb: %i %i %i %p", __FUNCTION__, userData->msg_type, userData->ext1, userData->ext2, userData->user);
                            UserNotifyCb(userData->msg_type, userData->ext1, userData->ext2, userData->user);
                        }
                    } /* If the callback type is set to notifycb */
                    else if(userData->CbType == CB_TYPE_DATA) {
                        /* Execute the users data callback if it is valid */
                        if(UserDataCb != NULL) {
                            ALOGV("%s: UserDataCb: %i %p %i %p %p", __FUNCTION__, userData->msg_type, userData->data, userData->index, userData->metadata, userData->user);
                            UserDataCb(userData->msg_type, userData->data, userData->index, userData->metadata, userData->user);
                        }
                    }
                } else {
                    /* If the callback type is set to notifycb */
                    if(userData->CbType == CB_TYPE_NOTIFY) {
                            ALOGV("%s: UserNotifyCb Stale: %llimS old", __FUNCTION__, GetTimestamp() - msg->CallerTS);
                    } /* If the callback type is set to notifycb */
                    else if(userData->CbType == CB_TYPE_DATA) {
                            ALOGV("%s: UserDataCb Stale: %llimS old", __FUNCTION__, GetTimestamp() - msg->CallerTS);
                    }
                }

                /* Cleanup allocated data */
                delete userData;
                delete msg;
                break;
            }

            case MSG_UPDATE_CALLBACKS:
            {
                /* Assert that we have a valid message */
                ALOG_ASSERT(msg->msg != NULL);

                /* Cast the the ThreadMsg void* data back to a CallbackData* */
                const CallbackData* callbackData = static_cast<const CallbackData*>(msg->msg);

                ALOGV("%s: UpdateCallbacks", __FUNCTION__);

                /* Copy the new callback pointers */
                UserNotifyCb = callbackData->NewUserNotifyCb;
                UserDataCb = callbackData->NewUserDataCb;

                /* Cleanup allocated data */
                delete callbackData;
                delete msg;
                break;
            }

            case MSG_EXIT_THREAD:
            {
                /* Delete current message */
                delete msg;
                /* Then delete all pending messages in the queue */
                std::unique_lock<std::mutex> lk(m_mutex);
                /* Whilst the queue is not empty */
                while (!m_queue.empty()) {
                    /* Pop the message from the queue and delete the allocated data */
                    msg = m_queue.front();
                    m_queue.pop();
                    delete msg;
                }

                ALOGV("%s: Exit Thread", __FUNCTION__);
                return;
            }

            default:
                /* Error if we get here */
	            ALOG_ASSERT(0);
        }
    }
}


/* based on current_timestamp() function from stack overflow:
 * https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux/17083824
 */

long long CallbackWorkerThread::GetTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

