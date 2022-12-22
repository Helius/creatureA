#include <CreatureBuilderImpl.h>
#include <QRandomGenerator>
#include <QDebug>

CreatureBuilderImpl::CreatureBuilderImpl(
    WorldMapWeak wmWeak,
    IMotionPtr motion,
    ISensorPtr sensor,
    IDividerPtr divider)
    : m_mapWeak(wmWeak)
    , m_motion(motion)
    , m_sensor(sensor)
    , m_divider(divider)
{
    Q_ASSERT(m_motion);
    Q_ASSERT(m_sensor);
    Q_ASSERT(m_divider);
}

void CreatureBuilderImpl::buildDefaultCreatures(size_t amount)
{
    if (auto map = m_mapWeak.lock()) {
        for (size_t i = 0; i < amount; ++i) {
            quint32 value = QRandomGenerator::global()->generate() % map->lenght();
            map->m_map[value] = CreatureA(m_sensor, m_motion, m_divider);
        }
    } else {
        qWarning() << "Cant create creatures: world is null";
    }
}
