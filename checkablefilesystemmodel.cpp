#include "CheckableFileSystemModel.h"

CheckableFileSystemModel::CheckableFileSystemModel(QObject *parent)
    : QFileSystemModel(parent) {}

Qt::ItemFlags CheckableFileSystemModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);
    if (index.column() == 0) {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}

QVariant CheckableFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::CheckStateRole && index.column() == 0) {
        QString path = filePath(index);
        return checkStates.value(path, Qt::Unchecked);
    }
    return QFileSystemModel::data(index, role);
}

bool CheckableFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::CheckStateRole && index.column() == 0) {
        QString path = filePath(index);
        checkStates[path] = static_cast<Qt::CheckState>(value.toInt());
        emit dataChanged(index, index);
        return true;
    }
    return QFileSystemModel::setData(index, value, role);
}

QStringList CheckableFileSystemModel::getCheckedFiles() const {
    QStringList selected;
    for (auto it = checkStates.begin(); it != checkStates.end(); ++it) {
        if (it.value() == Qt::Checked) {
            selected << it.key();
        }
    }
    return selected;
}
