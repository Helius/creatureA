#include "RenderWorldUseCase.h"
#include <QDebug>

namespace {

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

//auto CellTypeToColor = Overload {
//    [](const Wall &) { return QColor(0,0,0,0xff); },
//    [](const CreatureA & c) {
//        if (c.isAlive()) {
//            return QColor(c.isPredatorIndex(), 255-c.isPredatorIndex(), 0x06, 0xff);
//        }
//        return QColor(128,128,128,100);
//    },
//    [](auto) { return QColor(0xff,0xff,0xff,0xff); },
//  };

auto CellEnergyToColor = Overload {
    [](const Wall &) { return QColor(0xff,0xff,0xff,0xff); },
    [](const CreatureA & c) {
        if (c.isAlive()) {
            if (c.isPredatorIndex() > 0) {
                return QColor(0x9a,0x0b,0x06,200);
            } else {
                return QColor(0x0b,0x9a,0x06,200);
            }
        }
        return QColor(128,128,128,100);
    },
    [](auto) { return QColor(0xff,0xff,0xff,0xff); },
  };

} // namespase

RenderWorldUseCase::RenderWorldUseCase(WorldProcessorUnq processor, ICreatureBuilderPtr builder)
    : QObject()
    , m_processor(std::move(processor))
    , m_builder(builder)
{
    Q_ASSERT(m_processor);
    Q_ASSERT(m_builder);

    m_builder->buildDefaultCreatures(10);

    connect(&m_timer, &QTimer::timeout, this, &RenderWorldUseCase::updateWorld);
    connect(m_processor.get(), &WorldProcessor::ready, this, &RenderWorldUseCase::worldDataReady);
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
    m_timer.start();
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

//QColor RenderWorldUseCase::getBorderColor(size_t index) const
//{
//    if (index >= lenght()) {
//        return QColor();
//    }
//    const auto & cell = m_map.at(index);
//    return std::visit(CellTypeToColor, cell);
//}

void RenderWorldUseCase::updateWorld()
{
    m_processor->run(1);
    m_elapsedTimer.start();
}

void RenderWorldUseCase::worldDataReady(WorldMap::WMap map)
{
    m_map = std::move(map);
    qDebug() << "World process time: " << m_elapsedTimer.elapsed() << "ms";
    emit redrawWorld();
}
