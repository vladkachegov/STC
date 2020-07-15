#include "sqliteadapter.h"


SqliteAdapter::SqliteAdapter(QObject *parent) :QObject(parent)
{

}
QString SqliteAdapter::get_database_path() const
{
    return database_path;
}

void SqliteAdapter::set_database_path(const QString &value)
{
    database_path = value;
}

void SqliteAdapter::process_query()
{

    switch(query_key)
    {
    case(QUERY_TYPE::SELECT):
    {
        qDebug() << "select signal cought thread sql!";
        select_data();
        break;
    }
    case(QUERY_TYPE::INIT_TABLE):
    {
        qDebug() << "init table signal cought thread sql!";
        create_table();
        QSqlDatabase::removeDatabase(connection_name);
        select_data(); // updating data vector
        break;
    }
    case(QUERY_TYPE::INSERT):
    {
        qDebug() << "insert signal cought thread sql!";
        insert_data();
        QSqlDatabase::removeDatabase(connection_name);
        select_data(); // updating data vector
        break;
    }
    case(QUERY_TYPE::UPDATE):
    {
        qDebug() << "update signal cought thread sql!";
        update_data();
        QSqlDatabase::removeDatabase(connection_name);
        select_data(); // updating data vector
        break;

    }
    case(QUERY_TYPE::DELETE):
    {
        qDebug() << "update signal cought thread sql!";
        delete_data();
        QSqlDatabase::removeDatabase(connection_name);
        select_data(); // updating data vector
        break;
    }
    case(QUERY_TYPE::DELETE_ALL):
    {
        qDebug() << "DELETE ALL signal cought thread sql!";
        delete_all_data();
        QSqlDatabase::removeDatabase(connection_name);
        select_data(); // updating data vector
        break;
    }
    default:
    {
        qDebug()<< "smt went wrong with sql thread!";
        break;
    }
    }
    // insert 1 more select here to refresh data vector in cpp
    QSqlDatabase::removeDatabase(connection_name);
    emit db_refreshed();
}



QVector<DatabaseEntity> *SqliteAdapter::get_database_data()
{
    return database_data;
}

void SqliteAdapter::set_database_data(const QVector<DatabaseEntity> &value)
{
    database_data->append(value);

}

QUERY_TYPE SqliteAdapter::get_query_key() const
{
    return query_key;
}

void SqliteAdapter::set_query_params(const QUERY_TYPE &type, const QPair<int, int> &index)
{
    set_query_key(type);
    set_element_to_process(index);
}

void SqliteAdapter::set_query_params(const QUERY_TYPE &type)
{
    set_query_key(type);
}

void SqliteAdapter::set_query_key(const QUERY_TYPE &value)
{
    query_key = value;
}

void SqliteAdapter::on_thread_start()
{
    process_query();
}

void SqliteAdapter::on_clear_button_clicked()
{

}

void SqliteAdapter::set_element_to_process(const QPair<int, int> &value)
{
    element_to_process = value;
}

void SqliteAdapter::set_xml_parsed_data(const QVector<DatabaseEntity> &value)
{
    xml_parsed_data = value;
}

void SqliteAdapter::insert_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();

    QSqlQuery sql_query;
    Q_FOREACH(DatabaseEntity entity,xml_parsed_data)
    {
        sql_query.prepare(insert_query);
        sql_query.bindValue(0,entity.get_entity_data()[0]);
        sql_query.bindValue(1,entity.get_entity_data()[1]);
        sql_query.bindValue(2,entity.get_entity_data()[2]);
        sql_query.bindValue(3,entity.get_entity_data()[3]);
        sql_query.bindValue(4,entity.get_entity_data()[4]);
        sql_query.bindValue(5,entity.get_entity_data()[5]);
        sql_query.exec();
    }

    db.close();

}

void SqliteAdapter::select_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();

    database_data->clear();
    QVector<DatabaseEntity> content;
    QSqlQuery sql_query(select_query_string);
    while (sql_query.next())
    {
        QString rowid = sql_query.value(6).toString();
        DatabaseEntity entity(sql_query.value(0).toString(),
                              sql_query.value(1).toString(),
                              sql_query.value(2).toString(),
                              sql_query.value(3).toString(),
                              sql_query.value(4).toString(),
                              sql_query.value(5).toString(),
                              sql_query.value(6).toString()
                              );
        content.push_back(entity);

    }
    *database_data = content;

    db.close();
}

void SqliteAdapter::update_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();

    int row = element_to_process.first;
    int col = element_to_process.second;
    QString column_name = DatabaseEntity::db_column_names[col];
    QString new_value = database_data->at(row).get_entity_data()[col];
    QString rowid = database_data->at(row).get_entity_data()[6];
    QSqlQuery query;
    QString query_sql = update_table_query_string.arg(column_name);
    query.prepare(query_sql);
    query.bindValue(":new_column_value",new_value);
    query.bindValue(":row_id",rowid);
    bool done_well = query.exec();
    qDebug()<<query.lastError().text();

    db.close();
}

void SqliteAdapter::delete_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();

    qDebug()<< "delete signal cought";
    int row = element_to_process.first;    
    QString rowid = database_data->at(row).get_entity_data()[6];
    QSqlQuery query;
    query.prepare(delete_query_string);
    query.bindValue(":row_id",rowid);
    bool done_well = query.exec();
    qDebug()<<query.lastError().text();

    db.close();
}

void SqliteAdapter::create_table()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();


    QSqlQuery query;
    query.prepare(create_table_query_string);
    bool done_well = query.exec();
    qDebug()<<query.lastError().text();

    db.close();
}

void SqliteAdapter::delete_all_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(this->database_path);
    connection_name = db.connectionName();
    db.open();

    QSqlQuery query;
    query.prepare(delete_all_query_string);
    bool del_done_well = query.exec();
    query.prepare("VACUUM");        //free space
    bool vac_done_well = query.exec();
    qDebug()<<query.lastError().text();

    db.close();
}
