#include "ship.h"


bool Ship::IsDead() {
    return health_.IsZero();
}

int Ship::GetHealth() {
    return health_;
}

ShotResult Ship::Shoot(const Point& target) {
    if (top_left_.x_ == target.x_ && is_horizontal_ == false) {
        if (target.y_ < top_left_.y_ + health_.max_health_) {
            return health_.Shoot(target.y_ - top_left_.y_);
        }
    } else if (top_left_.y_ == target.y_) {
        if (target.x_ < top_left_.x_ + health_.max_health_) {
            return health_.Shoot(target.x_ - top_left_.x_);
        }
    }
    return ShotResult::kMiss;
}

void Ship::SetPlace(Point& new_top_left, bool is_horizontal) {
    top_left_ = new_top_left;
    is_horizontal_ = is_horizontal;
}

ShotResult Health::Shoot(uint8_t x) {
    uint8_t temp_health_ = health_points_ & 0xff - (1 << x);
    if (temp_health_ != health_points_) {
        health_points_ = temp_health_;
        return IsZero() ? ShotResult::kKill : ShotResult::kHit;
    }
    return ShotResult::kMiss;
}

bool Health::IsZero() {
    return !static_cast<bool>(health_points_);
}

Health::Health(uint8_t health_amount) {
    health_points_ = (1 << health_amount) - 1;
    max_health_ = health_amount;
}

std::ostream& operator<<(std::ostream& stream, const Point& coordinate) {
    stream << coordinate.x_ << ' ' << coordinate.y_;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Ship& ship) {
    stream << std::to_string(ship.health_.max_health_) << ' ';
    stream << (ship.is_horizontal_ ? "h" : "v") << ' ';
    stream << ship.top_left_;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const ShotResult& res) {
    switch (res)
    {
    case ShotResult::kMiss:
        stream << "miss";
        break;
    case ShotResult::kHit:
        stream << "hit";
        break;
    case ShotResult::kKill:
        stream << "kill";
        break;
    }
    return stream;
}