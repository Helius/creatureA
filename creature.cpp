#include "creature.h"
#include <QRandomGenerator>


CreatureA::CreatureA(ISensorPtr sensor, IMotionPtr motion, IDividerPtr divider)
    : m_sensor(sensor)
    , m_motion(motion)
    , m_divider(divider)
{
    for (uint i = 0; i < m_gene.size(); ++i) {
        m_gene[i] = 25;
        if (i%16 == 0) {
            m_gene[i] = 56;
        }
    }
}

CreatureA CreatureA::clone()
{
    CreatureA other(m_sensor, m_motion, m_divider);

    auto generator = QRandomGenerator::global();

    // copy genes
    for (uint i = 0; i < m_gene.size(); ++i) {
        other.m_gene[i] = m_gene[i];
    }

    // sometimes modify it (10% probability)
    if (generator->generate() % 100 <= 33) {
        uint rndIndex = generator->generate() % m_gene.size();
        uint rndGene = generator->generate() % m_gene.size();
        other.m_gene[rndIndex] = rndGene;
    }

    other.m_direcrion.set(generator->generate());

    m_energy /= 2;
    other.m_energy = m_energy;

    m_childCount++;

    return other;
}

void CreatureA::process(size_t index)
{
    bool finish = false;
    int maxCount = 16;
    if (m_alive) {
        do {
            uint32_t pcNorm = pc % m_gene.size();
            if (auto cmd = m_commands.find(m_gene[pcNorm]); cmd != m_commands.end()) {
                finish = cmd->second(*this, index);
                ++pc;
            } else {
                pc = m_gene[pcNorm];
            }

            --maxCount;

            if (m_energy < 0) {
                die();
            }

        } while(!finish && maxCount);
    }
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

uint CreatureA::isPredatorIndex() const
{
    return m_attackCount;
}


bool CreatureA::isAlive() const
{
    return m_alive;
}

const std::map<uint, CreatureA::Command>CreatureA::m_commands = {
    // фотосинтез 1
    {25, [](CreatureA & c, size_t index)->bool {
         c.m_energy += c.m_sensor->sunAmounnt(index);
         if (c.m_energy > 1000) {
             c.m_energy = 1000;
         }
         ++c.m_photonCount;
//         if (c.m_energy > m_devideEnegryThresold) {
//             c.m_divider->divideMe(c, index);
//         }
         return true;
     }
    },
    // поcворот 1
    {1, [](CreatureA & c, size_t )->bool {
         c.m_direcrion.turnLeft();
         c.m_energy -= 10;
         return true;
     }
    },
    // ощупывание пр-ва 5, аргументы: переходы 0 - пусто, 1 - еда, 2 - свой, 3 - чужой, 4 - стена
    {48, [](CreatureA & c, size_t index)->bool {
         uint offset = c.m_sensor->whoIsThereOffset(c.m_direcrion, index, c);
         c.pc = c.pc + offset - 1;
         return false;
     }
    },
    // шаг в направлении direction
    {10, [](CreatureA & c, size_t index)->bool {
         c.m_motion->moveTo(c.m_direcrion, index);
         c.m_energy -= 10;
         return true;
     }
    },
    // атака
    {32, [](CreatureA & c, size_t index)->bool {
         c.m_energy += c.m_motion->attack(c.m_direcrion, index) - 10;
         c.m_attackCount = 1;
//         if (c.m_energy > m_devideEnegryThresold) {
//             c.m_divider->divideMe(c, index);
//         }
         return true;
     }
    },
    // делиться
    {56, [](CreatureA & c, size_t index)->bool {
         if (c.getEnergy() > 10) {
             c.m_divider->divideMe(c, index);
             return true;
         }
         return false;
     }
    },
    // на каком я уровне
    {13, [](CreatureA & c, size_t index)->bool {
         auto level = c.m_sensor->myLevel(index);
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
         auto level = (64*c.getEnergy())/1000;
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
         if(c.m_sensor->lookAround(index)) {
             c.pc += 0;
         } else {
             c.pc += 1;
         }
         return false;
     }
    },

};