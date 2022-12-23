#pragma once

#include "worldmap.h"
#include <QObject>
#include <QThread>
#include <set>


struct WorldInfo {
    size_t aliveCreatures = 0;
    size_t deadCreatures = 0;
    size_t totalEnegry = 0;
    size_t maxEnergyPerCreature = 0;
    size_t maxChildCount = 0;
};


class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject * parent);
public slots:
    void runWorker(WorldMapPtr map, size_t cycles);
signals:
    void jobsDone(WorldInfo info);
private:
};



class WorldProcessor : public QObject
{
    Q_OBJECT
public:
    WorldProcessor(WorldMapPtr);
    ~WorldProcessor();
    void run(size_t cycles);
    void addWalls(std::set<size_t> indexes);
    void addCreatures();
    const WorldMap::WMap & map() const {return m_map->m_map; };
signals:
    void ready(const WorldMap::WMap & map);
    void runWorker(WorldMapPtr world, size_t cycles);
private slots:
    void workerReady(WorldInfo info);
private:
    size_t m_runCount = 0;
    WorldMapPtr m_map;
    Worker * m_worker;
    QThread m_workerThread;
};

using WorldProcessorUnq = std::unique_ptr<WorldProcessor>;