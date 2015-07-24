LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CATEGORY_PATH := dragon/libs
LOCAL_MODULE := libARSAL
LOCAL_DESCRIPTION := ARSDK Software Abstraction Layer

LOCAL_LIBRARIES := ARSDKBuildUtils
LOCAL_EXPORT_LDLIBS := -larsal

# Copy in build dir so bootstrap files are generated in build dir
LOCAL_AUTOTOOLS_COPY_TO_BUILD_DIR := 1

# Configure script is not at the root
LOCAL_AUTOTOOLS_CONFIGURE_SCRIPT := Build/configure

define LOCAL_AUTOTOOLS_CMD_POST_UNPACK
	$(Q) cd $(PRIVATE_SRC_DIR)/Build && ./bootstrap
endef

include $(BUILD_AUTOTOOLS)
