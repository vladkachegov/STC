#include "tablecontroller.h"

TableController::TableController(QObject *parent) : QObject(parent)
{
    xml_handler.moveToThread(&xml_thread);
    sql_adapter.moveToThread(&sql_thread);

    model->set_current_table_data(sql_adapter.get_database_data());
    QString db_name = QDir::currentPath()+"//mydata.db";
    bool db_file_exists = QFile::exists(db_name);
    if (!db_file_exists)
    {
        QFile newdb(db_name);
        sql_adapter.set_database_path(db_name);
        sql_adapter.set_query_params(QUERY_TYPE::INIT_TABLE);
    }else
    {
        sql_adapter.set_database_path(db_name);
        sql_adapter.set_query_params(QUERY_TYPE::SELECT);
    }
    connect(&sql_thread,&QThread::started,&sql_adapter,&SqliteAdapter::on_thread_start);
    connect(&sql_adapter,&SqliteAdapter::db_refreshed,&sql_thread,&QThread::quit);

    connect(&sql_thread,&QThread::finished,model,&CustomTableModel::on_data_changed);

    connect(model,&CustomTableModel::dataChanged,
            [=](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        qDebug()<< "data changed cought";
        sql_adapter.set_query_params(QUERY_TYPE::UPDATE,{topLeft.row(),topLeft.column()});
        sql_thread.start();
    });
    connect(model,&CustomTableModel::rowsAboutToBeRemoved,
            [=](const QModelIndex &parent, int first, int last)
    {
        // first and last args bring row number
        qDebug()<< "data removed cought";
        sql_adapter.set_query_params(QUERY_TYPE::DELETE,{first,first}); // i need only row number (arg => first).
        sql_thread.start();
    });
    connect(&xml_thread,&QThread::started,&xml_handler,&CustomXMLHandler::on_thread_start); // connect thread to XML handler
    connect(&xml_handler,&CustomXMLHandler::isFinished,&xml_thread,&QThread::quit); // connect handler to thread (stop thread)

    connect(&xml_handler,&CustomXMLHandler::dir_parsed_signal,[=]()
    {
        auto xml_parse_results = xml_handler.get_parse_results(); // results of parsing xml files        
        sql_thread.wait();
        sql_adapter.set_xml_parsed_data(xml_parse_results); // on thread stop ->throw parsed data to sql
        sql_adapter.set_query_params(QUERY_TYPE::INSERT);
        sql_thread.start();
    });
    sql_thread.start();
}

CustomTableModel *TableController::get_model()
{
    return model;
}

void TableController::delete_row(const int &row_index)
{
    model->removeRows(row_index,1,QModelIndex());
}

void TableController::entry_export(const int &row_index)
{
    auto element_to_export = model->get_current_table_data()->at(row_index);
    xml_handler.set_entity_to_export(element_to_export);
    xml_thread.start();
}

void TableController::on_clear_button_clicked()
{
    sql_adapter.set_query_params(QUERY_TYPE::DELETE_ALL);
    sql_thread.start();
}

void TableController::edit_policy_changed()
{
    model->on_edit_action();
}

void TableController::set_progress_window(ProgressWindow *value)
{
    pw = value;
}

void TableController::parse_xml_dir(const QString &dir_to_parse)
{

    connect(&xml_handler,&CustomXMLHandler::file_parsed_signal,pw,&ProgressWindow::on_parse_status_changed);
    xml_handler.set_dir_path(dir_to_parse);
    xml_thread.start();
    pw->show();
}
