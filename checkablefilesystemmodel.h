#ifndef CHECKABLEFILESYSTEMMODEL_H
#define CHECKABLEFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QHash>
#include <QStringList>
#include <QDebug>

class CheckableFileSystemModel : public QFileSystemModel {
    Q_OBJECT

public:
    explicit CheckableFileSystemModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void recursivelyCheckChildren(const QModelIndex &parent, Qt::CheckState state);
    QStringList getCheckedFiles() const;

private:
    // 文件勾选状态
    QHash<QString, Qt::CheckState> checkStates;

signals:
    // 将获取的文件列表发送给cmakegenerator
    void fileSelected(const QStringList files);
};

#endif // CHECKABLEFILESYSTEMMODEL_H
