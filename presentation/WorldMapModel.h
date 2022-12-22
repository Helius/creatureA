#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <usecase/RenderWorldUseCase.h>

class WorldMapModel : public QAbstractListModel {

    Q_OBJECT

public:
    enum Roles {
        CellType = Qt::UserRole + 1,
        MainColor,
        BorderColor,
    };
    Q_ENUM(Roles);

public:
    WorldMapModel(RenderWorldUseCaseUnq usecase);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    RenderWorldUseCaseUnq m_usecase;
};

using WorldMapModelUnq = std::unique_ptr<WorldMapModel>;