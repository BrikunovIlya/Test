#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(0));
    std::cout << "========= ТЕМНЫЕ ЛАБИРИНТЫ КАРКОЗЫ =========\n";
    Character player = createCharacter();
    Renderer renderer;
    
    int currentRoom = 0;
    const int TOTAL_ROOMS = 15;
    
    while (player.isAlive() && currentRoom < TOTAL_ROOMS) {
        Room room = generateRoom(currentRoom);
        room.enterRoom(player, renderer);
        
        if (!player.isAlive()) {
            std::cout << "\n| Твоё путешествие окончено...\n";
            break;
        }
        
        renderer.renderRoom(room, player);
        
        // Меню экипировки
        if (!player.inventory.empty()) {
            std::cout << "| Экипировать предмет? (1 - Да, 0 - Нет): ";
            int equipChoice;
            std::cin >> equipChoice;
            
            if (equipChoice == 1) {
                renderer.renderInventory(player);
                std::cout << "| Выбери предмет для экипировки (0 для отмены): ";
                int itemIndex;
                std::cin >> itemIndex;
                
                if (itemIndex > 0 && itemIndex <= player.inventory.size()) {
                    player.equipItem(itemIndex - 1);
                    renderer.displayText("| Предмет экипирован!");
                }
            }
        }
        
        if (currentRoom < TOTAL_ROOMS - 1) {
            std::cout << "| Перейти в следующую комнату? (1 - Да, 0 - Выход): ";
            int choice;
            std::cin >> choice;
            if (choice == 0) break;
        }
        
        currentRoom++;
        player.levelUp();
        
        if (currentRoom == TOTAL_ROOMS) {
            std::cout << "\n| ПОБЕДА! Ты прошел все комнаты и спас Каркозу!\n";
        }
    }
    
    std::cout << "\n| ======= ИГРА ОКОНЧЕНА ======= |\n";
    return 0;
}
