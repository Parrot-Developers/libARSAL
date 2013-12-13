LOCAL_PATH := $(call my-dir)

# Not with bionic
ifneq ("$(TARGET_LIBC)","bionic")

include $(CLEAR_VARS)

LOCAL_CATEGORY_PATH := dragon/libs
LOCAL_MODULE := libARSAL
LOCAL_DESCRIPTION := ARSDK Software Abstraction Layer

LOCAL_EXPORT_LDLIBS := -larsal

ifeq ("$(TARGET_PBUILD_FORCE_STATIC)","1")
LOCAL_AUTOTOOLS_CONFIGURE_ARGS := --disable-shared
endif


define LOCAL_AUTOTOOLS_CMD_POST_UNPACK
	$(Q) cd $(PRIVATE_SRC_DIR) && ./bootstrap
endef

define LOCAL_AUTOTOOLS_CMD_POST_DIRCLEAN
	$(Q) cd $(PRIVATE_SRC_DIR) && ./cleanup
endef

include $(BUILD_AUTOTOOLS)

endif
