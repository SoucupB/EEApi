CXX = g++
CXXFLAGS = -std=gnu++11 -IHeaders/ -IPrivateHeaders/ -O2
EXTRA_EXE_FLAGS = -lPsapi -lgdi32 -luser32

SRC_DIR = InjectorSources
OBJ_DIR = Injector
TARGET = $(OBJ_DIR)/DllInjector32BitsExec.exe
SOURCES_DIR = Sources
INJECTOR_NAME = InjectUtilities32bits

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJ_DIR)/$(INJECTOR_NAME).o
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(EXTRA_EXE_FLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/$(INJECTOR_NAME).o: $(SOURCES_DIR)/$(INJECTOR_NAME).cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@