#include "creature.h"

Creature::Creature()
{

}

bool Creature::receivePhotoEnergy(uint32_t amount)
{
    m_energy += amount;
    if (m_energy > m_devideEnegryThresold) {
        return true;
    }
    return false;
}
