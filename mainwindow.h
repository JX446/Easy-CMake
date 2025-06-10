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

private slots:

    void handle_dirBrowseButton_clicked();
    void handle_genButton_clicked();
    bool handle_saveButton_clicked();
    void handle_clearButton_clicked();
    void handle_projectNamePushButton_clicked();
    void handle_minimumVersionPushButton_clicked();
    void handle_exeFilenamePushButton_clicked();
    void handle_exeFileScopePushButton_clicked();
    void handle_exeFileOutputPushButton_clicked();
    void handle_cxxVersionPushButton_clicked();
    void handle_outputFileTypeOptionPushButton_clicked();

signals:
    void directorySelect(const QString &path);
};
#endif // MAINWINDOW_H
