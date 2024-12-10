
#include <string>
#include <iostream>
#include <optional>
#include <cstdint>

#include "lib/game/game.h"


void GameSample() {
    Game a;
    a.SetWidth(5);
    a.SetHeight(4);
    a.SetCount(3, 1);
    a.SetCount(2, 2);
    a.SetCount(1, 1);
    a.Start();
    // std::cout << a.Shoot() << '\n';
    a.Shoot();
    a.SetResult("hit");
    // std::cout << a.Shoot() << '\n';
    a.Shoot();
    a.SetResult("miss");
    // std::cout << a.Shoot() << '\n';
    a.Shoot();
    a.SetResult("hit");
    a.DrawEnemyField("before_kill.txt");
    // std::cout << a.Shoot() << '\n';
    a.Shoot();
    a.SetResult("kill");
}


int main() {
    Game a;

    a.Play();
    
    return 0;
}
