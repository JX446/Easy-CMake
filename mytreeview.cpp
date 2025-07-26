#include "mytreeview.h"

MyTreeView::MyTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    // setDragDropMode(QAbstractItemView::DropOnly);

    model = new CheckableFileSystemModel(this);  // 初始化模型
    QStringList filters;
    filters << "*.h" << "*.c";
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
}

void MyTreeView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        qDebug() << "mimeData URLs:" << event->mimeData()->urls();
        qDebug() << "Has URLs!";
        event->acceptProposedAction();  // 允许进入
    } else {
        event->ignore();
    }
}

void MyTreeView::dropEvent(QDropEvent *event) {
    qDebug() << "dropEvent triggered";
    QString path = event->mimeData()->urls().first().toLocalFile();

    // 拖拽时再绑定文件模型，实现初始时窗口为空的效果
    model->setRootPath(path);
    this->setModel(model);
    this->setRootIndex(model->index(path));   // 文件模型和视图控件模型都需设置根目录

    this->hideColumn(1);
    this->hideColumn(2);
    this->hideColumn(3);
}

void MyTreeView::loadDirectory(const QString &path) {
    // 为空直接返回
    if (path.isEmpty()) {
        return;
    }

    model->setRootPath(path);
    model->setHeaderData(0, Qt::Horizontal, path);

    this->setModel(model);
    this->setRootIndex(model->index(path));

    this->hideColumn(1);
    this->hideColumn(2);
    this->hideColumn(3);
}
