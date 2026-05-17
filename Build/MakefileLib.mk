CXX = g++
CXXFLAGS = -std=gnu++11 -IHeaders/ -IPrivateHeaders/ -O2 -lPsapi -lgdi32 -luser32
AR = ar rcs

SRC_DIR = Sources
OBJ_DIR = lib
TARGET = Engine.a

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(OBJ_DIR)/$(TARGET) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@