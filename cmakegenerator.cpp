#include "cmakegenerator.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

CMakeGenerator::CMakeGenerator(QObject *parent)
    : QObject(parent) {}

void CMakeGenerator::setProjectName(const QString &name) {
    m_projectName = name;
}

void CMakeGenerator::setExeFileName(const QString &exefilename) {
    m_exeFileName = exefilename;
}

void CMakeGenerator::setProjectPath(const QString &path) {
    m_projectPath = path;
}

void CMakeGenerator::setVersionRequired(const QString &version) {
    m_versionRequired = version;
}

void CMakeGenerator::setCxxVersionRequired(const QString &CXXversion) {
    m_cxxVersion = CXXversion;
}

void CMakeGenerator::setCxxVersionOption(bool option) {
    m_cxxVersion_option = option;
}

void CMakeGenerator::setExeFileScope(const QString &scope) {
    m_exeFileScope = scope;
}

void CMakeGenerator::setExeFileOutput(const QString &exefileoutput) {
    m_exeFileOutput = exefileoutput;
}

void CMakeGenerator::setOutputFileType(const QString &outputFileType) {
    m_outputFileType = outputFileType;
}

void CMakeGenerator::setMacroList(const QStringList &macroList) {
    m_macroList = macroList;
}

// 收集源文件
QString CMakeGenerator::collectSourceFiles(const QStringList &files) const {
    qDebug() << "rootpath:" << m_projectPath;
    QStringList result;
    result << "set(SOURCES";

    // 假设 CMAKE_CURRENT_SOURCE_DIR 是已知的项目根目录
    QString projectRootDir = m_projectPath;  // 这里的 CMAKE_CURRENT_SOURCE_DIR 需要在上下文中定义

    // 使用下标访问遍历文件列表
    for (int i = 0; i < files.size(); ++i) {
        QString file = files[i];  // 使用下标访问文件

        // 检查文件扩展名是否是 .c 或 .cpp
        if (file.endsWith(".c", Qt::CaseInsensitive) || file.endsWith(".cpp", Qt::CaseInsensitive)) {
            // 确保文件路径的分隔符一致性，去掉前缀路径
            if (file.startsWith(projectRootDir)) {
                file.remove(0, projectRootDir.length());
                if (file.startsWith("/") || file.startsWith("\\")) {
                    file.remove(0, 1);  // 移除路径分隔符
                }
            }

            result << QString("      %1").arg(file);
        }
    }

    result << ")\n";
    return result.join("\n");
}

// 收集头文件
QString CMakeGenerator::collectHeadFiles(const QStringList &files) const {
    qDebug() << "rootpath:" << m_projectPath;
    QStringList result;
    result << "set(HEADERS";

    // 假设 CMAKE_CURRENT_SOURCE_DIR 是已知的项目根目录
    QString projectRootDir = m_projectPath;  // 这里的 CMAKE_CURRENT_SOURCE_DIR 需要在上下文中定义

    // 使用下标访问遍历文件列表
    for (int i = 0; i < files.size(); ++i) {
        QString file = files[i];  // 使用下标访问文件

        // 检查文件扩展名是否是 .c 或 .cpp
        if (file.endsWith(".h", Qt::CaseInsensitive) || file.endsWith(".hpp", Qt::CaseInsensitive)) {
            // 确保文件路径的分隔符一致性，去掉前缀路径
            if (file.startsWith(projectRootDir)) {
                file.remove(0, projectRootDir.length());
                if (file.startsWith("/") || file.startsWith("\\")) {
                    file.remove(0, 1);  // 移除路径分隔符
                }
            }

            result << QString("      %1").arg(file);
        }
    }

    result << ")\n";
    return result.join("\n");
}

QString CMakeGenerator::generateCMakeContent(const QStringList &files) const {
    QStringList lines;

    lines << QString("# CMake 最低版本要求")
          << QString("cmake_minimum_required(VERSION %1)\n").arg(m_versionRequired);

    lines << QString("# 项目信息")
          << QString("project(%1 LANGUAGES CXX)\n").arg(m_projectName);

    lines << QString("# 设置 C++ 标准")
          << QString("set(CMAKE_CXX_STANDARD %1)\n").arg(m_cxxVersion);

    if (m_cxxVersion_option) {
        lines << "# 启用强制要求 C++ 标准"
              << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
    }

    if (!m_macroList.isEmpty()) {
        lines << "# 宏定义列表";
        for (int i = 0; i < m_macroList.count(); ++i) {
            QString macro = m_macroList.at(i);
            lines << QString("add_definitions(-D%1)\n").arg(macro);
        }
    }

    // 可选的 Qt 自动功能
    // lines << "set(CMAKE_AUTOMOC ON)\n";
    // lines << "set(CMAKE_AUTOUIC ON)\n";
    // lines << "set(CMAKE_AUTORCC ON)\n";
    // lines << "find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)\n";

    // 源文件
    lines << "# 源文件列表";
    QString sourceFiles = collectSourceFiles(files);
    lines << sourceFiles;

    // 头文件
    lines << "# 头文件列表";
    QString headFiles = collectHeadFiles(files);
    lines << headFiles;

    // 头文件包含目录
    QSet<QString> uniqueIncludeDirs;
    for (const QString &file : files) {
        if (file.endsWith(".h", Qt::CaseInsensitive) || file.endsWith(".hpp", Qt::CaseInsensitive)) {
            QFileInfo info(file);
            QString absDirPath = info.absolutePath();
            QString relDirPath = QDir(m_projectPath).relativeFilePath(absDirPath);
            if (!relDirPath.isEmpty()) {
                uniqueIncludeDirs.insert(relDirPath.replace("\\", "/"));
            }
        }
    }

    if (!uniqueIncludeDirs.isEmpty()) {
        lines << "# 头文件包含目录";
        for (const QString &dir : uniqueIncludeDirs) {
            lines << QString("include_directories(${CMAKE_CURRENT_SOURCE_DIR}/%1)\n").arg(dir);
        }
    }

    // 输出文件类型（可执行/静态库/动态库等）
    lines << "# 设置输出目标";
    if (m_outputFileType == "ExeFile") {
        lines << QString("add_executable(%1 ${SOURCES} ${HEADERS})\n").arg(m_exeFileName);
    } else if (m_outputFileType == "StaticLib") {
        lines << QString("add_library(%1 STATIC ${SOURCES} ${HEADERS})\n").arg(m_exeFileName);
    } else if (m_outputFileType == "DynamicLib") {
        lines << QString("add_library(%1 SHARED ${SOURCES} ${HEADERS})\n").arg(m_exeFileName);
    } else if (m_outputFileType == "InterfaceLib") {
        lines << QString("add_library(%1 INTERFACE ${SOURCES} ${HEADERS})\n").arg(m_exeFileName);
    }

    lines << "# 编译选项";
    lines << QString("target_compile_options(%1 %2 -Wall -g)\n").arg(m_exeFileName, m_exeFileScope);

    lines << "# 设置可执行文件输出路径";
    lines << QString("set(EXECUTABLE_OUTPUT_PATH %1)\n").arg(m_exeFileOutput);

    // 合并所有内容为字符串返回
    return lines.join("\n");
}

bool CMakeGenerator::generate(const QString &cmakecontent) {


    QFile cmakefile(QDir(m_projectPath).filePath("CMakeLists.txt"));
    if (!cmakefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入 CMakeLists.txt";
        return false;
    }

    QTextStream out(&cmakefile);
    out << cmakecontent;
    cmakefile.close();

    return true;
}
