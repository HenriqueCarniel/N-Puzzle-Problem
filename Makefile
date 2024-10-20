########################################
#   DIRETÓRIOS E OUTROS CAMINHOS
########################################
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

########################################
#   ARQUIVOS
########################################
SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC:.cpp=.o))
DEPS += $(shell find $(INCLUDE_DIR) -type f -name "*.h")

########################################
#   CONFIGS
########################################
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)
_CXXFLAGS_RELEASE = -Ofast
_CXXFLAGS_DEBUG = -g -DDEBUG -pg  # Adicione -pg para profiling
LDFLAGS = -pg  # Adicione -pg para o link

PROGRAM_NAME = main
TEST_SCRIPT = script.sh

########################################
#   REGRAS
########################################
.PHONY: all release debug clean build test

all: release

release: CXXFLAGS += $(_CXXFLAGS_RELEASE)
release: build

debug: CXXFLAGS += $(_CXXFLAGS_DEBUG)
debug: LDFLAGS += -pg  # Garantir que o link também tenha -pg
debug: build

build: $(OBJ)
	$(CXX) -o $(PROGRAM_NAME) $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(PROGRAM_NAME)
	rm -f $(BUILD_DIR)/*

# Compilação de código objeto depende de alteração no .cpp correspondente ou num dos includes
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
