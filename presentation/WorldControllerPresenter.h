#pragma once

#include <QObject>

#include "IWorldController.h"

class WorldControllerPresenter : public QObject {

    Q_OBJECT

    Q_PROPERTY(int sunLevel READ sunLevel WRITE setSunLevel NOTIFY sunLevelChanged)
    Q_PROPERTY(int mutationRate READ mutationRate WRITE setMutationRate NOTIFY mutationRateChanged)
    Q_PROPERTY(int divideEnergy READ divideEnergy WRITE setDivideEnergy NOTIFY divideEnergyChanged)
    Q_PROPERTY(int moveEnergy READ moveEnergy WRITE setMoveEnergy NOTIFY moveEnergyChanged)
    Q_PROPERTY(int maxEnergy READ maxEnergy WRITE setMaxEnergy NOTIFY maxEnergyChanged)
    Q_PROPERTY(int worldSpeed READ worldSpeed WRITE setWorldSpeed NOTIFY worldSpeedChanged)
    Q_PROPERTY(int showMode READ showMode WRITE setShowMode NOTIFY showModeChanged)

public:
    WorldControllerPresenter(IWorldControllerPtr controller);

    int sunLevel() const;
    int mutationRate() const;
    int divideEnergy() const;
    int moveEnergy() const;
    int maxEnergy() const;

    int worldSpeed() const;
    void setWorldSpeed(int newWorldSpeed);

    int showMode() const;
    void setShowMode(int newShowMode);

public slots:
    void setSunLevel(int);
    void setMaxEnergy(int newMaxEnergy);
    void setMoveEnergy(int newMoveEnergy);
    void setDivideEnergy(int newDivideEnergy);
    void setMutationRate(int newMutationRate);

signals:
    void sunLevelChanged();
    void mutationRateChanged();
    void divideEnergyChanged();
    void moveEnergyChanged();
    void maxEnergyChanged();

    void worldSpeedChanged();

    void showModeChanged();

private:
    IWorldControllerPtr m_controller;
    int m_sunLevel = 5;
    int m_mutationRate = 25;
    int m_divideEnergy = 200;
    int m_moveEnergy = 5;
    int m_maxEnergy = 1000;
    int m_worldSpeed = 1;
    int m_showMode;
};

using WorldControllerPresenterUnq = std::unique_ptr<WorldControllerPresenter>;