#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QDirIterator>
#include <QDebug>
#include <QFileSystemModel>

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

private slots:

    void handle_dirbrowseButton_clicked();
    void read_files(const QString &rootPath);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
