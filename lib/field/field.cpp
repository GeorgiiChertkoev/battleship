#include "field.h"

bool Field::SetWidth(uint64_t width) {
    uint64_t temp = width_;
    width_ = width;
    if (!PlaceShips()) {
        width_ = temp;
        return false;
    } 
    width_ = width;
    return true;
}

bool Field::SetHeight(uint64_t height) {
    uint64_t temp = height_;
    height_ = height;
    if (!PlaceShips()) {
        height_ = temp;
        return false;
    } 
    return true;
}

uint64_t Field::GetWidth() const {
    return width_;
}
uint64_t Field::GetHeight() const {
    return height_;
}


bool Field::SetCount(uint8_t ship_size, uint64_t number_of_ships) {
    if (ship_size > 4) {
        return false;
    }
    --ship_size;
    uint64_t temp = ship_counter_[ship_size];
    ship_counter_[ship_size] = number_of_ships;
    if (!PlaceShips()) {
        std::cout << "couldnt place \n";
        ship_counter_[ship_size] = temp;
        return false;
    }
    my_ships_alive_ = my_ships_alive_ - temp + number_of_ships;
    enemy_ships_alive_ = enemy_ships_alive_ - temp + number_of_ships;
    return true;
}

uint64_t Field::GetCount(uint8_t ship_size) {
    if (ship_size > 4) {
        return 0;
    }
    return ship_counter_[ship_size - 1];
}

void SurroundShip(std::unordered_map<Point, bool>& field, const Point& cur, const Point& from) {
    for (uint64_t dx = (cur.x_ >= 1 ? cur.x_ - 1 : 0); dx <= cur.x_ + 1; ++dx) {
        for (uint64_t dy = (cur.y_ >= 1 ? cur.y_ - 1 : 0); dy <= cur.y_ + 1; ++dy) {
            if (Point{dx, dy} == from || Point{dx, dy} == cur) continue;
            if (field.find({dx, dy}) != field.end() && field[{dx, dy}]) {
                SurroundShip(field, {dx, dy}, cur);
            } else {
                field[{dx, dy}] = 0;
            }
        }
    }
}

void SetValuesAroundPoint(std::unordered_map<Point, bool>& temp_field, uint64_t x, uint64_t y, bool value = true) {
    for (uint64_t dx = std::min(0ul, x - 1); dx <= x + 1; ++dx) {
        for (uint64_t dy = std::min(0ul, y - 1); dy <= y + 1; ++dy) {
            if (temp_field.find({dx, dy}) == temp_field.end()) {
                temp_field[{dx, dy}] = value;
            }
        }
    }
}


void MarkAroundShip(std::unordered_map<Point, bool>& temp_field, uint64_t x, uint64_t y,
                    bool is_horizontal, uint8_t ship_size) {
    for (uint64_t delta = 0; delta < ship_size; ++delta) {
        SetValuesAroundPoint(temp_field, x, y);
        if (is_horizontal) {
            ++x;
        } else {
            ++y;
        }
    }
}

bool TryToPlaceShip(std::unordered_map<Point, bool>& temp_field, uint64_t x, uint64_t y,
                    bool is_horizontal, uint8_t ship_size) {
    for (int j = 0; j < ship_size; ++j) {
        if (temp_field.find({x, y}) != temp_field.end()) {
            return false;
        }
        is_horizontal ? ++x : ++y;
    }
    (is_horizontal ? x : y) -= ship_size;

    MarkAroundShip(temp_field, x, y, is_horizontal, ship_size);
    return true;
}

bool Field::PlaceShip(std::unordered_map<Point, bool>& temp_field, std::vector<Ship>& ships, uint8_t ship_size) {
    bool placed = false;
    for (uint64_t y = 0; y < height_; ++y) {
        if (placed) break;
        for (uint64_t x = 0; x < width_; ++x) {
            if (placed) break;
            if (x + ship_size <= width_ && TryToPlaceShip(temp_field, x, y, true, ship_size)) {
                ships.push_back({x, y, true, ship_size});
                placed = true;
            } else if (y + ship_size <= height_ && TryToPlaceShip(temp_field, x, y, false, ship_size)) {
                ships.push_back({x, y, false, ship_size});
                placed = true;
            }
        }
    }
    return placed;
}

bool Field::PlaceShips() {
    std::unordered_map<Point, bool> temp_field;
    std::vector<Ship> ships;
    ships.reserve(ship_counter_[0] + ship_counter_[1] + 
                ship_counter_[2] + ship_counter_[3]);
    uint64_t ships_placed = 0;

    for (uint8_t ship_size = 4; ship_size >= 1; --ship_size) {
        for (uint64_t i = 0; i < ship_counter_[ship_size - 1]; ++i) {
            if (!PlaceShip(temp_field, ships, ship_size)) {
                return false;
            }
            ++ships_placed;
        }
    }
    ships_ = ships;
    return true;
}


void Field::ResetField() {
    width_ = 0;
    height_ = 0;
    ResetShips();
    last_shot_.reset();
    enemy_field_.clear();
}


void Field::ResetShips() {
    for (int i = 0; i < 4; ++i) {
        ship_counter_[i] = 0;
    }
    my_ships_alive_ = 0;
    enemy_ships_alive_ = 0;
    ships_.clear();
}


void Field::AddShip(uint8_t ship_size, bool is_horizontal, uint64_t x, uint64_t y) {
    ship_counter_[ship_size - 1] += 1;
    ++my_ships_alive_;
    ++enemy_ships_alive_;
    ships_.push_back(Ship(x, y, is_horizontal, ship_size));
}


const std::vector<Ship>& Field::GetShips() const {
    return ships_;
}

const std::unordered_map<Point, bool>& Field::GetEnemyField() const {
    return enemy_field_;
}

std::unordered_map<Point, bool>& Field::GetEnemyField() {
    return enemy_field_;
}


std::optional<Point>& Field::GetLastShot() {
    return last_shot_;
}

uint64_t Field::GetMyShipsAlive() const {
    return my_ships_alive_;
}

uint64_t Field::GetEnemyShipsAlive() const {
    return enemy_ships_alive_;
}

ShotResult Field::ShootMine(const Point& target) {
    for (Ship& ship : ships_) {
        ShotResult res = ship.Shoot(target);
        if (res != ShotResult::kMiss) {
            if (res == ShotResult::kKill) {
                --my_ships_alive_;
            }
            return res;
        }
    }
    return ShotResult::kMiss;
}

bool Field::SetResult(const std::string& shot_result, bool mark_if_killed) {
    if (!last_shot_.has_value()) {
        return false;
    }
    if (shot_result == "miss") {
        enemy_field_[last_shot_.value()] = false;
    } else if (shot_result == "hit") {
        enemy_field_[last_shot_.value()] = true;
    } else if (shot_result == "kill") {
        enemy_field_[last_shot_.value()] = true;
        SurroundShip(enemy_field_, last_shot_.value(), {last_shot_.value().x_ + 2, last_shot_.value().y_});
    } else {
        return false;
    }
    return true;
}

