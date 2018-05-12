CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := ./bin/runner
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11
LIB := 

LLVM_SRC_PATH := $$HOME/llvm/llvm
LLVM_BUILD_PATH := $$HOME/llvm/build
LLVM_BIN_PATH := $(LLVM_BUILD_PATH)/bin

LLVM_CCXFLAGS := $(LLVM_BIN_PATH)/llvm-config --cxxflags
LLVM_LDFLAGS := $(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs
LLVM_LDFLAGS_NOLIBS := $(LLVM_BIN_PATH)/llvm-config --ldflags
PLUGIN_LDFLAGS := -shared

CLANG_INCLUDES := \
	-I$(LLVM_SRC_PATH)/tools/clang/include \
	-I$(LLVM_BUILD_PATH)/tools/clang/include

INC := -I include $(LLVM_SRC_PATH)/include

CLANG_LIBS := \
	-Wl,--start-group \
	-lclangAST \
	-lclangASTMatchers \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangEdit \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangEdit \
	-lclangRewrite \
	-lclangRewriteFrontend \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangSerialization \
	-lclangToolingCore \
	-lclangTooling \
	-lclangFormat \
	-Wl,--end-group


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) -o $@ $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean: 
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

tests: 
	make -C ./tests

runtests:
	make -C ./tests run
