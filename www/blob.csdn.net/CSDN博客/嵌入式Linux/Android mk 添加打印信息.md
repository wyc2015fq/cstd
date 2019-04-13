
# Android.mk 添加打印信息 - 嵌入式Linux - CSDN博客

2018年11月01日 14:41:38[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：30



```python
# Copyright (C) 2009 The Android Open Source Project  
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
LOCAL_PATH:= $(call my-dir)  
  
include $(CLEAR_VARS)  
  
LOCAL_MODULE    := libgl2jni  
LOCAL_CFLAGS    := -Werror  
LOCAL_SRC_FILES := gl_code.cpp  
LOCAL_LDLIBS    := -llog -lGLESv2  
$(warning 'hehe fuck')  
$(warning $(LOCAL_SRC_FILES),$(LOCAL_PATH))  
$(warning $(all-subdir-makefiles),$(this.makefile))  
$(warning $(TARGET_ARCH))  
$(info erroinfo,hehe)  
$(message msg)  
$(warning  " LOCAL_LDLIBS =  $(LOCAL_LDLIBS)")  
$(info $(TARGET_PLATFORM))  
include $(BUILD_SHARED_LIBRARY) 
$(warning "########################TARGET_OUT_EXECUTABLES is$(TARGET_OUT_EXECUTABLES)")
```


