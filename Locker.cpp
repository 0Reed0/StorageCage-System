/**
 * @file Locker.cpp
 * @brief 储物箱类实现
 *
 * 包含 storeItem、takeItem、display、formatTime 等方法的具体实现
 */

#include "Locker.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ========== 构造函数 ==========

Locker::Locker(int boxId)
    : id(boxId)
    , isOccupied(false)
    , userName("")
    , phone("")
    , itemName("")
    , pickupCode("")
    , storeTime(0)
{
    // 初始化一个空闲储物箱，所有用户字段为空
}

// ========== 核心操作 ==========

void Locker::storeItem(const string& name, const string& tel,
                       const string& item, const string& code) {
    userName   = name;
    phone      = tel;
    itemName   = item;
    pickupCode = code;
    storeTime  = time(nullptr);   // 记录当前时间
    isOccupied = true;            // 标记为已占用
}

void Locker::takeItem() {
    // 清空所有用户相关数据，状态恢复为空闲
    isOccupied  = false;
    userName    = "";
    phone       = "";
    itemName    = "";
    pickupCode  = "";
    storeTime   = 0;
}

void Locker::display() const {
    cout << "----------------------------------------" << endl;
    cout << "  箱号：" << id << endl;
    cout << "  状态：" << (isOccupied ? "已占用" : "空闲") << endl;

    if (isOccupied) {
        cout << "  用户姓名：" << userName << endl;
        cout << "  联系电话：" << phone << endl;
        cout << "  物品名称：" << itemName << endl;
        cout << "  取件码："   << pickupCode << endl;
        cout << "  存入时间：" << formatTime(storeTime) << endl;
    }
    cout << "----------------------------------------" << endl;
}

// ========== 工具方法 ==========

string Locker::formatTime(time_t t) {
    if (t == 0) {
        return "未记录";
    }

    // localtime_s 是 Windows 安全版本，localtime_r 是 POSIX 版本
    // 这里使用 localtime_s（Windows）/ localtime_r（Linux）
    struct tm timeInfo;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&timeInfo, &t);
#else
    localtime_r(&t, &timeInfo);
#endif

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return string(buffer);
}
