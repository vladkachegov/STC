#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    
    auto delete_action = new QAction("Delete entry", this);
    auto edit_entries = new QAction("Enable editing", this);
    auto export_entry = new QAction("Export entry to XML", this);
    menu->addAction(delete_action);
    menu->addAction(edit_entries);
    menu->addAction(export_entry);

    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setModel(controller.get_model());


    connect(ui->clear_button,&QPushButton::clicked,&controller,&TableController::on_clear_button_clicked);
    connect(delete_action,&QAction::triggered,[=]()
    {
        int row = ui->tableView->selectionModel()->currentIndex().row();
        controller.delete_row(row);
        qDebug()<<row;
    });

    connect(ui->tableView,&QTableView::customContextMenuRequested,[=](const QPoint &pos){
        QModelIndex index=ui->tableView->indexAt(pos);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    });

    connect(edit_entries,&QAction::triggered,&controller,&TableController::edit_policy_changed);

    connect(edit_entries,&QAction::triggered,[=]()
    {
        auto current_menu_text = edit_entries->text();
        if (current_menu_text == "Enable editing")
        {
            edit_entries->setText("Disable editing");
        }else
        {
            edit_entries->setText("Enable editing");
        }
    });

    connect(export_entry,&QAction::triggered,[=]()
    {
        int row = ui->tableView->selectionModel()->currentIndex().row();
        controller.entry_export(row);
    });
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_xml_button_clicked()
{
    delete pw;
    pw = new ProgressWindow();
    QString dir_to_parse = QFileDialog::getExistingDirectory(nullptr,tr("Choose XML directory"),".",QFileDialog::ReadOnly);
    controller.set_progress_window(pw);
    controller.parse_xml_dir(dir_to_parse);
}




