#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CheckableFileSystemModel.h"
#include "mytreeview.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->dirbrowseButton, &QPushButton::clicked, this, &MainWindow::handle_dirbrowseButton_clicked);
    connect(this, &MainWindow::directorySelected, ui->dirtreeView, &MyTreeView::loadDirectory);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_dirbrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", "/");
    ui->showdirlineEdit->setText(dir);
    emit directorySelected(dir);
}

