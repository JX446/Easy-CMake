#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect(ui->dirbrowseButton, &QPushButton::clicked, this, &MainWindow::on_dirbrowseButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::on_commitButton_clicked()
// {
//     // // 获取输入内容
//     // QString program = ui->cmdLineEdit->text();

//     // // 创建Process对象
//     // QProcess *myprocess = new QProcess(this);
//     // myprocess->start(program);
// }

// void MainWindow::on_cancelButton_clicked()
// {
//     this->close();
// }

void MainWindow::on_dirbrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", QCoreApplication::applicationFilePath());
    ui->showdirlineEdit->setText(dir);
    read_files(dir);

}

void MainWindow::read_files(const QString &rootPath)
{
    QDirIterator it(rootPath,
                    QStringList() << "*.h" << "*.c",
                    QDir::Files,
                    QDirIterator::Subdirectories
                    );
    while(it.hasNext()) {
        QString filepath = it.next();
        qDebug() << "找到文件：" << filepath;
    }
    QFileSystemModel *model = new QFileSystemModel;
    QStringList filters;
    filters << "*.h" << "*.c";
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
    model->setRootPath(rootPath);
    ui->dirtreeView->setModel(model);
    ui->dirtreeView->setRootIndex(model->index(rootPath));

}
