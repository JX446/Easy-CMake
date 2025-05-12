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

private:
    Ui::MainWindow *ui;

private slots:

    void handle_dirbrowseButton_clicked();

signals:
    void directorySelected(const QString &path);
};
#endif // MAINWINDOW_H
