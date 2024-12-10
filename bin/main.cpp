
#include <string>
#include <iostream>
#include <optional>
#include <cstdint>

#include "lib/game/game.h"

int main() {
    Game a;
    // a.Load("/home/geo/prog/labs/labwork5-GeorgiiChertkoev/input.txt");
    // a.CreateMaster();
    a.SetWidth(5);
    a.SetHeight(4);
    a.SetCount(3, 1);
    a.SetCount(2, 2);
    a.SetCount(1, 1);
    std::cout << a.Shoot() << '\n';
    a.SetResult("hit");
    std::cout << a.Shoot() << '\n';
    a.SetResult("miss");
    std::cout << a.Shoot() << '\n';
    a.SetResult("hit");
    a.DrawEnemyField("before_kill.txt");
    std::cout << a.Shoot() << '\n';
    a.SetResult("kill");
    // std::cout << a.Shoot() << '\n';
    // a.SetResult("hit");
    // std::cout << a.Shoot() << '\n';
    // a.SetResult("hit");

    a.DrawEnemyField("after.txt");
    a.Play();

    
    a.DrawEnemyField("enemy.txt");
    
    
    return 0;
}
