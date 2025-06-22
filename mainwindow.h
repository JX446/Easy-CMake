#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QDirIterator>
#include <QDebug>
#include <QFileSystemModel>
#include <QIcon>
#include "cmakegenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<CMakeGenerator> generator;
    void updateGeneratorSettings();
    QProcess cmakeProcess;
    QProcess buildProcess;

private slots:

    void handle_dirBrowseButton_clicked();
    void handle_genButton_clicked();
    bool handle_saveButton_clicked();
    void handle_clearButton_clicked();
    void handle_outputFileDirPushButton_clicked();
    void handle_addMacroPushButton_clicked();
    void handle_deleteMacroPushButton_clicked();
    void handle_buildPushButton_clicked();


signals:
    void directorySelect(const QString &path);
};
#endif // MAINWINDOW_H
