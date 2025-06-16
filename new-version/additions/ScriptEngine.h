
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

