# Mathlib SIMD Test(NEON)
#Copyright (c) a1batross

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := neon_test

APP_PLATFORM := android-12

LOCAL_CFLAGS := -O3 -fomit-frame-pointer -ggdb -funsafe-math-optimizations -ftree-vectorize -fgraphite-identity -floop-interchange -funsafe-loop-optimizations -finline-limit=256 -mthumb -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -DVECTORIZE_SINCOS -fPIC -DHAVE_EFFICIENT_UNALIGNED_ACCESS -fPIE -pie
LOCAL_LDFLAGS := -fPIE -pie
LOCAL_CONLYFLAGS += -std=c99
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../common		            \
	$(LOCAL_PATH)/../engine		            \
	$(LOCAL_PATH)/../		            \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	mathlib_simd_test.c.neon

LOCAL_LDLIBS := -lm -llog

include $(BUILD_EXECUTABLE)
