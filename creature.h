#pragma once
#include <array>
#include <Direction.h>
#include <Abilities.h>

class CreatureA
{
public:
    CreatureA(ISensorPtr sensor, IMotionPtr motion, IDividerPtr divider);
    bool receivePhotoEnergy(uint32_t amount);
private:
    std::array<uint8_t, 64> m_gene = {25};
    uint32_t m_energy = 0;
    uint32_t age = 0;
    static constexpr uint32_t m_devideEnegryThresold = 1000;
    ISensorPtr m_sensor;
    IMotionPtr m_motion;
    IDividerPtr m_divider;
};

