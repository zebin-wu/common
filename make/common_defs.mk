#
# Copyright (c) 2020 KNpTrue
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# The common definition
#

#
# Link a target
#
# $1: Target name
# $2: Source files
# $3: Extra link switchs
#
define METHOD_LD
	echo 'LINK $(notdir $1)';\
	    $(CXX) -o $1 $(2:%.cpp=$(BUILD_DIR)/%.o) $3 $(LDFLAGS)

-include $(2:%.cpp=$(BUILD_DIR)/%.d)
endef

#
# Make a static library
#
# $1: Target name
# $2: Source files
#
define METHOD_AR
	echo 'AR $(notdir $1)';\
	    $(AR) cr $1 $(2:%.cpp=$(BUILD_DIR)/%.o)
endef

#
# Rule to build a target
#
# $1: Target name
# $2: Method
# $3: Source files
# $4: Extra link switchs
#
define BUILD_TARGET_RULES
$1: $(3:%.cpp=$(BUILD_DIR)/%.o)
	$(QUIET)[ -d $(shell dirname $1) ] || mkdir -p $(shell dirname $1);\
	    $(call $2, $1, $3, $4)
endef

#
# Add common include directory to $(INCLUDES)
#
INCLUDES += \
	$(COMMON_DIR)/include\
	$(COMMON_DIR)/src/include\
	$(COMMON_DIR)/src/platform/$(PLATFORM)/include\
	$(NULL)

#
# Source file of libcommon
#
SOURCES_LIBCOMMON := \
	$(wildcard $(COMMON_DIR)/src/common/*.cpp)\
	$(wildcard $(COMMON_DIR)/src/platform/$(PLATFORM)/*.cpp)\
	$(NULL)

#
# Source files of all
#
SOURCES += \
	$(SOURCES_LIBCOMMON)\
	$(NULL)

#
# Defines of library event
#
LIBCOMMON_NAME = libcommon
LIBCOMMON_DYNAMIC = $(LIB_DIR)/$(LIBCOMMON_NAME).so
LIBCOMMON_STATIC = $(LIB_DIR)/$(LIBCOMMON_NAME).a

#
# Compile command line switch of CPP
#
CPPFLAGS += -fPIC -ffunction-sections -fdata-sections

#
# Compile command line switch of LD
#
LDFLAGS += -fPIC -Wl,--gc-sections
