#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "checkablefilesystemmodel.h"
#include "mytreeview.h"
#include "cmakegenerator.h"

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
    connect(ui->outputFileDirPushButton, &QPushButton::clicked, this, &MainWindow::handle_outputFileDirPushButton_clicked);

    // 自动更新设置
    connect(ui->projectNameLineEdit, &QLineEdit::editingFinished, this, [&]() {
        generator->setProjectName(ui->projectNameLineEdit->text());
    });
    connect(ui->minimumVersionLineEdit, &QLineEdit::editingFinished, this, [&]() {
        generator->setVersionRequired(ui->minimumVersionLineEdit->text());
    });
    connect(ui->outputFileNameLineEdit, &QLineEdit::editingFinished, this, [&]() {
        generator->setExeFileName(ui->outputFileNameLineEdit->text());
    });
    connect(ui->exeFileScopeComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setExeFileScope(text);
    });
    connect(ui->outputFileTypeOptionComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setOutputFileType(text);
    });
    connect(ui->cxxVersionComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setCxxVersionRequired(text);
    });
    connect(ui->cxxVersionOptionComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setCxxVersionOption(text == "ENFORCED");
    });

    // 鼠标悬停输出目录时输出完整内容
    connect(ui->outputFileDirLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        QFontMetrics fm(ui->outputFileDirLineEdit->font());
        int textWidth = fm.horizontalAdvance(text);
        int editWidth = ui->outputFileDirLineEdit->width() - 10; // 减去边距用于准确判断

        if (textWidth > editWidth) {
            ui->outputFileDirLineEdit->setToolTip(text); // 设置 tooltip 为当前文本
        } else {
            ui->outputFileDirLineEdit->setToolTip(""); // 文本没超出，取消 tooltip
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_dirBrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择一个目录", "/");
    ui->showDirlineEdit->setText(dir);
    ui->outputFileDirLineEdit->setText(dir);
    emit directorySelect(dir);
}

void MainWindow::handle_genButton_clicked()
{
    if (ui->showDirlineEdit->text().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先选择一个文件目录！");
        return;
    }

    updateGeneratorSettings();

    QStringList files_selected = ui->dirtreeView->model->getCheckedFiles();
    QString text = generator->generateCMakeContent(files_selected);
    ui->cmakePlainTextEdit->setPlainText(text);
}

bool MainWindow::handle_saveButton_clicked()
{
    if (ui->cmakePlainTextEdit->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先生成CMake文件！");
        return false;
    }

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

void MainWindow::handle_clearButton_clicked()
{
    ui->cmakePlainTextEdit->clear();
}

void MainWindow::handle_outputFileDirPushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择输出目录", ui->showDirlineEdit->text());
    ui->outputFileDirLineEdit->setText(dir);
    generator->setExeFileOutput(dir);
}

void MainWindow::updateGeneratorSettings()
{
    generator->setProjectPath(ui->showDirlineEdit->text());
    generator->setProjectName(ui->projectNameLineEdit->text());
    generator->setVersionRequired(ui->minimumVersionLineEdit->text());
    generator->setExeFileName(ui->outputFileNameLineEdit->text());
    generator->setExeFileScope(ui->exeFileScopeComboBox->currentText());
    generator->setOutputFileType(ui->outputFileTypeOptionComboBox->currentText());
    generator->setCxxVersionRequired(ui->cxxVersionComboBox->currentText());
    generator->setCxxVersionOption(ui->cxxVersionOptionComboBox->currentText() == "ENFORCED");
    generator->setExeFileOutput(ui->outputFileDirLineEdit->text());
}
