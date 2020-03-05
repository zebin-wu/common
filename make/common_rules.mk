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
# The common rules
#

#
# Common target
#
common: cppstyle $(LIBCOMMON_DYNAMIC) $(LIBCOMMON_STATIC)

#
# CPP style macros
#
BUILD_CPPSTYLE_DIR ?= $(BUILD_DIR)/cppstyle
CPPSTYLE ?= cpplint --quiet
CPPSTYLE_INCLUDES := $(filter-out include, $(foreach dir, $(INCLUDES), $(shell find $(dir) -maxdepth 1 -type d)))
CPPSTYLE_HEADERS := $(foreach dir, $(CPPSTYLE_INCLUDES), $(wildcard $(dir)/*.hpp))
CPPSTYLE_SOURCES := $(SOURCES)
CPPSTYLE_FILES := $(CPPSTYLE_HEADERS) $(CPPSTYLE_SOURCES)
CPPSTYLE_TARGETS := $(CPPSTYLE_FILES:%=$(BUILD_CPPSTYLE_DIR)/%.cs)

#
# Clean cppstyle directory
#
.PHONY: clean_cppstyle
clean_cppstyle:
	$(QUIET)echo 'Clean cppstyle directory.'; rm -rf $(BUILD_CPPSTYLE_DIR)

#
# Rules to check CPP style
#
.PHONY: cppstyle
cppstyle: $(CPPSTYLE_TARGETS)

#
# Rule for each file to be checked
#
$(CPPSTYLE_TARGETS):$(BUILD_CPPSTYLE_DIR)/%.cs:%
	$(QUIET)echo 'CHECK	$<';\
	    $(CPPSTYLE) $< && (mkdir -p $(dir $@); touch $@)

#
# Rule to build common dynamic library
#
$(eval $(call BUILD_TARGET_RULES, $(LIBCOMMON_DYNAMIC), METHOD_LD,\
	$(SOURCES_LIBCOMMON),\
	-shared))

#
# Rule to build common static library
#
$(eval $(call BUILD_TARGET_RULES, $(LIBCOMMON_STATIC), METHOD_AR,\
	$(SOURCES_LIBCOMMON)))

#
# Rule to compile source code
#
$(BUILD_DIR)/%.o: %.cpp
	$(QUIET)echo 'CXX	$<';\
	    mkdir -p $(dir $@); $(CXX) $(CPPFLAGS) -c -o $@ $<

#
# Rule to make dependencies files
#
$(BUILD_DIR)/%.d: %.cpp Makefile
	$(QUIET)echo 'DEP	$<';\
		(mkdir -p $(dir $@); $(CXX) -MM $(CPPFLAGS) $< | \
		sed 's,.*\.o[ :]*,$(@:%.d=%.o) $@: ,g' > $@) || rm -f $@

#
# Clean build directory
#
.PHONY: clean
clean:
	$(QUIET)echo 'Clean build directory'; rm -rf $(BUILD_DIR)
