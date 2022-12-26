#include "abilityimpl.h"
#include "creature.h"
#include <QtDebug>

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;


SensorImpl::SensorImpl(WorldMapWeak wmap)
    : m_mapWeak(wmap)
{
}

uint8_t SensorImpl::whoIsThereOffset(Direction dir, size_t index, const CreatureA & me)
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
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            return std::visit(CellToType, map->m_map.at(*iopt));
        }
    }
    return 3; // wall
}

uint SensorImpl::sunAmounnt(size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        return (map->getSunLevel()*(map->height() - index/map->width()))/map->height();
    }
    return 0;
}

uint SensorImpl::myLevel(size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        return (4*index/map->width())/map->height();
    }
    return 0;
}

bool SensorImpl::lookAround(size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        return !!map->findFreeSpace(index);
    }
    return false;
}


MotionImpl::MotionImpl(WorldMapWeak wmap)
    : m_mapWeak(wmap)
{
}

void MotionImpl::moveTo(Direction dir, size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            if (std::holds_alternative<EmptySpace> (map->m_map.at(*iopt))) {
                map->moveObject(index, *iopt);
            }
        }
    }
}

int MotionImpl::attack(Direction dir, size_t index)
{
    int energy = 0;
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            if (std::holds_alternative<CreatureA>(map->m_map.at(*iopt))) {
                auto c = std::get<CreatureA>(map->m_map.at(*iopt));
                energy = c.getEnergy();
                map->moveObject(index, *iopt);
            }
        }
    }
    return energy;
}


CreatureCreatorImlp::CreatureCreatorImlp(WorldMapWeak wmap)
    : m_mapWeak(wmap)
{}

void CreatureCreatorImlp::divideMe(CreatureA & parent, size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        if (auto empty = map->findFreeSpace(index)) {
            map->addCreature(parent.clone(), *empty);
        } else {
            parent.die();
        }
    }
}


