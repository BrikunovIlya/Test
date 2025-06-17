# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -I src
LDFLAGS := -lgdi32 -lwinmm

# Целевой исполняемый файл
TARGET := metroidvania.exe

# Директории с исходным кодом
SRC_DIR := src
GAME_DIR := $(SRC_DIR)/game
ENTITIES_DIR := $(SRC_DIR)/entities
SYSTEMS_DIR := $(SRC_DIR)/systems
UTILS_DIR := $(SRC_DIR)/utils

# Поиск исходных файлов
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS += $(wildcard $(GAME_DIR)/*.cpp)
SRCS += $(wildcard $(ENTITIES_DIR)/*.cpp)
SRCS += $(wildcard $(SYSTEMS_DIR)/*.cpp)
SRCS += $(wildcard $(UTILS_DIR)/*.cpp)

# Генерация списка объектных файлов
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

# Основная цель сборки
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Паттерн для компиляции .cpp файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Пересборка
rebuild: clean all

# Запуск
run: $(TARGET)
	./$(TARGET)

# Флаг для предотвращения конфликтов с файлами
.PHONY: all clean rebuild run