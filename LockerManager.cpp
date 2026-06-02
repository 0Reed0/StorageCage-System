/**
 * @file LockerManager.cpp
 * @brief 储物箱管理类实现
 *
 * 包含数据持久化、存物取物、查询、管理统计等功能的完整实现
 */

#include "LockerManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// 数据文件路径
const string LockerManager::FILENAME = "locker.txt";

// ========== 构造与析构 ==========

LockerManager::LockerManager() : nextId(1) {
    // 初始化随机数种子（用于生成取件码）
    srand(static_cast<unsigned int>(time(nullptr)));

    // 启动时自动加载数据
    loadData();

    // 如果文件中没有任何储物箱，初始化几个默认的以供使用
    if (lockers.empty()) {
        // 预置 5 个储物箱，编号 1~5，初始均为空闲
        for (int i = 0; i < 5; ++i) {
            lockers.push_back(Locker(nextId));
            ++nextId;
        }
        cout << "[系统] 首次运行，已自动创建 5 个储物箱（箱号 1~5）" << endl;
    }
}

LockerManager::~LockerManager() {
    // 退出时自动保存数据
    saveData();
    cout << "[系统] 数据已保存到 locker.txt，感谢使用！" << endl;
}

// ========== 数据持久化 ==========

void LockerManager::loadData() {
    ifstream fin(FILENAME);
    if (!fin) {
        // 文件不存在或无法打开，可能是首次运行
        return;
    }

    lockers.clear();    // 清空现有数据
    nextId = 1;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;   // 跳过空行

        // 文件格式：箱号|占用标志|姓名|电话|物品|取件码|时间戳
        stringstream ss(line);
        string token;
        vector<string> fields;

        while (getline(ss, token, '|')) {
            fields.push_back(token);
        }

        // 至少需要7个字段
        if (fields.size() < 7) continue;

        int    id     = stoi(fields[0]);
        bool   occ    = (fields[1] == "1");
        string name   = fields[2];
        string phone  = fields[3];
        string item   = fields[4];
        string code   = fields[5];
        time_t t      = static_cast<time_t>(stoll(fields[6]));

        Locker locker(id);
        if (occ) {
            locker.storeItem(name, phone, item, code);
            locker.setStoreTime(t);  // 恢复文件中的原始时间
        }

        lockers.push_back(locker);
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    fin.close();
    cout << "[系统] 已从 locker.txt 加载 " << lockers.size() << " 个储物箱" << endl;
}

void LockerManager::saveData() const {
    ofstream fout(FILENAME);
    if (!fout) {
        cerr << "[错误] 无法打开文件 " << FILENAME << " 进行写入！" << endl;
        return;
    }

    // 逐行写入，字段用 | 分隔
    // 格式：箱号|占用(1/0)|姓名|电话|物品|取件码|时间戳
    for (const auto& locker : lockers) {
        fout << locker.getId()         << "|"
             << (locker.getIsOccupied() ? "1" : "0") << "|"
             << locker.getUserName()   << "|"
             << locker.getPhone()      << "|"
             << locker.getItemName()   << "|"
             << locker.getPickupCode() << "|"
             << locker.getStoreTime()  << endl;
    }

    fout.close();
}

// ========== 核心业务 ==========

int LockerManager::storeItem(const string& name, const string& phone,
                              const string& item) {
    // 查找第一个空闲储物箱
    int idx = findEmptyIndex();
    if (idx == -1) {
        return -1;  // 无空闲箱
    }

    // 生成4位随机取件码
    string code = generateCode();

    // 执行存入操作
    lockers[idx].storeItem(name, phone, item, code);

    cout << endl;
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║         存 物 成 功             ║" << endl;
    cout << "╠══════════════════════════════════╣" << endl;
    cout << "║  箱号：" << setw(4) << lockers[idx].getId() << "                   ║" << endl;
    cout << "║  取件码：" << setw(4) << code << "                   ║" << endl;
    cout << "║  请牢记取件码！                 ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << endl;

    return lockers[idx].getId();
}

bool LockerManager::takeItem(int id, const string& code) {
    Locker* locker = findLockerById(id);

    // 检查箱号是否存在
    if (locker == nullptr) {
        cout << "[提示] 箱号 " << id << " 不存在！" << endl;
        return false;
    }

    // 检查是否被占用
    if (!locker->getIsOccupied()) {
        cout << "[提示] " << id << " 号箱当前为空闲状态，无需取物。" << endl;
        return false;
    }

    // 验证取件码
    if (locker->getPickupCode() != code) {
        cout << "[提示] 取件码错误！" << endl;
        return false;
    }

    // 验证成功，显示物品信息后清空
    cout << endl;
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║         取 物 成 功             ║" << endl;
    cout << "╠══════════════════════════════════╣" << endl;
    cout << "║  箱号：" << setw(4) << locker->getId() << "                   ║" << endl;
    cout << "║  物品：" << locker->getItemName() << endl;
    cout << "║  用户：" << locker->getUserName() << endl;
    cout << "║  存入时间：" << Locker::formatTime(locker->getStoreTime()) << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << endl;

    locker->takeItem();
    return true;
}

// ========== 查询功能 ==========

bool LockerManager::searchById(int id) const {
    for (const auto& locker : lockers) {
        if (locker.getId() == id) {
            locker.display();
            return true;
        }
    }
    cout << "[提示] 未找到箱号为 " << id << " 的储物箱。" << endl;
    return false;
}

bool LockerManager::searchByName(const string& name) const {
    bool found = false;
    for (const auto& locker : lockers) {
        if (locker.getUserName() == name && locker.getIsOccupied()) {
            locker.display();
            found = true;
        }
    }
    if (!found) {
        cout << "[提示] 未找到姓名为 \"" << name << "\" 的用户记录。" << endl;
    }
    return found;
}

bool LockerManager::searchByPhone(const string& phone) const {
    bool found = false;
    for (const auto& locker : lockers) {
        if (locker.getPhone() == phone && locker.getIsOccupied()) {
            locker.display();
            found = true;
        }
    }
    if (!found) {
        cout << "[提示] 未找到电话为 \"" << phone << "\" 的用户记录。" << endl;
    }
    return found;
}

// ========== 查看功能 ==========

void LockerManager::showAll() const {
    if (lockers.empty()) {
        cout << "[提示] 当前没有储物箱。" << endl;
        return;
    }
    cout << "\n===== 全部储物箱（共 " << lockers.size() << " 个）=====" << endl;
    for (const auto& locker : lockers) {
        locker.display();
    }
}

void LockerManager::showEmpty() const {
    int count = 0;
    cout << "\n===== 空闲储物箱 =====" << endl;
    for (const auto& locker : lockers) {
        if (!locker.getIsOccupied()) {
            locker.display();
            ++count;
        }
    }
    if (count == 0) {
        cout << "[提示] 当前没有空闲储物箱。" << endl;
    } else {
        cout << "共 " << count << " 个空闲储物箱。" << endl;
    }
}

void LockerManager::showOccupied() const {
    int count = 0;
    cout << "\n===== 已占用储物箱 =====" << endl;
    for (const auto& locker : lockers) {
        if (locker.getIsOccupied()) {
            locker.display();
            ++count;
        }
    }
    if (count == 0) {
        cout << "[提示] 当前没有已占用的储物箱。" << endl;
    } else {
        cout << "共 " << count << " 个已占用储物箱。" << endl;
    }
}

// ========== 管理员功能 ==========

void LockerManager::addLocker() {
    Locker newLocker(nextId);
    lockers.push_back(newLocker);
    cout << "[成功] 已添加储物箱，箱号为 " << nextId << endl;
    ++nextId;
}

bool LockerManager::deleteLocker(int id) {
    for (auto it = lockers.begin(); it != lockers.end(); ++it) {
        if (it->getId() == id) {
            if (it->getIsOccupied()) {
                cout << "[提示] " << id << " 号箱当前被占用，无法删除！请先取物。" << endl;
                return false;
            }
            lockers.erase(it);
            cout << "[成功] 已删除 " << id << " 号储物箱。" << endl;
            return true;
        }
    }
    cout << "[提示] 未找到箱号为 " << id << " 的储物箱。" << endl;
    return false;
}

void LockerManager::showStatistics() const {
    int total    = static_cast<int>(lockers.size());
    int occupied = 0;

    for (const auto& locker : lockers) {
        if (locker.getIsOccupied()) {
            ++occupied;
        }
    }

    int empty = total - occupied;
    double usageRate = (total > 0) ? (100.0 * occupied / total) : 0.0;

    cout << endl;
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║       统  计  信  息            ║" << endl;
    cout << "╠══════════════════════════════════╣" << endl;
    cout << "║  总箱数：" << setw(6) << total    << "                ║" << endl;
    cout << "║  已占用：" << setw(6) << occupied << "                ║" << endl;
    cout << "║  空闲数：" << setw(6) << empty    << "                ║" << endl;
    cout << "║  使用率：" << setw(5) << fixed << setprecision(1)
              << usageRate << "%               ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << endl;
}

// ========== 辅助查询 ==========

Locker* LockerManager::findLockerById(int id) {
    for (auto& locker : lockers) {
        if (locker.getId() == id) {
            return &locker;
        }
    }
    return nullptr;
}

// ========== 内部工具 ==========

string LockerManager::generateCode() const {
    // 生成 1000~9999 之间的4位随机数作为取件码
    int code = rand() % 9000 + 1000;
    return to_string(code);
}

int LockerManager::findEmptyIndex() const {
    for (size_t i = 0; i < lockers.size(); ++i) {
        if (!lockers[i].getIsOccupied()) {
            return static_cast<int>(i);
        }
    }
    return -1;  // 没有空闲箱
}
