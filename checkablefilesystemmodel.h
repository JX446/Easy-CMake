#ifndef CHECKABLEFILESYSTEMMODEL_H
#define CHECKABLEFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QHash>
#include <QStringList>

class CheckableFileSystemModel : public QFileSystemModel {
    Q_OBJECT

public:
    explicit CheckableFileSystemModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QStringList getCheckedFiles() const;

private:
    QHash<QString, Qt::CheckState> checkStates;
};

#endif // CHECKABLEFILESYSTEMMODEL_H
