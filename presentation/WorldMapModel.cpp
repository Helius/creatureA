#include "WorldMapModel.h"
#include "worldmap.h"
#include <QDebug>


WorldMapModel::WorldMapModel(RenderWorldUseCaseUnq usecase)
    : QAbstractListModel()
    , m_usecase(std::move(usecase))
{
    Q_ASSERT(m_usecase);
    connect(m_usecase.get(), &RenderWorldUseCase::redrawWorld, this, &WorldMapModel::update);
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
//    case BorderColor:
//        return m_usecase->getBorderColor(ind);
    case MainColor:
        return m_usecase->getCellColor(ind);
    }
    return QVariant();
}

QHash<int, QByteArray> WorldMapModel::roleNames() const
{
    return {
//        {BorderColor, "borderColor"},
        {MainColor, "mainColor"},
        };
}

void WorldMapModel::update()
{
    static int prevRowCount = 0;
    if (rowCount({}) != prevRowCount) {
        qDebug() << "reset, rows" << prevRowCount << rowCount({});
        beginResetModel();
        endResetModel();
        prevRowCount = rowCount({});
    } else {
        QElapsedTimer timer;
        timer.start();
        emit dataChanged(index(0), index(rowCount({})-1), {Roles::MainColor});
        qDebug() << "Update model time:" << timer.elapsed() << "ms";
    }
}
