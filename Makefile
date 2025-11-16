CXX ?= g++

# Choose debug or release flags with `make DEBUG=1` for debug build
ifeq ($(DEBUG),1)
CXXFLAGS ?= -std=c++17 -Iinclude -g -O0 -Wall -Wextra
else
CXXFLAGS ?= -std=c++17 -Iinclude -O2 -Wall -Wextra
endif

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

r-mainC: $(TARGET_C)
	./$(TARGET_C)

r-mainT: $(TARGET_T)
	./$(TARGET_T)

debug-mainC: DEBUG=1
debug-mainC: clean $(TARGET_C)

debug-mainT: DEBUG=1
debug-mainT: clean $(TARGET_T)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET_C) $(TARGET_T)

.PHONY: all clean run-mainC run-mainT debug-mainC debug-mainT
