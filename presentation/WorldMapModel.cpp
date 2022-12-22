#include "WorldMapModel.h"
#include "worldmap.h"


WorldMapModel::WorldMapModel(RenderWorldUseCaseUnq usecase)
: QAbstractListModel()
    , m_usecase(std::move(usecase))
{
    Q_ASSERT(m_usecase);
}

int WorldMapModel::rowCount(const QModelIndex &) const
{
    return m_usecase->lenght();
}

QVariant WorldMapModel::data(const QModelIndex &index, int role) const
{
    size_t ind = index.row();
    if (ind >= m_usecase->lenght()) {
        return QVariant();
    }

    switch(role) {
    case BorderColor:
        return m_usecase->getCellColor(ind);
    case MainColor:
        return m_usecase->getCellColor(ind);
    }
    return QVariant();
}

QHash<int, QByteArray> WorldMapModel::roleNames() const
{
    return {
        {BorderColor, "borderColor"},
        {MainColor, "mainColor"},
    };
}
