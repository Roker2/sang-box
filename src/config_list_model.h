#ifndef CONFIG_LIST_MODEL_H
#define CONFIG_LIST_MODEL_H

#include <QAbstractItemModel>

#include "config_manager.h"

class ConfigListModel : public QAbstractItemModel
{
    Q_OBJECT

    using ConfigManagerPtr = std::shared_ptr<ConfigManager>;

    enum ConfigRoles {
        NameRole = Qt::UserRole + 1,
        SelectedRole,
    };

public:
    explicit ConfigListModel(ConfigManagerPtr configManager);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

public slots:
    void switchConfig(int index);
    void deleteConfig(int index);
    void editConfig(int index);
    void importConfig();

private slots:
    void processChanges(int index);
    void onBeginAddConfig();
    void onEndAddConfig();

private:
    QHash<int, QByteArray> roleNames() const override;

    ConfigManagerPtr m_configManager;
};

#endif // CONFIG_LIST_MODEL_H
