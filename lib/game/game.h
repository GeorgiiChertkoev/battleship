#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <optional>
#include <cstdint>
#include <cstdlib>

#include "lib/field/field.h"
#include "lib/strategy/strategy.h"

class Game {
public:
    Game() = default;
    Game(const std::string& path);

    void Play(std::istream& input = std::cin, std::ostream& output = std::cout);

    std::string Ping();
    void Stop();
    bool Start();
    bool Load(const std::string& path);
    bool Dump(const std::string& path);
    bool SetWidth(uint64_t width);
    bool SetHeight(uint64_t height);
    uint64_t GetWidth();
    uint64_t GetHeight();
    bool SetCount(uint8_t ship_size, uint64_t number_of_ships);
    uint64_t GetCount(uint8_t ship_size);
    bool SetResult(const std::string& result);
    bool SetStrategy(const std::string& strategy_name);
    bool IsWin();
    bool IsLose();
    bool IsFinished();
    ShotResult Shoot(const Point& target);
    Point Shoot();
    bool DrawEnemyField(const std::string& path);
    
    void CreateMaster();
    void ResetGame();

private:
    bool SetWidthFromCommand(std::string& cmd);
    bool SetHeightFromCommand(std::string& cmd);
    bool CanStart();

    bool is_master_ = false;
    bool is_started_ = false;
    bool is_paused_ = false;
    bool is_ordered = false;
    Field field_;
    BaseStrategy* strategy_ = new CustomStrategy;
};