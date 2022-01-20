# Config
BUILD_SRC := true
# Use -fPIC in CFLAGS for dynamic libraries
CFLAGS := -c -D_FORTIFY_SOURCE=2 -fsanitize=address -pedantic -std=c99 -Wall -Werror -Wextra -Wunused -Wno-unused-function
# Extra files to remove when using "make clean"
CLEAN_FILES := 
CXXFLAGS := -c -fsanitize=address -pedantic -std=c++98 -Wall -Werror -Wextra -Wunused -Wno-unused-function
# Set to cpp for c++ projects
LANG := c
# Use -shared -Wl,-soname,libname.so in LDFLAGS for dynamic libraries.
# Use -Wl,-rpath='/path/to/lib' when linking a dynamic library.
LDFLAGS := -fsanitize=address -Wl,-rpath='./'
LDLIBS := -ldl -pthread
PROJECT := htc
SRC_EXT := .tar.gz
# Set static to true when building static libraries.
STATIC := false
SUBDIRS := ./examples/hello
# Dynamic libraries use .so extension while static llibraries use .a.
TARGET_NAME := main

# Config Paths
BIN_PATH := ./bin
DIST_PATH := ..
INC_PATH := -I ./include
LD_PATH := -L ./bin
BUILD_PATH := ./build
SRC_PATH := ./src

# Config Programs
AR := ar -rc
CC := gcc
CD := cd
CHMOD := chmod +x
CXX := g++
MAKE := make --no-print-directory
MKDIR := mkdir -p
MV := mv -u
RANLIB := ranlib
RM := rm -rf
TAR := tar cavf

# Main (Do not edit below this line)
ifneq ($(LANG),cpp)
	COMPILER := $(CC)
	FLAGS := $(CFLAGS) $(INC_PATH)
else
	COMPILER := $(CXX)
	FLAGS := $(CXXFLAGS) $(INC_PATH)
endif

LD_PATH := -L ./ -L $(BIN_PATH) $(LD_PATH)
LDFLAGS := $(LD_PATH) $(LDFLAGS) $(LDLIBS)
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
TARGET := "$(BIN_PATH)/$(TARGET_NAME)"
TARGET_BUILD := "$(BUILD_PATH)/$(TARGET_NAME)"
CLEAN_FILES := $(CLEAN_FILES) $(BIN_PATH) $(BUILD_PATH) $(TARGET)

.PHONY: all
all: $(SUBDIRS) $(TARGET)

.PHONY: clean
clean: $(SUBDIRS)
	@$(RM) $(CLEAN_FILES)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c*
	@$(MKDIR) $(BUILD_PATH)
	@$(COMPILER) $(FLAGS) -o $@ $<

run: $(TARGET)
	@$(CHMOD) $(TARGET)
	@$(TARGET)

.PHONY: source
source: $(SUBDIRS)
ifeq ($(BUILD_SRC),true)
	@$(TAR) "$(DIST_PATH)/$(PROJECT)$(SRC_EXT)" "./"
endif

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

$(TARGET): $(TARGET_BUILD)
	@$(MKDIR) $(BIN_PATH)
	@$(MV) $(TARGET_BUILD) $(TARGET)

$(TARGET_BUILD): $(OBJ)
	@$(MKDIR) $(BUILD_PATH)
ifneq ($(STATIC),true)
	@$(COMPILER) -o $@ $? $(LDFLAGS)
else
	@$(AR) $(TARGET_BUILD) $(OBJ)
	@$(RANLIB) $(TARGET_BUILD)
endif

# This is free and unencumbered software released into the public domain.

# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# For more information, please refer to <http://unlicense.org/>
