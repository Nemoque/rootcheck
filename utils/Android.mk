# This makefile supplies the rules for building a library of JNI code for
# use by our example of how to bundle a shared library with an APK.

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := rootcheck

# This is the target being built.
LOCAL_MODULE:= librootcheck


# All of the source files that we will compile.
LOCAL_SRC_FILES:= \
  single-list.c \
  double-list.c \
  root-check.c \
  native-jni.c
  

# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := \
	libutils liblog libopenssl

# No static libraries.
LOCAL_STATIC_LIBRARIES :=

# Also need the JNI headers.
LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE)

# No special compiler flags.
LOCAL_CFLAGS +=

include $(BUILD_SHARED_LIBRARY)
