#ifndef CMAKEGENERATOR_H
#define CMAKEGENERATOR_H

#include <QObject>
#include <QString>
#include <QStringList>

class CMakeGenerator : public QObject {
    Q_OBJECT

public:
    explicit CMakeGenerator(QObject *parent = nullptr);

    // === 项目信息设置 ===
    void setProjectName(const QString &name);
    void setProjectPath(const QString &path);
    void setCMakeVersionRequired(const QString &version);

    // === 可执行文件相关设置 ===
    void setExeFileName(const QString &exeFileName);
    void setExeFileScope(const QString &scope);
    void setExeFileOutput(const QString &exeFileOutput);
    void setOutputFileType(const QString &outputFileType);

    // === 编译配置 ===
    void setLanguage(const QString &language);                         // "C" / "CXX"
    void setLanguageVersionRequired(const QString &version);           // 如 "14"
    void setLanguageVersionRequiredStrict(bool strict);                // 是否严格启用标准
    void setMacroList(const QStringList &macroList);                   // 宏定义

    // === 核心生成函数 ===
    QString generateCMakeContent(const QStringList &files) const;
    bool generate(const QString &cmakeContent);  // 写入 CMakeLists.txt

private:
    // === 配置参数 ===
    QString m_projectName              = "default_projectname";
    QString m_projectPath;
    QString m_cmakeVersionRequired     = "3.14";
    QString m_language                 = "CXX";              // 默认 C++
    QString m_languageVersionRequired  = "14";               // 默认 C++14
    bool    m_languageVersionRequiredStrict = true;

    QString m_exeFileName              = "default_outputfilename";
    QString m_exeFileScope             = "PRIVATE";
    QString m_exeFileOutput            = "${CMAKE_CURRENT_SOURCE_DIR}/build";
    QString m_outputFileType           = "ExeFile";
    QStringList m_macroList;

    // === 文件处理工具函数 ===
    QString collectSourceFiles(const QStringList &files) const;
    QString collectHeaderFiles(const QStringList &files) const;
};

#endif // CMAKEGENERATOR_H
