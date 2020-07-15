#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H
#include <QAbstractTableModel>
#include <customxmlhandler.h>
class CustomTableModel : public QAbstractTableModel
{

    enum ACTION_TYPE
    {
        DELETE,EXPORT,ENABLE_EDIT
    };

public:
    CustomTableModel();
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    // QAbstractItemModel interface
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void refresh_model_view(); // rename to ==> refresh model view and rm * args
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    QVector<DatabaseEntity> *get_current_table_data() const;
    void set_current_table_data(QVector<DatabaseEntity> *value);

public slots:
    void on_edit_action();
    void on_data_changed();
signals:
    void data_changed();

private:
    int rows = 0;
    int cols = DatabaseEntity::db_columns;
    QVector<DatabaseEntity> local_table_data;
    QVector<DatabaseEntity> *current_table_data;
    bool is_editable = false;
};

#endif // CUSTOMTABLEMODEL_H
