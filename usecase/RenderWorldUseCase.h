#pragma once

#include <QObject>
#include <worldprocessor.h>
#include <QColor>

class RenderWorldUseCase : public QObject {

    Q_OBJECT

public:
    enum CellTypes {
        EmptyCell = 0,
        WallCell,
        CreatureCell,
    };

public:
    RenderWorldUseCase(WorldProcessorPtr processor);

    size_t lenght() const;
    QColor getCellColor(size_t index) const;
    const WorldMap::WMap & map() const;

signals:
    void redrawWorld();

private slots:
    void worldDataReady(WorldMap::WMap map, WorldInfo info);

private:
    QColor cellToColor(const Cell & c) const;
    QColor similarityToColor (const Cell & c) const;

private:
    WorldProcessorPtr m_processor;
    WorldMap::WMap m_map;
    WorldInfo m_info;
};



using RenderWorldUseCaseUnq = std::unique_ptr<RenderWorldUseCase>;