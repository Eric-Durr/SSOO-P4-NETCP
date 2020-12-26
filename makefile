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
BIN_NAME = netcp
TEST_NAME = netcp_test	

# File extentions
SRC_EXT = cpp
LIB_EXT = hpp


# Source files lists 
SOURCES= $(shell find . -type f \( -iname '*.$(SRC_EXT)' !  -iname 'includer.test.cpp' \) | sort -k 1nr | cut -f2-)
TEST_SOURCES = $(shell find . -type f \( -iname '*.$(SRC_EXT)' ! -iname '$(BIN_NAME)*' !  \) | sort -k 1nr | cut -f2-)

# Object files naming
# OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o) 
# Dependancy files naming
DEPS = $(OBJECTS:.o=.d)

# Config compiling flags
COMPILE_FLAGS = -std=c++14 -Wall -g -pthread
INCLUDES = -I include/ -I /usr/local/include
# Space-separated pkg-config libraries used by this project
LIBS = catch.h

compile:
	make send


.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(SEND_BIN_NAME)
	@$(RM) $(RECEIVE_BIN_NAME)
	@$(RM) $(TEST_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)


.PHONY: docs clean
docs:
	@echo "Updating documentation"
	@$(RM) -r $(DOCUMENTATION_PATH) index.html
	doxygen $(DOXYGEN_CONFIGURATION_FILE_NAME)
	

.PHONY: all test clean
test:
	@echo "Making tests: $(TEST_NAME)"
	$(CXX) -o $(TEST_NAME) $(TEST_SOURCES) $(COMPILE_FLAGS)
	./$(TEST_NAME) 

send:
	@echo "Making netcp program: $(BIN_NAME)"
	$(CXX) -o $(BIN_NAME) $(SOURCES)  $(COMPILE_FLAGS)
