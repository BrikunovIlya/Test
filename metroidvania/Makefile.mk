## @file Makefile.mk
## @brief Файл сборки для компиляции проекта Metroidvania.
## 
## Этот Makefile определяет правила для компиляции исходных файлов C++,
## создания исполняемого файла, очистки временных файлов и запуска программы.

## @brief Компилятор C++.
## @details Указывает используемый компилятор (g++).
CXX := g++

## @brief Флаги компиляции.
## @details Включают стандарт C++17, строгие предупреждения и оптимизацию.
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -I src

## @brief Флаги компоновки.
## @details Подключают библиотеки gdi32 и winmm для Windows.
LDFLAGS := -lgdi32 -lwinmm

## @brief Имя целевого исполняемого файла.
## @details Имя выходного файла — metroidvania.exe.
TARGET := metroidvania.exe

## @brief Корневая директория исходных файлов.
## @details Указывает расположение исходных файлов (src).
SRC_DIR := src

## @brief Директория с файлами игровой логики.
## @details Поддиректория src/game.
GAME_DIR := $(SRC_DIR)/game

## @brief Директория с файлами сущностей.
## @details Поддиректория src/entities.
ENTITIES_DIR := $(SRC_DIR)/entities

## @brief Директория с файлами систем.
## @details Поддиректория src/systems.
SYSTEMS_DIR := $(SRC_DIR)/systems

## @brief Директория с утилитами.
## @details Поддиректория src/utils.
UTILS_DIR := $(SRC_DIR)/utils

## @brief Список исходных файлов.
## @details Собирает все .cpp файлы из указанных директорий.
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS += $(wildcard $(GAME_DIR)/*.cpp)
SRCS += $(wildcard $(ENTITIES_DIR)/*.cpp)
SRCS += $(wildcard $(SYSTEMS_DIR)/*.cpp)
SRCS += $(wildcard $(UTILS_DIR)/*.cpp)

## @brief Список объектных файлов.
## @details Генерируется из списка исходных файлов (.cpp → .o).
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

## @brief Основная цель сборки.
## @details Компилирует и компонует проект в исполняемый файл.
all: $(TARGET)

## @brief Правило для создания исполняемого файла.
## @param TARGET Имя целевого файла (metroidvania.exe).
## @param OBJS Список объектных файлов.
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

## @brief Паттерн для компиляции .cpp файлов в .o.
## @param %.o Выходной объектный файл.
## @param %.cpp Исходный файл C++.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

## @brief Очистка временных и выходных файлов.
## @details Удаляет объектные файлы и исполняемый файл.
clean:
	rm -f $(OBJS) $(TARGET)

## @brief Пересборка проекта.
## @details Выполняет очистку и повторную сборку.
rebuild: clean all

## @brief Запуск программы.
## @details Запускает собранный исполняемый файл.
run: $(TARGET)
	./$(TARGET)

## @brief Указывает фиктивные цели.
## @details Предотвращает конфликты с файлами, имеющими такие же имена.
.PHONY: all clean rebuild run