#include "config_list_model.h"

ConfigListModel::ConfigListModel(ConfigManagerPtr configManager)
    : QAbstractItemModel()
    , m_configManager(configManager)
{}

int ConfigListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_configManager->configCount();
}

QVariant ConfigListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_configManager->configCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return m_configManager->configName(index.row());
        break;
    default:
        return QVariant();
        break;
    }
}

QHash<int, QByteArray> ConfigListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
