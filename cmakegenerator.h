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
    void setVersionRequired(const QString &Version);

    QString generateCMakeContent(const QStringList &files) const;
    bool generate(const QString &cmakecontent);

private:

    QString projectName;
    QString projectPath;
    QString VersionRequired = "3.14";
    QStringList sources;

    QString collectSourceFiles(const QStringList &files) const;
};

#endif // CMAKEGENERATOR_H
