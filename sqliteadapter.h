#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <customxmlhandler.h>
enum class QUERY_TYPE
{
    INIT_TABLE,
    SELECT,
    INSERT,
    DELETE,
    UPDATE,
    DELETE_ALL
};

class SqliteAdapter : public QObject
{

    Q_OBJECT
public:
    explicit SqliteAdapter(QObject *parent = 0);

    QString get_database_path() const;
    void set_database_path(const QString &value);
    void process_query();


    QVector<DatabaseEntity> *get_database_data();
    void set_database_data(const QVector<DatabaseEntity> &value);

    QUERY_TYPE get_query_key() const;
    void set_query_params(const QUERY_TYPE &type, const QPair<int,int> &index);
    void set_query_params(const QUERY_TYPE &type);

    void set_xml_parsed_data(const QVector<DatabaseEntity> &value);




    void set_element_to_process(const QPair<int, int> &value);

signals:
    void db_refreshed();
public slots:
    void on_thread_start(); // thread works with this method
    void on_clear_button_clicked();
private:
    QPair<int,int> element_to_process;
    QUERY_TYPE query_key;
    QSqlDatabase db1;
    QVector<DatabaseEntity> *database_data = new QVector<DatabaseEntity>(); // empty vector on initialization
    QVector<DatabaseEntity> xml_parsed_data;                                // insert method gets data from this vector

    // operating with thread switch
    void set_query_key(const QUERY_TYPE &value);    

    void insert_data();
    void select_data();
    void update_data();
    void delete_data();
    void create_table();
    void delete_all_data();



    QString connection_name;
    QString database_path;
    const QString select_query_string = "SELECT *,rowid FROM XML_CONTENTS";
    const QString insert_query = "INSERT INTO XML_CONTENTS (texteditor,fileformats,encoding,hasintellisense,hasplugins,cancompile)"
                                 "VALUES (?,?,?,?,?,?);";
    const QString delete_query_string = "DELETE FROM XML_CONTENTS WHERE ROWID=:row_id";
    const QString delete_all_query_string = "DELETE FROM XML_CONTENTS";
    const QString create_table_query_string = "CREATE TABLE XML_CONTENTS ("
                                              "texteditor varchar(255) NOT NULL,"
                                              "fileformats varchar(255) NOT NULL,"
                                              "encoding varchar(255) NOT NULL,"
                                              "hasintellisense varchar(255) NOT NULL,"
                                              "hasplugins varchar(255) NOT NULL,"
                                              "cancompile varchar(255) NOT NULL"
                                              ");";
    const QString update_table_query_string = "UPDATE XML_CONTENTS "
                                              "SET  %1= :new_column_value "            // column name = new value
            "WHERE ROWID = :row_id;";      // set row id (ROWID property)

    const QString database_type_string = "QSQLITE";
};

#endif // SQLITEADAPTER_H
