#pragma once

#include "worldmap.h"

#include <set>

#include <QObject>
#include <QThread>
#include <QTimer>

#include <IWorldController.h>


struct WorldInfo {
    size_t aliveCreatures = 0;
    size_t deadCreatures = 0;
    long long int totalEnegry = 0;
    int maxEnergyPerCreature = 0;
    size_t maxChildCount = 0;
    long long int attacCount = 0;
    long long int photonCount = 0;
    uint maxAge = 0;
};


class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject * parent);
public slots:
    void runWorker(WorldMapPtr map, size_t cycles, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy);
    void resetWorker(WorldMapPtr map, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy);
signals:
    void jobsDone(WorldInfo info);
private:
};



class WorldProcessor
    : public QObject
    , public IWorldController
{
    Q_OBJECT


public:
    WorldProcessor(WorldMapPtr);
    ~WorldProcessor();
    void run(size_t cycles);
//    void addWalls(std::set<size_t> indexes);
    const WorldMap::WMap & map() const {return m_map->m_map; };

    // IWorldController interface
public:
    void setSunLevel(int maxValue) override;
    void setPause(bool pause) override;
    bool isPaused() const override;
    void setMaxCreatureEnergy(int e) override;
    void setMutationRate(uint e) override;
    void setMoveEnergy(int e) override;
    void setDivideEnergy(int e) override;
    void setWorldSpeed(uint s) override;
    uint getMaxCreatureEnergy() const;
    void restart() override;
public slots:
    void addCreatures(size_t amount);

signals:
    void ready(WorldMap::WMap map, WorldInfo info);
    void runWorker(WorldMapPtr world, size_t cycles, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy);
    void resetWorker(WorldMapPtr world, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy);

private slots:
    void workerReady(WorldInfo info);

private:
    uint m_runCount = 0;
    int m_sunLevel = 10;
    int m_maxCreatureEnergy = 1000;
    uint m_mutationRate = 25;
    int m_moveEnergy = 10;
    int m_divideEnergy = 100;
    uint m_worldSpeed = 1;
    ShowMode m_showMode = ShowMode::ShowPredators;
    WorldMapPtr m_map;
    Worker * m_worker;
    QThread m_workerThread;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    bool m_inProgress = false;

    // IWorldController interface
public:
    void setShowMode(ShowMode mode) override;
    ShowMode showMode() const override;
};

using WorldProcessorPtr = std::shared_ptr<WorldProcessor>;