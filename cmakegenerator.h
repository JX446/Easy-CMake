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
    void setExeFileName(const QString &exefilename);
    void setExeFileScope(const QString &scope);
    void setExeFileOutput(const QString &exefileoutput);
    void setVersionRequired(const QString &Version);
    void setCxxVersionRequired(const QString &CXXversion);
    void setCxxVersionOption(bool option);
    void setOutputFileType(const QString &outputFileType);

    QString generateCMakeContent(const QStringList &files) const;
    bool generate(const QString &cmakecontent);

private:

    QString m_projectName = "default_projectname";
    QString m_projectPath;
    QString m_exeFileName = "default_outputfilename";
    QString m_exeFileScope = "PRIVATE";
    QString m_versionRequired = "3.14";
    QString m_cxxVersion = "14";
    QString m_exeFileOutput = "${CMAKE_CURRENT_SOURCE_DIR}/build";
    QString m_outputFileType = "ExeFile";
    bool m_cxxVersion_option = true;

    QString collectSourceFiles(const QStringList &files) const;
    QString collectHeadFiles(const QStringList &files) const;
};

#endif // CMAKEGENERATOR_H
