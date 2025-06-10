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
    connect(ui->dirBrowseButton, &QPushButton::clicked, this, &MainWindow::handle_dirBrowseButton_clicked);
    connect(ui->genButton, &QPushButton::clicked, this, &MainWindow::handle_genButton_clicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handle_saveButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::handle_clearButton_clicked);
    connect(this, &MainWindow::directorySelect, ui->dirtreeView, &MyTreeView::loadDirectory);
    connect(ui->projectNamePushButton, &QPushButton::clicked, this, &MainWindow::handle_projectNamePushButton_clicked);
    connect(ui->outputFileNamePushButton, &QPushButton::clicked, this, &MainWindow::handle_exeFilenamePushButton_clicked);
    connect(ui->minimumVersionPushButton, &QPushButton::clicked, this, &MainWindow::handle_minimumVersionPushButton_clicked);
    connect(ui->cxxVersionPushButton, &QPushButton::clicked, this, &MainWindow::handle_cxxVersionPushButton_clicked);
    connect(ui->exeFileScopePushButton, &QPushButton::clicked, this, &MainWindow::handle_exeFileScopePushButton_clicked);
    connect(ui->outputFileOutputPushButton, &QPushButton::clicked, this, &MainWindow::handle_exeFileOutputPushButton_clicked);
    connect(ui->outputFileTypeOptionPushButton, &QPushButton::clicked, this, &MainWindow::handle_outputFileTypeOptionPushButton_clicked);
}

// 主析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 处理浏览目录按钮
void MainWindow::handle_dirBrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", "/");
    ui->showDirlineEdit->setText(dir);
    emit directorySelect(dir);
}

// 处理生成按钮
void MainWindow::handle_genButton_clicked()
{
    if (ui->showDirlineEdit->text().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先选择一个文件目录！");
    }
    else {
        QStringList files_selected = ui->dirtreeView->model->getCheckedFiles();
        generator->setProjectPath(ui->showDirlineEdit->text());
        QString text = generator->generateCMakeContent(files_selected);
        ui->cmakePlainTextEdit->setPlainText(text);
    }
}

// 处理保存按钮
bool MainWindow::handle_saveButton_clicked()
{
    if (ui->cmakePlainTextEdit->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先生成Cmake文件！");
        return false;
    }
    else {
        QString rootpath = ui->showDirlineEdit->text();
        QString cmakecontent = ui->cmakePlainTextEdit->toPlainText();
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
    ui->cmakePlainTextEdit->clear();
}

// 项目名称输入按钮
void MainWindow::handle_projectNamePushButton_clicked()
{
    QString projectname = ui->projectNameLineEdit->text();
    generator->setProjectName(projectname);
}

// 最小版本要求按钮
void MainWindow::handle_minimumVersionPushButton_clicked()
{
    QString minimum_version = ui->minimumVersionLineEdit->text();
    generator->setVersionRequired(minimum_version);
}


// 编译文件文件名称按钮
void MainWindow::handle_exeFilenamePushButton_clicked()
{
    QString exefilename = ui->outputFileNameLineEdit->text();
    generator->setExeFileName(exefilename);
}

// 编译文件作用域按钮
void MainWindow::handle_exeFileScopePushButton_clicked()
{
    QString exefilescope = ui->exeFileScopeComboBox->currentText();
    generator->setExeFileScope(exefilescope);
}

// 编译文件输出路径按钮
void MainWindow::handle_exeFileOutputPushButton_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", QString("%1").arg(ui->showDirlineEdit->text()));
    ui->outputFileOutputLineEdit->setText(dir);
    generator->setExeFileOutput(dir);
}

void MainWindow::handle_outputFileTypeOptionPushButton_clicked() {
    QString outputFileType = ui->outputFileTypeOptionComboBox->currentText();
    generator->setOutputFileType(outputFileType);
}

// C++语言版本要求按钮
void MainWindow::handle_cxxVersionPushButton_clicked()
{
    QString cxxversion = ui->cxxVersionComboBox->currentText();
    bool cxxversionoption = ui->cxxVersionOptionComboBox->currentText() == "ENFORCED"? true : false;
    generator->setCxxVersionRequired(cxxversion);
    generator->setCxxVersionOption(cxxversionoption);
}

