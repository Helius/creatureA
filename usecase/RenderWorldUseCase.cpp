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
    if (index >= m_map.size()) {
        return QColor();
    }
    const auto & cell = m_map.at(index);
    try {
        //return std::visit(CellEnergyToColor, cell);
        return cellToColor(cell);
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
        auto creature = std::get<CreatureA>(c);
        if (creature.isAlive()) {
            return (creature.attackCount() > creature.photoneCount())
                ? QColor(200,10,10,200)
                : QColor(30,180,10,200);
        } else {
            return QColor(128,128,128,100);
        }
    }
    return QColor(0xff,0xff,0xff,0xff);
}
