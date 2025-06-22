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
    connect(ui->addMacroPushButton, &QPushButton::clicked, this, &MainWindow::handle_addMacroPushButton_clicked);
    connect(ui->deleteMacroPushButton, &QPushButton::clicked, this, &MainWindow::handle_deleteMacroPushButton_clicked);
    connect(ui->buildPushButton, &QPushButton::clicked, this, &MainWindow::handle_buildPushButton_clicked);

    // 自动更新设置
    connect(ui->projectNameLineEdit, &QLineEdit::editingFinished, this, [&]() {
        generator->setProjectName(ui->projectNameLineEdit->text());
    });
    connect(ui->minimumVersionLineEdit, &QLineEdit::editingFinished, this, [&]() {
        generator->setCMakeVersionRequired(ui->minimumVersionLineEdit->text());
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
    connect(ui->languageVersionComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setLanguageVersionRequired(text);
    });
    connect(ui->languageVersionOptionComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setLanguageVersionRequiredStrict(text == "ENFORCED");
    });
    connect(ui->languageChooseComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        generator->setLanguage(text);
    });
    connect(ui->languageChooseComboBox, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        ui->languageVersionComboBox->clear();
        if (text == "C") {
            ui->languageVersionComboBox->addItems({"C89", "C99", "C11", "C17"});
            ui->languageVersionComboBox->setCurrentIndex(2);
        } else {
            ui->languageVersionComboBox->addItems({"C++98", "C++03", "C++11", "C++14", "C++17", "C++20", "C++23"});
            ui->languageVersionComboBox->setCurrentIndex(2);
        }
    });
    emit ui->languageVersionComboBox->currentTextChanged(ui->languageChooseComboBox->currentText());


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

    // 输出监控
    connect(&cmakeProcess, &QProcess::readyReadStandardOutput, this, [&]() {
        ui->buildLogTextEdit->append(QString::fromLocal8Bit(cmakeProcess.readAllStandardOutput()));
    });

    connect(&cmakeProcess, &QProcess::readyReadStandardError, this, [&]() {
        ui->buildLogTextEdit->append(QString::fromLocal8Bit(cmakeProcess.readAllStandardError()));
    });

    // 结束后重置进度条
    connect(&cmakeProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus status) {
                ui->buildProgressBar->setRange(0, 100);
                ui->buildProgressBar->setValue(100);

                if (exitCode == 0 && status == QProcess::NormalExit) {
                    ui->buildLogTextEdit->append("✅ 构建配置完成！");
                } else {
                    ui->buildLogTextEdit->append("❌ 构建失败！");
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

    handle_genButton_clicked();

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

void MainWindow::handle_addMacroPushButton_clicked()
{
    QString macroName = ui->addMacroLineEdit->text().trimmed();
    if (!macroName.isEmpty()) {
        // 检查重复
        bool exists = false;
        for (int i = 0; i < ui->macroListWidget->count(); ++i) {
            if (ui->macroListWidget->item(i)->text() == macroName) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            ui->macroListWidget->addItem(macroName);
            ui->addMacroLineEdit->clear();
        } else {
            QMessageBox::warning(this, "重复宏", "该宏已存在！");
            ui->addMacroLineEdit->clear();
        }
    }
    QStringList macroList;
    for (int i = 0; i < ui->macroListWidget->count(); ++i) {
        macroList << ui->macroListWidget->item(i)->text();
    }
    generator->setMacroList(macroList);

}

void MainWindow::handle_deleteMacroPushButton_clicked()
{
    auto selectedItems = ui->macroListWidget->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i) {
        auto item = selectedItems.at(i);
        delete ui->macroListWidget->takeItem(ui->macroListWidget->row(item));
    }
    QStringList macroList;
    for (int i = 0; i < ui->macroListWidget->count(); ++i) {
        macroList << ui->macroListWidget->item(i)->text();
    }
    generator->setMacroList(macroList);
}

void MainWindow::handle_buildPushButton_clicked()
{
    if (ui->showDirlineEdit->text().isEmpty()) {
        QMessageBox::critical(this, "错误", "请先选择一个文件目录！");
        return;
    }

    handle_saveButton_clicked();

    ui->buildLogTextEdit->clear();

    QString basePath = ui->outputFileDirLineEdit->text();
    QString buildPath = basePath + "/build";

    // 如果 buildPath 存在，整个删掉
    QDir buildDir(buildPath);
    if (buildDir.exists()) {
        buildDir.removeRecursively();
    }
    // 重新创建 build 目录
    QDir().mkpath(buildPath);

    // 设置工作目录
    cmakeProcess.setWorkingDirectory(buildPath);

    // 启动 cmake 配置命令
    cmakeProcess.start("cmake", QStringList() << basePath);  // 如果 CMakeLists.txt 在 basePath 下

    // 设置为“忙碌状态”的进度条（不确定进度）
    ui->buildProgressBar->setRange(0, 0);

}

void MainWindow::updateGeneratorSettings()
{
    generator->setProjectPath(ui->showDirlineEdit->text());
    generator->setProjectName(ui->projectNameLineEdit->text());
    generator->setCMakeVersionRequired(ui->minimumVersionLineEdit->text());
    generator->setExeFileName(ui->outputFileNameLineEdit->text());
    generator->setExeFileScope(ui->exeFileScopeComboBox->currentText());
    generator->setOutputFileType(ui->outputFileTypeOptionComboBox->currentText());
    generator->setLanguageVersionRequired(ui->languageVersionComboBox->currentText());
    generator->setLanguageVersionRequiredStrict(ui->languageVersionOptionComboBox->currentText() == "ENFORCED");
    generator->setExeFileOutput(ui->outputFileDirLineEdit->text());
    generator->setLanguage(ui->languageChooseComboBox->currentText());

    QStringList macroList;
    for (int i = 0; i < ui->macroListWidget->count(); ++i) {
        macroList << ui->macroListWidget->item(i)->text();
    }
    generator->setMacroList(macroList);

}
