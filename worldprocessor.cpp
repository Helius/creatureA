#include "worldprocessor.h"
#include <QRandomGenerator>
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
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        if (!m_inProgress) {
            run(m_cycles);
        }
    });
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
    m_timer.start();
}

WorldProcessor::~WorldProcessor()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void WorldProcessor::run(size_t cycles)
{
    m_inProgress = true;
    m_runCount += cycles;
    emit runWorker(m_map, cycles, m_sunLevel);
}

//void WorldProcessor::addWalls(std::set<size_t> indexes)
//{
//    for (const auto & ind: indexes) {
//        m_map->addWall(ind);
//    }
//}

void WorldProcessor::addCreatures(size_t amount)
{
    while(amount--) {
        CreatureA c(*m_map);
        m_map->addCreature(c, QRandomGenerator::global()->generate()%m_map->width());
    }
}


void WorldProcessor::setSunLevel(uint maxValue)
{
    m_sunLevel = maxValue;
}

uint WorldProcessor::getSunLevel()
{
    return m_sunLevel;
}

void WorldProcessor::setPause(bool pause)
{
    if (pause) {
        m_timer.stop();
    } else {
        m_timer.start();
    }
}

bool WorldProcessor::isPaused() const
{
    return m_timer.isActive();
}

void WorldProcessor::workerReady(WorldInfo info)
{
    qDebug() << "creatures: alive/dead" << info.aliveCreatures << "/" << info.deadCreatures
             << ", energy max/total" << info.maxEnergyPerCreature << "/" << info.totalEnegry
             << ", max child" << info.maxChildCount
             << "total attac/photon" << info.attacCount << info.photonCount;
    emit ready(m_map->m_map, info);
    m_inProgress = false;
}

uint WorldProcessor::cycles() const
{
    return m_cycles;
}

void WorldProcessor::setCycles(uint newCycles)
{
    m_cycles = newCycles;
}

//==============================

Worker::Worker(QObject *parent)
    : QObject(parent)
{

}

void Worker::runWorker(WorldMapPtr map, size_t cycles, uint sunLevel)
{
    WorldInfo info;

    map->setSunLevel(sunLevel);

    for (size_t i = 0; i < cycles; ++i) {
        size_t index = 0;

        for (auto & v : map->m_map) {
            if (auto * creature = std::get_if<CreatureA>(&v)) {
                creature->markProcessed(false);
            }
        }
        for (auto & variant : map->m_map) {
            if (auto * creature = std::get_if<CreatureA>(&variant)) {
                if (!creature->isProcessed()) {
                    creature->markProcessed(true);
                    creature->process(index);

                    if (i == cycles - 1) {
                        if (creature->isAlive()) {
                            info.aliveCreatures++;
                        } else {
                            info.deadCreatures ++;
                        }

                        info.totalEnegry += creature->getEnergy();
                        info.attacCount += creature->attackCount();
                        info.photonCount += creature->photoneCount();

                        if (creature->childCount() > info.maxChildCount) {
                            info.maxChildCount = creature->childCount();
                        }
                        if (creature->getEnergy() > info.maxEnergyPerCreature) {
                            info.maxEnergyPerCreature = creature->getEnergy();
                        }
                    }
                }
            }
            ++index;
        }
    }
    emit jobsDone(info);
}
