#include "RenderWorldUseCase.h"
#include <QDebug>

namespace {

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

auto CellEnergyToColor = Overload {
    [](const Wall &) { return QColor(0xff,0xff,0xff,0xff); },
    [](const CreatureA & c) {
        if (c.isAlive()) {
            if (c.isPredatorIndex() > 0) {
                return QColor(0x9a,0x0b,0x06,200);
            } else {
                return QColor(0x0b,0x9a,0x06,200);
            }
//            return QColor(0xff,0,0,(c.getEnergy()*255)/1000);
        }
        return QColor(128,128,128,100);
    },
    [](auto) { return QColor(0xff,0xff,0xff,0xff); },
  };

} // namespase

RenderWorldUseCase::RenderWorldUseCase(WorldProcessorPtr processor, ICreatureBuilderPtr builder)
    : QObject()
    , m_processor(processor)
    , m_builder(builder)
{
    Q_ASSERT(m_processor);
    Q_ASSERT(m_builder);

    m_builder->buildDefaultCreatures(10);

    connect(m_processor.get(), &WorldProcessor::ready, this, &RenderWorldUseCase::worldDataReady);
}

size_t RenderWorldUseCase::lenght() const
{
    return m_map.size();
}

QColor RenderWorldUseCase::getCellColor(size_t index) const
{
    if (index >= m_map.size()) {
        return QColor();
    }
    const auto & cell = m_map.at(index);
    try {
        return std::visit(CellEnergyToColor, cell);
    } catch (std::exception e) {
        qWarning() << "something wrong accessing variant: " << e.what();

    }
    return QColor();
}


void RenderWorldUseCase::worldDataReady(WorldMap::WMap map)
{
    m_map = std::move(map);
    emit redrawWorld();
}
