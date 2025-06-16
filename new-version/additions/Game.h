#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include "Room.h"
#include <vector>
#include <string>

struct ClassInfo {
    std::string name;
    std::string description;
    int baseHp;
    int baseMana;
    int baseAttack;
    int baseDefense;
    int levelUpHpGain;
    int levelUpManaGain;
    int levelUpAttackGain;
    int levelUpDefenseGain;
};

Character createCharacter();
Room generateRoom(int level);
const std::vector<ClassInfo>& getClassOptions();
Item generateItemForClass(const std::string& className, Rarity rarity);

#endif


