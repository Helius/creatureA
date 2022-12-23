#pragma once

#include <QObject>
#include <worldprocessor.h>
#include <QColor>
#include <ICreatureBuilder.h>
#include <QTimer>

class RenderWorldUseCase : public QObject {

    Q_OBJECT

public:
    enum CellTypes {
        EmptyCell = 0,
        WallCell,
        CreatureCell,
    };

public:
    RenderWorldUseCase(WorldProcessorUnq processor, ICreatureBuilderPtr builder);

    size_t lenght() const;
    QColor getCellColor(size_t index) const;
//    QColor getBorderColor(size_t index) const;
    const WorldMap::WMap & map() const;

signals:
    void redrawWorld();

private slots:
    void updateWorld();
    void worldDataReady(const WorldMap::WMap &map);
private:
    WorldProcessorUnq m_processor;
    ICreatureBuilderPtr m_builder;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    WorldMap::WMap m_map;
};



using RenderWorldUseCaseUnq = std::unique_ptr<RenderWorldUseCase>;