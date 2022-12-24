#pragma once

#include <QObject>
#include <worldprocessor.h>
#include <QColor>
#include <ICreatureBuilder.h>

class RenderWorldUseCase : public QObject {

    Q_OBJECT

public:
    enum CellTypes {
        EmptyCell = 0,
        WallCell,
        CreatureCell,
    };

public:
    RenderWorldUseCase(WorldProcessorPtr processor, ICreatureBuilderPtr builder);

    size_t lenght() const;
    QColor getCellColor(size_t index) const;
    const WorldMap::WMap & map() const;

signals:
    void redrawWorld();

private slots:
    void worldDataReady(WorldMap::WMap map);
private:
    WorldProcessorPtr m_processor;
    ICreatureBuilderPtr m_builder;
    WorldMap::WMap m_map;
};



using RenderWorldUseCaseUnq = std::unique_ptr<RenderWorldUseCase>;