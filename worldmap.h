#pragma once

#include <array>
#include <variant>
#include "creature.h"
#include "Direction.h"

struct EmptySpace {
};

struct Wall {
};


template <std::size_t width, std::size_t height>
class WorldMap
{
public:

    size_t moveTo(size_t index, Direction dir) {

        if (dir == Direction::Up)
        {
            if (index >= width) {
                auto nInd = index - width;
                if (std::holds_alternative<EmptySpace>(m_map.at(nInd))) {
                    return nInd;
                }
            }
            return index;
        }
    }

private:
    using wmap = std::array<std::variant<CreatureA, Wall, EmptySpace>, width * height>;
    wmap m_map;
};