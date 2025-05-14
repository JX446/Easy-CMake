#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "checkablefilesystemmodel.h"
#include "mytreeview.h"
#include"cmakegenerator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->dirbrowseButton, &QPushButton::clicked, this, &MainWindow::handle_dirbrowseButton_clicked);
    connect(ui->genButton, &QPushButton::clicked, this, &MainWindow::handle_genButton_clicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handle_saveButton_clicked);
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


void MainWindow::handle_genButton_clicked()
{
    QStringList files_selected = ui->dirtreeView->model->getCheckedFiles();
    std::unique_ptr<CMakeGenerator> generator = std::make_unique<CMakeGenerator>();
    generator->setProjectPath(ui->showdirlineEdit->text());
    QString text = generator->generateCMakeContent(files_selected);
    ui->cmakeplainTextEdit->setPlainText(text);
}

bool MainWindow::handle_saveButton_clicked()
{
    QString rootpath = ui->showdirlineEdit->text();
    QString cmakecontent = ui->cmakeplainTextEdit->toPlainText();
    QFile cmakefile(rootpath + "/CMakeLists.txt");
    if (!cmakefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入 CMakeLists.txt";
        QMessageBox::critical(this, "错误", "保存失败！");
        return false;
    }

    QTextStream out(&cmakefile);
    out << cmakecontent;
    cmakefile.close();
    QMessageBox::information(this, "提示", "保存成功！");

    return true;
}

