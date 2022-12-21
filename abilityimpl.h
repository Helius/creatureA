#pragma once

#include <Abilities.h>

class SensorImpl : public ISensor {
public:

    // ISensor interface
public:
    uint8_t whoIsThereOffset(Direction dir) override;
};

class MotionImpl : public IMotion {
public:

    // IMotion interface
public:
    void moveTo(Direction dir) override;
};

class DividerImlp : public IDivider {
public:

    // IDivider interface
public:
    void divideMe() override;
};