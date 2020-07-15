#include "customtablemodel.h"

CustomTableModel::CustomTableModel()
{    
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    return this->rows;
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    return this->cols;
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();
        DatabaseEntity entity = local_table_data.at(row);
        QString entity_col_data = entity.get_entity_data()[col];
        return QVariant(entity_col_data);
    }else
    {
        return QVariant();
    }
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if (is_editable)
    {
        flags.setFlag(Qt::ItemIsEditable,true);
    }else
    {
        flags.setFlag(Qt::ItemIsEditable,false);
    }
    return flags;
}

void CustomTableModel::refresh_model_view()
{
    beginResetModel();
    local_table_data = *current_table_data;
    this->rows = current_table_data->size();
    endResetModel();
}

QVector<DatabaseEntity> *CustomTableModel::get_current_table_data() const
{
    return current_table_data;
}

void CustomTableModel::on_edit_action()
{
    is_editable = !is_editable;
}

void CustomTableModel::on_data_changed()
{
    refresh_model_view();
}

bool CustomTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (is_editable)
    {
        // how can FALSE happen?

        beginRemoveRows(QModelIndex(),row,row + count -1);
        local_table_data.remove(row);
        rows = local_table_data.size();
        endRemoveRows();

        return true;
    }else
    {
        return false;
    }
}

void CustomTableModel::set_current_table_data(QVector<DatabaseEntity> *value)
{
    current_table_data = value;
    local_table_data = *current_table_data;
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && value.toString() != "")
    {
        int row = index.row();
        int col = index.column();
        (*current_table_data)[row].set_entity_data(col,value.toString()); // updating c++ database representation
        emit dataChanged(index,index);
        return true;
    }else
    {
        return false;
    }
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case(0):
            {
                return tr("Text Editor");
                break;
            }
            case(1):
            {
                return QVariant("File formats");
                break;
            }
            case(2):
            {
                return QVariant("Encoding");
                break;
            }
            case(3):
            {
                return QVariant("Has intellisense");
                break;
            }
            case(4):
            {
                return QVariant("Has plugins");
                break;
            }
            case(5):
            {
                return QVariant("Can compile");
                break;
            }
            default:
            {
                return QVariant();
                break;
            }
            }

        }else if (orientation == Qt::Vertical)
        {
            return QVariant(section);
        }
    }else
    {
        return QVariant();
    }
    return QVariant();
}






