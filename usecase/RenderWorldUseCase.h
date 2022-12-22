#pragma once

#include <QObject>
#include <worldprocessor.h>
#include <QColor>
#include <ICreatureBuilder.h>

class RenderWorldUseCase : public QObject {

public:
    enum CellTypes {
        EmptyCell = 0,
        WallCell,
        CreatureCell,
    };

public:
    RenderWorldUseCase(WorldProcessorUnq processor, ICreatureBuilderPtr builder);
    size_t lenght() const;
    const Cell & get(size_t index) const;
    QColor getCellColor(size_t index) const;
private:
    WorldProcessorUnq m_processor;
    ICreatureBuilderPtr m_builder;
};



using RenderWorldUseCaseUnq = std::unique_ptr<RenderWorldUseCase>;