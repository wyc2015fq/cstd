
# JNI简单的例子 - 嵌入式Linux - CSDN博客

2015年04月03日 16:02:38[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：767



```python
/*
 * Copyright 2009 Cedric Priscal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <nativehelper/JNIHelp.h>
#include <nativehelper/jni.h>
#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <utils/Log.h>
#include <cutils/properties.h>
#include <fcntl.h>
/*
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <cutils/properties.h>
#include <jni.h>
#include <utils/Log.h>
#include <stdio.h>
#include <nativehelper/JNIHelp.h>
#include <nativehelper/jni.h>
*/
#define LOG_TAG "pttjni"
#define DEBUG_TRACE_EN
//#define DEBUG_DATA_EN
#ifdef DEBUG_TRACE_EN
#define TRACE(fmt,...)	ALOGD("*pttswitch jni(%s):"fmt,__FUNCTION__,##__VA_ARGS__)
#else
#define TRACE(...)
#endif
#define DEVICE_PATH		"/sys/class/dsp/pttswitch"
#define LOGD ALOGD
#define LOGE ALOGE
#define LOGW ALOGW
static const char *classPathName = "com/zte/ppt/PPTApi";
static int gFd = -1;
//================================================================

static jint j_open(JNIEnv *env, jobject obj,jint speed)
{
	int fd;
	char devices[64];
	if(gFd > 0){
		LOGW("device had been opened before (fd=%d)\n",gFd);
		return -1;
	}
	sprintf(devices,"%s",DEVICE_PATH);
	fd = open(devices, O_RDWR);
	if (fd < 0){
		LOGE("Cannot open device port:%s\n",devices);
		return -1;
	}
	gFd = fd;
	TRACE("open %s, (%d)\n",devices,gFd);
	return gFd;
}
static jint j_close(JNIEnv *env, jobject obj)
{
	int m = 0;
	m = close(gFd);
	gFd = -1;
	TRACE("Close port");
	return m;
}
#define CN_TEMP_READBUF_LENTH	512
static char g_TempReadBuf[CN_TEMP_READBUF_LENTH];
static jint j_read(JNIEnv *env, jobject obj,jbyteArray vBuffer,int wait_second)
{
	int arrlen =(env)->GetArrayLength(vBuffer);
	jbyte *tmp = env->GetByteArrayElements(vBuffer, NULL);
	int i,pReadLength = -1;
	int count;
	
	struct timeval tv;
	//tv.tv_sec = 10;
	//tv.tv_usec = 0;
	fd_set fds;
	if(!tmp || arrlen < 1){
		LOGE("Read error:buffer NULL or arrlen=%d\n",arrlen);
		return -1;
	}
	if(gFd < 0){
		goto error_exit;
	}
	if(arrlen < CN_TEMP_READBUF_LENTH){
		count = arrlen;
	}else{
		count = CN_TEMP_READBUF_LENTH;
	}
	TRACE("wait read(fd=%d,count=%d)\n",gFd,count);
	FD_ZERO(&fds);
	FD_SET(gFd, &fds);
	if(wait_second > 0){
		tv.tv_sec = wait_second;
		tv.tv_usec = 0;
		if (select(gFd+1, &fds, NULL, NULL, &tv) <= 0){
			LOGD("select timeout..\n");
			pReadLength = 0;
			goto error_exit;
		}
	}else{
		if (select(gFd+1, &fds, NULL, NULL, NULL) <= 0){
			LOGD("select return error!\n");
			goto error_exit;
		}
	}
	TRACE("read ... \n");
	pReadLength = read(gFd,g_TempReadBuf,count);
	memcpy(tmp,g_TempReadBuf,pReadLength);
#ifdef DEBUG_DATA_EN
	LOGD("Read Length=%d,arrlen=%d\n",pReadLength,arrlen);
	for(i=0;i<pReadLength;i++)
	{
		LOGD("0x%02x ",tmp[i]);
	}
#endif
error_exit:
	env->ReleaseByteArrayElements(vBuffer,tmp,0);
	return pReadLength;
}
#define CN_TEMP_WRITE_LENTH		512
static char g_TempWriteBuf[CN_TEMP_WRITE_LENTH];
static jint j_write(JNIEnv *env, jobject obj,jbyteArray vBuffer,jint vLength)
{
	int i = 0;
	int count;
	jbyte *tmp = env->GetByteArrayElements(vBuffer, NULL);
	if(!tmp || vLength < 1){
		LOGD("write error: buffer is NULL(%d)\n",vLength);
		return -1;
	}
	
	if(gFd < 0){
		goto exit;
	}
	if(vLength > CN_TEMP_WRITE_LENTH){
		count = CN_TEMP_WRITE_LENTH;
	}else{
		count = vLength;
	}
	memcpy(g_TempWriteBuf,tmp,count);
#ifdef DEBUG_DATA_EN
	LOGD("write lenth=%d\n",count);
	for(i=0;i<vLength;i++){
		LOGD("0x%02x",g_TempWriteBuf[i]);
	}
#endif
	i = write(gFd,g_TempWriteBuf,count);
	TRACE("write fd=%d,count=%d,ret=%d \n",gFd,vLength,i);
exit:
	env->ReleaseByteArrayElements(vBuffer,tmp,0);
	return i;
}
static JNINativeMethod gMethods[] = {
	{"open", 	"(I)I",		(void*)j_open},
	{"close",	"()I",		(void*)j_close},
	{"read", 	"([BI)I", 	(void*)j_read},
	{"write", 	"([BI)I", 	(void*)j_write},
};
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        fprintf(stderr,
            "Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        fprintf(stderr, "RegisterNatives failed for '%s'\n", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
static int register_NativeMethods(JNIEnv* env)
{
	int result;
//	result = jniRegisterNativeMethods(env, classPathName, gMethods, NELEM(gMethods));
	result = registerNativeMethods(env, classPathName, gMethods, NELEM(gMethods));
	if(result < 0){
		return JNI_FALSE;
	}
	return JNI_TRUE;
}
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;
	
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("GetEnv failed!");
		return result;
	}
	
	if(register_NativeMethods(env) != JNI_TRUE){
        LOGE("ERROR:regittsrNatives failed");
		return result;
    }
    return JNI_VERSION_1_4;
}
```
```python
<pre class="java" name="code">#
# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(my-dir)
#########################################
# jni part
# allow jni build if java is supported, necessary for PDK
ifneq ($(TARGET_BUILD_JAVA_SUPPORT_LEVEL),)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES:= \
	pttswitch.cpp
LOCAL_MODULE := libptt_jni
LOCAL_SHARED_LIBRARIES := \
	libnativehelper \
	libcutils \
	libutils \
include $(BUILD_SHARED_LIBRARY)
endif # JAVA_SUPPORT
```


