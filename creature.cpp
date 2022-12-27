#include "creature.h"
#include <cmath>
#include <QRandomGenerator>
#include <QDebug>



CreatureA::CreatureA(IWorldEnv & wenv)
    : m_wenv(&wenv)
{
    for (uint i = 0; i < m_gene.size(); ++i) {
        m_gene[i] = 25;
//        if (i%16 == 0) {
//            m_gene[i] = 56;
//        }
    }
}

CreatureA CreatureA::clone()
{
    CreatureA other(*m_wenv);

    auto generator = QRandomGenerator::global();

    // copy genes
    for (uint i = 0; i < m_gene.size(); ++i) {
        other.m_gene[i] = m_gene[i];
    }

    // sometimes modify it (10% probability)
    if (generator->generate() % 100 <= m_wenv->mutationRate()) {
        uint rndIndex = generator->generate() % m_gene.size();
        uint rndGene = generator->generate() % m_gene.size();
        other.m_gene[rndIndex] = rndGene;
    }

    other.m_direcrion.set(generator->generate());

    m_energy /= 2;
    other.m_energy = m_energy;

    if (m_attackCount + m_photonCount > 1000) {
        m_attackCount /= 8;
        m_photonCount /= 8;
    }

    other.m_attackCount = m_attackCount;
    other.m_photonCount = m_photonCount;

    m_childCount++;

    return other;
}

int32_t CreatureA::getEnergy() const
{
    return m_energy;
}

uint32_t CreatureA::childCount() const
{
    return m_childCount;
}

void CreatureA::die()
{
    m_alive = false;
}

bool CreatureA::similarTo(const CreatureA &c) const
{
    uint diff = 0;
    for(uint i = 0; i < m_gene.size(); ++i)
    {
        if (m_gene.at(i) != c.m_gene.at(i)) {
            ++diff;
            if (diff > 1)
                return false;
        }
    }
    return true;
}

int CreatureA::attackCount() const
{
    return m_attackCount;
}

int CreatureA::photoneCount() const
{
    return m_photonCount;
}

void CreatureA::markProcessed(bool processed)
{
    m_processed = processed;
}

bool CreatureA::isProcessed() const
{
    return m_processed;
}

int CreatureA::getDivercity() const
{
    int difference = 0;
    for (uint i = 0; i < m_gene.size(); ++i) {
        difference += 25 - m_gene[i];
    }
    return difference;
}

uint CreatureA::getAge() const
{
    return m_age;
}


bool CreatureA::isAlive() const
{
    return m_alive;
}

void CreatureA::process(size_t index)
{
    bool finish = false;
    int maxCount = 8;
    ++m_age;
    if (m_alive) {
        do {
            uint32_t pcNorm = pc % m_gene.size();
            if (auto cmd = m_commands.find(m_gene[pcNorm]); cmd != m_commands.end()) {
                finish = cmd->second(*this, index);
                ++pc;
            } else {
                pc += m_gene[pcNorm];
            }

            --maxCount;

            if (m_energy < 0) {
                die();
            }

        } while(!finish && maxCount);
    }
}

const std::map<uint, CreatureA::Command>CreatureA::m_commands = {
    // поcворот 1
    {1, [](CreatureA & c, size_t )->bool {
         c.m_direcrion.turnLeft();
         c.m_energy -= c.m_wenv->moveEnergy();
         return true;
     }
    },
    // поcворот 2
    {5, [](CreatureA & c, size_t )->bool {
         c.m_direcrion.turnRight();
         c.m_energy -= c.m_wenv->moveEnergy();
         return true;
     }
    },
    // ощупывание пр-ва 5, аргументы: переходы 0 - пусто, 1 - свой, 2 - чужой, 3 - стена
    {48, [](CreatureA & c, size_t index)->bool {
         uint offset = c.m_wenv->whoIsThereOffset(c.m_direcrion, index, c);
         c.pc += offset;
         return false;
     }
    },
    // шаг в направлении direction
    {10, [](CreatureA & c, size_t index)->bool {
         c.m_wenv->moveTo(c.m_direcrion, index);
         c.m_energy -= c.m_wenv->moveEnergy();
         return true;
     }
    },
    // фотосинтез 1
    {25, [](CreatureA & c, size_t index)->bool {
         auto e = c.m_wenv->sunAmounnt(index);
         c.m_energy += e;
         c.m_photonCount += e;
         if (c.m_energy > static_cast<int>(c.m_wenv->maxCreatureEnergy())) {
             c.m_wenv->divideMe(c, index);
         }
         return true;
     }
    },
    // атака
    {32, [](CreatureA & c, size_t index)->bool {
         int e = c.m_wenv->attack(c.m_direcrion, index);
         c.m_energy += e - c.m_wenv->moveEnergy();
         c.m_attackCount += e;
         if (c.m_energy > static_cast<int>(c.m_wenv->maxCreatureEnergy())) {
             c.m_wenv->divideMe(c, index);
         }
         return true;
     }
    },
    // делиться
    {56, [](CreatureA & c, size_t index)->bool {
         if (c.getEnergy() > static_cast<int>(c.m_wenv->minDivideEnergy())) {
             c.m_wenv->divideMe(c, index);
             return true;
         }
         return false;
     }
    },
    // на каком я уровне
    {13, [](CreatureA & c, size_t index)->bool {
         auto level = c.m_wenv->myLevel(index);
         if (level < c.m_gene[c.pc+1]) {
             c.pc += 1;
         } else {
             c.pc += 2;
         }
         return false;
     }
    },
    // какая моя энергия
    {40, [](CreatureA & c, size_t)->bool {
         auto level = (64*c.getEnergy())/c.m_wenv->maxCreatureEnergy();
         if (level < c.m_gene[c.pc+1]) {
             c.pc += 1;
         } else {
             c.pc += 2;
         }
         return false;
     }
    },
    // окружен ли я?
    {62, [](CreatureA & c, size_t index)->bool {
         if(c.m_wenv->lookAround(index)) {
             c.pc += 0;
         } else {
             c.pc += 1;
         }
         return false;
     }
    },

};