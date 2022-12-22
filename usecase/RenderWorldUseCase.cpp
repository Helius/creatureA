#include "RenderWorldUseCase.h"

namespace {

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

//auto TypeOfCell = Overload {
//    [](Wall) { return RenderWorldUseCase::CellTypes::WallCell; },
//    [](CreatureA) { return RenderWorldUseCase::CellTypes::CreatureCell; },
//    [](auto) { return RenderWorldUseCase::CellTypes::EmptyCell; },
//  };

auto CellToColor = Overload {
    [](Wall) { return QColor(0,0,0,0xff); },
    [](CreatureA) { return QColor(0x0b, 0x9A, 0x06, 0xff); },
    [](auto) { return QColor(0xff,0xff,0xff,0xff); },
  };
}

RenderWorldUseCase::RenderWorldUseCase(WorldProcessorUnq processor, ICreatureBuilderPtr builder)
    : QObject()
    , m_processor(std::move(processor))
    , m_builder(builder)
{
    Q_ASSERT(m_processor);
    Q_ASSERT(m_builder);
    m_builder->buildDefaultCreatures(100);
}

size_t RenderWorldUseCase::lenght() const
{
    return m_processor->map()->lenght();
}

const Cell &RenderWorldUseCase::get(size_t index) const
{
    return m_processor->map()->m_map.at(index);
}

QColor RenderWorldUseCase::getCellColor(size_t index) const
{
    Q_ASSERT(index < lenght());
    const auto & cell = m_processor->map()->m_map.at(index);
    return std::visit(CellToColor, cell);
}
