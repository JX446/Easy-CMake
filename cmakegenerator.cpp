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

void CMakeGenerator::setProjectPath(const QString &path) {
    m_projectPath = path;
}

void CMakeGenerator::setVersionRequired(const QString &version) {
    m_versionRequired = version;
}

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

            result << QString(" %1").arg(file);
        }
    }

    result << ")\n";
    return result.join("\n");
}

QString CMakeGenerator::generateCMakeContent(const QStringList &files) const {
    QStringList lines;

    lines << QString("cmake_minimum_required(VERSION %1)").arg(m_versionRequired) + "\n";
    lines << QString("project(%1 LANGUAGES CXX)").arg(m_projectName) + "\n";
    lines << "set(CMAKE_CXX_STANDARD 17)\n";
    lines << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
    lines << "set(CMAKE_AUTOMOC ON)\n";
    lines << "set(CMAKE_AUTOUIC ON)\n";
    lines << "set(CMAKE_AUTORCC ON)\n";
    lines << "find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)\n";

    // 假设 collectSourceFiles 返回的是 QStringList
    QString result = collectSourceFiles(files);
    lines << result;  // 将 QStringList 合并为一个字符串并加入到 lines 中
    lines << "add_executable(${PROJECT_NAME} ${SOURCES})\n";
    lines << "target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)";

    // 将返回结果从qstringlist变成qstring
    return lines.join("\n");
}

bool CMakeGenerator::generate(const QString &cmakecontent) {


    QFile cmakefile(m_projectPath + "/CMakeLists.txt");
    if (!cmakefile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入 CMakeLists.txt";
        return false;
    }

    QTextStream out(&cmakefile);
    out << cmakecontent;
    cmakefile.close();

    return true;
}
