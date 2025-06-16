#include "ScriptEngine.h"
#include "Renderer.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "nlohmann/json.hpp"
#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

void ScriptEngine::combatLoop(Character& player, Room& room, Renderer& renderer) {
    renderer.displayText("| Бой начинается!");
    
    Character& enemy = room.enemy;
    player.resetCombatStatus();
    enemy.resetCombatStatus();
    
    int turn = 0; // 0 - игрок, 1 - враг
    
    while (player.isAlive() && enemy.isAlive()) {
        if (turn == 0) {
            // Ход игрока
            renderer.renderCombatStatus(player, enemy);
            renderer.renderCombatMenu();
            
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1: // Атака
                    player.attackAction(enemy);
                    renderer.displayText("| Ты атаковал врага!");
                    break;
                case 2: // Защита
                    player.guardAction();
                    renderer.displayText("| Ты принимаешь защитную стойку!");
                    break;
                case 3: // Лечение
                    player.healAction();
                    renderer.displayText("| Ты восстанавливаешь здоровье!");
                    break;
                case 4: // Усиление
                    player.boostAction();
                    renderer.displayText("| Ты усилил свои способности!");
                    break;
                case 5: // Инвентарь
                    renderer.renderInventory(player);
                    
                    if (!player.inventory.empty()) {
                        renderer.displayText("| Выбери предмет (0 для отмены): ");
                        int itemIndex;
                        std::cin >> itemIndex;
                        
                        if (itemIndex > 0 && itemIndex <= player.inventory.size()) {
                            Item& item = player.inventory[itemIndex-1];
                            if (item.isConsumable()) {
                                player.heal(item.healAmount);
                                player.restoreMana(item.manaRestore);
                                
                                if (item.boostAttack > 0 || item.boostDefense > 0) {
                                    player.attackBoost = item.boostAttack;
                                    player.defenseBoost = item.boostDefense;
                                    player.boostTurns = item.boostDuration;
                                }
                                
                                renderer.displayText("| Использован: " + item.name);
                                player.inventory.erase(player.inventory.begin() + (itemIndex-1));
                            }
                        }
                    }
                    break;
                default:
                    renderer.displayText("| Неверный выбор! Пропускаешь ход.");
            }
            
            // Проверка смерти врага
            if (!enemy.isAlive()) {
                renderer.displayText("| Ты победил " + enemy.name + "!");
                break;
            }
            
            player.applyTurnEffects();
            turn = 1;
        }
        else {
            // Ход врага
            renderer.displayText("\n| Ход " + enemy.name + ":");
            room.executeEnemyAction(enemy, player, renderer);
            renderer.renderCombatStatus(player, enemy);
            
            // Проверка смерти игрока
            if (!player.isAlive()) {
                renderer.displayText("| Ты пал в бою...");
                break;
            }
            
            enemy.applyTurnEffects();
            turn = 0;
        }
    }
}

void ScriptEngine::execute(const std::string& scriptFile, Character& player, Room* room, Renderer& renderer) {
    std::ifstream file(scriptFile);
    if (!file.is_open()) {
        renderer.displayText("| Ошибка: Не удалось загрузить скрипт " + scriptFile);
        return;
    }

    json script;
    file >> script;

    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    for (const auto& event : script["events"]) {
        if (event["trigger"] == "enter") {
            for (const auto& action : event["actions"]) {
                if (action["type"] == "dialog") {
                    renderer.displayText("| " + action["text"].get<std::string>());
                }
                else if (action["type"] == "spawn") {
                    room->enemy = Character(
                        action["enemy"].get<std::string>(), 
                        "Враг"
                    );
                    room->enemy.level = action["level"];
                    room->enemy.updateStats();
                    
                    // Загрузка действий врага
                    if (action.find("actions") != action.end()) {
                        room->enemyActions.clear();
                        for (const auto& a : action["actions"]) {
                            room->enemyActions.push_back(a.get<std::string>());
                        }
                    }
                    
                    renderer.displayText("| Перед тобой появляется " + room->enemy.name + "!");
                }
                else if (action["type"] == "battle") {
                    combatLoop(player, *room, renderer);
                }
                else if (action["type"] == "treasure") {
                    std::string rarityStr = action["rarity"].get<std::string>();
                    Rarity rarity = COMMON;
                    if (rarityStr == "UNCOMMON") rarity = UNCOMMON;
                    else if (rarityStr == "RARE") rarity = RARE;
                    else if (rarityStr == "EPIC") rarity = EPIC;
                    else if (rarityStr == "LEGENDARY") rarity = LEGENDARY;
                    else if (rarityStr == "MYTHIC") rarity = MYTHIC;
                    
                    Item treasure = generateItemForClass(player.className, rarity);
                    room->treasure = treasure;
                    player.addItem(treasure);
                    
                    renderer.displayText("| Ты находишь " + treasure.name + 
                         " (" + treasure.getRarityName() + ")!");
                    renderer.displayText("| " + treasure.description);
                    
                    if (treasure.hpBonus > 0) 
                        renderer.displayText("| +" + std::to_string(treasure.hpBonus) + " HP");
                    if (treasure.manaBonus > 0) 
                        renderer.displayText("| +" + std::to_string(treasure.manaBonus) + " MP");
                    if (treasure.attackBonus > 0) 
                        renderer.displayText("| +" + std::to_string(treasure.attackBonus) + " ATK");
                    if (treasure.defenseBonus > 0) 
                        renderer.displayText("| +" + std::to_string(treasure.defenseBonus) + " DEF");
                }
            }
        }
    }
}
```
