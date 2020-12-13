# ====================================================================
# ======================================================================

CXX ?= g++

# Routes 
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
TEST_PATH = test
DOCUMENTATION_PATH = docs
DOXYGEN_CONFIGURATION_FILE_NAME = Doxyfile

# Executable file name 
SEND_BIN_NAME = netcp_send
RECEIVE_BIN_NAME = netcp_receive
TEST_NAME = netcp_test	

# File extentions
SRC_EXT = cpp
LIB_EXT = hpp


# Source files lists 
SOURCES_OF_SEND = $(shell find . -type f \( -iname '*.$(SRC_EXT)' ! -iname '$(RECEIVE_BIN_NAME)*' ! -iname 'includer.test.cpp' \) | sort -k 1nr | cut -f2-)
SOURCES_OF_RECEIVE = $(shell find . -type f \( -iname '*.$(SRC_EXT)' ! -iname '$(SEND_BIN_NAME)*' ! -iname 'includer.test.cpp' \) | sort -k 1nr | cut -f2-)
TEST_SOURCES = $(shell find . -type f \( -iname '*.$(SRC_EXT)' ! -iname '$(SEND_BIN_NAME)*' ! -iname '$(RECEIVE_BIN_NAME)*' \) | sort -k 1nr | cut -f2-)

# Object files naming
# OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o) 
# Dependancy files naming
DEPS = $(OBJECTS:.o=.d)

# Config compiling flags
COMPILE_FLAGS = -std=c++14 -Wall -g
INCLUDES = -I include/ -I /usr/local/include
# Space-separated pkg-config libraries used by this project
LIBS = catch.h

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(SEND_BIN_NAME)
	@$(RM) $(RECEIVE_BIN_NAME)
	@$(RM) $(TEST_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

.PHONY: docs clean
docs:
	@echo "Updating documentation"
	@$(RM) -r $(DOCUMENTATION_PATH) index.html
	doxygen $(DOXYGEN_CONFIGURATION_FILE_NAME)
	

.PHONY: all test clean
test:
	@echo "Making tests: $(TEST_NAME)"
	$(CXX) -o $(TEST_NAME) $(TEST_SOURCES) 
	./$(TEST_NAME) 

send:
	@echo "Making netcp send program: $(SEND_BIN_NAME)"
	$(CXX) -o $(SEND_BIN_NAME) $(SOURCES_OF_SEND)  
receive:
	@echo "Making netcp RECEIVE program: $(RECEIVE_BIN_NAME)"
	$(CXX) -o $(RECEIVE_BIN_NAME) $(SOURCES_OF_RECEIVE) 
