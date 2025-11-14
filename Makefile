CXX ?= g++
CXXFLAGS ?= -std=c++17 -Iinclude -O2 -Wall -Wextra

SRCS_TEMPLATES := src/templates.cpp
SRCS_MAINC := src/mainC.cpp 
SRCS_MAINT := src/mainT.cpp $(SRCS_TEMPLATES)

OBJ_DIR := bin
BUILD_DIR := build
TARGET_C := $(BUILD_DIR)/mainC
TARGET_T := $(BUILD_DIR)/mainT

all: $(TARGET_C) $(TARGET_T)

$(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET_C): $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_MAINC)) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET_T): $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_MAINT)) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

run-mainC: $(TARGET_C)
	./$(TARGET_C)

run-mainT: $(TARGET_T)
	./$(TARGET_T)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET_C) $(TARGET_T)

.PHONY: all clean run-mainC run-mainT
