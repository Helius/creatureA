#include "WorldControllerPresenter.h"


WorldControllerPresenter::WorldControllerPresenter(
    IWorldControllerPtr controller)
    : QObject()
    , m_controller(controller)
{
    Q_ASSERT(m_controller);
    //TODO: restore values from settings with default values
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

int WorldControllerPresenter::mutationRate() const
{
    return m_mutationRate;
}

void WorldControllerPresenter::setMutationRate(int newMutationRate)
{
    if (m_mutationRate == newMutationRate)
        return;
    m_mutationRate = newMutationRate;
    m_controller->setMutationRate(m_mutationRate);

    emit mutationRateChanged();
}

int WorldControllerPresenter::divideEnergy() const
{
    return m_divideEnergy;
}

void WorldControllerPresenter::setDivideEnergy(int newDivideEnergy)
{
    if (m_divideEnergy == newDivideEnergy)
        return;
    m_divideEnergy = newDivideEnergy;
    m_controller->setDivideEnergy(m_divideEnergy);
    emit divideEnergyChanged();
}

int WorldControllerPresenter::moveEnergy() const
{
    return m_moveEnergy;
}

void WorldControllerPresenter::setMoveEnergy(int newMoveEnergy)
{
    if (m_moveEnergy == newMoveEnergy)
        return;
    m_moveEnergy = newMoveEnergy;
    m_controller->setMoveEnergy(m_moveEnergy);
    emit moveEnergyChanged();
}

int WorldControllerPresenter::maxEnergy() const
{
    return m_maxEnergy;
}

void WorldControllerPresenter::setMaxEnergy(int newMaxEnergy)
{
    if (m_maxEnergy == newMaxEnergy)
        return;
    m_maxEnergy = newMaxEnergy;
    m_controller->setMaxCreatureEnergy(m_maxEnergy);
    emit maxEnergyChanged();
}

int WorldControllerPresenter::worldSpeed() const
{
    return m_worldSpeed;
}

void WorldControllerPresenter::setWorldSpeed(int newWorldSpeed)
{
    if (m_worldSpeed == newWorldSpeed)
        return;
    m_worldSpeed = newWorldSpeed;
    m_controller->setWorldSpeed(m_worldSpeed);
    emit worldSpeedChanged();
}

int WorldControllerPresenter::showMode() const
{
    return m_showMode;
}

void WorldControllerPresenter::setShowMode(int newShowMode)
{
    if (m_showMode == newShowMode)
        return;
    m_showMode = newShowMode;
    switch (m_showMode) {
    case 0:
        m_controller->setShowMode(IWorldController::ShowMode::ShowPredators);
        break;
    case 1:
        m_controller->setShowMode(IWorldController::ShowMode::ShowEnergy);
        break;
    case 2:
        m_controller->setShowMode(IWorldController::ShowMode::ShowDivercity);
        break;

    }
    emit showModeChanged();
}
