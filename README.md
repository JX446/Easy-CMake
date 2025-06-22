# <img src="icon.png" alt="Icon" width="120"> Easy-CMake
# Quick CMakeLists.txt GUI Generator 快速生成CMakeLists.txt的GUI程序

## 简介 Introduction

这是一个基于Qt的轻量级GUI工具，帮助用户快速自动生成符合项目结构的`CMakeLists.txt`文件，支持设置项目名称、编译选项、版本要求等，极大简化手动编写CMake脚本的繁琐工作。

A lightweight Qt-based GUI tool to quickly generate `CMakeLists.txt` files matching your project structure. It supports setting project name, compile options, version requirements, and more, greatly simplifying manual CMake script writing.

---

## 功能 Features

- 自动扫描项目目录，生成源文件和头文件列表  
- 设置项目名、输出文件名及输出路径  
- 配置CMake最低版本及C/C++语言版本  
- 支持宏定义和编译选项自定义  
- 生成完整、可直接使用的`CMakeLists.txt`文件  
- 友好的进度条和日志输出反馈

- Automatically scan project directory to generate source and header file lists  
- Set project name, executable name, and output directory  
- Configure minimum CMake version and C/C++ language standards  
- Support custom macros and compile options  
- Generate complete and ready-to-use `CMakeLists.txt` files  
- Friendly progress bar and log output feedback

---

## 使用方法 How to Use

1. 打开程序，选择项目根目录  
2. 配置项目名称、CMake版本、语言标准等参数  
3. 点击生成按钮，即可快速获得符合规范的`CMakeLists.txt`  
4. 将生成文件放置到项目根目录，使用CMake进行编译即可

1. Launch the application and select the project root directory  
2. Configure project name, CMake version, language standard, and other settings  
3. Click the generate button to quickly produce a compliant `CMakeLists.txt` file  
4. Place the generated file in your project root and build with CMake as usual

---

## 依赖 Dependencies

- Qt 5.12 及以上版本 (Qt 6.x 也支持)  
- C++17 编译器（建议）  

- Qt 5.12 or above (Qt 6.x supported)  
- C++17 compiler (recommended)  

---

## 贡献 Contributing

欢迎提交 issue 和 pull request，或者提出功能建议。请保持代码风格一致并添加必要注释。

Feel free to submit issues and pull requests or suggest new features. Please keep code style consistent and add necessary comments.

---

## 许可 License

MIT License

---


