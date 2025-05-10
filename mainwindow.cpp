#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CheckableFileSystemModel.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->dirbrowseButton, &QPushButton::clicked, this, &MainWindow::handle_dirbrowseButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_dirbrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", "/");
    ui->showdirlineEdit->setText(dir);
    read_files(dir);
}

void MainWindow::read_files(const QString &rootPath)
{
    // QDirIterator it(rootPath,
    //                 QStringList() << "*.h" << "*.c",
    //                 QDir::Files,
    //                 QDirIterator::Subdirectories
    //                 );
    // while(it.hasNext()) {
    //     QString filepath = it.next();
    //     qDebug() << "找到文件：" << filepath;
    // }
    CheckableFileSystemModel *model = new CheckableFileSystemModel;
    QStringList filters;
    filters << "*.h" << "*.c";
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
    model->setRootPath(rootPath);
    ui->dirtreeView->setModel(model);
    ui->dirtreeView->setRootIndex(model->index(rootPath));
    int columnCount = ui->dirtreeView->model()->columnCount();
    for (int col = 1; col < columnCount; ++col) {
        ui->dirtreeView->hideColumn(col);  // 隐藏除第0列外的所有列
    }
}
