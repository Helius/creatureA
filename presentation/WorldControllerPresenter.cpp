#include "WorldControllerPresenter.h"


WorldControllerPresenter::WorldControllerPresenter(
    IWorldControllerPtr controller)
    : QObject()
    , m_controller(controller)
{
    Q_ASSERT(m_controller);
    m_sunLevel = m_controller->getSunLevel();
}

int WorldControllerPresenter::sunLevel() const
{
    return m_sunLevel;
}

void WorldControllerPresenter::setSunLevel(int sunLevel)
{
    if (m_sunLevel != sunLevel) {
        m_sunLevel = sunLevel;
        m_controller->setSunLevel(m_sunLevel);
        emit sunLevelChanged();
    }
}
