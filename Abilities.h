#pragma once

#include <memory>
#include "WorldGeometry.h"

class CreatureA;

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual uint8_t whoIsThereOffset(Direction dir, size_t index, const CreatureA & me) = 0;
    virtual uint sunAmounnt(size_t index) = 0;
    virtual uint myLevel(size_t index) = 0;
    virtual bool lookAround(size_t index) = 0;
};

using ISensorPtr = std::shared_ptr<ISensor>;

class IMotion {
public:
    virtual ~IMotion() = default;
    virtual void moveTo(Direction dir, size_t index) = 0;
    virtual int attack(Direction dir, size_t index) = 0;
};

using IMotionPtr = std::shared_ptr<IMotion>;

class IDivider {
public:
    virtual ~IDivider() = default;
    virtual void divideMe(CreatureA & creature, size_t fromIndex) = 0;
};

using IDividerPtr = std::shared_ptr<IDivider>;