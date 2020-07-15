#include "databaseentity.h"

const QMap<int,QString> DatabaseEntity::db_column_names =
{
    {0,"texteditor"},
    {1,"fileformats"},
    {2,"encoding"},
    {3,"hasintellisense"},
    {4,"hasplugins"},
    {5,"cancompile"}
};

DatabaseEntity::DatabaseEntity()
{

}

DatabaseEntity::DatabaseEntity(QString _text_editor, QString _file_formats, QString _encoding,
                               QString _has_intellisence, QString _has_plugins, QString _can_compile, QString _rowid)
{
    entity_data[0]= _text_editor;
    entity_data[1]= _file_formats;
    entity_data[2]= _encoding;
    entity_data[3]= _has_intellisence;
    entity_data[4]= _has_plugins;
    entity_data[5]= _can_compile;
    entity_data[6]= _rowid;
}

QMap<int, QString> DatabaseEntity::get_entity_data() const
{
    return entity_data;
}

void DatabaseEntity::set_entity_data(const int &column_id, const QString &new_value)
{
    entity_data[column_id] = new_value;
}
