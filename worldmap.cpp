#include "worldmap.h"
#include "WorldGeometry.h"
#include <QRandomGenerator>
#include <QDebug>

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

WorldMap::WorldMap(size_t width, size_t height)
    : m_width(width)
    , m_height(height)
{
    Q_ASSERT(m_width > 0 && m_height > 0);
    m_map = WMap(m_width * m_height, {EmptySpace()});
}

//void WorldMap::addWall(size_t ind) {
//    Q_ASSERT(ind < m_width * m_height);
//    m_map.at(ind) = Wall();
//}

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

void WorldMap::setSunLevel(int sunLevel)
{
    m_sunLevel = sunLevel;
}

int WorldMap::getSunLevel() const
{
    return m_sunLevel;
}

void WorldMap::setMinDivideEnergy(int newMinDivideEnergy)
{
    m_minDivideEnergy = newMinDivideEnergy;
}

void WorldMap::setMoveEnegrgy(int newMoveEnegrgy)
{
    m_moveEnegrgy = newMoveEnegrgy;
}

void WorldMap::setMaxCreatureEnergy(int newMaxCreatureEnergy)
{
    m_maxCreatureEnergy = newMaxCreatureEnergy;
}

void WorldMap::setMutationRate(uint newMutationRate)
{
    m_mutationRate = newMutationRate;
}

uint8_t WorldMap::whoIsThereOffset(Direction dir, size_t index, const CreatureA &me)
{
    auto CellToType = Overload {
        [](const Wall &) { return 3; },
        [&me](const CreatureA & c) {
            if (c.isAlive()) {
                if (me.similarTo(c)) {
                    return 1;
                }
            }
            return 2;
        },
        [](auto) { return 0; },
        };
    NearestSpace ns(index, m_width, m_height);
    if (auto iopt = ns.go(dir)) {
        return std::visit(CellToType, m_map.at(*iopt));
    }
    return 3; // wall
}

int WorldMap::sunAmounnt(size_t index)
{
    return (m_sunLevel*(m_height - index/m_width))/m_height;
}

uint WorldMap::myLevel(size_t index)
{
    return (64 * index/m_width)/m_height;
}

bool WorldMap::lookAround(size_t index)
{
    return !!findFreeSpace(index);
}

void WorldMap::moveTo(Direction dir, size_t index)
{
    NearestSpace ns(index, m_width, m_height);
    if (auto iopt = ns.go(dir)) {
        if (std::holds_alternative<EmptySpace> (m_map.at(*iopt))) {
            moveObject(index, *iopt);
        }
    }
}

int WorldMap::attack(Direction dir, size_t index)
{
    int energy = 0;
    NearestSpace ns(index, m_width, m_height);
    if (auto iopt = ns.go(dir)) {
        if (std::holds_alternative<CreatureA>(m_map.at(*iopt))) {
            auto c = std::get<CreatureA>(m_map.at(*iopt));
            energy = c.getEnergy();
            moveObject(index, *iopt);
        }
    }
    return energy;

}

void WorldMap::divideMe(CreatureA &creature, size_t index)
{
    if (auto empty = findFreeSpace(index)) {
        addCreature(creature.clone(), *empty);
    } else {
        creature.die();
    }
}

uint WorldMap::mutationRate() const
{
    return m_mutationRate;
}

int WorldMap::maxCreatureEnergy() const
{
    return m_maxCreatureEnergy;
}

int WorldMap::moveEnergy() const
{
    return m_moveEnegrgy;
}

int WorldMap::minDivideEnergy() const
{
    return m_minDivideEnergy;
}
