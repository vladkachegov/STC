#ifndef TABLECONTROLLER_H
#define TABLECONTROLLER_H
#include <QObject>
#include "customtablemodel.h"
#include "customxmlhandler.h"
#include "sqliteadapter.h"
#include "progresswindow.h"
class TableController : public QObject
{
    Q_OBJECT
public:
    explicit TableController(QObject *parent = nullptr);
    CustomTableModel *get_model();
    void delete_row(const int &row_index);
    void entry_export(const int &row_index);
    void set_progress_window(ProgressWindow *value);
    void parse_xml_dir(const QString &dir_to_parse);
signals:
public slots:
    void on_clear_button_clicked();
    void edit_policy_changed();

private:
    CustomTableModel *model = new CustomTableModel();
    QThread xml_thread;
    QThread sql_thread;
    CustomXMLHandler xml_handler;
    SqliteAdapter sql_adapter;
    ProgressWindow *pw;
};

#endif // TABLECONTROLLER_H
