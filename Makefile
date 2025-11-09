CXX ?= g++
CXXFLAGS ?= -std=c++17 -Iinclude -O2 -Wall -Wextra

SRCS_TEMPLATES := src/templates.cpp
SRCS_MAIN := src/main.cpp $(SRCS_TEMPLATES)
SRCS_MAINT := src/mainT.cpp $(SRCS_TEMPLATES)

BIN_DIR := build
TARGET := $(BIN_DIR)/main
TARGET_T := $(BIN_DIR)/mainT

all: $(TARGET) $(TARGET_T)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build/%.o: src/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(patsubst src/%.cpp,build/%.o,$(SRCS_MAIN))
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_T): $(patsubst src/%.cpp,build/%.o,$(SRCS_MAINT))
	$(CXX) $(CXXFLAGS) $^ -o $@

run-main: $(TARGET)
	./$(TARGET)

run-mainT: $(TARGET_T)
	./$(TARGET_T)

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET) $(TARGET_T)

.PHONY: all clean run-main run-mainT
