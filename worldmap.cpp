#include "worldmap.h"
#include "WorldGeometry.h"
#include <QRandomGenerator>
#include <QDebug>

WorldMap::WorldMap(size_t width, size_t height)
    : m_width(width)
    , m_height(height)
{
    Q_ASSERT(m_width > 0 && m_height > 0);
    m_map = WMap(m_width * m_height, {EmptySpace()});
}

void WorldMap::addWall(size_t ind) {
    Q_ASSERT(ind < m_width * m_height);
    m_map.at(ind) = Wall();
}

void WorldMap::addCreature(CreatureA creature, size_t ind)
{
    Q_ASSERT(ind < m_width * m_height);
    m_map[ind] = { creature };

}

void WorldMap::moveObject(size_t from, size_t to)
{
    Q_ASSERT(from < m_width * m_height);
    Q_ASSERT(to < m_width * m_height);
    m_map[to] = m_map[from];
    m_map[from] = {EmptySpace()};
}

std::optional<size_t> WorldMap::findFreeSpace(size_t ind)
{
    Q_ASSERT(ind < m_width * m_height);

    Direction dir;
    dir.set(QRandomGenerator::global()->generate());
    NearestSpace ns(ind, m_width, m_height);
    for (uint i = 0; i < Direction::maxNumber; ++i) {
        if (auto newInd = ns.go(dir)) {
            if (std::holds_alternative<EmptySpace>(m_map.at(*newInd))) {
                return newInd;
            }
        }
        dir.turnRight();
    }
    return {};
}

void WorldMap::setSunLevel(uint sunLevel)
{
    m_sunLevel = sunLevel;
}

uint WorldMap::getSunLevel() const
{
    return m_sunLevel;
}
