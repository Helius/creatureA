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
    size_t totalEnegry = 0;
    size_t maxEnergyPerCreature = 0;
    size_t maxChildCount = 0;
    size_t attacCount = 0;
    size_t photonCount = 0;
};


class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject * parent);
public slots:
    void runWorker(WorldMapPtr map, size_t cycles, uint sunLevel);
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
    void addCreatures();
    const WorldMap::WMap & map() const {return m_map->m_map; };

    // IWorldController interface
public:
    void setSunLevel(uint maxValue) override;
    uint getSunLevel() override;
    void setPause(bool pause) override;
    bool isPaused() const override;

    uint cycles() const;
    void setCycles(uint newCycles);

signals:
    void ready(WorldMap::WMap map, WorldInfo info);
    void runWorker(WorldMapPtr world, size_t cycles, uint sunLevel);
private slots:
    void workerReady(WorldInfo info);
private:
    uint m_runCount = 0;
    uint m_cycles = 10;
    uint m_sunLevel = 10;
    WorldMapPtr m_map;
    Worker * m_worker;
    QThread m_workerThread;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    bool m_inProgress = false;
};

using WorldProcessorPtr = std::shared_ptr<WorldProcessor>;