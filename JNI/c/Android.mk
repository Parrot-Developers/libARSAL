LOCAL_PATH := $(call my-dir)

# libSAL
include $(CLEAR_VARS)

LOCAL_MODULE := LIBARSAL-prebuilt
LOCAL_SRC_FILES := lib/libarsal.so

include $(PREBUILT_SHARED_LIBRARY)

# WRAPPER_LIB
#include $(CLEAR_VARS)

#LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include 
#LOCAL_LDLIBS := -llog
#LOCAL_MODULE := sal
#LOCAL_STATIC_LIBRARIES := LIBSAL-prebuilt
#include $(BUILD_SHARED_LIBRARY)

