#pragma once

#include <array>
#include <functional>
#include <map>
#include <WorldGeometry.h>
#include <variant>

class CreatureA;

struct EmptySpace {
};

struct Wall {
};

using Cell = std::variant<EmptySpace, Wall, CreatureA>;

class IWorldEnv {
public:
    virtual ~IWorldEnv() = default;

    virtual uint8_t whoIsThereOffset(Direction dir, size_t index, const CreatureA & me) = 0;
    virtual int sunAmounnt(size_t index) = 0;
    virtual uint myLevel(size_t index) = 0;
    virtual bool lookAround(size_t index) = 0;

    virtual size_t moveTo(Direction dir, size_t index) = 0;
    virtual int attack(Direction dir, size_t & index) = 0;
    virtual Cell & getCell(size_t index) = 0;

    virtual void divideMe(CreatureA & creature, size_t fromIndex) = 0;

    virtual uint mutationRate() const = 0;
    virtual int maxCreatureEnergy() const = 0;
    virtual int moveEnergy() const = 0;
    virtual int minDivideEnergy() const = 0;
    virtual int getSunLevel() const = 0;
};

class CreatureA
{
public:
    CreatureA(IWorldEnv & wenv);

    void process(size_t index);
    CreatureA clone();
    int getEnergy() const;
    uint childCount() const;
    bool isAlive() const;
    void die();
    bool similarTo(const CreatureA & c) const;
    int attackCount() const;
    int photoneCount() const;
    void markProcessed(bool);
    bool isProcessed() const;
    int getDivercity() const;
    uint getAge() const;

private:
    std::array<uint8_t, 64> m_gene = {25};
    int m_energy = 500;
    uint m_age = 0;
    uint m_childCount = 0;
    int m_attackCount = 0;
    int m_photonCount = 0;
    uint pc = 0; // programm counter
    static constexpr int32_t m_devideEnegryThresold = 500;
    Direction m_direcrion;

    IWorldEnv * m_wenv;

    using Command = std::function<bool(CreatureA &, size_t index)>;
    static const std::map<uint, Command> m_commands;

    bool m_alive = true;
    bool m_processed = false;

};

