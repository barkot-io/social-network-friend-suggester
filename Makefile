CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR  = src
OBJ_DIR  = obj
TARGET   = social_net

SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
