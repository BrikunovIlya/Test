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
```

