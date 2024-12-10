#pragma once
#include <vector>
#include <cstdint>
#include <utility>
#include <ostream>



enum class ShotResult {
    kMiss = 0,
    kHit = 1,
    kKill = 2
};

std::ostream& operator<<(std::ostream& stream, const ShotResult& res);


struct Health {
    uint8_t health_points_;
    uint8_t max_health_;

    Health() = default;
    Health(uint8_t health_amount);

    bool IsZero();

    operator int() const {
        int res = 0;
        for (int i = 0; i < 8; ++i) {
            res += (health_points_ >> i) & 1;
        }
        return res;
    }

    ShotResult Shoot(uint8_t x);
};


struct Point {
    uint64_t x_;
    uint64_t y_;

    Point() = default;
    Point(uint64_t x, uint64_t y) : x_(x), y_(y) {}
    Point(std::pair<uint64_t, uint64_t> pair) : x_(pair.first), y_(pair.second) {}

    bool operator==(const Point& other) const {return x_ == other.x_ && y_ == other.y_;}
};

std::ostream& operator<<(std::ostream& stream, const Point& coordinate);


template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t h1 = std::hash<uint64_t>{}(p.x_);
        std::size_t h2 = std::hash<uint64_t>{}(p.y_);
        return h1 ^ (h2 << 1);
    }
};

class Ship {
public:
    Ship() = default;

    Ship(uint64_t x, uint64_t y, bool is_horizontal, uint8_t size) : 
        top_left_(x, y),
        is_horizontal_(is_horizontal),
        health_(size)
    {}

    ShotResult Shoot(const Point& target);
    int GetHealth();
    bool IsDead();

    void SetPlace(Point& new_top_left, bool is_horizontal);

    friend std::ostream& operator<<(std::ostream& stream, const Ship& ship);


private:
    Health health_;
    bool is_horizontal_;
    Point top_left_;
};
