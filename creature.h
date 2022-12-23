#pragma once

#include <array>
#include <functional>
#include <map>
#include "Abilities.h"
#include "Direction.h"

class CreatureA
{
public:
    CreatureA(ISensorPtr sensor, IMotionPtr motion, IDividerPtr divider);

    void process(size_t index);
    CreatureA clone();
    int32_t getEnergy() const;
    uint32_t childCount() const;
    bool isAlive() const;
    void die();
    bool similarTo(const CreatureA & c) const;
    uint isPredatorIndex() const;

private:
    std::array<uint8_t, 64> m_gene = {25};
    int32_t m_energy = 1000;
    uint32_t age = 0;
    uint32_t m_childCount = 0;
    uint32_t m_attackCount = 0;
    uint32_t m_photonCount = 0;
    uint pc = 0; // programm counter
    Direction m_direcrion;
    static constexpr int32_t m_devideEnegryThresold = 1000;
    ISensorPtr m_sensor;
    IMotionPtr m_motion;
    IDividerPtr m_divider;

    using Command = std::function<bool(CreatureA &, size_t index)>;
    static const std::map<uint, Command> m_commands;

    bool m_alive = true;

};

