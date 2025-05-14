#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>
#include "checkablefilesystemmodel.h"

// MyTreeView.h
class MyTreeView : public QTreeView {
    Q_OBJECT
public:
    explicit MyTreeView(QWidget *parent = nullptr);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    CheckableFileSystemModel *model;

public slots:
    void loadDirectory(const QString &path);
};

#endif // MYTREEVIEW_H
