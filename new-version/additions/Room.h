#ifndef ROOM_H
#define ROOM_H

#include <string>
#include "Character.h"
#include "Item.h"

class Renderer;

class Room {
public:
    std::string description;
    Character enemy;
    Item treasure;
    bool isLight;
    std::string scriptFile;
    std::vector<std::string> enemyActions;

    Room(std::string desc = "Пустая комната", Character e = Character("", ""), 
         Item t = Item(), bool light = true, std::string script = "",
         std::vector<std::string> actions = {"attack", "attack", "guard"});
    
    void enterRoom(Character& player, Renderer& renderer);
    void executeEnemyAction(Character& enemy, Character& player, Renderer& renderer);
};

#endif
```

**ScriptEngine.h**
```cpp
#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <string>
#include "Character.h"
#include "Room.h"

class Renderer;

class ScriptEngine {
public:
    static void execute(const std::string& scriptFile, Character& player, Room* room, Renderer& renderer);
    static void combatLoop(Character& player, Room& room, Renderer& renderer);
};

#endif
```

**Renderer.h**
```cpp
#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "Character.h"
#include "Room.h"

class Renderer {
public:
    void displayDescription(const std::string& text);
    void displayText(const std::string& text);
    void renderRoom(const Room& room, const Character& player);
    void renderCombatMenu();
    void renderCombatStatus(const Character& player, const Character& enemy);
    void renderInventory(const Character& player);
};

#endif
