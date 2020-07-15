#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "progresswindow.h"
#include "tablecontroller.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_xml_button_clicked();

private:
    TableController controller;
    Ui::MainWindow *ui;
    QMenu *menu = new QMenu(this);
    ProgressWindow *pw = nullptr;

};
#endif // MAINWINDOW_H
