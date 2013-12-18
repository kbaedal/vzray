CXX = g++

SRC_ABS_DIR = $(shell readlink -f src)

CXXFLAGS = -I$(SRC_ABS_DIR) -g -o

SOURCES = $(wildcard src/*.cpp)
SOURCES += $(wildcard src/aabb/*.cpp)
SOURCES += $(wildcard src/camera/*.cpp)
SOURCES += $(wildcard src/material/*.cpp)
SOURCES += $(wildcard src/matrix/*.cpp)
SOURCES += $(wildcard src/parser/*.cpp)
SOURCES += $(wildcard src/randomc/*.cpp)
SOURCES += $(wildcard src/renderer/*.cpp)
SOURCES += $(wildcard src/scene/*.cpp)
SOURCES += $(wildcard src/shapes/*.cpp)
SOURCES += $(wildcard src/texture/*.cpp)
SOURCES += $(wildcard src/utils/*.cpp)

OBJECTS = $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))

all: dirs bin/vzray 

dirs:
	@mkdir -p bin obj deps

bin/vzray : $(OBJECTS)
	@echo "Linking $@"
	@$(CXX) $(CXXFLAGS) bin/vzray $(OBJECTS)
	
obj/%.o : src/%.cpp
	@echo "Building object $@"	
	@$(CXX) $(CXXFLAGS) $@ -c $<
			
obj/%.o : src/aabb/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<

obj/%.o : src/camera/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/material/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/matrix/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<

obj/%.o : src/parser/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/randomc/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/renderer/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/scene/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<

obj/%.o : src/shapes/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/textures/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
obj/%.o : src/utils/%.cpp
	@echo "Building object $@"
	@$(CXX) $(CXXFLAGS) $@ -c $<
	
clean :
	@echo "Cleaning bin/ and obj/..."
	@$(RM) bin/* obj/*

