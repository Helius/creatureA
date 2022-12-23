#pragma once
#include "ICreatureBuilder.h"
#include "worldmap.h"
#include "Abilities.h"

class CreatureBuilderImpl : public ICreatureBuilder {
public:
    CreatureBuilderImpl(WorldMapPtr wmap, IMotionPtr motion, ISensorPtr sensor, IDividerPtr divider);

    // ICreatureBuilder interface
public:
    void buildDefaultCreatures(size_t amount) override;

private:
    WorldMapWeak m_mapWeak;
    IMotionPtr m_motion;
    ISensorPtr m_sensor;
    IDividerPtr m_divider;
};