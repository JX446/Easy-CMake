#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "checkablefilesystemmodel.h"
#include "mytreeview.h"
#include "cmakegenerator.h"

// 主构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , generator(std::make_unique<CMakeGenerator>())
{
    ui->setupUi(this);
    this->setWindowTitle("EasyCmake");
    this->setWindowIcon(QIcon(":/icon.png"));
    connect(ui->dirbrowseButton, &QPushButton::clicked, this, &MainWindow::handle_dirbrowseButton_clicked);
    connect(ui->genButton, &QPushButton::clicked, this, &MainWindow::handle_genButton_clicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handle_saveButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::handle_clearButton_clicked);
    connect(this, &MainWindow::directorySelected, ui->dirtreeView, &MyTreeView::loadDirectory);
    connect(ui->projectnamepushButton, &QPushButton::clicked, this, &MainWindow::handle_projectnamepushButton_clicked);
    connect(ui->exefilenamepushButton, &QPushButton::clicked, this, &MainWindow::handle_exefilenamepushButton_clicked);
    connect(ui->minimumversionpushButton, &QPushButton::clicked, this, &MainWindow::handle_minimumversionpushButton_clicked);
    connect(ui->cxxversionpushButton, &QPushButton::clicked, this, &MainWindow::handle_cxxversionpushButton_clicked);
    connect(ui->exefilescopepushButton, &QPushButton::clicked, this, &MainWindow::handle_exefilescopepushButton_clicked);
    connect(ui->exefileoutputpushButton, &QPushButton::clicked, this, &MainWindow::handle_exefileoutputpushButton_clicked);
}

// 主析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 处理浏览目录按钮
void MainWindow::handle_dirbrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", "/");
    ui->showdirlineEdit->setText(dir);
    emit directorySelected(dir);
}

// 处理生成按钮
void MainWindow::handle_genButton_clicked()
{
    if (ui->showdirlineEdit->text().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先选择一个文件目录！");
    }
    else {
        QStringList files_selected = ui->dirtreeView->model->getCheckedFiles();
        generator->setProjectPath(ui->showdirlineEdit->text());
        QString text = generator->generateCMakeContent(files_selected);
        ui->cmakeplainTextEdit->setPlainText(text);
    }
}

// 处理保存按钮
bool MainWindow::handle_saveButton_clicked()
{
    if (ui->cmakeplainTextEdit->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先生成Cmake文件！");
        return false;
    }
    else {
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
}

// 处理清空按钮
void MainWindow::handle_clearButton_clicked()
{
    ui->cmakeplainTextEdit->clear();
}


// 项目名称输入按钮
void MainWindow::handle_projectnamepushButton_clicked()
{
    QString projectname = ui->projectnamelineEdit->text();
    generator->setProjectName(projectname);
}

// 最小版本要求按钮
void MainWindow::handle_minimumversionpushButton_clicked()
{
    QString minimum_version = ui->minumumversionlineEdit->text();
    generator->setVersionRequired(minimum_version);
}


// 可执行文件名称按钮
void MainWindow::handle_exefilenamepushButton_clicked()
{
    QString exefilename = ui->exefilenamelineEdit->text();
    generator->setExefileName(exefilename);
}

// 可执行文件作用域按钮
void MainWindow::handle_exefilescopepushButton_clicked()
{
    QString exefilescope = ui->exefilescopecomboBox->currentText();
    generator->setExefileScope(exefilescope);
}

// 可执行文件输出路径按钮
void MainWindow::handle_exefileoutputpushButton_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", QString("%1").arg(ui->showdirlineEdit->text()));
    ui->exefileoutputlineEdit->setText(dir);
    generator->setExefileOutput(dir);

}

// C++语言版本要求按钮
void MainWindow::handle_cxxversionpushButton_clicked()
{
    QString cxxversion = ui->cxxversioncomboBox->currentText();
    bool cxxversionoption = ui->cxxversionoptioncomboBox->currentText() == "ENFORCED"? true : false;
    generator->setCxxversionReqired(cxxversion);
    generator->setCxxversionoption(cxxversionoption);
}

