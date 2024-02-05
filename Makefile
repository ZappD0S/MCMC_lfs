# adapted from https://makefiletutorial.com/#makefile-cookbook

CXX = g++

# -D_GLIBCXX_DEBUG

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CXXFLAGS = -O3 -fvisibility=hidden -march=native -std=c++20 \
           -Wall -Wextra -Wpedantic -Werror -MMD -MP
# CXXFLAGS = -g -Og -std=c++20 -Wall -Wextra -Wpedantic -Werror  -MMD -MP

BUILD_DIR := ./build
SRC_DIR := ./cpp_ext

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIR) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Find all the C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
HMCLIB_SRCS := $(shell find $(SRC_DIR)/hmclib -name '*.cpp')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
HMCLIB_OBJS := $(HMCLIB_SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
HMCLIB_DEPS := $(HMCLIB_OBJS:.o=.d)

$(BUILD_DIR)/libhmclib.dll: $(HMCLIB_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(HMCLIB_OBJS) -o $@ -shared -fPIC -flto -Wl,--out-implib,$(@:.dll=.lib)
$(HMCLIB_OBJS) : $(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(INC_FLAGS) $(CXXFLAGS) -fPIC -flto -c $< -o $@

.PHONY: hmclib
hmclib: $(BUILD_DIR)/libhmclib.dll


MEX_SRCS := $(shell find $(SRC_DIR)/mex_funcs -maxdepth 1 -name '*.cpp')

MEX_OBJS := $(MEX_SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
MEX_DEPS := $(MEX_OBJS:.o=.d)

MEX_LIBS := $(MEX_OBJS:.cpp.o=.mexw64)

$(MEX_LIBS) : %.mexw64: %.cpp.o hmclib
	mkdir -p $(dir $@)
	$(CXX) $< -o $@ -shared -fPIC -flto \
		-L"/c/Program Files/MATLAB/R2023b/extern/lib/win64/mingw64" -L$(BUILD_DIR) -Wl,-rpath $(BUILD_DIR) \
		 -lhmclib -lmex -lMatlabEngine -lMatlabDataArray


$(MEX_OBJS) : $(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(INC_FLAGS) -isystem "/c/Program Files/MATLAB/R2023b/extern/include" \
		$(CXXFLAGS) -fPIC -flto \
		-c $< -o $@


.PHONY: mex
mex: $(MEX_LIBS)
	cp $(MEX_LIBS) $(BUILD_DIR)

all: hmclib mex

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(HMCLIB_DEPS)
-include $(MEX_DEPS)
