#include "Room.h"
#include "ScriptEngine.h"
#include "Renderer.h"

Room::Room(std::string desc, Character e, Item t, bool light, std::string script, std::vector<std::string> actions)
    : description(desc), enemy(e), treasure(t), isLight(light), scriptFile(script), enemyActions(actions) {}

void Room::enterRoom(Character& player, Renderer& renderer) {
    renderer.displayDescription(description);
    if (!scriptFile.empty()) {
        ScriptEngine::execute(scriptFile, player, this, renderer);
    }
}

void Room::executeEnemyAction(Character& enemy, Character& player, Renderer& renderer) {
    if (enemyActions.empty()) return;
    
    // Выбираем случайное действие из доступных
    int actionIndex = rand() % enemyActions.size();
    std::string action = enemyActions[actionIndex];
    
    if (action == "attack") {
        enemy.attackAction(player);
        renderer.displayText("| " + enemy.name + " атакует тебя!");
    }
    else if (action == "guard") {
        enemy.guardAction();
        renderer.displayText("| " + enemy.name + " принимает защитную стойку!");
    }
    else if (action == "heal" && enemy.hp < enemy.maxHp * 0.5) {
        enemy.healAction();
        renderer.displayText("| " + enemy.name + " восстанавливает здоровье!");
    }
    else if (action == "boost" && !enemy.isBoosted()) {
        enemy.boostAction();
        renderer.displayText("| " + enemy.name + " усиливает свои способности!");
    }
    else {
        // По умолчанию атака
        enemy.attackAction(player);
        renderer.displayText("| " + enemy.name + " атакует тебя!");
    }
}
