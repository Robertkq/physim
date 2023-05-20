APP_NAME = physim

OUTPUT_DIR = out
SRC_DIR = src

SFML_DIR = C:/Users/rober/Libraries/SFML-2.5.1
IMGUI_DIR = C:/Users/rober/Libraries/imgui
IMGUI-SFML_DIR = C:/Users/rober/Libraries/imgui-sfml
WIN_DIR = C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64

INCLUDE_DIR := -Iinclude/ \
	-I"$(SFML_DIR)/include/" \
	-I"$(IMGUI_DIR)/" \
	-I"$(IMGUI-SFML_DIR)/" 

SRCS := $(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(IMGUI_DIR)/*.cpp) \
	$(wildcard $(IMGUI-SFML_DIR)/*.cpp)

OBJS := $(addprefix $(OUTPUT_DIR)/, $(notdir $(SRCS:.cpp=.o)))

LIB_DIR := -L"$(SFML_DIR)/lib/" \
	-L"$(WIN_DIR)/"

LIB_LINK = -lsfml-graphics-s \
	-lsfml-window-s \
	-lsfml-system-s \
	-lwinmm \
	-lopengl32 \
	-lfreetype \
	-luser32 \
	-lgdi32

CXX := g++
CXXFLAGS := -std=c++17 -DSFML_STATIC -m64

.PHONY = clean all run

clean:
	powershell -Command "rm out\*"

all: $(OUTPUT_DIR)/$(APP_NAME)

$(OUTPUT_DIR)/$(APP_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ ${LIB_DIR} ${LIB_LINK}

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

$(OUTPUT_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

$(OUTPUT_DIR)/%.o: $(IMGUI-SFML_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

run:
	./$(OUTPUT_DIR)/$(APP_NAME)
