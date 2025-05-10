#include "cmakegenerator.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

CMakeGenerator::CMakeGenerator(QObject *parent)
    : QObject(parent) {}

void CMakeGenerator::setProjectName(const QString &name) {
    projectName = name;
}

void CMakeGenerator::setProjectPath(const QString &path) {
    projectPath = path;
}

QStringList CMakeGenerator::collectSourceFiles() const {
    QStringList result;
    QDir dir(projectPath);
    QStringList filters = {"*.cpp", "*.h", "*.ui", "*.qrc"};

    for (const QString &filter : filters) {
        QStringList files = dir.entryList(QStringList(filter), QDir::Files, QDir::Name);
        for (const QString &file : files) {
            result << file;
        }
    }

    return result;
}

QString CMakeGenerator::generateCMakeContent(const QStringList &sources) const {
    QStringList lines;

    lines << "cmake_minimum_required(VERSION 3.14)";
    lines << QString("project(%1 LANGUAGES CXX)").arg(projectName);
    lines << "";
    lines << "set(CMAKE_CXX_STANDARD 17)";
    lines << "set(CMAKE_AUTOMOC ON)";
    lines << "set(CMAKE_AUTOUIC ON)";
    lines << "set(CMAKE_AUTORCC ON)";
    lines << "";
    lines << "find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)";
    lines << "";
    lines << "add_executable(${PROJECT_NAME}";

    for (const QString &file : sources) {
        lines << QString("    %1").arg(file);
    }

    lines << ")";
    lines << "";
    lines << "target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)";

    return lines.join("\n");
}

bool CMakeGenerator::generate() {
    QStringList sources = collectSourceFiles();
    QString cmakeContent = generateCMakeContent(sources);

    QFile file(projectPath + "/CMakeLists.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入 CMakeLists.txt";
        return false;
    }

    QTextStream out(&file);
    out << cmakeContent;
    file.close();

    return true;
}
