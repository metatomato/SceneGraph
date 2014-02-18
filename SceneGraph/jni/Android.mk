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
DEFAULT_LOCAL_PATH := $(call my-dir)

include $(DEFAULT_LOCAL_PATH)/Bullet/Android.mk
include $(DEFAULT_LOCAL_PATH)/GUS/Android.mk



include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(DEFAULT_LOCAL_PATH)/Bullet

LOCAL_PATH 		:= $(DEFAULT_LOCAL_PATH)
LOCAL_MODULE    := scenegraph
LOCAL_CFLAGS    := -D ANDROID_TEGRA -fpermissive -frtti -std=c++11
LOCAL_SRC_FILES := JNIUtils.cpp gl_code.cpp
LOCAL_LDLIBS    := -lstdc++ -lm  -ldl -lc -llog -lGLESv2 -landroid

LOCAL_SHARED_LIBRARIES := gus bullet

include $(BUILD_SHARED_LIBRARY)
