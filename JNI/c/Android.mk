LOCAL_PATH := $(call my-dir)

# libSAL
include $(CLEAR_VARS)

LOCAL_MODULE := LIBARSAL-prebuilt
LOCAL_SRC_FILES := lib/libarsal.a

include $(PREBUILT_STATIC_LIBRARY)

# WRAPPER_LIB
include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include 
LOCAL_LDLIBS := -llog
LOCAL_MODULE := libarsal_android
LOCAL_SRC_FILES := ARGenericNativeData_JNI.c
LOCAL_STATIC_LIBRARIES := LIBSAL-prebuilt
include $(BUILD_SHARED_LIBRARY)

