
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <map>
#include "Item.h"

class Character {
public:
    std::string name, className;
    int hp, maxHp, attack, defense, mana, maxMana, level;
    int levelUpHpGain, levelUpManaGain, levelUpAttackGain, levelUpDefenseGain;
    bool alive;
    
    // Боевые статусы
    bool isGuarding;
    int attackBoost;
    int defenseBoost;
    int boostTurns;
    
    // Система инвентаря
    std::vector<Item> inventory;
    std::vector<Item> equippedItems;
    
    Character(std::string n = "Безымянный", std::string c = "Воин");
    void updateStats();
    void displayStats() const;
    bool isAlive() const { return alive; }
    void takeDamage(int damage);
    int dealDamage() const;
    void heal(int amount);
    void restoreMana(int amount);
    void levelUp();
    void addItem(const Item& item);
    void equipItem(int index);
    void applyItemEffects();
    void resetCombatStatus();
    void applyTurnEffects();
    
    // Боевые действия
    void attackAction(Character& target);
    void guardAction();
    void healAction();
    void boostAction();
    
    // Проверки состояний
    bool isBoosted() const { return attackBoost > 0 || defenseBoost > 0; }
    bool hasStatus() const { return isGuarding || isBoosted(); }
};

#endif
```

**Item.h**
```cpp
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

enum Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY,
    MYTHIC
};

enum ItemType {
    WEAPON,
    ARMOR,
    CONSUMABLE,
    SCROLL
};

class Item {
public:
    std::string name;
    std::string description;
    int value;
    Rarity rarity;
    ItemType type;
    std::string className;
    
    // Статы
    int hpBonus;
    int attackBonus;
    int defenseBonus;
    int manaBonus;
    
    // Эффекты
    int healAmount;
    int manaRestore;
    int boostAttack;
    int boostDefense;
    int boostDuration;
    
    // Требования
    int levelRequirement;
    
    Item(std::string n = "Пусто", std::string d = "", int v = 0, 
         Rarity r = COMMON, ItemType t = CONSUMABLE, std::string c = "", 
         int hp = 0, int att = 0, int def = 0, int mn = 0,
         int heal = 0, int restore = 0, int batt = 0, int bdef = 0, int dur = 0, int lvl = 1);
    
    std::string getRarityName() const;
    std::string getTypeName() const;
    bool isEquippable() const;
    bool isConsumable() const;
};

#endif
```

