#include "progresswindow.h"
#include "ui_progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("XML parsing log");
}

ProgressWindow::~ProgressWindow()
{
    qDebug() << "PW DELETING";
    delete ui;
}

void ProgressWindow::on_parse_status_changed(const int &file_number, const int &total_files_number,
                                             const QString &file_path, const int &isOk, const QString &error_text)
{
    QString parsed = parse_counter.arg(file_number).arg(total_files_number);
    QString status_text = file_path;
    if (isOk == 1)
    {
        status_text.append(file_status_ok);
    }else
    {
        status_text.append(file_status_error);
        status_text.append("        ");
        status_text.append(error_text);

    }
    ui->status_label->setText(parsed);
    ui->textBrowser->append(status_text);

}
