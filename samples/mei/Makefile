# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2012-2019, Intel Corporation. All rights reserved.
#

obj-$(CONFIG_SAMPLE_INTEL_MEI_DAL_TEST) := dal_test.o

hostprogs-y := mei-amt-version

HOSTCFLAGS_mei-amt-version.o += -I$(objtree)/usr/include

always := $(hostprogs-y)

all: mei-amt-version
