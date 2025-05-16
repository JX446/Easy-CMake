#include "CheckableFileSystemModel.h"
#include <QTimer>

CheckableFileSystemModel::CheckableFileSystemModel(QObject *parent)
    : QFileSystemModel(parent) {}

// 在第一列加入勾选框
Qt::ItemFlags CheckableFileSystemModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);
    if (index.column() == 0) {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

// 返回是否被勾选
QVariant CheckableFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::CheckStateRole && index.column() == 0) {
        QString path = filePath(index);
        return checkStates.value(path, Qt::Unchecked);
    }
    return QFileSystemModel::data(index, role);
}

// 响应点击勾选框行为（当用户点击复选框时，Qt的视图系统就会自动调用 setData() 来让你处理这个行为，index value role这三个值会在点击时调用对应的函数自动获取）
bool CheckableFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::CheckStateRole && index.column() == 0) {
        QString path = filePath(index);
        Qt::CheckState newState = static_cast<Qt::CheckState>(value.toInt());

        // 更新当前项的勾选状态
        checkStates[path] = newState;

        // 如果当前状态是unchecked，则递归查找父目录，确保父目录也是unchecked
        if (newState == Qt::Unchecked) {
            QModelIndex parentIndex = index.parent();
            while (parentIndex.isValid() && filePath(parentIndex) != rootPath()) {
                QString parentPath = filePath(parentIndex);
                // 如果父目录是 checked 状态，则更新为 unchecked
                if (checkStates[parentPath] == Qt::Checked) {
                    checkStates[parentPath] = Qt::Unchecked;
                    emit dataChanged(parentIndex, parentIndex);
                }
                parentIndex = parentIndex.parent();  // 向上递归至父目录
            }
        }

        // 如果是目录，递归勾选其所有子项
        if (isDir(index)) {
            // 提前加载目录
            fetchMore(index);
            QTimer::singleShot(5, this, [=]() {
                recursivelyCheckChildren(index, newState);
            });
        }

        emit dataChanged(index, index);
        QTimer::singleShot(10, this, [=]() {
            for (auto it = checkStates.constBegin(); it != checkStates.constEnd(); ++it) {
                qDebug().noquote().nospace() << QString("%1 : %2")
                .arg(it.key(), -30)
                    .arg(it.value() == Qt::Checked ? "Checked" : "Unchecked");
            }
        });
        return true;
    }

    return QFileSystemModel::setData(index, value, role);
}

// 递归实现
void CheckableFileSystemModel::recursivelyCheckChildren(const QModelIndex &parent, Qt::CheckState state) {
    checkStates[filePath(parent)] = state;
    int childCount = rowCount(parent);
    for (int i = 0; i < childCount; ++i) {
        QModelIndex childIndex = this->index(i, 0, parent);
        QString childPath = filePath(childIndex);
        if (childPath.endsWith(".cpp") || childPath.endsWith(".c") ||
            childPath.endsWith(".h")   || childPath.endsWith(".hpp")) {
            // qDebug() << "child";
            checkStates[childPath] = state;
            emit dataChanged(childIndex, childIndex);
        }

        // 如果子项本身是目录，也递归处理
        if (isDir(childIndex)) {
            // 提前加载目录
            fetchMore(childIndex);
            QTimer::singleShot(5, this, [=]() {
                recursivelyCheckChildren(childIndex, state);
            });
        }
    }
}

// 获取所有被勾选的文件路径
QStringList CheckableFileSystemModel::getCheckedFiles() const {
    QStringList selected;
    for (auto it = checkStates.begin(); it != checkStates.end(); ++it) {
        if (it.value() == Qt::Checked) {
            selected << it.key();
        }
    }
    return selected;
}

// 重写获取headerdata逻辑
QVariant CheckableFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QFileInfo(this->rootPath()).fileName();  // 假设你存了这个路径名
        }
    }
    return QFileSystemModel::headerData(section, orientation, role);
}


