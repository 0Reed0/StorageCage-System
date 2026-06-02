/**
 * @file main.cpp
 * @brief 智能储物箱管理系统 —— 主程序
 *
 * 程序入口，负责菜单交互与用户输入处理。
 *
 * 编译方式（Windows / Linux / macOS 通用）：
 *   g++ -std=c++11 -o locker.exe main.cpp Locker.cpp LockerManager.cpp
 *
 * 运行：
 *   ./locker.exe    （Linux/macOS）
 *   locker.exe      （Windows）
 */

#include "LockerManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ========== 函数声明 ==========

/// 显示主菜单
void showMainMenu();

/// 显示查询子菜单
void showSearchMenu();

/// 显示查看子菜单
void showViewMenu();

/// 显示管理员子菜单
void showAdminMenu();

/// 清空输入缓冲区（处理非法输入）
void clearInput();

/// 读取一个整数，带输入校验，prompt 为提示文字
int  readInt(const string& prompt);

/// 读取非空字符串
string readNonEmptyString(const string& prompt);

/// 暂停，等待用户按回车继续
void pauseForEnter();

// ========== 全局管理器 ==========
// 使用全局对象便于析构时自动保存数据
LockerManager manager;

// ========== main ==========

int main() {
    // 设置控制台为 UTF-8 编码，解决 Windows 下中文乱码问题
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001 > nul");
#endif

    cout << endl;
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║   智 能 储 物 箱 管 理 系 统       ║" << endl;
    cout << "║   Smart Locker Management System    ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    cout << endl;

    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        choice = readInt("请输入您的选择：");

        switch (choice) {
        // ===== 1. 存物 =====
        case 1: {
            cout << "\n===== 存 物 =====" << endl;

            string name = readNonEmptyString("请输入您的姓名：");
            string phone = readNonEmptyString("请输入您的电话：");
            string item = readNonEmptyString("请输入物品名称：");

            int boxId = manager.storeItem(name, phone, item);
            if (boxId == -1) {
                cout << "[提示] 储物箱已满，暂无空闲箱可用！" << endl;
            }
            pauseForEnter();
            break;
        }

        // ===== 2. 取物 =====
        case 2: {
            cout << "\n===== 取 物 =====" << endl;

            int boxId = readInt("请输入箱号：");
            cout << "请输入取件码：";
            string code;
            getline(cin, code);

            manager.takeItem(boxId, code);
            pauseForEnter();
            break;
        }

        // ===== 3. 查询 =====
        case 3: {
            int searchChoice = -1;
            while (searchChoice != 0) {
                showSearchMenu();
                searchChoice = readInt("请输入查询方式：");

                switch (searchChoice) {
                case 1: {
                    int id = readInt("请输入要查询的箱号：");
                    manager.searchById(id);
                    pauseForEnter();
                    break;
                }
                case 2: {
                    string name = readNonEmptyString("请输入要查询的姓名：");
                    manager.searchByName(name);
                    pauseForEnter();
                    break;
                }
                case 3: {
                    string phone = readNonEmptyString("请输入要查询的电话：");
                    manager.searchByPhone(phone);
                    pauseForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    cout << "[提示] 无效选项，请重新输入！（0-3）" << endl;
                    pauseForEnter();
                    break;
                }
            }
            break;
        }

        // ===== 4. 查看储物箱 =====
        case 4: {
            int viewChoice = -1;
            while (viewChoice != 0) {
                showViewMenu();
                viewChoice = readInt("请输入查看方式：");

                switch (viewChoice) {
                case 1:
                    manager.showAll();
                    pauseForEnter();
                    break;
                case 2:
                    manager.showEmpty();
                    pauseForEnter();
                    break;
                case 3:
                    manager.showOccupied();
                    pauseForEnter();
                    break;
                case 0:
                    break;
                default:
                    cout << "[提示] 无效选项，请重新输入！（0-3）" << endl;
                    pauseForEnter();
                    break;
                }
            }
            break;
        }

        // ===== 5. 管理员功能 =====
        case 5: {
            int adminChoice = -1;
            while (adminChoice != 0) {
                showAdminMenu();
                adminChoice = readInt("请输入管理操作：");

                switch (adminChoice) {
                case 1:
                    manager.addLocker();
                    pauseForEnter();
                    break;
                case 2: {
                    int id = readInt("请输入要删除的箱号：");
                    manager.deleteLocker(id);
                    pauseForEnter();
                    break;
                }
                case 3:
                    manager.showStatistics();
                    pauseForEnter();
                    break;
                case 0:
                    break;
                default:
                    cout << "[提示] 无效选项，请重新输入！（0-3）" << endl;
                    pauseForEnter();
                    break;
                }
            }
            break;
        }

        // ===== 0. 退出 =====
        case 0:
            cout << "[系统] 正在退出..." << endl;
            break;

        default:
            cout << "[提示] 无效选项，请重新输入！（0-5）" << endl;
            pauseForEnter();
            break;
        }
    }

    return 0;
}

// ========== 菜单显示函数 ==========

void showMainMenu() {
    cout << "+--------------------------------------+" << endl;
    cout << "|          主  菜  单                  |" << endl;
    cout << "|  1. 存物                            |" << endl;
    cout << "|  2. 取物                            |" << endl;
    cout << "|  3. 查询                            |" << endl;
    cout << "|  4. 查看储物箱                      |" << endl;
    cout << "|  5. 管理员功能                      |" << endl;
    cout << "|  0. 退出系统                        |" << endl;
    cout << "+--------------------------------------+" << endl;
}

void showSearchMenu() {
    cout << "\n+--------------------------------------+" << endl;
    cout << "|          查  询  菜  单              |" << endl;
    cout << "|  1. 按箱号查询                      |" << endl;
    cout << "|  2. 按姓名查询                      |" << endl;
    cout << "|  3. 按电话查询                      |" << endl;
    cout << "|  0. 返回主菜单                      |" << endl;
    cout << "+--------------------------------------+" << endl;
}

void showViewMenu() {
    cout << "\n+--------------------------------------+" << endl;
    cout << "|          查  看  菜  单              |" << endl;
    cout << "|  1. 查看所有储物箱                  |" << endl;
    cout << "|  2. 查看空闲储物箱                  |" << endl;
    cout << "|  3. 查看已占用储物箱                |" << endl;
    cout << "|  0. 返回主菜单                      |" << endl;
    cout << "+--------------------------------------+" << endl;
}

void showAdminMenu() {
    cout << "\n+--------------------------------------+" << endl;
    cout << "|        管 理 员 菜 单                |" << endl;
    cout << "|  1. 添加储物箱                      |" << endl;
    cout << "|  2. 删除空闲储物箱                  |" << endl;
    cout << "|  3. 查看统计信息                    |" << endl;
    cout << "|  0. 返回主菜单                      |" << endl;
    cout << "+--------------------------------------+" << endl;
}

// ========== 输入工具函数 ==========

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "[提示] 输入无效，请输入一个整数！" << endl;
            clearInput();
        }
    }
}

string readNonEmptyString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        // 去除首尾空白（简单处理：检查是否全为空字符）
        if (input.empty()) {
            cout << "[提示] 输入不能为空，请重新输入！" << endl;
            continue;
        }
        // 检查是否只包含空白字符
        bool allSpace = true;
        for (char c : input) {
            if (c != ' ' && c != '\t') {
                allSpace = false;
                break;
            }
        }
        if (allSpace) {
            cout << "[提示] 输入不能全是空白字符，请重新输入！" << endl;
            continue;
        }
        return input;
    }
}

void pauseForEnter() {
    cout << "\n[按回车键继续...]";
    // 先读取一个字符（阻塞等待），再清空缓冲区剩余内容
    cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
