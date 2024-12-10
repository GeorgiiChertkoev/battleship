#pragma once
#include <unordered_map>
#include <optional>

#include "lib/field/field.h"


class BaseStrategy {
public:
    virtual Point Shoot(Field& field) = 0;
    virtual ~BaseStrategy() = default;
};

class OrderedStrategy : public BaseStrategy {
    Point Shoot(Field& field) override;
};

class CustomStrategy : public BaseStrategy {
    Point Shoot(Field& field) override;
};