/*
 * Copyright (C) 2017 TeamNexus
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

#define LOG_TAG "libOMX.Exynos"
#define LOG_NDEBUG 0

#include <cutils/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#ifdef USE_DMA_BUF
#include <gralloc_priv.h>
#endif

#include <hardware/hardware.h>
#include <media/hardware/HardwareAPI.h>
#include <media/hardware/OMXPluginBase.h>
#include <media/openmax/OMX_IVCommon.h>
#include <system/graphics.h>
#include <system/window.h>
#include <ui/GraphicBuffer.h>
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
 
#include <exynos_format.h>
#include <exynos/Exynos_OMX_Def.h>
#include <ion.h>

#include <ExynosVideoApi.h>
#include <Exynos_OMX_Baseport.h>
#include <Exynos_OSAL_Android.h>
#include <Exynos_OSAL_ETC.h>
#include <Exynos_OSAL_Log.h>
#include <Exynos_OSAL_Memory.h>

using namespace android;

#ifdef __cplusplus
extern "C" {
#endif

static int lockCnt = 0;
static int mem_cnt = 0;


/*************************************************************
 * Exynos_OSAL_ETC.c
 */

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_ETC.c:390 */
OMX_COLOR_FORMATTYPE Exynos_OSAL_HAL2OMXColorFormat(
    unsigned int nHALFormat)
{
    OMX_COLOR_FORMATTYPE eOMXFormat = OMX_COLOR_FormatUnused;

    switch (nHALFormat) {
    case HAL_PIXEL_FORMAT_EXYNOS_YCbCr_420_SP:
    case HAL_PIXEL_FORMAT_EXYNOS_YCbCr_420_SP_M:
        eOMXFormat = OMX_COLOR_FormatYUV420SemiPlanar;
        break;
    case HAL_PIXEL_FORMAT_EXYNOS_YCbCr_420_SP_M_TILED:
        eOMXFormat = (OMX_COLOR_FORMATTYPE)OMX_SEC_COLOR_FormatNV12Tiled;
        break;
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
    case HAL_PIXEL_FORMAT_EXYNOS_YCrCb_420_SP_M:
        eOMXFormat = (OMX_COLOR_FORMATTYPE)OMX_SEC_COLOR_FormatNV21Linear;
        break;
    case HAL_PIXEL_FORMAT_EXYNOS_YCbCr_420_P:
    case HAL_PIXEL_FORMAT_EXYNOS_YCbCr_420_P_M:
        eOMXFormat = OMX_COLOR_FormatYUV420Planar;
        break;
    case HAL_PIXEL_FORMAT_YV12:
    case HAL_PIXEL_FORMAT_EXYNOS_YV12_M:
        eOMXFormat = (OMX_COLOR_FORMATTYPE)OMX_SEC_COLOR_FormatYVU420Planar;
        break;
    case HAL_PIXEL_FORMAT_YCbCr_422_I:
        eOMXFormat = OMX_COLOR_FormatYCbYCr;
        break;
    case HAL_PIXEL_FORMAT_BGRA_8888:
        eOMXFormat = OMX_COLOR_Format32bitARGB8888;
        break;
    case HAL_PIXEL_FORMAT_EXYNOS_ARGB_8888:
        eOMXFormat = OMX_COLOR_Format32bitBGRA8888;
        break;
    case HAL_PIXEL_FORMAT_RGBA_8888:
        eOMXFormat = (OMX_COLOR_FORMATTYPE)OMX_SEC_COLOR_Format32bitABGR8888;
        break;
    default:
        Exynos_OSAL_Log(EXYNOS_LOG_ERROR, "HAL format is unsupported(0x%x)", nHALFormat);
        eOMXFormat = OMX_COLOR_FormatUnused;
        break;
    }

    return eOMXFormat;
}


/*************************************************************
 * Exynos_OSAL_LOG.c
 */
 
/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_LOG.c:32 */
void _Exynos_OSAL_Log(EXYNOS_LOG_LEVEL logLevel, const char *tag, const char *msg, ...)
{
    va_list argptr;

    va_start(argptr, msg);

    switch (logLevel) {
    case EXYNOS_LOG_TRACE:
        __android_log_vprint(ANDROID_LOG_DEBUG, tag, msg, argptr);
        break;
    case EXYNOS_LOG_INFO:
        __android_log_vprint(ANDROID_LOG_INFO, tag, msg, argptr);
        break;
    case EXYNOS_LOG_WARNING:
        __android_log_vprint(ANDROID_LOG_WARN, tag, msg, argptr);
        break;
    case EXYNOS_LOG_ERROR:
        __android_log_vprint(ANDROID_LOG_ERROR, tag, msg, argptr);
        break;
    default:
        __android_log_vprint(ANDROID_LOG_VERBOSE, tag, msg, argptr);
    }

    va_end(argptr);
}


/*************************************************************
 * Exynos_OSAL_Memory.c
 */

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Memory.c:39 */
OMX_PTR Exynos_OSAL_Malloc(OMX_U32 size)
{
    mem_cnt++;
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "alloc count: %d", mem_cnt);

    return (OMX_PTR)malloc(size);
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Memory.c:47 */
void Exynos_OSAL_Free(OMX_PTR addr)
{
    mem_cnt--;
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "free count: %d", mem_cnt);

    if (addr)
        free(addr);

    return;
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Memory.c:58 */
OMX_PTR Exynos_OSAL_Memset(OMX_PTR dest, OMX_S32 c, OMX_S32 n)
{
    return memset(dest, c, n);
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Memory.c:63 */
OMX_PTR Exynos_OSAL_Memcpy(OMX_PTR dest, OMX_PTR src, OMX_S32 n)
{
    return memcpy(dest, src, n);
}


/*************************************************************
 * Exynos_OSAL_Mutex.c
 */

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Mutex.c:66 */
OMX_ERRORTYPE Exynos_OSAL_MutexLock(OMX_HANDLETYPE mutexHandle)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)mutexHandle;
    int result;

    if (mutex == NULL)
        return OMX_ErrorBadParameter;

    if (pthread_mutex_lock(mutex) != 0)
        return OMX_ErrorUndefined;

    return OMX_ErrorNone;
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Mutex.c:80 */
OMX_ERRORTYPE Exynos_OSAL_MutexUnlock(OMX_HANDLETYPE mutexHandle)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)mutexHandle;
    int result;

    if (mutex == NULL)
        return OMX_ErrorBadParameter;

    if (pthread_mutex_unlock(mutex) != 0)
        return OMX_ErrorUndefined;

    return OMX_ErrorNone;
}


/*************************************************************
 * Exynos_OSAL_SharedMemory.c
 */

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_SharedMemory.c:51 */
typedef struct _EXYNOS_SHAREDMEM_LIST
{
    OMX_U32                        IONBuffer;
    OMX_PTR                        mapAddr;
    OMX_U32                        allocSize;
    OMX_BOOL                       owner;
    struct _EXYNOS_SHAREDMEM_LIST *pNextMemory;
} EXYNOS_SHAREDMEM_LIST;

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_SharedMemory.c:60 */
typedef struct _EXYNOS_SHARED_MEMORY
{
    OMX_HANDLETYPE         hIONHandle;
    EXYNOS_SHAREDMEM_LIST *pAllocMemory;
    OMX_HANDLETYPE         hSMMutex;
} EXYNOS_SHARED_MEMORY;

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_SharedMemory.c:150 */
OMX_PTR Exynos_OSAL_SharedMemory_Alloc(OMX_HANDLETYPE handle, OMX_U32 size, MEMORY_TYPE memoryType)
{
    EXYNOS_SHARED_MEMORY  *pHandle         = (EXYNOS_SHARED_MEMORY *)handle;
    EXYNOS_SHAREDMEM_LIST *pSMList         = NULL;
    EXYNOS_SHAREDMEM_LIST *pElement        = NULL;
    EXYNOS_SHAREDMEM_LIST *pCurrentElement = NULL;
    ion_buffer             IONBuffer       = 0;
    OMX_PTR                pBuffer         = NULL;
    unsigned int mask;
    unsigned int flag;

    if (pHandle == NULL)
        goto EXIT;

    pElement = (EXYNOS_SHAREDMEM_LIST *)Exynos_OSAL_Malloc(sizeof(EXYNOS_SHAREDMEM_LIST));
    if (pElement == NULL)
        goto EXIT;
    Exynos_OSAL_Memset(pElement, 0, sizeof(EXYNOS_SHAREDMEM_LIST));
    pElement->owner = OMX_TRUE;

    /* priority is like as SECURE > CONTIG > CACHED > NORMAL */
    switch ((int)memoryType) {
    case (SECURE_MEMORY | CONTIG_MEMORY | CACHED_MEMORY):  /* SECURE */
    case (SECURE_MEMORY | CONTIG_MEMORY):
    case (SECURE_MEMORY | CACHED_MEMORY):
    case SECURE_MEMORY:
        mask = ION_HEAP_EXYNOS_CONTIG_MASK;
        flag = ION_EXYNOS_MFC_INPUT_MASK;
#ifdef USE_NON_SECURE_DRM
        if (memoryType & CONTIG_MEMORY) {
            mask = ION_HEAP_EXYNOS_CONTIG_MASK;
            flag = ION_EXYNOS_VIDEO_MASK;
        } else {
            mask = ION_HEAP_SYSTEM_MASK;
            flag = ION_FLAG_CACHED;
        }
#endif
        break;
    case (CONTIG_MEMORY | CACHED_MEMORY):  /* CONTIG */
    case CONTIG_MEMORY:
        mask = ION_HEAP_EXYNOS_CONTIG_MASK;
        flag = ION_EXYNOS_MFC_INPUT_MASK;
        break;
    case CACHED_MEMORY:  /* CACHED */
        mask = ION_HEAP_SYSTEM_MASK;
        flag = ION_FLAG_CACHED;
        break;
    default:  /* NORMAL */
        mask = ION_HEAP_SYSTEM_MASK;
        flag = ION_FLAG_CACHED;
        break;
    }

#ifdef USE_IMPROVED_BUFFER
    if (flag & ION_FLAG_CACHED)  /* use improved cache oprs */
        flag |= ION_FLAG_CACHED_NEEDS_SYNC | ION_FLAG_PRESERVE_KMAP;
#endif

    IONBuffer = ion_alloc((ion_client)(intptr_t)pHandle->hIONHandle, size, 0, mask, flag);
    if ((IONBuffer <= 0) &&
        (memoryType == CONTIG_MEMORY)) {
        flag = 0;
        IONBuffer = ion_alloc((ion_client)(intptr_t)pHandle->hIONHandle, size, 0, mask, flag);
    }

    if (IONBuffer <= 0) {
        Exynos_OSAL_Log(EXYNOS_LOG_ERROR, "ion_alloc Error: %d", IONBuffer);
        Exynos_OSAL_Free((OMX_PTR)pElement);
        goto EXIT;
    }

    pBuffer = ion_map(IONBuffer, size, 0);
    if (pBuffer == MAP_FAILED) {
        Exynos_OSAL_Log(EXYNOS_LOG_ERROR, "ion_map Error");
        ion_free(IONBuffer);
        Exynos_OSAL_Free((OMX_PTR)pElement);
        pBuffer = NULL;
        goto EXIT;
    }

    pElement->IONBuffer = IONBuffer;
    pElement->mapAddr = pBuffer;
    pElement->allocSize = size;
    pElement->pNextMemory = NULL;

    Exynos_OSAL_MutexLock(pHandle->hSMMutex);
    pSMList = pHandle->pAllocMemory;
    if (pSMList == NULL) {
        pHandle->pAllocMemory = pSMList = pElement;
    } else {
        pCurrentElement = pSMList;
        while (pCurrentElement->pNextMemory != NULL) {
            pCurrentElement = pCurrentElement->pNextMemory;
        }
        pCurrentElement->pNextMemory = pElement;
    }
    Exynos_OSAL_MutexUnlock(pHandle->hSMMutex);

    mem_cnt++;
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "SharedMemory mem count: %d", mem_cnt);

EXIT:
    return pBuffer;
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_SharedMemory.c:427 */
int Exynos_OSAL_SharedMemory_VirtToION(OMX_HANDLETYPE handle, OMX_PTR pBuffer)
{
    EXYNOS_SHARED_MEMORY  *pHandle         = (EXYNOS_SHARED_MEMORY *)handle;
    EXYNOS_SHAREDMEM_LIST *pSMList         = NULL;
    EXYNOS_SHAREDMEM_LIST *pCurrentElement = NULL;
    EXYNOS_SHAREDMEM_LIST *pFindElement    = NULL;
    int ion_addr = 0;
    if (pHandle == NULL || pBuffer == NULL)
        goto EXIT;

    Exynos_OSAL_MutexLock(pHandle->hSMMutex);
    pSMList = pHandle->pAllocMemory;
    if (pSMList == NULL) {
        Exynos_OSAL_MutexUnlock(pHandle->hSMMutex);
        goto EXIT;
    }

    pCurrentElement = pSMList;
    if (pSMList->mapAddr == pBuffer) {
        pFindElement = pSMList;
    } else {
        while ((pCurrentElement != NULL) && (((EXYNOS_SHAREDMEM_LIST *)(pCurrentElement->pNextMemory)) != NULL) &&
               (((EXYNOS_SHAREDMEM_LIST *)(pCurrentElement->pNextMemory))->mapAddr != pBuffer))
            pCurrentElement = pCurrentElement->pNextMemory;

        if ((((EXYNOS_SHAREDMEM_LIST *)(pCurrentElement->pNextMemory)) != NULL) &&
            (((EXYNOS_SHAREDMEM_LIST *)(pCurrentElement->pNextMemory))->mapAddr == pBuffer)) {
            pFindElement = pCurrentElement->pNextMemory;
        } else {
            Exynos_OSAL_MutexUnlock(pHandle->hSMMutex);
            Exynos_OSAL_Log(EXYNOS_LOG_WARNING, "Can not find SharedMemory");
            goto EXIT;
        }
    }
    Exynos_OSAL_MutexUnlock(pHandle->hSMMutex);

    ion_addr = pFindElement->IONBuffer;

EXIT:
    return ion_addr;
}


/*************************************************************
 * Exynos_OSAL_Android.cpp
 */

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Android.cpp:182 */
OMX_COLOR_FORMATTYPE Exynos_OSAL_GetANBColorFormat(OMX_IN OMX_U32 handle)
{
    FunctionIn();

    OMX_COLOR_FORMATTYPE ret = OMX_COLOR_FormatUnused;
#ifdef USE_DMA_BUF
    private_handle_t *priv_hnd = (private_handle_t *)(uintptr_t) handle;

    ret = Exynos_OSAL_HAL2OMXColorFormat(priv_hnd->format);
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "ColorFormat: 0x%x", ret);
#endif

EXIT:
    FunctionOut();

    return ret;
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Android.cpp:77 */
OMX_ERRORTYPE Exynos_OSAL_LockANBHandle(
    OMX_IN OMX_U32 handle,
    OMX_IN OMX_U32 width,
    OMX_IN OMX_U32 height,
    OMX_IN OMX_COLOR_FORMATTYPE format,
    OMX_OUT OMX_U32 *pStride,
    OMX_OUT OMX_PTR planes)
{
    FunctionIn();

    OMX_ERRORTYPE ret = OMX_ErrorNone;
    GraphicBufferMapper &mapper = GraphicBufferMapper::get();
    buffer_handle_t bufferHandle = (buffer_handle_t)(uintptr_t) handle;
#ifdef USE_DMA_BUF
    private_handle_t *priv_hnd = (private_handle_t *) bufferHandle;
#endif
    Rect bounds((uint32_t)width, (uint32_t)height);
    ExynosVideoPlane *vplanes = (ExynosVideoPlane *) planes;
    void *vaddr[MAX_BUFFER_PLANE];

    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: handle: 0x%x", __func__, handle);

    int usage = 0;
    switch ((int)format) {
    case OMX_COLOR_FormatYUV420Planar:
    case OMX_COLOR_FormatYUV420SemiPlanar:
    case OMX_SEC_COLOR_FormatNV12Tiled:
        usage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN;
        break;
#ifdef USE_ANDROIDOPAQUE
    case OMX_COLOR_FormatAndroidOpaque:
    {
        OMX_COLOR_FORMATTYPE formatType;
#ifdef USE_DMA_BUF
        formatType = Exynos_OSAL_GetANBColorFormat((OMX_U32)(uintptr_t)priv_hnd);
        if ((formatType == OMX_COLOR_FormatYUV420SemiPlanar) ||
            (formatType == (OMX_COLOR_FORMATTYPE)OMX_SEC_COLOR_FormatNV12Tiled))
            usage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN;
        else
#endif
            usage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_HW_VIDEO_ENCODER;
    }
        break;
#endif
    default:
        usage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN;
        break;
    }

    Exynos_OSAL_Log(EXYNOS_LOG_INFO, "%s:  bufferHandle=0x%x(%d)  ->numFds=0x%x(%d)  ->numInts=0x%x(%d)  ->version=0x%x(%d)", __func__,
			bufferHandle, bufferHandle,
			bufferHandle->numFds, bufferHandle->numFds,
			bufferHandle->numInts, bufferHandle->numInts,
			bufferHandle->version, bufferHandle->version);
	
    if (mapper.lock(bufferHandle, usage, bounds, vaddr) != 0) {
        Exynos_OSAL_Log(EXYNOS_LOG_ERROR, "%s: mapper.lock() fail", __func__);
        ret = OMX_ErrorUndefined;
        goto EXIT;
    }
    lockCnt++;
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: lockCnt:%d", __func__, lockCnt);

#ifdef USE_DMA_BUF
    vplanes[0].fd = priv_hnd->fd;
    vplanes[0].offset = 0;
    vplanes[1].fd = priv_hnd->fd1;
    vplanes[1].offset = 0;
    vplanes[2].fd = priv_hnd->fd2;
    vplanes[2].offset = 0;
#endif
    vplanes[0].addr = vaddr[0];
    vplanes[1].addr = vaddr[1];
    vplanes[2].addr = vaddr[2];

    *pStride = (OMX_U32)priv_hnd->stride;

    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: buffer locked: 0x%x", __func__, *vaddr);

EXIT:
    FunctionOut();

    return ret;
}

/* hardware/samsung_slsi-cm/openmax/osal/Exynos_OSAL_Android.cpp:156 */
OMX_ERRORTYPE Exynos_OSAL_UnlockANBHandle(OMX_IN OMX_U32 handle)
{
    FunctionIn();

    OMX_ERRORTYPE ret = OMX_ErrorNone;
    GraphicBufferMapper &mapper = GraphicBufferMapper::get();
    buffer_handle_t bufferHandle = (buffer_handle_t)(uintptr_t) handle;

    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: handle: 0x%x", __func__, handle);

    if (mapper.unlock(bufferHandle) != 0) {
        Exynos_OSAL_Log(EXYNOS_LOG_ERROR, "%s: mapper.unlock() fail", __func__);
        ret = OMX_ErrorUndefined;
        goto EXIT;
    }
    lockCnt--;
    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: lockCnt:%d", __func__, lockCnt);

    Exynos_OSAL_Log(EXYNOS_LOG_TRACE, "%s: buffer unlocked: 0x%x", __func__, handle);

EXIT:
    FunctionOut();

    return ret;
}

#ifdef __cplusplus
}
#endif
