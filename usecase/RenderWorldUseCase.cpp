#include "RenderWorldUseCase.h"
#include <QDebug>

namespace {

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;


} // namespase

RenderWorldUseCase::RenderWorldUseCase(WorldProcessorPtr processor)
    : QObject()
    , m_processor(processor)
{
    Q_ASSERT(m_processor);
    connect(m_processor.get(), &WorldProcessor::ready,
        this, &RenderWorldUseCase::worldDataReady);
    m_processor->addCreatures(10);
}

size_t RenderWorldUseCase::lenght() const
{
    return m_map.size();
}

QColor RenderWorldUseCase::getCellColor(size_t index) const
{
    auto CellEnergyToColor = Overload {
        [](const Wall &) { return QColor(0xff,0xff,0xff,0xff); },
        [this](const CreatureA & c) {
            return QColor(0xff,0,0,(c.getEnergy()*255)/m_processor->getMaxCreatureEnergy());
        },
        [](auto) { return QColor(0xff,0xff,0xff,0xff); },
        };

    if (index >= m_map.size()) {
        return QColor();
    }
    const auto & cell = m_map.at(index);
    try {
        switch (m_processor->showMode()) {
        case WorldProcessor::ShowMode::ShowPredators:
            return cellToColor(cell);
        case WorldProcessor::ShowMode::ShowEnergy:
            return std::visit(CellEnergyToColor, cell);
        case WorldProcessor::ShowMode::ShowDivercity:
            return QColor(0, 0, 0xff);
        }
    } catch (std::exception e) {
        qWarning() << "something wrong accessing variant: " << e.what();
        return QColor(0,0,0);
    }
    return QColor();
}


void RenderWorldUseCase::worldDataReady(WorldMap::WMap map, WorldInfo info)
{
    m_info = std::move(info);
    m_map = std::move(map);
    emit redrawWorld();
}

QColor RenderWorldUseCase::cellToColor(const Cell &c) const
{
    if (std::holds_alternative<CreatureA>(c)) {
        const CreatureA & creature = std::get<CreatureA>(c);
        if (creature.isAlive()) {
         //   qDebug() << " ====== creature attack/photone:" << creature.attackCount() << creature.photoneCount();
            return (creature.attackCount() > creature.photoneCount())
                ? QColor(200,30,10,200)
                : QColor(30,180,10,200);
        } else {
            return QColor(128,128,128,100);
        }
    }
    return QColor(0xff,0xff,0xff,0xff);
}
