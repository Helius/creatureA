#pragma once

#include <QObject>

#include "IWorldController.h"

class WorldControllerPresenter : public QObject {

    Q_OBJECT

    Q_PROPERTY(int sunLevel READ sunLevel WRITE setSunLevel NOTIFY sunLevelChanged)

public:
    WorldControllerPresenter(IWorldControllerPtr controller);

    int sunLevel() const;

public slots:
    void setSunLevel(int);

signals:
    void sunLevelChanged();

private:
    IWorldControllerPtr m_controller;
    int m_sunLevel;
};

using WorldControllerPresenterUnq = std::unique_ptr<WorldControllerPresenter>;