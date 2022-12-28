#pragma once

#include <array>
#include <variant>
#include <memory>
#include <QtGlobal>
#include "creature.h"


//struct WorldFeatures {
//    uint
//};


class WorldMap : public IWorldEnv
{
public:
    WorldMap() = delete;
    WorldMap(size_t width, size_t height);
    size_t width() const {return m_width; }
    size_t height() const {return m_height; }
    size_t lenght() const { return m_width * m_height; }
//    void addWall(size_t ind);
    void addCreature(const CreatureA &creature, size_t ind);
    void moveObject(size_t from, size_t to);
    std::optional<size_t> findFreeSpace(size_t ind);

    // IWorldEnv interface
public:
    uint8_t whoIsThereOffset(Direction dir, size_t index, const CreatureA &me) override;
    int sunAmounnt(size_t index) override;
    uint myLevel(size_t index) override;
    bool lookAround(size_t index) override;
    size_t moveTo(Direction dir, size_t index) override;
    int attack(Direction dir, size_t & index) override;
    void divideMe(CreatureA &creature, size_t index) override;

    uint mutationRate() const override;
    int maxCreatureEnergy() const override;
    int moveEnergy() const override;
    int minDivideEnergy() const override;
    int getSunLevel() const override;

    void setMutationRate(uint newMutationRate);
    void setMaxCreatureEnergy(int newMaxCreatureEnergy);
    void setMoveEnegrgy(int newMoveEnegrgy);
    void setMinDivideEnergy(int newMinDivideEnergy);
    void setSunLevel(int sunLevel);

    using WMap = std::vector<Cell>;
    WMap m_map;
private:
    size_t m_width = 0;
    size_t m_height = 0;
    int m_sunLevel = 10;
    uint m_mutationRate = 25;
    int m_maxCreatureEnergy = 1000;
    int m_moveEnegrgy = 1;
    int m_minDivideEnergy = 100;


    // IWorldEnv interface
public:
    Cell &getCell(size_t index) override;
};

using WorldMapWeak = std::weak_ptr<WorldMap>;
using WorldMapPtr = std::shared_ptr<WorldMap>;