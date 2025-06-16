#include "Game.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <map>

const std::vector<ClassInfo>& getClassOptions() {
    static std::vector<ClassInfo> classes = {
        {"Рыцарь", "Благородный воин в сияющих доспехах. Защитник слабых и грозный враг нечисти.", 
         30, 20, 6, 8, 6, 2, 2, 2},
        {"Маг", "Повелитель тайных знаний. Его заклинания могут как исцелить, так и испепелить.", 
         20, 40, 4, 4, 3, 4, 1, 1},
        {"Клирик", "Служитель культа, несущий свет веры. Лечит союзников и обращает в бегство нежить.", 
         25, 35, 5, 6, 4, 3, 1, 1},
        {"Вор", "Тень в ночи. Мастер скрытности и удара в спину. Его кинжалы находят слабые места.", 
         22, 25, 8, 5, 3, 2, 3, 1},
        {"Охотник", "Следопыт, знающий тайны дикой природы. Его стрелы никогда не промахиваются.", 
         24, 25, 7, 6, 4, 2, 2, 1},
        {"Пиротехник", "Безумный изобретатель, одержимый огнем. В его руках даже простая искра становится оружием.", 
         20, 35, 5, 5, 3, 3, 1, 1},
        {"Воин", "Наемник, для которого битва — искусство. Силен и вынослив, но не ищите в нем изящества.", 
         28, 15, 7, 7, 5, 1, 2, 2},
        {"Лишенный", "Изгой, потерявший все. Его тело покрыто шрамами, а душа — тьмой. Выживает любой ценой.", 
         18, 10, 3, 3, 2, 1, 1, 1},
        {"Паладин", "Святой воин, чья вера — щит, а воля — меч. Неустрашим перед лицом древнего зла.", 
         32, 25, 6, 9, 6, 2, 2, 3}
    };
    return classes;
}

Item generateItemForClass(const std::string& className, Rarity rarity) {
    struct ClassItem {
        std::string name;
        std::string description;
        ItemType type;
        int hp;
        int mp;
        int att;
        int def;
        int heal;
        int restore;
        int batt;
        int bdef;
    };
    
    static std::map<std::string, std::vector<ClassItem>> classItems = {
        {"Рыцарь", {
            {"Меч Солнечного Света", "Клинок, испускающий ослепительное сияние", WEAPON, 0,0,15,0,0,0,0,0},
            {"Щит Непоколебимой Веры", "Легендарный щит, отражающий любые атаки", ARMOR, 0,0,0,20,0,0,0,0},
            {"Доспехи Вечной Стражи", "Доспехи, пропитанные магией древних защитников", ARMOR, 30,0,0,25,0,0,0,0},
            {"Эликсир Рыцаря", "Восстанавливает силы и укрепляет дух", CONSUMABLE, 0,0,0,0,50,20,0,0},
            {"Свиток Гневного Удара", "Временно усиливает боевые способности", SCROLL, 0,0,0,0,0,0,50,25},
            {"Плащ Закатного Пламени", "Плащ, сотканный из лучей заходящего солнца", ARMOR, 15,10,5,10,0,0,0,0}
        }},
        {"Маг", {
            {"Посох Вечной Зимы", "Посох, замораживающий все на своем пути", WEAPON, 0,20,5,0,0,0,0,0},
            {"Гримуар Забытых Истин", "Книга, содержащая знания древних цивилизаций", WEAPON, 0,30,0,0,0,0,0,0},
            {"Мантия Звездной Пыли", "Мантия, сотканная из материи созвездий", ARMOR, 10,25,0,5,0,0,0,0},
            {"Эликсир Прозрения", "Усиливает магические способности", CONSUMABLE, 0,50,0,0,0,0,30,0},
            {"Свиток Ледяного Копья", "Призывает ледяное копье для атаки врага", SCROLL, 0,0,0,0,0,0,0,0},
            {"Кольцо Хаоса", "Усиливает магическую мощь владельца", ARMOR, 0,20,10,0,0,0,0,0}
        }},
        // ... аналогично для других классов ...
    };
    
    static std::map<Rarity, std::string> rarityPrefix = {
        {COMMON, "Потрепанный"},
        {UNCOMMON, "Прочный"},
        {RARE, "Редкий"},
        {EPIC, "Мощный"},
        {LEGENDARY, "Легендарный"},
        {MYTHIC, "Мифический"}
    };
    
    static std::map<Rarity, float> rarityMultiplier = {
        {COMMON, 1.0},
        {UNCOMMON, 1.3},
        {RARE, 1.7},
        {EPIC, 2.2},
        {LEGENDARY, 2.8},
        {MYTHIC, 3.5}
    };
    
    if (classItems.find(className) == classItems.end()) {
        return Item("Пыль забвения", "Бесполезный мусор", 0, COMMON);
    }
    
    int itemIndex = rand() % classItems[className].size();
    ClassItem baseItem = classItems[className][itemIndex];
    
    float multiplier = rarityMultiplier[rarity];
    std::string name = rarityPrefix[rarity] + " " + baseItem.name;
    
    return Item(
        name,
        baseItem.description,
        static_cast<int>(100 * multiplier),
        rarity,
        baseItem.type,
        className,
        static_cast<int>(baseItem.hp * multiplier),
        static_cast<int>(baseItem.att * multiplier),
        static_cast<int>(baseItem.def * multiplier),
        static_cast<int>(baseItem.mp * multiplier),
        static_cast<int>(baseItem.heal * multiplier),
        static_cast<int>(baseItem.restore * multiplier),
        static_cast<int>(baseItem.batt * multiplier),
        static_cast<int>(baseItem.bdef * multiplier),
        3,
        static_cast<int>(5 * multiplier)
    );
}

Character createCharacter() {
    std::string name;
    std::cout << "\n| Введи имя: ";
    std::cin >> name;

    const auto& classes = getClassOptions();
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "| Выбери свой класс (введи номер):\n";
    std::cout << std::string(70, '-') << "\n";
    
    for (int i = 0; i < classes.size(); i++) {
        std::cout << "| " << std::setw(2) << (i+1) << ". " << std::setw(12) << std::left << classes[i].name 
                  << " | HP: " << std::setw(2) << classes[i].baseHp 
                  << " | MP: " << std::setw(2) << classes[i].baseMana
                  << " | ATK: " << std::setw(2) << classes[i].baseAttack
                  << " | DEF: " << std::setw(2) << classes[i].baseDefense << " |\n";
        std::cout << "|     " << classes[i].description << "\n";
        if (i < classes.size()-1) std::cout << "|" << std::string(68, '-') << "\n";
    }
    std::cout << std::string(70, '=') << "\n";

    int choice;
    while (true) {
        std::cout << "| Твой выбор: ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "| Неверный ввод. Попробуй снова.\n";
            continue;
        }
        
        if (choice >= 1 && choice <= classes.size()) {
            break;
        }
        std::cout << "| Неверный выбор. Введи число от 1 до " << classes.size() << ".\n";
    }

    const ClassInfo& selected = classes[choice-1];
    std::cout << "\n| Ты выбрал путь " << selected.name << "!\n";
    std::cout << "| " << selected.description << "\n";
    std::cout << "| Начальные характеристики: HP " << selected.baseHp 
              << ", MP " << selected.baseMana
              << ", ATK " << selected.baseAttack
              << ", DEF " << selected.baseDefense << "\n";
    std::cout << "| Прирост на уровень: +" << selected.levelUpHpGain << " HP, +" 
              << selected.levelUpManaGain << " MP, +" 
              << selected.levelUpAttackGain << " ATK, +" 
              << selected.levelUpDefenseGain << " DEF\n\n";

    return Character(name, selected.name);
}

Room generateRoom(int level) {
    std::vector<std::string> descriptions = {
        "Заброшенная часовня с разбитыми витражами",
        "Лаборатория алхимика с кипящими колбами",
        "Тронный зал древнего короля",
        "Оружейная с заржавевшим оружием",
        "Библиотека с пыльными фолиантами",
        "Подземная темница с цепями на стенах",
        "Обсерватория с разбитым телескопом",
        "Тайное святилище культа",
        "Затопленный подвал с гниющими бочками",
        "Мастерская изобретателя со странными механизмами",
        "Оранжерея с ядовитыми растениями",
        "Зал героев с разрушенными статуями",
        "Сокровищница с пустыми сундуками",
        "Проклятый склеп с шевелящимися тенями",
        "Портал в иное измерение с мерцающей энергией"
    };
    
    std::vector<std::vector<std::string>> enemyActions = {
        {"attack", "attack", "guard"},
        {"attack", "guard", "heal"},
        {"attack", "boost", "heal"},
        {"attack", "attack", "boost"},
        {"attack", "guard", "guard"},
        {"attack", "heal", "boost"},
        {"attack", "attack", "heal"},
        {"attack", "boost", "guard"},
        {"attack", "heal", "guard"},
        {"attack", "attack", "boost"},
        {"attack", "guard", "heal"},
        {"attack", "boost", "heal"},
        {"attack", "attack", "guard"},
        {"attack", "heal", "boost"},
        {"attack", "boost", "boost"} // Финальный босс
    };
    
    std::string desc = descriptions[level % descriptions.size()];
    return Room(desc, Character("", ""), Item(), true, 
               "scripts/room" + std::to_string(level) + ".json",
               enemyActions[level % enemyActions.size()]);
}
```

