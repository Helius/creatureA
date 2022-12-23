#include "worldprocessor.h"
#include <QDebug>

WorldProcessor::WorldProcessor(WorldMapPtr worldMap)
    : QObject(nullptr)
    , m_map(worldMap)
    , m_worker(new Worker(nullptr))
{
    m_worker->moveToThread(&m_workerThread);
    connect(this, &WorldProcessor::runWorker, m_worker, &Worker::runWorker);
    connect(m_worker, &Worker::jobsDone, this, &WorldProcessor::workerReady);
    m_workerThread.start();
    qDebug() << "world processor, wmap:" << m_map.get();
}

WorldProcessor::~WorldProcessor()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void WorldProcessor::run(size_t cycles)
{
    m_runCount += cycles;
    emit runWorker(m_map, cycles);

}

void WorldProcessor::addWalls(std::set<size_t> indexes)
{
    for (const auto & ind: indexes) {
        m_map->addWall(ind);
    }
}

void WorldProcessor::addCreatures()
{

}

void WorldProcessor::workerReady(WorldInfo info)
{
    qDebug() << "creatures: alive/dead" << info.aliveCreatures << "/" << info.deadCreatures
             << ", energy max/total" << info.maxEnergyPerCreature << "/" << info.totalEnegry
             << ", max child" << info.maxChildCount;
    emit ready(m_map->m_map);
}

//==============================

Worker::Worker(QObject *parent)
    : QObject(parent)
{

}

void Worker::runWorker(WorldMapPtr map, size_t cycles)
{
    WorldInfo info;

    for (size_t i = 0; i < cycles; ++i) {
        size_t index = 0;
        for (auto & variant : map->m_map) {
            if (auto * creature = std::get_if<CreatureA>(&variant)) {
                creature->process(index);
                if (i == cycles - 1) {
                    if (creature->isAlive()) {
                        info.aliveCreatures++;
                    } else {
                        info.deadCreatures ++;
                    }

                    info.totalEnegry += creature->getEnergy();

                    if (creature->childCount() > info.maxChildCount) {
                        info.maxChildCount = creature->childCount();
                    }
                    if (creature->getEnergy() > info.maxEnergyPerCreature) {
                        info.maxEnergyPerCreature = creature->getEnergy();
                    }
                }
            }
            ++index;
        }
    }
    emit jobsDone(info);
}
