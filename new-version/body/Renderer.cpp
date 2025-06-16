#include "Renderer.h"
#include <iostream>
#include <iomanip>
#include <vector>

void Renderer::displayDescription(const std::string& text) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "| " << text << "\n";
}

void Renderer::displayText(const std::string& text) {
    std::cout << text << "\n";
}

void Renderer::renderRoom(const Room& room, const Character& player) {
    std::cout << std::string(60, '-') << "\n";
    std::cout << "| Ты стоишь в комнате. ";
    if (room.isLight) std::cout << "Свет освещает пространство.\n";
    else std::cout << "Тьма окружает тебя.\n";

    std::cout << "| Впереди ты видишь: ";
    if (room.enemy.isAlive()) 
        std::cout << "[E - " << room.enemy.name << "] ";
    if (room.treasure.name != "Пусто") 
        std::cout << "[$ - " << room.treasure.name << "] ";
    if (!room.enemy.isAlive() && room.treasure.name == "Пусто") 
        std::cout << "ничего.";
    std::cout << "\n";

    player.displayStats();
    std::cout << std::string(60, '-') << "\n";
}

void Renderer::renderCombatMenu() {
    std::cout << "\n| ====== ВАШ ХОД ======\n";
    std::cout << "| 1. Атака\n";
    std::cout << "| 2. Защита\n";
    std::cout << "| 3. Лечение (10 MP)\n";
    std::cout << "| 4. Усиление (15 MP)\n";
    std::cout << "| 5. Инвентарь\n";
    std::cout << "| Выбери действие: ";
}

void Renderer::renderCombatStatus(const Character& player, const Character& enemy) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "| " << player.name << " (HP: " << player.hp << "/" << player.maxHp;
    std::cout << " | MP: " << player.mana << "/" << player.maxMana << ")\n";
    
    if (player.isGuarding) std::cout << "| [В РЕЖИМЕ ЗАЩИТЫ]\n";
    if (player.attackBoost > 0) std::cout << "| [АТАКА +" << player.attackBoost << "%]\n";
    if (player.defenseBoost > 0) std::cout << "| [ЗАЩИТА +" << player.defenseBoost << "%]\n";
    
    std::cout << "| --------------------\n";
    std::cout << "| " << enemy.name << " (HP: " << enemy.hp << "/" << enemy.maxHp << ")\n";
    
    if (enemy.isGuarding) std::cout << "| [В РЕЖИМЕ ЗАЩИТЫ]\n";
    if (enemy.attackBoost > 0) std::cout << "| [АТАКА +" << enemy.attackBoost << "%]\n";
    if (enemy.defenseBoost > 0) std::cout << "| [ЗАЩИТА +" << enemy.defenseBoost << "%]\n";
    
    std::cout << std::string(60, '=') << "\n";
}

void Renderer::renderInventory(const Character& player) {
    if (player.inventory.empty()) {
        displayText("| Инвентарь пуст!");
        return;
    }
    
    displayText("| ===== ИНВЕНТАРЬ =====");
    for (int i = 0; i < player.inventory.size(); i++) {
        const Item& item = player.inventory[i];
        std::cout << "| " << (i+1) << ". " << item.name << " [" << item.getRarityName() << " " << item.getTypeName() << "]\n";
        std::cout << "|    " << item.description << "\n";
        
        if (item.hpBonus > 0) std::cout << "|    +" << item.hpBonus << " HP ";
        if (item.manaBonus > 0) std::cout << "| +" << item.manaBonus << " MP ";
        if (item.attackBonus > 0) std::cout << "| +" << item.attackBonus << " ATK ";
        if (item.defenseBonus > 0) std::cout << "| +" << item.defenseBonus << " DEF ";
        if (item.healAmount > 0) std::cout << "| Лечит: " << item.healAmount << " HP ";
        if (item.manaRestore > 0) std::cout << "| Восст. MP: " << item.manaRestore << " ";
        if (item.boostAttack > 0) std::cout << "| Усил. атаки: +" << item.boostAttack << "% ";
        if (item.boostDefense > 0) std::cout << "| Усил. защиты: +" << item.boostDefense << "% ";
        
        std::cout << "\n";
    }
    displayText("| ======================");
}

