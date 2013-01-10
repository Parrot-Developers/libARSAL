LOCAL_PATH := $(call my-dir)

# Not with bionic
ifneq ("$(TARGET_LIBC)","bionic")

include $(CLEAR_VARS)

LOCAL_MODULE := libSAL
LOCAL_DESCRIPTION := ardrone3 SDK Software Abstraction Layer

LOCAL_EXPORT_LDLIBS := -lsal

define LOCAL_AUTOTOOLS_CMD_POST_UNPACK
	$(Q) cd $(PRIVATE_SRC_DIR) && ./bootstrap
endef

define LOCAL_AUTOTOOLS_CMD_POST_DIRCLEAN
	$(Q) cd $(PRIVATE_SRC_DIR) && ./cleanup
endef

include $(BUILD_AUTOTOOLS)

endif
