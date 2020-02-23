/*
 * Copyright (C) 2018 The Android Open Source Project
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

//#define LOG_NDEBUG 0

#define LOG_TAG "android.hardware.power@1.3-service.exynos7420-libperfmgr"
#define ATRACE_TAG (ATRACE_TAG_POWER | ATRACE_TAG_HAL)

#include <fcntl.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <time.h>
#include <unistd.h>
#include <utils/Log.h>
#include <utils/Trace.h>

#include "InteractionHandler.h"

#define MSINSEC 1000L
#define USINMS 1000000L

InteractionHandler::InteractionHandler(std::shared_ptr<HintManager> const & hint_manager)
    : mState(INTERACTION_STATE_UNINITIALIZED),
      mMinDurationMs(1400),
      mMaxDurationMs(5650),
      mDurationMs(0),
      mHintManager(hint_manager) {
}

InteractionHandler::~InteractionHandler() {
    Exit();
}

bool InteractionHandler::Init() {
    std::lock_guard<std::mutex> lk(mLock);

    if (mState != INTERACTION_STATE_UNINITIALIZED)
        return true;

    mState = INTERACTION_STATE_IDLE;
    mThread = std::unique_ptr<std::thread>(
        new std::thread(&InteractionHandler::Routine, this));

    return true;
}

void InteractionHandler::Exit() {
    std::unique_lock<std::mutex> lk(mLock);
    if (mState == INTERACTION_STATE_UNINITIALIZED)
        return;

    mState = INTERACTION_STATE_UNINITIALIZED;
    lk.unlock();

    mCond.notify_all();
    mThread->join();
}

void InteractionHandler::PerfLock() {
    ALOGV("%s: acquiring perf lock", __func__);
    if (!mHintManager->DoHint("INTERACTION")) {
        ALOGE("%s: do hint INTERACTION failed", __func__);
    }
    ATRACE_INT("interaction_lock", 1);
}

void InteractionHandler::PerfRel() {
    ALOGV("%s: releasing perf lock", __func__);
    if (!mHintManager->EndHint("INTERACTION")) {
        ALOGE("%s: end hint INTERACTION failed", __func__);
    }
    ATRACE_INT("interaction_lock", 0);
}

long long InteractionHandler::CalcTimespecDiffMs(struct timespec start,
                                               struct timespec end) {
    long long diff_in_us = 0;
    diff_in_us += (end.tv_sec - start.tv_sec) * MSINSEC;
    diff_in_us += (end.tv_nsec - start.tv_nsec) / USINMS;
    return diff_in_us;
}

void InteractionHandler::Acquire(int32_t duration) {
    ATRACE_CALL();

    std::lock_guard<std::mutex> lk(mLock);
    if (mState == INTERACTION_STATE_UNINITIALIZED) {
        ALOGW("%s: called while uninitialized", __func__);
        return;
    }

    int inputDuration = duration + 650;
    int finalDuration;
    if (inputDuration > mMaxDurationMs)
        finalDuration = mMaxDurationMs;
    else if (inputDuration > mMinDurationMs)
        finalDuration = inputDuration;
    else
        finalDuration = mMinDurationMs;

    struct timespec cur_timespec;
    clock_gettime(CLOCK_MONOTONIC, &cur_timespec);
    if (mState != INTERACTION_STATE_IDLE && finalDuration <= mDurationMs) {
        long long elapsed_time = CalcTimespecDiffMs(mLastTimespec, cur_timespec);
        // don't hint if previous hint's duration covers this hint's duration
        if (elapsed_time <= (mDurationMs - finalDuration)) {
            ALOGV("%s: Previous duration (%d) cover this (%d) elapsed: %lld",
                  __func__, mDurationMs, finalDuration, elapsed_time);
            return;
        }
    }
    mLastTimespec = cur_timespec;
    mDurationMs = finalDuration;

    ALOGV("%s: input: %d final duration: %d", __func__,
          duration, finalDuration);

    if (mState == INTERACTION_STATE_IDLE) {
        PerfLock();
        mState = INTERACTION_STATE_INTERACTION;
    }

    mHandled = false;
    mCond.notify_one();
}

void InteractionHandler::Release() {
    std::lock_guard<std::mutex> lk(mLock);
    ATRACE_CALL();
    PerfRel();
    mState = INTERACTION_STATE_IDLE;
}

void InteractionHandler::Routine() {
    std::unique_lock<std::mutex> lk(mLock, std::defer_lock);

    while (true) {
        lk.lock();
        mCond.wait(lk, [&] { return mState != INTERACTION_STATE_IDLE; });
        if (mState == INTERACTION_STATE_UNINITIALIZED)
            return;
        mHandled = true;
        mState = INTERACTION_STATE_WAITING;
        lk.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(mDurationMs));
        if (mHandled)
            Release();
    }
}
