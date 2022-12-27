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
            run(m_worldSpeed);
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
    emit runWorker(m_map, cycles, m_sunLevel, m_maxCreatureEnergy, m_mutationRate, m_moveEnergy, m_divideEnergy);
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


void WorldProcessor::setSunLevel(int maxValue)
{
    m_sunLevel = maxValue;
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

void WorldProcessor::setMaxCreatureEnergy(int e)
{
    m_maxCreatureEnergy = e;
}

void WorldProcessor::setMutationRate(uint e)
{
    m_mutationRate = e;
}

void WorldProcessor::setMoveEnergy(int e)
{
    m_moveEnergy = e;
}

void WorldProcessor::setDivideEnergy(int e)
{
    m_divideEnergy = e;
}

void WorldProcessor::setWorldSpeed(uint s)
{
    m_worldSpeed = s;
}

uint WorldProcessor::getMaxCreatureEnergy() const
{
    return m_maxCreatureEnergy;
}

void WorldProcessor::restart()
{
    emit resetWorker(m_map, m_sunLevel, m_maxCreatureEnergy, m_mutationRate, m_moveEnergy, m_divideEnergy);
}

void WorldProcessor::workerReady(WorldInfo info)
{
    qDebug() << "creatures: alive/dead" << info.aliveCreatures << "/" << info.deadCreatures
             << "energy max/total" << info.maxEnergyPerCreature << "/" << info.totalEnegry
             << "max child" << info.maxChildCount
             << "total attac/photon" << info.attacCount << info.photonCount << "rate:" << static_cast<double>(info.attacCount)/static_cast<double>(info.photonCount)
             << "max age" << info.maxAge;
    emit ready(m_map->m_map, info);
    m_inProgress = false;
}

void WorldProcessor::setShowMode(ShowMode mode)
{
    m_showMode = mode;
}

IWorldController::ShowMode WorldProcessor::showMode() const
{
    return m_showMode;
}

//==============================

Worker::Worker(QObject *parent)
    : QObject(parent)
{

}

void Worker::runWorker(WorldMapPtr map, size_t cycles, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy)
{
    WorldInfo info;

    map->setSunLevel(sunLevel);
    map->setMaxCreatureEnergy(maxEnergy);
    map->setMutationRate(mutationRate);
    map->setMoveEnegrgy(moveEnergy);
    map->setMinDivideEnergy(minDivideEnergy);

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
                        if (creature->getAge() > info.maxAge) {
                            info.maxAge = creature->getAge();
                        }
                    }
                }
            }
            ++index;
        }
    }
    emit jobsDone(info);
}

void Worker::resetWorker(WorldMapPtr map, uint sunLevel, uint maxEnergy, uint mutationRate, uint moveEnergy, uint minDivideEnergy)
{
    map->setSunLevel(sunLevel);
    map->setMaxCreatureEnergy(maxEnergy);
    map->setMutationRate(mutationRate);
    map->setMoveEnegrgy(moveEnergy);
    map->setMinDivideEnergy(minDivideEnergy);

    for (auto & v : map->m_map) {
        v = {EmptySpace()};
    }
    int amount = 10;
    while(amount--) {
        CreatureA c(*map);
        map->addCreature(c, QRandomGenerator::global()->generate()%map->width());
    }
}
