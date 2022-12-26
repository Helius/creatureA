#pragma once

#include <array>
#include <variant>
#include <memory>
#include <QtGlobal>
#include "creature.h"


//struct WorldFeatures {
//    uint
//};

struct EmptySpace {
};

struct Wall {
};

using Cell = std::variant<EmptySpace, Wall, CreatureA>;

class WorldMap : public IWorldEnv
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

    // IWorldEnv interface
public:
    uint8_t whoIsThereOffset(Direction dir, size_t index, const CreatureA &me) override;
    uint sunAmounnt(size_t index) override;
    uint myLevel(size_t index) override;
    bool lookAround(size_t index) override;
    void moveTo(Direction dir, size_t index) override;
    int attack(Direction dir, size_t index) override;
    void divideMe(CreatureA &creature, size_t index) override;

    uint mutationRate() const override;
    uint maxCreatureEnergy() const override;
    uint moveEnergy() const override;
    uint minDivideEnergy() const override;
    uint getSunLevel() const override;

    void setMutationRate(uint newMutationRate);
    void setMaxCreatureEnergy(uint newMaxCreatureEnergy);
    void setMoveEnegrgy(uint newMoveEnegrgy);
    void setMinDivideEnergy(uint newMinDivideEnergy);
    void setSunLevel(uint sunLevel);

    using WMap = std::vector<Cell>;
    WMap m_map;
private:
    size_t m_width = 0;
    size_t m_height = 0;
    uint m_sunLevel = 10;
    uint m_mutationRate = 25;
    uint m_maxCreatureEnergy = 1000;
    uint m_moveEnegrgy = 1;
    uint m_minDivideEnergy = 100;

};

using WorldMapWeak = std::weak_ptr<WorldMap>;
using WorldMapPtr = std::shared_ptr<WorldMap>;