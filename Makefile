LLVM_CPPFLAGS = `llvm-config --cppflags`
LLVM_LDFLAGS = `llvm-config --ldflags`
LLVM_LIBS = `llvm-config --libs`

WIN_LDLIBS =

ifeq ($(target), win32)
PLATFORM = mingw32
CXX = i686-w64-mingw32-g++
PLATFORM_LDLIBS = $(WIN_LDLIBS)
PLATFORM_LDFLAGS =
TARGET = bin/$(PLATFORM)/profundity.exe
else ifeq ($(target), win64)
PLATFORM = mingw64
CXX = x86_64-w64-mingw32-g++
PLATFORM_LDLIBS = $(WIN_LDLIBS)
PLATFORM_LDFLAGS =
TARGET = bin/$(PLATFORM)/profundity.exe
else
PLATFORM = linux
PLATFORM_LDLIBS =
PLATFORM_LDFLAGS =
TARGET = bin/$(PLATFORM)/profundity
endif

OBJDIR = obj/$(PLATFORM)

SRC = $(shell find src -type f -name '*.cpp' -printf '%P\n')
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
CPPFLAGS = -DGLEW_STATIC -I src -I ../include $(LLVM_CPPFLAGS)
CXXFLAGS = -Wall -std=c++20
LDFLAGS = $(PLATFORM_LDFLAGS) $(LLVM_LDFLAGS)
LDLIBS = $(PLATFORM_LDLIBS) $(LLVM_LIBS)

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f -R $(OBJDIR)/*

clean_all:
	rm -f -R bin/*
	rm -f -R obj/*
