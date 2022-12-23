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
        [](const Wall &) { return 4; },
        [&me](const CreatureA & c) {
            if (c.isAlive()) {
                if (me.similarTo(c)) {
                    return 2;
                } else {
                    return 3;
                }
            }
            return 1;
        },
        [](auto) { return 4; },
        };
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            return std::visit(CellToType, map->m_map.at(*iopt));
        }
    }
    return 4; // wall
}

uint SensorImpl::sunAmounnt(size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        return map->height() / (50*(index/map->width() + 1));
    } else {
        qWarning() << "map is empty";
    }
    return 0;
}


MotionImpl::MotionImpl(WorldMapWeak wmap)
    : m_mapWeak(wmap)
{

}

void MotionImpl::moveTo(Direction dir, size_t index)
{
    auto CellToType = Overload {
        [](const EmptySpace &) { return true; },
        [](auto) { return false; },
        };
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            if (std::visit(CellToType, map->m_map.at(*iopt))) {
                map->moveObject(index, *iopt);
            }
        }
    }

}

int MotionImpl::attack(Direction dir, size_t index)
{
    if (auto map = m_mapWeak.lock()) {
        NearestSpace ns(index, map->width(), map->height());
        if (auto iopt = ns.go(dir)) {
            if (std::holds_alternative<CreatureA>(map->m_map.at(*iopt))) {
                auto c = std::get<CreatureA>(map->m_map.at(*iopt));
                map->moveObject(index, *iopt);
                return c.getEnergy();
            }
        }
    }
    return 0;
}


CreatureCreatorImlp::CreatureCreatorImlp(WorldMapWeak wmap)
    : m_mapWeak(wmap)
{

}

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


