#include "strategy.h"

Point OrderedStrategy::Shoot(Field& field) {
    if (!(field.GetLastShot().has_value())) {
        return {0, 0};
    }
    Point last_shot = field.GetLastShot().value();
    
    if ((last_shot.x_ + 1) == field.GetWidth()) {
        return {0, last_shot.y_ + 1};
    }
    return {(last_shot.x_ + 1) % field.GetWidth(), last_shot.y_ % field.GetHeight()};
}

Point CustomStrategy::Shoot(Field& field) {
    if (!(field.GetLastShot().has_value())) {
        return {0, 0};
    }
    Point next_shot = field.GetLastShot().value();
    const std::unordered_map<Point, bool>& enemy_field = field.GetEnemyField();
    while (enemy_field.find(next_shot) != enemy_field.end()) {
        if ((next_shot.y_ + 1) == field.GetHeight()) {
            next_shot = {next_shot.x_ + 1, 0};
        } else {
            ++next_shot.y_;
        }
    }
    return next_shot;
}

