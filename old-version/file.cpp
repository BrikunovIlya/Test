#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

int rollDice(int numDice, int sides) {
    int result = 0;
    for (int i = 0; i < numDice; ++i) {
        result += rand() % sides + 1;
    }
    return result;
}

enum class Rarity { Common, Uncommon, Rare, Legendary };

class Character;

class Spell {
public:
    string name;
    int manaCost, damage, duration, level;
    string effect;

    Spell(string n, int mc, int d, string e, int dur = 0, int lvl = 1)
        : name(n), manaCost(mc), damage(d), effect(e), duration(dur), level(lvl) {}

    void upgrade() {
        if (level < 3) {
            level++;
            damage += 5;
            manaCost += 5;
            if (duration > 0) duration++;
            cout << "| Заклинание " << name << " улучшено до уровня " << level << "!\n";
        } else {
            cout << "| Заклинание " << name << " достигло максимального уровня!\n";
        }
    }
};

class Ability {
public:
    string name;
    int staminaCost, manaCost, damage;
    string effect;
    int duration;

    Ability(string n, int sc, int mc, int d, string e, int dur = 0)
        : name(n), staminaCost(sc), manaCost(mc), damage(d), effect(e), duration(dur) {}
};

class Resource {
public:
    string name;
    int count;

    Resource(string n, int c = 0) : name(n), count(c) {}
};

class Item {
public:
    string name, type, classRestriction;
    int attackBonus, defenseBonus, manaBonus, staminaBonus, sanityBonus;
    int upgradeLevel, maxUpgradeLevel;
    Rarity rarity;

    Item() : name(""), type(""), classRestriction(""), attackBonus(0), defenseBonus(0), 
             manaBonus(0), staminaBonus(0), sanityBonus(0), upgradeLevel(0), maxUpgradeLevel(5), rarity(Rarity::Common) {}

    Item(string n, string t, string cr, int a, int d, int m, int s, int sb, int mul = 5, Rarity r = Rarity::Common)
        : name(n), type(t), classRestriction(cr), attackBonus(a), defenseBonus(d), manaBonus(m), 
          staminaBonus(s), sanityBonus(sb), upgradeLevel(0), maxUpgradeLevel(mul), rarity(r) {}

    string getRarityString() const {
        switch (rarity) {
            case Rarity::Common: return "Обычное";
            case Rarity::Uncommon: return "Необычное";
            case Rarity::Rare: return "Редкое";
            case Rarity::Legendary: return "Легендарное";
            default: return "Неизвестно";
        }
    }

    void displayInfo() const {
        cout << "| " << name << " +" << upgradeLevel << " (" << type << ", " << getRarityString();
        if (!classRestriction.empty()) cout << ", только для " << classRestriction;
        cout << ")\n";
        if (attackBonus > 0) cout << "|  Атака +" << attackBonus << "\n";
        if (defenseBonus > 0) cout << "|  Защита +" << defenseBonus << "\n";
        if (manaBonus > 0) cout << "|  Мана +" << manaBonus << "\n";
        if (staminaBonus > 0) cout << "|  Выносливость +" << staminaBonus << "\n";
        if (sanityBonus > 0) cout << "|  Рассудок +" << sanityBonus << "\n";
    }

    void upgrade(int attackBoost, int defenseBoost, int manaBoost, int staminaBoost, int sanityBoost) {
        if (upgradeLevel < maxUpgradeLevel) {
            attackBonus += attackBoost;
            defenseBonus += defenseBoost;
            manaBonus += manaBoost;
            staminaBonus += staminaBoost;
            sanityBonus += sanityBoost;
            upgradeLevel++;
            cout << "| " << name << " улучшен до +" << upgradeLevel << "!\n";
        } else {
            cout << "| " << name << " полностью улучшен!\n";
        }
    }
};

class Quest {
public:
    string name, description;
    int soulsReward;
    Item itemReward;
    bool completed;

    Quest(string n, string d, int sr, Item ir = Item())
        : name(n), description(d), soulsReward(sr), itemReward(ir), completed(false) {}
};

class Trader {
public:
    vector<Item> stock;

    Trader() {
        stock.push_back(Item("Зелье здоровья", "Расходуемое", "", 0, 0, 0, 0, 0, 0, Rarity::Common));
        stock.push_back(Item("Фляга Эстуса", "Расходуемое", "", 0, 0, 0, 10, 0, 0, Rarity::Uncommon));
        stock.push_back(Item("Кольцо надежды", "Кольцо", "", 0, 0, 0, 0, 5, 0, Rarity::Rare));
    }

    void trade(Character& player);
};

class Blacksmith {
public:
    void upgradeItem(Character& player, Item& item);
    void upgradeSpell(Character& player, Spell& spell);
};

class Character {
public:
    string name, className;
    int level, souls, reputation, killCount, sanity;
    int health, maxHealth, stamina, maxStamina, attack, baseAttack, defense, baseDefense, mana, maxMana;
    int vitality, endurance, strength, dexterity, intelligence;
    vector<Item> inventory;
    Item equippedWeapon, equippedArmor, equippedAmulet, equippedRing;
    vector<Ability> abilities;
    vector<Spell> spells;
    vector<Quest> activeQuests;
    vector<Resource> resources;
    map<string, int> activeEffects;
    const int maxInventorySize = 8;

    Character(string n, string c) : name(n), className(c), level(1), souls(0), reputation(0), killCount(0), sanity(100),
        health(0), maxHealth(0), stamina(0), maxStamina(0), attack(0), baseAttack(0), defense(0), baseDefense(0),
        mana(0), maxMana(0), vitality(0), endurance(0), strength(0), dexterity(0), intelligence(0),
        equippedWeapon(), equippedArmor(), equippedAmulet(), equippedRing() {
        initializeStats();
        initializeEquipment();
        initializeAbilities();
        initializeSpells();
    }

    void initializeStats() {
        if (className == "Рыцарь") { vitality = 12; endurance = 11; strength = 13; dexterity = 9; intelligence = 7; }
        else if (className == "Маг") { vitality = 8; endurance = 8; strength = 7; dexterity = 9; intelligence = 14; }
        else if (className == "Клирик") { vitality = 10; endurance = 9; strength = 9; dexterity = 8; intelligence = 12; }
        else if (className == "Вор") { vitality = 9; endurance = 10; strength = 8; dexterity = 14; intelligence = 8; }
        else if (className == "Охотник") { vitality = 10; endurance = 12; strength = 10; dexterity = 12; intelligence = 7; }
        else if (className == "Пиротехник") { vitality = 9; endurance = 10; strength = 9; dexterity = 10; intelligence = 12; }
        else if (className == "Воин") { vitality = 11; endurance = 12; strength = 14; dexterity = 8; intelligence = 6; }
        else if (className == "Лишенный") { vitality = 10; endurance = 10; strength = 10; dexterity = 10; intelligence = 10; }
        else if (className == "Паладин") { vitality = 11; endurance = 10; strength = 11; dexterity = 8; intelligence = 11; }
        updateStats();
        health = maxHealth;
        stamina = maxStamina;
        mana = maxMana;
        sanity = 100;
    }

    void initializeEquipment() {
        if (className == "Рыцарь") { equippedWeapon = Item("Длинный меч", "Оружие", "Рыцарь", 5, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Доспех рыцаря", "Броня", "Рыцарь", 0, 4, 0, 0, 0, 0, Rarity::Common); }
        else if (className == "Маг") { equippedWeapon = Item("Катализатор", "Оружие", "Маг", 2, 0, 10, 0, 0, 0, Rarity::Common); equippedArmor = Item("Роба", "Броня", "", 0, 1, 0, 0, 0, 0, Rarity::Common); }
        else if (className == "Клирик") { equippedWeapon = Item("Булава", "Оружие", "Клирик", 4, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Одеяние клирика", "Броня", "Клирик", 0, 2, 5, 0, 0, 0, Rarity::Common); }
        else if (className == "Вор") { equippedWeapon = Item("Кинжал", "Оружие", "Вор", 3, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Кожаная броня", "Броня", "", 0, 2, 0, 5, 0, 0, Rarity::Common); }
        else if (className == "Охотник") { equippedWeapon = Item("Короткий лук", "Оружие", "Охотник", 4, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Одежда охотника", "Броня", "Охотник", 0, 2, 0, 5, 0, 0, Rarity::Common); }
        else if (className == "Пиротехник") { equippedWeapon = Item("Пламя пиромантии", "Оружие", "Пиротехник", 3, 0, 5, 0, 0, 0, Rarity::Common); equippedArmor = Item("Рваный плащ", "Броня", "", 0, 1, 0, 0, 0, 0, Rarity::Common); }
        else if (className == "Воин") { equippedWeapon = Item("Боевой топор", "Оружие", "Воин", 6, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Кольчуга", "Броня", "", 0, 3, 0, 0, 0, 0, Rarity::Common); }
        else if (className == "Лишенный") { equippedWeapon = Item("Дубина", "Оружие", "", 3, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Лохмотья", "Броня", "", 0, 1, 0, 0, 0, 0, Rarity::Common); }
        else if (className == "Паладин") { equippedWeapon = Item("Священный меч", "Оружие", "Паладин", 4, 0, 0, 0, 0, 0, Rarity::Common); equippedArmor = Item("Святая броня", "Броня", "Паладин", 0, 3, 5, 0, 0, 0, Rarity::Common); }
    }

    void initializeAbilities() {
        if (className == "Рыцарь") abilities.push_back(Ability("Удар щитом", 15, 0, 5, "Оглушение", 1));
        else if (className == "Маг") abilities.push_back(Ability("Стрела души", 5, 15, 10 + intelligence / 5, "Урон"));
        else if (className == "Клирик") abilities.push_back(Ability("Исцеление", 10, 15, 15, "Восстановление HP"));
        else if (className == "Вор") abilities.push_back(Ability("Удар в спину", 20, 0, 15, "Критический"));
        else if (className == "Охотник") abilities.push_back(Ability("Прицельный выстрел", 15, 0, 10, "Пробитие"));
        else if (className == "Пиротехник") abilities.push_back(Ability("Огненный шар", 10, 10, 12, "Горение", 2));
        else if (className == "Воин") abilities.push_back(Ability("Рассечение", 20, 0, 10, "Урон по площади"));
        else if (className == "Лишенный") abilities.push_back(Ability("Отчаянный удар", 15, 0, 8, "Урон"));
        else if (className == "Паладин") abilities.push_back(Ability("Кара", 15, 10, 12, "Святой урон"));
    }

    void initializeSpells() {
        if (className == "Маг" || className == "Пиротехник" || className == "Клирик") {
            spells.push_back(Spell("Огненная стрела", 10, 10 + intelligence / 5, "Горение", 2));
            spells.push_back(Spell("Ледяной шип", 15, 12 + intelligence / 5, "Заморозка", 2));
        }
        if (className == "Паладин" || className == "Клирик") {
            spells.push_back(Spell("Свет очищения", 20, 15, "Святой урон"));
        }
    }

    void updateStats() {
        maxHealth = 50 + vitality * 10 + equippedAmulet.defenseBonus + equippedRing.defenseBonus;
        if (activeEffects.count("Проклятие")) maxHealth *= 0.8;
        health = min(maxHealth, health);
        maxStamina = 50 + endurance * 5 + equippedArmor.staminaBonus + equippedAmulet.staminaBonus + equippedRing.staminaBonus;
        stamina = min(maxStamina, stamina);
        baseAttack = strength / 2;
        attack = baseAttack + equippedWeapon.attackBonus + equippedAmulet.attackBonus + equippedRing.attackBonus;
        baseDefense = 2 + (endurance + vitality) / 10;
        defense = baseDefense + equippedArmor.defenseBonus + equippedAmulet.defenseBonus + equippedRing.defenseBonus;
        maxMana = 20 + intelligence * 5 + equippedWeapon.manaBonus + equippedArmor.manaBonus + equippedAmulet.manaBonus + equippedRing.manaBonus;
        mana = min(maxMana, mana);
        updateAbilities();
    }

    void updateAbilities() {
        for (auto& ability : abilities) {
            if (ability.name == "Стрела души") ability.damage = 10 + intelligence / 5;
        }
    }

    void displayStats() const {
        cout << "\n╔════════════════════════════╗\n";
        cout << "║ Избранный: " << name << string(16 - name.length(), ' ') << "║\n";
        cout << "╠════════════════════════════╣\n";
        cout << "║ Класс: " << className << string(20 - className.length(), ' ') << "║\n";
        cout << "║ Уровень: " << level << string(18 - to_string(level).length(), ' ') << "║\n";
        cout << "║ Души: " << souls << string(21 - to_string(souls).length(), ' ') << "║\n";
        cout << "║ Репутация: " << reputation << string(16 - to_string(reputation).length(), ' ') << "║\n";
        cout << "║ Здоровье: " << health << "/" << maxHealth << string(17 - (to_string(health).length() + to_string(maxHealth).length() + 1), ' ') << "║\n";
        cout << "║ Выносливость: " << stamina << "/" << maxStamina << string(13 - (to_string(stamina).length() + to_string(maxStamina).length() + 1), ' ') << "║\n";
        cout << "║ Мана: " << mana << "/" << maxMana << string(21 - (to_string(mana).length() + to_string(maxMana).length() + 1), ' ') << "║\n";
        cout << "║ Рассудок: " << sanity << "/100" << string(17 - (to_string(sanity).length() + 4), ' ') << "║\n";
        cout << "║ Атака: " << attack << string(20 - to_string(attack).length(), ' ') << "║\n";
        cout << "║ Защита: " << defense << string(19 - to_string(defense).length(), ' ') << "║\n";
        cout << "║ Живучесть: " << vitality << string(16 - to_string(vitality).length(), ' ') << "║\n";
        cout << "║ Выносливость: " << endurance << string(13 - to_string(endurance).length(), ' ') << "║\n";
        cout << "║ Сила: " << strength << string(21 - to_string(strength).length(), ' ') << "║\n";
        cout << "║ Ловкость: " << dexterity << string(17 - to_string(dexterity).length(), ' ') << "║\n";
        cout << "║ Интеллект: " << intelligence << string(16 - to_string(intelligence).length(), ' ') << "║\n";
        cout << "╠════════════════════════════╣\n";
        cout << "║ Экипировка:               ║\n";
        equippedWeapon.displayInfo();
        equippedArmor.displayInfo();
        equippedAmulet.displayInfo();
        equippedRing.displayInfo();
        cout << "╠════════════════════════════╣\n";
        cout << "║ Заклинания:               ║\n";
        if (spells.empty()) cout << "║  Нет заклинаний          ║\n";
        else for (const auto& spell : spells) {
            cout << "║  " << spell.name << " (ур. " << spell.level << ")" << string(20 - (spell.name.length() + to_string(spell.level).length() + 5), ' ') << "║\n";
        }
        cout << "╠════════════════════════════╣\n";
        cout << "║ Инвентарь:                ║\n";
        if (inventory.empty()) cout << "║  Пусто                   ║\n";
        else for (const auto& item : inventory) cout << "║  " << item.name << string(25 - item.name.length(), ' ') << "║\n";
        cout << "╠════════════════════════════╣\n";
        cout << "║ Ресурсы:                  ║\n";
        for (const auto& res : resources) cout << "║  " << res.name << ": " << res.count << string(23 - (res.name.length() + to_string(res.count).length() + 2), ' ') << "║\n";
        cout << "╠════════════════════════════╣\n";
        cout << "║ Эффекты:                  ║\n";
        if (activeEffects.empty()) cout << "║  Нет                     ║\n";
        else for (const auto& effect : activeEffects) cout << "║  " << effect.first << "(" << effect.second << ")" << string(23 - (effect.first.length() + to_string(effect.second).length() + 2), ' ') << "║\n";
        cout << "╚════════════════════════════╝\n";
    }

    bool isAlive() const { return health > 0; }

    void takeDamage(int damage) {
        int actualDamage = max(0, damage - defense);
        if (activeEffects.count("Блок")) actualDamage /= 2;
        health = max(0, health - actualDamage);
        if (!isAlive()) souls /= 2;
    }

    void loseSanity(int amount) {
        sanity = max(0, sanity - amount);
        if (sanity <= 50 && !activeEffects.count("Страх")) {
            activeEffects["Страх"] = 5;
            cout << "| Тьма шепчет в твоём разуме, навевая страх...\n";
        }
        if (sanity <= 20 && !activeEffects.count("Безумие")) {
            activeEffects["Безумие"] = 10;
            cout << "| Безумие охватывает тебя, твои действия дрожат...\n";
        }
        if (sanity <= 0) {
            cout << "| Твой разум поглощён бездной, ты теряешь контроль...\n";
            health = 0;
        }
    }

    void gainSanity(int amount) {
        sanity = min(100, sanity + amount);
        if (sanity > 50) activeEffects.erase("Страх");
        if (sanity > 20) activeEffects.erase("Безумие");
    }

    int calculateDamage() {
        if (stamina < 10) return 0;
        stamina -= 10;
        int damage = rollDice(1, 6) + attack;
        if (activeEffects.count("Благословение")) damage *= 1.2;
        if (activeEffects.count("Слепота")) damage /= 2;
        if (activeEffects.count("Страх")) damage -= 2;
        if (activeEffects.count("Безумие")) damage = rollDice(1, damage);
        return damage;
    }

    void gainSouls(int amount) { souls += amount; cout << "| Получено " << amount << " душ!\n"; }

    void gainReputation(int amount) { 
        reputation = max(-100, min(100, reputation + amount)); 
    }

    void addResource(string name, int count) {
        for (auto& res : resources) {
            if (res.name == name) { res.count += count; return; }
        }
        resources.push_back(Resource(name, count));
    }

    void levelUp() {
        cout << "\n=== Улучшение ===\n";
        displayStats();
        int cost = getLevelUpCost();
        cout << "| Требуется душ: " << cost << "\n";
        cout << "| 1. Живучесть | 2. Выносливость | 3. Сила | 4. Ловкость | 5. Интеллект | 0. Выход\nВыбор: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "| Неверный ввод! Попробуй снова.\n";
            return;
        }
        if (souls < cost) { 
            cout << "| Недостаточно душ!\n"; 
            return; 
        }
        switch (choice) {
            case 1: vitality++; break;
            case 2: endurance++; break;
            case 3: strength++; break;
            case 4: dexterity++; break;
            case 5: intelligence++; break;
            case 0: return;
            default: cout << "| Неверный выбор!\n"; return;
        }
        souls -= cost;
        level++;
        updateStats();
        cout << "| Характеристика увеличена! Уровень: " << level << "\n";
    }

    int getLevelUpCost() const { 
        return level * 100 + 100; 
    }

    void addItemToInventory(const Item& item) {
        if (item.classRestriction.empty() || item.classRestriction == className) {
            if (inventory.size() < maxInventorySize) {
                inventory.push_back(item);
                cout << "| Получено: " << item.name << " (" << item.getRarityString() << ")\n";
            } else {
                cout << "| Инвентарь полон! Выбросить предмет (0 для отмены, 1-" << inventory.size() << "): ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice > 0 && choice <= inventory.size()) {
                    cout << "| Выброшено: " << inventory[choice - 1].name << "\n";
                    inventory[choice - 1] = item;
                    cout << "| Получено: " << item.name << " (" << item.getRarityString() << ")\n";
                }
            }
        } else cout << "| Нельзя использовать " << item.name << " - ограничение по классу!\n";
    }

    void equipItem(const Item& item) {
        if (!item.classRestriction.empty() && item.classRestriction != className) {
            cout << "| Нельзя надеть " << item.name << " - не тот класс!\n";
            return;
        }
        if (item.type == "Оружие") equippedWeapon = item;
        else if (item.type == "Броня") equippedArmor = item;
        else if (item.type == "Амулет") equippedAmulet = item;
        else if (item.type == "Кольцо") equippedRing = item;
        updateStats();
        cout << "| Надето: " << item.name << "\n";
    }

    bool useAbility(int index, Character& target) {
        if (index < 0 || index >= abilities.size()) return false;
        Ability& ability = abilities[index];
        if (stamina < ability.staminaCost || mana < ability.manaCost) {
            cout << "| Недостаточно выносливости или маны!\n";
            return false;
        }
        stamina -= ability.staminaCost;
        mana -= ability.manaCost;
        if (ability.effect == "Урон" || ability.effect == "Святой урон") {
            target.takeDamage(ability.damage);
            cout << "| " << name << " использует " << ability.name << " и наносит " << ability.damage << " урона!\n";
        } else if (ability.effect == "Восстановление HP") {
            heal(ability.damage);
            cout << "| " << name << " использует " << ability.name << " и восстанавливает " << ability.damage << " HP!\n";
        } else if (ability.effect == "Оглушение") {
            target.activeEffects["Оглушение"] = ability.duration;
            target.takeDamage(ability.damage);
            cout << "| " << name << " оглушает " << target.name << "!\n";
        } else if (ability.effect == "Критический") {
            int critDamage = ability.damage * 2;
            target.takeDamage(critDamage);
            cout << "| " << name << " наносит критический удар на " << critDamage << " урона!\n";
        } else if (ability.effect == "Пробитие") {
            int pierceDamage = ability.damage + target.defense;
            target.takeDamage(pierceDamage);
            cout << "| " << name << " пробивает броню на " << pierceDamage << " урона!\n";
        } else if (ability.effect == "Горение") {
            target.activeEffects["Горение"] = ability.duration;
            target.takeDamage(ability.damage);
            cout << "| " << name << " поджигает " << target.name << " на " << ability.damage << " урона!\n";
        } else if (ability.effect == "Урон по площади") {
            target.takeDamage(ability.damage);
            cout << "| " << name << " размахивает оружием, нанося " << ability.damage << " урона!\n";
        }
        return true;
    }

    bool castSpell(int index, Character& target) {
        if (index < 0 || index >= spells.size()) return false;
        Spell& spell = spells[index];
        if (mana < spell.manaCost) {
            cout << "| Недостаточно маны для заклинания " << spell.name << "!\n";
            return false;
        }
        mana -= spell.manaCost;
        int scaledDamage = spell.damage + (spell.level - 1) * 5;
        if (spell.effect == "Горение") {
            target.activeEffects["Горение"] = spell.duration;
            target.takeDamage(scaledDamage);
            cout << "| " << name << " вызывает " << spell.name << ", поджигая " << target.name << " на " << scaledDamage << " урона!\n";
        } else if (spell.effect == "Заморозка") {
            target.activeEffects["Заморозка"] = spell.duration;
            target.takeDamage(scaledDamage);
            cout << "| " << name << " вызывает " << spell.name << ", замораживая " << target.name << " на " << scaledDamage << " урона!\n";
        } else if (spell.effect == "Святой урон") {
            target.takeDamage(scaledDamage);
            cout << "| " << name << " вызывает " << spell.name << ", нанося " << scaledDamage << " святого урона!\n";
        }
        return true;
    }

    bool useItem(int index) {
        if (index < 0 || index >= inventory.size()) return false;
        Item& item = inventory[index];
        if (item.type != "Расходуемое") {
            cout << "| Нельзя использовать " << item.name << " в бою!\n";
            return false;
        }
        if (item.name == "Зелье здоровья") {
            heal(20);
            cout << "| Использовано Зелье здоровья, восстановлено 20 HP!\n";
        } else if (item.name == "Фляга Эстуса") {
            stamina = min(maxStamina, stamina + 20);
            cout << "| Использована Фляга Эстуса, восстановлено 20 выносливости!\n";
        }
        inventory.erase(inventory.begin() + index);
        return true;
    }

    bool dodge() { 
        return rollDice(1, 20) + dexterity / 5 > 15 && stamina >= 15; 
    }

    bool block() {
        if (stamina >= 15) { 
            stamina -= 15; 
            activeEffects["Блок"] = 1; 
            return true; 
        }
        return false;
    }

    bool counterAttack(Character& target) {
        if (stamina >= 20 && activeEffects.count("Блок")) {
            stamina -= 20;
            int damage = rollDice(1, 8) + attack;
            target.takeDamage(damage);
            cout << "| Контратака наносит " << damage << " урона!\n";
            return true;
        }
        return false;
    }

    void heal(int amount) { 
        health = min(maxHealth, health + amount); 
    }

    void rest() { 
        health = maxHealth; 
        stamina = maxStamina; 
        mana = maxMana; 
        gainSanity(20); 
        cout << "| Ты отдыхаешь у света костра, восстанавливая силы и разум...\n"; 
    }

    void updateEffects() {
        for (auto it = activeEffects.begin(); it != activeEffects.end();) {
            if (it->first == "Горение") takeDamage(3);
            else if (it->first == "Яд") takeDamage(5);
            else if (it->first == "Заморозка") stamina = max(0, stamina - 5);
            it->second--;
            if (it->second <= 0) it = activeEffects.erase(it);
            else ++it;
        }
    }

    void addQuest(const Quest& quest) { 
        activeQuests.push_back(quest); 
        cout << "| Новый квест: " << quest.name << " - " << quest.description << "\n"; 
    }

    void completeQuest(int index) {
        if (index >= 0 && index < activeQuests.size() && !activeQuests[index].completed) {
            activeQuests[index].completed = true;
            gainSouls(activeQuests[index].soulsReward);
            if (activeQuests[index].itemReward.name != "") addItemToInventory(activeQuests[index].itemReward);
            cout << "| Квест выполнен: " << activeQuests[index].name << "\n";
        }
    }
};

void Trader::trade(Character& player) {
    cout << "\n=== Скрюченный торговец из бездны ===\n";
    cout << "| Твои души: " << player.souls << "\n";
    cout << "| Товары:\n";
    for (size_t i = 0; i < stock.size(); i++) {
        cout << "| " << i + 1 << ". " << stock[i].name << " - " << (i + 1) * 50 << " душ (" << stock[i].getRarityString() << ")\n";
    }
    cout << "| 0. Уйти\nВыбор: ";
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "| Неверный ввод! Попробуй снова.\n";
        return;
    }
    if (choice > 0 && choice <= stock.size()) {
        int cost = choice * 50;
        if (player.souls >= cost) {
            player.souls -= cost;
            player.addItemToInventory(stock[choice - 1]);
        } else {
            cout << "| Недостаточно душ, смертный...\n";
        }
    }
}

void Blacksmith::upgradeItem(Character& player, Item& item) {
    int soulCost = (item.upgradeLevel + 1) * 100;
    int titaniteCost = item.upgradeLevel + 1;
    bool hasTitanite = false;
    for (auto& res : player.resources) {
        if (res.name == "Титанит" && res.count >= titaniteCost) {
            hasTitanite = true;
            res.count -= titaniteCost;
            break;
        }
    }
    if (player.souls >= soulCost && hasTitanite) {
        player.souls -= soulCost;
        item.upgrade(2, 1, 2, 2, item.rarity == Rarity::Rare ? 2 : 0);
        cout << "| Кузнец шепчет слова из бездны, улучшая предмет...\n";
    } else {
        cout << "| Нужно " << soulCost << " душ и " << titaniteCost << " титанита, смертный!\n";
    }
}

void Blacksmith::upgradeSpell(Character& player, Spell& spell) {
    int soulCost = spell.level * 150;
    int crystalCost = spell.level;
    bool hasCrystals = false;
    for (auto& res : player.resources) {
        if (res.name == "Кристаллы" && res.count >= crystalCost) {
            hasCrystals = true;
            res.count -= crystalCost;
            break;
        }
    }
    if (player.souls >= soulCost && hasCrystals) {
        player.souls -= soulCost;
        spell.upgrade();
    } else {
        cout << "| Нужно " << soulCost << " душ и " << crystalCost << " кристаллов для улучшения заклинания!\n";
    }
}

Item generateUniqueItem(string className) {
    vector<Item> items = {
        Item("Цвайхендер", "Оружие", "Рыцарь", 8, 0, 0, 0, 0, 0, Rarity::Uncommon),
        Item("Посох лунного света", "Оружие", "Маг", 3, 0, 15, 0, 0, 0, Rarity::Rare),
        Item("Талисман света", "Оружие", "Клирик", 2, 0, 10, 0, 5, 0, Rarity::Uncommon),
        Item("Клинок разбойника", "Оружие", "Вор", 5, 0, 0, 5, 0, 0, Rarity::Uncommon),
        Item("Чёрный лук", "Оружие", "Охотник", 6, 0, 0, 5, 0, 0, Rarity::Rare),
        Item("Хаотическое пламя", "Оружие", "Пиротехник", 4, 0, 10, 0, 0, 0, Rarity::Rare),
        Item("Большой меч", "Оружие", "Воин", 10, 0, 0, 0, 0, 0, Rarity::Uncommon),
        Item("Старое копьё", "Оружие", "", 5, 0, 0, 0, 0, 0, Rarity::Common),
        Item("Щит солнечного света", "Броня", "Паладин", 0, 6, 5, 0, 5, 0, Rarity::Rare),
        Item("Амулет бездны", "Амулет", "", 0, 0, 10, 0, -5, 0, Rarity::Rare),
        Item("Кольцо вечного шепота", "Кольцо", "", 2, 2, 0, 0, 10, 0, Rarity::Legendary),
        Item("Плащ теней", "Броня", "", 0, 3, 0, 5, 0, 0, Rarity::Uncommon),
        Item("Клинок безымянного", "Оружие", "", 12, 0, 0, 0, -10, 0, Rarity::Legendary),
        Item("Ледяной доспех", "Броня", "", 0, 5, 0, 0, 5, 0, Rarity::Rare),
        Item("Амулет звездочёта", "Амулет", "", 0, 0, 15, 0, 5, 0, Rarity::Legendary),
        Item("Клинок Жёлтого Знака", "Оружие", "", 10, 0, 0, 0, -5, 0, Rarity::Legendary),
        Item("Плащ Каркозы", "Броня", "", 0, 4, 0, 5, 5, 0, Rarity::Rare),
        Item("Кольцо теней Хаста", "Кольцо", "", 3, 0, 5, 0, 5, 0, Rarity::Rare),
        Item("Амулет безумного зова", "Амулет", "", 0, 0, 10, 0, -10, 0, Rarity::Legendary),
        Item("Посох древних шепотов", "Оружие", "Маг", 4, 0, 20, 0, 0, 0, Rarity::Legendary),
        Item("Щит звездного света", "Броня", "Паладин", 0, 7, 0, 0, 10, 0, Rarity::Legendary),
        Item("Кинжал призрачной тени", "Оружие", "Вор", 6, 0, 0, 5, 0, 0, Rarity::Rare),
        Item("Кольцо огненной души", "Кольцо", "Пиротехник", 2, 0, 10, 0, 0, 0, Rarity::Rare),
        Item("Доспехи безмолвия", "Броня", "", 0, 6, 0, 0, 5, 0, Rarity::Rare),
        Item("Лук звёздного ветра", "Оружие", "Охотник", 8, 0, 0, 5, 0, 0, Rarity::Legendary),
        Item("Амулет шепчущего камня", "Амулет", "", 0, 0, 5, 0, 10, 0, Rarity::Rare),
        Item("Кольцо Жёлтого Короля", "Кольцо", "", 0, 3, 0, 0, 5, 0, Rarity::Legendary),
        Item("Плащ беззвёздной ночи", "Броня", "", 0, 4, 0, 5, -5, 0, Rarity::Rare),
        Item("Клинок звёздного разлома", "Оружие", "", 9, 0, 0, 0, 0, 0, Rarity::Rare),
        Item("Щит Хаста", "Броня", "", 0, 8, 0, 0, -10, 0, Rarity::Legendary),
        Item("Посох призыва теней", "Оружие", "Маг", 5, 0, 15, 0, -5, 0, Rarity::Rare),
        Item("Амулет вечного света", "Амулет", "", 0, 0, 10, 0, 15, 0, Rarity::Legendary),
        Item("Кольцо пепла и звёзд", "Кольцо", "", 2, 2, 5, 0, 0, 0, Rarity::Rare),
        Item("Доспехи Жёлтого Знака", "Броня", "", 0, 7, 0, 0, -5, 0, Rarity::Legendary),
        Item("Кинжал беззвучного крика", "Оружие", "Вор", 7, 0, 0, 5, 0, 0, Rarity::Rare)
    };
    return items[rand() % items.size()];
}

Character generateMonster(int playerLevel, int killCount) {
    vector<pair<string, int>> monsters = {
        {"Пустой", 1}, {"Скелет", 2}, {"Рыцарь", 4}, {"Демон", 6}, {"Тень", 3}, {"Призрак", 5},
        {"Чудовище глубин", 7}, {"Шептун бездны", 4}, {"Щупальце хаоса", 6}, {"Слуга Хаста", 5}
    };
    auto monster = monsters[rand() % monsters.size()];
    int level = monster.second + rollDice(1, 3) - 1;
    Character enemy(monster.first, "Враг");
    enemy.vitality = 8 + level;
    enemy.endurance = 8 + level;
    enemy.strength = 10 + level * 2;
    enemy.dexterity = 5 + level;
    if (monster.first == "Чудовище глубин") enemy.abilities.push_back(Ability("Рёв бездны", 10, 0, 0, "Страх", 3));
    if (monster.first == "Шептун бездны") enemy.abilities.push_back(Ability("Шёпот безумия", 5, 0, 0, "Безумие", 5));
    if (monster.first == "Щупальце хаоса") enemy.abilities.push_back(Ability("Хватка хаоса", 15, 0, 10, "Оглушение", 2));
    if (monster.first == "Слуга Хаста") enemy.abilities.push_back(Ability("Жёлтый взгляд", 10, 0, 5, "Безумие", 3));
    if (killCount > 10) {
        enemy.vitality += 5;
        enemy.strength += 5;
        enemy.name = "Древний " + enemy.name;
        enemy.souls += 50;
    }
    enemy.updateStats();
    enemy.gainSouls(level * 50);
    return enemy;
}

Character generateBoss(int playerLevel) {
    Character boss("Хастур, Жёлтый Король", "Босс");
    boss.vitality = 20 + playerLevel * 3;
    boss.endurance = 20 + playerLevel;
    boss.strength = 25 + playerLevel * 4;
    boss.dexterity = 15 + playerLevel;
    boss.abilities.push_back(Ability("Взгляд Жёлтого Знака", 15, 0, 20, "Безумие", 5));
    boss.abilities.push_back(Ability("Шёпот Каркозы", 10, 0, 15, "Страх", 3));
    boss.updateStats();
    boss.gainSouls(playerLevel * 300);
    return boss;
}

void battle(Character& player, Character& enemy);
void levelUpMenu(Character& player);
void enemyTurn(Character& enemy, Character& player);

class Room {
public:
    string type, environment, description;
    Character enemy;
    Item treasure;
    bool isLight;

    Room() : type(""), environment(""), description(""), enemy("", ""), treasure(), isLight(false) {}

    Room(string t, string env, string desc, Character e = Character("", ""), Item i = Item(), bool light = false)
        : type(t), environment(env), description(desc), enemy(e), treasure(i), isLight(light) {}

    void enterRoom(Character& player) {
        cout << "\n" << string(50, '=') << "\n";
        cout << description << "\n";
        if (!environment.empty()) {
            cout << "| Окружение: " << environment << "\n";
            if (environment == "Туман") player.activeEffects["Слепота"] = 3;
            else if (environment == "Яд") player.activeEffects["Яд"] = 5;
            else if (environment == "Тьма") player.activeEffects["Страх"] = 2;
            else if (environment == "Лед") player.activeEffects["Заморозка"] = 2;
            else if (environment == "Лава") player.activeEffects["Горение"] = 3;
            else if (environment == "Эхо бездны") player.loseSanity(5);
        }
        if (!isLight) {
            player.loseSanity(10);
            cout << "| Тьма пожирает твой разум... (-10 рассудка)\n";
        } else {
            player.gainSanity(5);
            cout << "| Свет успокаивает твою душу... (+5 рассудка)\n";
        }
        cout << string(50, '-') << "\n";

        if (type == "Монстр" || type == "Босс") battle(player, enemy);
        else if (type == "Сокровище") player.addItemToInventory(treasure);
        else if (type == "Костёр") { 
            player.rest(); 
            levelUpMenu(player); 
        } else if (type == "Торговец") { 
            Trader trader; 
            trader.trade(player); 
        } else if (type == "Кузнец") {
            Blacksmith smith;
            cout << "| Суровый кузнец из бездны предлагает свои услуги.\n";
            cout << "| 1. Улучшить оружие | 2. Улучшить броню | 3. Улучшить амулет | 4. Улучшить кольцо | 5. Улучшить заклинание | 0. Уйти\nВыбор: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                return;
            }
            if (choice == 1) smith.upgradeItem(player, player.equippedWeapon);
            else if (choice == 2) smith.upgradeItem(player, player.equippedArmor);
            else if (choice == 3) smith.upgradeItem(player, player.equippedAmulet);
            else if (choice == 4) smith.upgradeItem(player, player.equippedRing);
            else if (choice == 5 && !player.spells.empty()) {
                cout << "| Заклинания:\n";
                for (size_t i = 0; i < player.spells.size(); i++) {
                    cout << "| " << i + 1 << ". " << player.spells[i].name << " (ур. " << player.spells[i].level << ")\n";
                }
                cout << "| Выбери заклинание: ";
                int spellChoice;
                if (!(cin >> spellChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (spellChoice > 0 && spellChoice <= player.spells.size()) {
                    smith.upgradeSpell(player, player.spells[spellChoice - 1]);
                }
            }
        } else if (type == "Святилище") {
            cout << "| Древнее святилище источает нечеловеческий свет...\n";
            cout << "| 1. Помолиться | 0. Пройти мимо\nВыбор: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                return;
            }
            if (choice == 1) {
                int roll = rollDice(1, 3);
                if (roll == 1) {
                    player.activeEffects["Проклятие"] = 5;
                    cout << "| Тёмное проклятие проникает в твою душу!\n";
                } else if (roll == 2) {
                    player.activeEffects["Благословение"] = 5;
                    cout << "| Неземной свет наделяет тебя силой!\n";
                } else {
                    player.gainSouls(50);
                    cout << "| Ты находишь 50 душ под алтарём!\n";
                }
            }
        } else if (type == "Событие") {
            int event = rollDice(1, 6);
            if (event == 1) {
                cout << "| Раненый пилигрим молит о помощи.\n";
                cout << "| 1. Помочь (50 душ) | 2. Убить | 0. Пройти мимо\nВыбор: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice == 1 && player.souls >= 50) {
                    player.souls -= 50;
                    player.gainReputation(20);
                    player.addItemToInventory(Item("Кольцо пилигрима", "Кольцо", "", 0, 0, 0, 0, 5, 0, Rarity::Uncommon));
                    cout << "| Пилигрим благодарит тебя и дарит кольцо.\n";
                } else if (choice == 2) {
                    player.gainSouls(100);
                    player.gainReputation(-30);
                    cout << "| Ты забираешь души пилигрима...\n";
                }
            } else if (event == 2) {
                cout << "| Ты находишь алтарь с шепчущими голосами.\n";
                cout << "| 1. Принести жертву (20 HP) | 0. Уйти\nВыбор: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice == 1 && player.health > 20) {
                    player.takeDamage(20);
                    player.gainSouls(150);
                    cout << "| Твоя кровь питает алтарь, и ты получаешь 150 душ!\n";
                } else if (choice == 1) {
                    cout << "| У тебя недостаточно здоровья для жертвы!\n";
                }
            } else if (event == 3) {
                cout << "| Перед тобой ловушка с щупальцами!\n";
                int dodgeRoll = rollDice(1, 20) + player.dexterity / 5;
                if (dodgeRoll > 15) {
                    cout << "| Ты ловко уклоняешься от щупалец!\n";
                } else {
                    player.takeDamage(10);
                    player.loseSanity(5);
                    cout << "| Щупальца ранят тебя на 10 урона и терзают разум (-5 рассудка)!\n";
                }
            } else if (event == 4) {
                cout << "| Безумный старец предлагает загадку из глубин.\n";
                cout << "| 1. Ответить (Интеллект > 10) | 0. Отказаться\nВыбор: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice == 1 && player.intelligence > 10) {
                    player.gainSouls(80);
                    cout << "| Ты правильно ответил и получил 80 душ!\n";
                } else if (choice == 1) {
                    player.loseSanity(10);
                    cout << "| Ответ неверный, голоса бездны терзают твой разум (-10 рассудка)!\n";
                }
            } else if (event == 5) {
                cout << "| Ты находишь сундук, источающий тьму!\n";
                cout << "| 1. Открыть | 0. Уйти\nВыбор: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice == 1) {
                    if (rollDice(1, 2) == 1) {
                        player.addItemToInventory(generateUniqueItem(player.className));
                        cout << "| Ты находишь ценный предмет!\n";
                    } else {
                        player.activeEffects["Проклятие"] = 3;
                        player.loseSanity(15);
                        cout << "| Тьма из сундука проклинает тебя и пожирает разум (-15 рассудка)!\n";
                    }
                }
            } else {
                cout << "| Шепчущий идол смотрит на тебя...\n";
                cout << "| 1. Прикоснуться | 0. Уйти\nВыбор: ";
                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    return;
                }
                if (choice == 1) {
                    player.loseSanity(20);
                    player.gainSouls(200);
                    cout << "| Ты слышишь голоса из бездны, теряя 20 рассудка, но получаешь 200 душ!\n";
                }
            }
        } else if (type == "Квест") {
            vector<Quest> quests = {
                Quest("Убить Хаста", "Победить Жёлтого Короля Хаста", 1000, Item("Кольцо Жёлтого Короля", "Кольцо", "", 0, 3, 0, 0, 5, 0, Rarity::Legendary)),
                Quest("Очистить глубины", "Убить 5 слуг Хаста", 300, Item("Кольцо защиты", "Кольцо", "", 0, 2, 0, 0, 5, 0, Rarity::Rare)),
                Quest("Собрать звёзды", "Найти Амулет звездочёта", 400, Item("Амулет звездочёта", "Амулет", "", 0, 0, 15, 0, 5, 0, Rarity::Legendary))
            };
            player.addQuest(quests[rand() % quests.size()]);
        } else if (type == "Тайник") {
            cout << "| Ты находишь тайный проход, скрытый во мраке!\n";
            player.addItemToInventory(treasure);
            player.addResource("Титанит", 5);
            if (rollDice(1, 10) > 8) {
                player.spells.push_back(Spell("Тёмный разлом", 20, 15 + player.intelligence / 5, "Урон"));
                cout << "| Ты находишь свиток с заклинанием 'Тёмный разлом'!\n";
            }
        } else if (type == "Лестница") {
            cout << "| Каменная лестница уходит в непроглядную тьму...\n";
        } else if (type == "Портал") {
            cout << "| Перед тобой зияет портал, пульсирующий жёлтым светом Хаста...\n";
            cout << "| 1. Войти | 0. Уйти\nВыбор: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                return;
            }
            if (choice == 1) {
                cout << "| Ты шагаешь в портал, и мир растворяется в жёлтом кошмаре...\n";
            }
        } else if (type == "Алтарь Хаоса") {
            cout << "| Алтарь Хаоса шепчет о запретной мощи...\n";
            cout << "| 1. Принести жертву (50 душ) | 0. Уйти\nВыбор: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                return;
            }
            if (choice == 1 && player.souls >= 50) {
                player.souls -= 50;
                player.strength += 2;
                player.loseSanity(10);
                player.updateStats();
                cout << "| Твоя сила увеличивается на 2, но разум мутнеет (-10 рассудка)!\n";
            } else if (choice == 1) {
                cout << "| Недостаточно душ для жертвы!\n";
            }
        } else if (type == "Пещера бездны") {
            cout << "| Глубины шепчут твоё имя, разум дрожит...\n";
            player.loseSanity(15);
        }
    }
};

Room generateRoom(int playerLevel, int killCount, string className, int dungeonLevel) {
    vector<string> roomTypes = {
        "Монстр", "Монстр", "Сокровище", "Костёр", "Торговец", "Кузнец", "Святилище", "Событие", 
        "Квест", "Тайник", "Пусто", "Алтарь Хаоса", "Пещера бездны", "Портал"
    };
    vector<string> environments = {"", "Туман", "Яд", "Тьма", "Лед", "Лава", "Эхо бездны"};
    string type = roomTypes[rand() % roomTypes.size()];
    string env = environments[rand() % environments.size()];
    string desc;
    bool isLight = (type == "Костёр" || type == "Святилище" || rollDice(1, 10) > 8);

    switch (dungeonLevel % 5) {
        case 0: desc = "| Разрушенный зал, где стены покрыты Жёлтым Знаком"; break;
        case 1: desc = "| Сырой склеп, полный шорохов Каркозы"; break;
        case 2: desc = "| Тронный зал, где тени шепчут имя Хаста"; break;
        case 3: desc = "| Ледяная бездна, где звёзды молчат"; break;
        case 4: desc = "| Огненные недра, где пламя танцует под Жёлтым Знаком"; break;
    }
    desc += env.empty() ? "." : ", окутанный " + env + ".";

    if (type == "Монстр") return Room(type, env, desc, generateMonster(playerLevel, killCount));
    if (type == "Сокровище") return Room(type, env, desc, Character("", ""), generateUniqueItem(className));
    if (type == "Костёр") return Room(type, "", "| Тусклый костёр мерцает, отгоняя безумие.", Character("", ""), Item(), true);
    if (type == "Торговец") return Room(type, "", "| Скрюченная фигура шепчет о товарах из бездны.");
    if (type == "Кузнец") return Room(type, "", "| Молот стучит, отзываясь эхом в пустоте.");
    if (type == "Святилище") return Room(type, "", "| Древнее святилище источает нечеловеческий свет.", Character("", ""), Item(), true);
    if (type == "Событие") return Room(type, "", "| Шорохи и голоса зовут из теней.");
    if (type == "Квест") return Room(type, "", "| Незримый голос шепчет о задании из бездны.");
    if (type == "Тайник") return Room(type, "", "| Скрытый проход открывает сокровища безымянных.", Character("", ""), generateUniqueItem(className));
    if (type == "Алтарь Хаоса") return Room(type, "", "| Алтарь Хаоса пылает тёмной энергией.");
    if (type == "Пещера бездны") return Room(type, "Эхо бездны", "| Глубины зовут, разум трещит по швам.", Character("", ""), Item(), false);
    if (type == "Портал" && dungeonLevel > 5) return Room(type, "", "| Пульсирующий портал ведёт к Хастару.");
    return Room(type, env, desc);
}

void levelUpMenu(Character& player) {
    while (true) {
        cout << "\n=== Костёр ===\n";
        cout << "| 1. Улучшить характеристики | 2. Улучшить заклинание | 3. Продолжить | 4. Проверить квесты | 0. Выйти\nВыбор: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "| Неверный ввод! Попробуй снова.\n";
            continue;
        }
        if (choice == 1) player.levelUp();
        else if (choice == 2 && !player.spells.empty()) {
            Blacksmith smith;
            cout << "| Заклинания:\n";
            for (size_t i = 0; i < player.spells.size(); i++) {
                cout << "| " << i + 1 << ". " << player.spells[i].name << " (ур. " << player.spells[i].level << ")\n";
            }
            cout << "| Выбери заклинание: ";
            int spellChoice;
            if (!(cin >> spellChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                continue;
            }
            if (spellChoice > 0 && spellChoice <= player.spells.size()) {
                smith.upgradeSpell(player, player.spells[spellChoice - 1]);
            }
        } else if (choice == 3) break;
        else if (choice == 4) {
            if (player.activeQuests.empty()) cout << "| Нет активных квестов.\n";
            else for (size_t i = 0; i < player.activeQuests.size(); i++) {
                cout << "| " << i + 1 << ". " << player.activeQuests[i].name << " - " << (player.activeQuests[i].completed ? "Выполнен" : "Активен") << "\n";
            }
        } else if (choice == 0) exit(0);
        else cout << "| Неверный выбор! Попробуй снова.\n";
    }
}

void battle(Character& player, Character& enemy) {
    cout << "\n| " << enemy.name << " выползает из теней! (Уровень " << enemy.level << ")\n";
    bool bossFight = enemy.className == "Босс";
    while (player.isAlive() && enemy.isAlive()) {
        player.updateEffects();
        enemy.updateEffects();
        player.displayStats();
        cout << "| " << enemy.name << " Здоровье: " << enemy.health << "/" << enemy.maxHealth << "\n";
        cout << "\n| 1. Атака | 2. Способность | 3. Заклинание | 4. Уклонение | 5. Блок | 6. Контратака | 7. Использовать предмет | 0. Сбежать\nВыбор: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "| Неверный ввод! Попробуй снова.\n";
            continue;
        }

        if (choice == 1) {
            int damage = player.calculateDamage();
            if (damage > 0) {
                enemy.takeDamage(damage);
                cout << "| Ты наносишь " << damage << " урона!\n";
            } else cout << "| Ты слишком устал!\n";
        } else if (choice == 2 && !player.abilities.empty()) {
            cout << "| Способности:\n";
            for (size_t i = 0; i < player.abilities.size(); i++) {
                cout << "| " << i + 1 << ". " << player.abilities[i].name << " (Вын: " << player.abilities[i].staminaCost << ", Мана: " << player.abilities[i].manaCost << ")\n";
            }
            cout << "| Выбери способность: ";
            int abilityChoice;
            if (!(cin >> abilityChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                continue;
            }
            player.useAbility(abilityChoice - 1, enemy);
        } else if (choice == 3 && !player.spells.empty()) {
            cout << "| Заклинания:\n";
            for (size_t i = 0; i < player.spells.size(); i++) {
                cout << "| " << i + 1 << ". " << player.spells[i].name << " (Мана: " << player.spells[i].manaCost << ", Ур: " << player.spells[i].level << ")\n";
            }
            cout << "| Выбери заклинание: ";
            int spellChoice;
            if (!(cin >> spellChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Попробуй снова.\n";
                continue;
            }
            player.castSpell(spellChoice - 1, enemy);
        } else if (choice == 4) {
            if (player.dodge()) cout << "| Ты ловко уклоняешься!\n";
            else cout << "| Уклонение не удалось!\n";
            player.stamina -= 15;
        } else if (choice == 5) {
            if (player.block()) cout << "| Ты готовишься к удару!\n";
            else cout << "| Слишком устал для блока!\n";
        } else if (choice == 6) {
            if (!player.counterAttack(enemy)) cout << "| Контратака провалилась!\n";
        } else if (choice == 7) {
            if (player.inventory.empty()) {
                cout << "| Нет доступных предметов!\n";
            } else {
                cout << "| Предметы:\n";
                for (size_t i = 0; i < player.inventory.size(); i++) {
                    cout << "| " << i + 1 << ". " << player.inventory[i].name << "\n";
                }
                cout << "| Выбери предмет: ";
                int itemChoice;
                if (!(cin >> itemChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "| Неверный ввод! Попробуй снова.\n";
                    continue;
                }
                if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                    player.useItem(itemChoice - 1);
                } else {
                    cout << "| Неверный выбор!\n";
                }
            }
        } else if (choice == 0) {
            if (rollDice(1, 20) + player.dexterity / 5 > 15) {
                cout << "| Ты сбежал из боя!\n";
                return;
            }
            cout << "| Побег не удался!\n";
        } else {
            cout << "| Неверный выбор! Попробуй снова.\n";
        }

        if (enemy.isAlive() && enemy.activeEffects.find("Оглушение") == enemy.activeEffects.end()) {
            enemyTurn(enemy, player);
        }

        if (!enemy.isAlive()) {
            player.killCount++;
            player.gainSouls(enemy.souls);
            player.addResource("Титанит", rollDice(1, 2));
            player.addResource("Кристаллы", rollDice(1, 2));
            player.gainReputation(enemy.className == "Враг" ? 5 : -10);
            if (bossFight && !player.activeQuests.empty() && player.activeQuests[0].name == "Убить Хаста") player.completeQuest(0);
            cout << "| " << enemy.name << " поглощён бездной!\n";
        }
        player.stamina = min(player.maxStamina, player.stamina + 5);
        enemy.stamina = min(enemy.maxStamina, enemy.stamina + 5);
    }
    if (!player.isAlive()) cout << "\nТЫ УМЕР\n";
}

void enemyTurn(Character& enemy, Character& player) {
    if (enemy.health < enemy.maxHealth / 2 && rollDice(1, 20) > 15) {
        cout << "| " << enemy.name << " пытается раствориться во тьме!\n";
    } else if (player.activeEffects.count("Блок") && rollDice(1, 20) > 10) {
        enemy.block();
        cout << "| " << enemy.name << " готовится к защите!\n";
    } else if (!enemy.abilities.empty() && rollDice(1, 2) == 1) {
        for (const auto& ability : enemy.abilities) {
            if (ability.effect == "Страх") {
                player.activeEffects["Страх"] = ability.duration;
                cout << "| " << enemy.name << " использует " << ability.name << ", навевая страх!\n";
            } else if (ability.effect == "Безумие") {
                player.loseSanity(10);
                cout << "| " << enemy.name << " использует " << ability.name << ", терзая твой разум (-10 рассудка)!\n";
            } else if (ability.effect == "Оглушение") {
                player.activeEffects["Оглушение"] = ability.duration;
                player.takeDamage(ability.damage);
                cout << "| " << enemy.name << " использует " << ability.name << ", оглушая тебя на " << ability.damage << " урона!\n";
            }
        }
    } else {
        int damage = enemy.calculateDamage();
        if (!player.dodge()) {
            player.takeDamage(damage);
            cout << "| " << enemy.name << " наносит " << damage << " урона!\n";
        } else cout << "| Ты уклоняешься от удара!\n";
    }
}

void visualizeDungeon(const vector<vector<Room>>& dungeon, int currentLevel, int currentRoom) {
    cout << "\n=== Карта бездны ===\n";
    for (int level = 0; level < dungeon.size(); level++) {
        cout << "Глубина " << level + 1 << ": ";
        for (int i = 0; i < dungeon[level].size(); i++) {
            if (level == currentLevel && i == currentRoom) {
                cout << "[Ты] ";
            } else if (dungeon[level][i].type == "Лестница") {
                cout << "[Л] ";
            } else if (dungeon[level][i].type == "Портал") {
                cout << "[Пт] ";
            } else if (dungeon[level][i].type == "Босс") {
                cout << "[Б] ";
            } else if (dungeon[level][i].type == "Монстр") {
                cout << "[М] ";
            } else if (dungeon[level][i].type == "Сокровище") {
                cout << "[С] ";
            } else if (dungeon[level][i].type == "Костёр") {
                cout << "[К] ";
            } else {
                cout << "[?] ";
            }
        }
        cout << "\n";
    }
    cout << "Легенда: [Ты] - Твоё местоположение, [Л] - Лестница, [Пт] - Портал, [Б] - Босс, [М] - Монстр, [С] - Сокровище, [К] - Костёр, [?] - Прочее\n";
}

Character createCharacter() {
    string name;
    cout << "Назови своё имя, смертный, что вступает в бездну: ";
    cin >> name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> classes = {"Рыцарь", "Маг", "Клирик", "Вор", "Охотник", "Пиротехник", "Воин", "Лишенный", "Паладин"};
    cout << "\nВыбери свой путь в тенях Каркозы:\n";
    for (size_t i = 0; i < classes.size(); i++) cout << i + 1 << ". " << classes[i] << "\n";
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > 9) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "| Неверный выбор! Введи число от 1 до 9: ";
    }
    choice = max(1, min(9, choice)) - 1;

    return Character(name, classes[choice]);
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    
    cout << "\n" << string(50, '=') << "\n";
    cout << "| Добро пожаловать в Каркозу, Избранный!\n";
    cout << "| Мир пал под тенью Хаста, Жёлтого Короля. Города забыты, звёзды молчат, а разум людей пожирает бездна.\n";
    cout << "| Ты - последний, кто бросает вызов его воле. Спустись в глубины кошмара, найди Хаста и верни свет... или стань его тенью.\n";
    cout << string(50, '=') << "\n";

    Character player = createCharacter();
    vector<vector<Room>> dungeon;
    bool finalBossReached = false;

    int level = 0;
    while (player.isAlive() && !finalBossReached) {
        cout << "\n| Глубина " << level + 1 << " - ";
        switch (level % 5) {
            case 0: cout << "Забытые Руины Каркозы\n"; break;
            case 1: cout << "Склеп Шепчущих Теней\n"; break;
            case 2: cout << "Трон Жёлтого Короля\n"; break;
            case 3: cout << "Ледяные Просторы Бездны\n"; break;
            case 4: cout << "Огненные Чрева Хаоса\n"; break;
        }
        dungeon.push_back(vector<Room>());
        int path = 2;
        if (level > 0) {
            cout << "| Две тропы разрывают тьму...\n";
            cout << "| 1. Налево (Опасность) | 2. Направо (Тишина)\nВыбор: ";
            if (!(cin >> path)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Выбран путь направо.\n";
                path = 2;
            }
        }
        int rooms = 5 + rollDice(1, 3);
        for (int i = 0; i < rooms; i++) {
            if (i == rooms - 1 && level % 2 == 0 && level > 5 && rollDice(1, 10) > 7) {
                dungeon[level].emplace_back("Портал", "", "| Пульсирующий портал ведёт к Хастару.", Character("", ""), Item(), true);
            } else if (i == rooms - 1) {
                dungeon[level].emplace_back("Лестница", "", "| Каменная лестница уходит в непроглядную тьму.", Character("", ""), Item(), false);
            } else if (path == 1 && rollDice(1, 10) > 8) {
                dungeon[level].emplace_back("Тайник", "", "| Скрытый проход открывает сокровища безымянных.", Character("", ""), generateUniqueItem(player.className), false);
            } else {
                dungeon[level].emplace_back(generateRoom(player.level, player.killCount, player.className, level));
            }

            visualizeDungeon(dungeon, level, i);
            cout << "| Зал " << i + 1 << ":\n";
            dungeon[level][i].enterRoom(player);

            if (!player.isAlive()) {
                cout << "\n| Твоя душа растворяется в бездне...\n";
                break;
            }
            if (dungeon[level][i].type == "Портал") {
                cout << "| Ты вступаешь в портал...\n";
                Character finalBoss = generateBoss(player.level);
                battle(player, finalBoss);
                if (player.isAlive()) finalBossReached = true;
                break;
            }
            if (dungeon[level][i].type == "Лестница") continue;

            cout << "| Нажми 1, чтобы продолжить (0 для выхода): ";
            int cont;
            if (!(cin >> cont)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "| Неверный ввод! Продолжаем...\n";
                cont = 1;
            }
            if (!cont) break;
        }
        if (!player.isAlive()) break;
        level++;
    }

    if (player.isAlive() && finalBossReached) {
        cout << "\n" << string(50, '=') << "\n";
        cout << "| Ты победил Хаста, Жёлтого Короля...\n";
        cout << "| Тени Каркозы отступают, но шёпот бездны навсегда останется в твоём разуме.\n";
    } else if (player.isAlive()) {
        cout << "\n" << string(50, '=') << "\n";
        cout << "| Ты выбрался из глубин, но Хаст всё ещё ждёт...\n";
    }

    cout << "\n| Итоговые данные:\n";
    player.displayStats();
    cout << string(50, '=') << "\n";
    return 0;
}