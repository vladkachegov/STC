#ifndef DATABASEENTITY_H
#define DATABASEENTITY_H
#include <QString>
#include <QMap>

class DatabaseEntity
{
public:
    //methods
    DatabaseEntity();
    DatabaseEntity(QString _text_editor,QString _file_formats,
                   QString _encoding, QString _has_intellisence,
                   QString _has_plugins,QString _can_compile,
                   QString _rowid = "");
    QMap<int, QString> get_entity_data() const;
    void set_entity_data(const int &column_id,const QString &new_value);

    // variables and constants
    static const int db_columns = 6; // number of database columns
    static  const QMap<int,QString> db_column_names;


private:
    QMap<int,QString> entity_data;
};

#endif // DATABASEENTITY_H
