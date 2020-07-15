#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = nullptr);
    ~ProgressWindow();
public slots:
    void on_parse_status_changed(const int &file_number, const int &total_files_number,
                                 const QString &file_path, const int &isOk, const QString &error_text);


private:
    Ui::ProgressWindow *ui;
    const QString file_status_ok = "<span style=\"color:#00e600;\" >"
                                   "...ok"
                                   "</span>"
                                   "\n";
    const QString file_status_error = "<span style=\"color:#ff0000;\" >"
                                      "...error"
                                      "</span>"
                                      "\n";
    QString parse_counter = "File %1 of %2";
};

#endif // PROGRESSWINDOW_H
