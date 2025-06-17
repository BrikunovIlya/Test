#pragma once
#include "../game/game.h"
#include <fstream>

/**
 * @brief Класс для управления сохранением и загрузкой состояния игры.
 * 
 * Класс SaveSystem предоставляет статические методы для сериализации и десериализации
 * данных игры в файл и из файла.
 */
class SaveSystem {
public:
    /**
     * @brief Сохраняет текущее состояние игры в указанный файл.
     * @param game Экземпляр класса Game, содержащий данные об игре.
     * @param filename Имя файла для сохранения.
     * @return true, если сохранение успешно, иначе false.
     */
    static bool Save(const Game& game, const std::string& filename);

    /**
     * @brief Загружает состояние игры из указанного файла.
     * @param game Экземпляр класса Game, в который загружаются данные.
     * @param filename Имя файла для загрузки.
     * @return true, если загрузка успешна, иначе false.
     */
    static bool Load(Game& game, const std::string& filename);
};