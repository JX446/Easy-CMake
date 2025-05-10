#ifndef CMAKEGENERATOR_H
#define CMAKEGENERATOR_H

#include <QObject>
#include <QString>
#include <QStringList>

class CMakeGenerator : public QObject {
    Q_OBJECT

public:
    explicit CMakeGenerator(QObject *parent = nullptr);

    void setProjectName(const QString &name);
    void setProjectPath(const QString &path);
    bool generate();

private:
    QString projectName;
    QString projectPath;

    QStringList collectSourceFiles() const;
    QString generateCMakeContent(const QStringList &sources) const;
};

#endif // CMAKEGENERATOR_H
