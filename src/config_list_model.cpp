#include "config_list_model.h"

ConfigListModel::ConfigListModel(ConfigManagerPtr configManager)
    : QAbstractItemModel()
    , m_configManager(configManager)
{
    QObject::connect(m_configManager.get(), &ConfigManager::configRenamed,
                     this, &ConfigListModel::processChanges);
    QObject::connect(m_configManager.get(), &ConfigManager::configChanged,
                     this, &ConfigListModel::updateCurrentConfigData);
    QObject::connect(m_configManager.get(), &ConfigManager::beginAddConfig,
                     this, &ConfigListModel::onBeginAddConfig);
    QObject::connect(m_configManager.get(), &ConfigManager::endAddConfig,
                     this, &ConfigListModel::onEndAddConfig);
}

int ConfigListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_configManager->configCount();
}


int ConfigListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ConfigListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_configManager->configCount())
        return QVariant();

    switch (role) {
    case NameRole:
        return m_configManager->configName(index.row());
        break;
    case SelectedRole:
        return m_configManager->configIndex() == index.row();
        break;
    default:
        return QVariant();
        break;
    }
}

QModelIndex ConfigListModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, nullptr);
}

QModelIndex ConfigListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

void ConfigListModel::switchConfig(int index)
{
    if (index == m_configManager->configIndex())
        return;
    const QModelIndex prevModelIndex = this->index(m_configManager->configIndex(), 0);
    if (index >= 0) {
        m_configManager->switchConfig(index);
    }
    emit dataChanged(prevModelIndex, prevModelIndex, {SelectedRole});
}

void ConfigListModel::deleteConfig(int index)
{
    if (index >= 0 && m_configManager->configCount() > 0)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_configManager->removeConfig(index);
        endRemoveRows();
    }
}

void ConfigListModel::editConfig(int index)
{
    if (index >= 0 && m_configManager->configCount() > 0)
    {
        m_configManager->editConfig(index);
        const QModelIndex modelIndex = this->index(index, 0);
        emit dataChanged(modelIndex, modelIndex, {NameRole});
    }
}

void ConfigListModel::importConfig()
{
    m_configManager->importConfig();
}

void ConfigListModel::processChanges(int index)
{
    const QModelIndex modelIndex = this->index(index, 0);
    emit dataChanged(modelIndex, modelIndex);
}

void ConfigListModel::onBeginAddConfig()
{
    const int count = rowCount();
    beginInsertRows(QModelIndex(), count, count);
}

void ConfigListModel::onEndAddConfig()
{
    endInsertRows();
}

void ConfigListModel::updateCurrentConfigData()
{
    const QModelIndex modelIndex = this->index(m_configManager->configIndex(), 0);
    emit dataChanged(modelIndex, modelIndex, {SelectedRole});
}

QHash<int, QByteArray> ConfigListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SelectedRole] = "selected";
    return roles;
}
