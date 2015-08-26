LOCAL_PATH := $(call my-dir)

# JNI Wrapper
include $(CLEAR_VARS)

LOCAL_CFLAGS := -g
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon
endif
LOCAL_MODULE := libarsal_android
LOCAL_SRC_FILES := JNI/c/ARSAL_JNI_Manager.c JNI/c/ARSAL_JNI_Print.c JNI/c/ARSAL_JNINativeData.c JNI/c/ARSAL_JNI_Md5Manager.c
LOCAL_LDLIBS := -llog -lz
LOCAL_SHARED_LIBRARIES := libARSAL-prebuilt
include $(BUILD_SHARED_LIBRARY)
