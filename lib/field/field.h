#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <optional>
#include <algorithm>

#include "lib/ship/ship.h"


class Field {
public:
    bool SetWidth(uint64_t width);
    bool SetHeight(uint64_t height);

    uint64_t GetWidth() const;
    uint64_t GetHeight() const;

    bool SetCount(uint8_t ship_size, uint64_t number_of_ships);
    uint64_t GetCount(uint8_t ship_size);

    bool PlaceShips();
    void ResetField();
    void ResetShips();
    void AddShip(uint8_t ship_size, bool is_horizontal, uint64_t x, uint64_t y);

    uint64_t GetMyShipsAlive() const;
    uint64_t GetEnemyShipsAlive() const;

    const std::vector<Ship>& GetShips() const;

    const std::unordered_map<Point, bool>& GetEnemyField() const;
    std::unordered_map<Point, bool>& GetEnemyField();

    std::optional<Point>& GetLastShot();
    bool SetResult(const std::string& shot_result, bool mark_if_killed = false);

    ShotResult ShootMine(const Point& target);

private:
    bool PlaceShip(std::unordered_map<Point, bool>& temp_field, std::vector<Ship>& ships, uint8_t ship_size);

    uint64_t ship_counter_[4]{0, 0, 0, 0};
    std::vector<Ship> ships_;
    std::unordered_map<Point, bool> enemy_field_;
    std::optional<Point> last_shot_;
    uint64_t my_ships_alive_ = 0;
    uint64_t enemy_ships_alive_ = 0;
    uint64_t width_ = 0;
    uint64_t height_ = 0;
};

