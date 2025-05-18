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
    void setExefileName(const QString &exefilename);
    void setExefileScope(const QString &scope);
    void setVersionRequired(const QString &Version);
    void setCxxversionReqired(const QString &CXXversion);
    void setCxxversionoption(const bool &option);
    QString generateCMakeContent(const QStringList &files) const;
    bool generate(const QString &cmakecontent);

private:

    QString m_projectName = "default_projectname";
    QString m_projectPath;
    QString m_exefilename = "default_exefilename";
    QString m_exefilescope = "PRIVATE";
    QString m_versionRequired = "3.14";
    QString m_cxxversion = "14";
    bool m_cxxversion_option = true;
    QStringList m_sources;

    QString collectSourceFiles(const QStringList &files) const;
};

#endif // CMAKEGENERATOR_H
