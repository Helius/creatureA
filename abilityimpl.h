#pragma once

#include <Abilities.h>
#include <worldmap.h>

class SensorImpl : public ISensor {
public:
    SensorImpl(WorldMapWeak wmap);
    // ISensor interface
public:
    uint8_t whoIsThereOffset(Direction dir, size_t index, const CreatureA & me) override;
    uint sunAmounnt(size_t index) override;
private:
    WorldMapWeak m_mapWeak;
};


class MotionImpl : public IMotion {
public:
    MotionImpl(WorldMapWeak wmap);
    // IMotion interface
public:
    void moveTo(Direction dir, size_t index) override;
    int attack(Direction dir, size_t index) override;
private:
    WorldMapWeak m_mapWeak;
};


class CreatureCreatorImlp : public IDivider {
public:
    CreatureCreatorImlp(WorldMapWeak wmap);
    // IDivider interface
public:
    void divideMe(CreatureA &creature, size_t index) override;
private:
    WorldMapWeak m_mapWeak;
};