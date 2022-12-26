#pragma once

#include <array>
#include <variant>
#include <QtGlobal>
#include "creature.h"


struct EmptySpace {
};

struct Wall {
};

using Cell = std::variant<EmptySpace, Wall, CreatureA>;

class WorldMap
{
public:
    WorldMap() = delete;
    WorldMap(size_t width, size_t height);
    size_t width() const {return m_width; }
    size_t height() const {return m_height; }
    size_t lenght() const { return m_width * m_height; }
//    void addWall(size_t ind);
    void addCreature(CreatureA creature, size_t ind);
    void moveObject(size_t from, size_t to);
    std::optional<size_t> findFreeSpace(size_t ind);
    void setSunLevel(uint sunLevel);
    uint getSunLevel() const;

    using WMap = std::vector<Cell>;
    WMap m_map;
private:
    size_t m_width = 0;
    size_t m_height = 0;
    uint m_sunLevel = 10;
};

using WorldMapWeak = std::weak_ptr<WorldMap>;
using WorldMapPtr = std::shared_ptr<WorldMap>;