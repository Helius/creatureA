#pragma once

#include <memory>
#include <Direction.h>

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual uint8_t whoIsThereOffset(Direction dir) = 0;
};

using ISensorPtr = std::shared_ptr<ISensor>;

class IMotion {
public:
    virtual ~IMotion() = default;
    virtual void moveTo(Direction dir) = 0;
};

using IMotionPtr = std::shared_ptr<IMotion>;

class IDivider {
public:
    virtual ~IDivider() = default;
    virtual void divideMe() = 0;
};

using IDividerPtr = std::shared_ptr<IDivider>();