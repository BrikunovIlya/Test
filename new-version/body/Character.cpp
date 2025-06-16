**Character.cpp**
```cpp
#include "Character.h"
#include <iostream>
#include <algorithm>

Character::Character(std::string n, std::string c) : 
    name(n), className(c), level(1), alive(true),
    isGuarding(false), attackBoost(0), defenseBoost(0), boostTurns(0) {
    updateStats();
    hp = maxHp;
    mana = maxMana;
}

void Character::updateStats() {
    maxHp = 0;
    maxMana = 0;
    attack = 0;
    defense = 0;
    
    if (className == "Рыцарь") { 
        maxHp = 30 + level * 6; 
        maxMana = 20 + level * 2;
        attack = 6 + level * 2; 
        defense = 8 + level * 2;
        levelUpHpGain = 6; 
        levelUpManaGain = 2;
        levelUpAttackGain = 2; 
        levelUpDefenseGain = 2;
    }
    else if (className == "Маг") { 
        maxHp = 20 + level * 3; 
        maxMana = 40 + level * 4;
        attack = 4 + level * 1; 
        defense = 4 + level * 1;
        levelUpHpGain = 3; 
        levelUpManaGain = 4;
        levelUpAttackGain = 1; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Клирик") { 
        maxHp = 25 + level * 4; 
        maxMana = 35 + level * 3;
        attack = 5 + level * 1; 
        defense = 6 + level * 1;
        levelUpHpGain = 4; 
        levelUpManaGain = 3;
        levelUpAttackGain = 1; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Вор") { 
        maxHp = 22 + level * 3; 
        maxMana = 25 + level * 2;
        attack = 8 + level * 3; 
        defense = 5 + level * 1;
        levelUpHpGain = 3; 
        levelUpManaGain = 2;
        levelUpAttackGain = 3; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Охотник") { 
        maxHp = 24 + level * 4; 
        maxMana = 25 + level * 2;
        attack = 7 + level * 2; 
        defense = 6 + level * 1;
        levelUpHpGain = 4; 
        levelUpManaGain = 2;
        levelUpAttackGain = 2; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Пиротехник") { 
        maxHp = 20 + level * 3; 
        maxMana = 35 + level * 3;
        attack = 5 + level * 1; 
        defense = 5 + level * 1;
        levelUpHpGain = 3; 
        levelUpManaGain = 3;
        levelUpAttackGain = 1; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Воин") { 
        maxHp = 28 + level * 5; 
        maxMana = 15 + level * 1;
        attack = 7 + level * 2; 
        defense = 7 + level * 2;
        levelUpHpGain = 5; 
        levelUpManaGain = 1;
        levelUpAttackGain = 2; 
        levelUpDefenseGain = 2;
    }
    else if (className == "Лишенный") { 
        maxHp = 18 + level * 2; 
        maxMana = 10 + level * 1;
        attack = 3 + level * 1; 
        defense = 3 + level * 1;
        levelUpHpGain = 2; 
        levelUpManaGain = 1;
        levelUpAttackGain = 1; 
        levelUpDefenseGain = 1;
    }
    else if (className == "Паладин") { 
        maxHp = 32 + level * 6; 
        maxMana = 25 + level * 2;
        attack = 6 + level * 2; 
        defense = 9 + level * 3;
        levelUpHpGain = 6; 
        levelUpManaGain = 2;
        levelUpAttackGain = 2; 
        levelUpDefenseGain = 3;
    }
    else {
        maxHp = 20 + level * 3; 
        maxMana = 20 + level * 2;
        attack = 5 + level * 1; 
        defense = 5 + level * 1;
        levelUpHpGain = 3; 
        levelUpManaGain = 2;
        levelUpAttackGain = 1; 
        levelUpDefenseGain = 1;
    }
    
    // Применяем эффекты предметов
    applyItemEffects();
    
    // Гарантируем, что текущие HP не превышают максимум
    if (hp > maxHp) hp = maxHp;
    if (mana > maxMana) mana = maxMana;
}

void Character::displayStats() const {
    std::cout << "| " << name << " (Класс: " << className << " Ур. " << level << ")\n";
    std::cout << "| HP: " << hp << "/" << maxHp;
    std::cout << " | MP: " << mana << "/" << maxMana;
    std::cout << " | ATK: " << attack;
    std::cout << " | DEF: " << defense << "\n";
    
    if (isGuarding) std::cout << "| [Защита активна]\n";
    if (attackBoost > 0) std::cout << "| [Атака +" << attackBoost << " (" << boostTurns << " ходов)]\n";
    if (defenseBoost > 0) std::cout << "| [Защита +" << defenseBoost << " (" << boostTurns << " ходов)]\n";
}

void Character::takeDamage(int damage) {
    int reduction = defense;
    if (isGuarding) reduction *= 2; // Блок удваивает защиту
    
    int actualDamage = std::max(1, damage - reduction);
    hp -= actualDamage;
    
    if (hp <= 0) { 
        hp = 0; 
        alive = false; 
    }
}

int Character::dealDamage() const {
    int baseDamage = attack;
    if (attackBoost > 0) baseDamage += baseDamage * attackBoost / 100;
    return std::max(1, baseDamage);
}

void Character::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

void Character::restoreMana(int amount) {
    mana += amount;
    if (mana > maxMana) mana = maxMana;
}

void Character::addItem(const Item& item) {
    inventory.push_back(item);
}

void Character::equipItem(int index) {
    if (index < 0 || index >= inventory.size()) return;
    
    Item& item = inventory[index];
    if (!item.isEquippable()) return;
    
    // Снимаем предметы того же типа
    auto it = std::remove_if(equippedItems.begin(), equippedItems.end(),
        [&](const Item& equipped) {
            return equipped.type == item.type;
        });
    equippedItems.erase(it, equippedItems.end());
    
    // Надеваем новый предмет
    equippedItems.push_back(item);
    applyItemEffects();
}

void Character::applyItemEffects() {
    // Сбрасываем базовые статы
    int baseHp = 0;
    int baseMana = 0;
    int baseAttack = 0;
    int baseDefense = 0;
    
    // Рассчитываем базовые статы без предметов
    if (className == "Рыцарь") { 
        baseHp = 30 + level * 6; 
        baseMana = 20 + level * 2;
        baseAttack = 6 + level * 2; 
        baseDefense = 8 + level * 2;
    }
    // ... аналогично для других классов ...
    
    maxHp = baseHp;
    maxMana = baseMana;
    attack = baseAttack;
    defense = baseDefense;
    
    // Применяем бонусы от экипированных предметов
    for (const Item& item : equippedItems) {
        maxHp += item.hpBonus;
        maxMana += item.manaBonus;
        attack += item.attackBonus;
        defense += item.defenseBonus;
    }
    
    // Гарантируем, что текущие значения не превышают максимумы
    if (hp > maxHp) hp = maxHp;
    if (mana > maxMana) mana = maxMana;
}

void Character::resetCombatStatus() {
    isGuarding = false;
    attackBoost = 0;
    defenseBoost = 0;
    boostTurns = 0;
}

void Character::applyTurnEffects() {
    if (boostTurns > 0) {
        boostTurns--;
        if (boostTurns == 0) {
            attackBoost = 0;
            defenseBoost = 0;
        }
    }
    isGuarding = false;
}

void Character::attackAction(Character& target) {
    int damage = dealDamage();
    target.takeDamage(damage);
}

void Character::guardAction() {
    isGuarding = true;
}

void Character::healAction() {
    if (mana >= 10) {
        heal(maxHp / 3);
        mana -= 10;
    }
}

void Character::boostAction() {
    if (mana >= 15) {
        attackBoost = 30; // +30% к атаке
        defenseBoost = 15; // +15% к защите
        boostTurns = 3;
        mana -= 15;
    }
}
```

**Item.cpp**
```cpp
#include "Item.h"
#include <map>

Item::Item(std::string n, std::string d, int v, Rarity r, ItemType t, std::string c,
           int hp, int att, int def, int mn, int heal, int restore, int batt, int bdef, int dur, int lvl) :
    name(n), description(d), value(v), rarity(r), type(t), className(c),
    hpBonus(hp), attackBonus(att), defenseBonus(def), manaBonus(mn),
    healAmount(heal), manaRestore(restore), 
    boostAttack(batt), boostDefense(bdef), boostDuration(dur), levelRequirement(lvl) {}

std::string Item::getRarityName() const {
    static std::map<Rarity, std::string> names = {
        {COMMON, "Обычный"},
        {UNCOMMON, "Необычный"},
        {RARE, "Редкий"},
        {EPIC, "Эпический"},
        {LEGENDARY, "Легендарный"},
        {MYTHIC, "Мифический"}
    };
    return names[rarity];
}

std::string Item::getTypeName() const {
    static std::map<ItemType, std::string> names = {
        {WEAPON, "Оружие"},
        {ARMOR, "Броня"},
        {CONSUMABLE, "Расходник"},
        {SCROLL, "Свиток"}
    };
    return names[type];
}

bool Item::isEquippable() const {
    return type == WEAPON || type == ARMOR;
}

bool Item::isConsumable() const {
    return type == CONSUMABLE || type == SCROLL;
}
```
