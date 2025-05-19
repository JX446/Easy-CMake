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

    void handle_dirbrowseButton_clicked();
    void handle_genButton_clicked();
    bool handle_saveButton_clicked();
    void handle_clearButton_clicked();
    void handle_projectnamepushButton_clicked();
    void handle_minimumversionpushButton_clicked();
    void handle_exefilenamepushButton_clicked();
    void handle_exefilescopepushButton_clicked();
    void handle_exefileoutputpushButton_clicked();
    void handle_cxxversionpushButton_clicked();

signals:
    void directorySelected(const QString &path);
};
#endif // MAINWINDOW_H
