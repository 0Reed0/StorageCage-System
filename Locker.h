/**
 * @file Locker.h
 * @brief 储物箱类声明
 *
 * 智能储物箱管理系统 —— 储物箱类
 * 封装单个储物箱的数据与操作
 */

#ifndef LOCKER_H
#define LOCKER_H

#include <string>
#include <ctime>

using namespace std;

class Locker {
private:
    int id;                 // 箱号
    bool isOccupied;        // 是否占用
    string userName;        // 用户姓名
    string phone;           // 电话号码
    string itemName;        // 物品名称
    string pickupCode;      // 4位取件码
    time_t storeTime;       // 存入时间（时间戳）

public:
    // ========== 构造函数 ==========

    /**
     * @brief 构造函数，初始化一个空的储物箱
     * @param boxId 箱号
     */
    Locker(int boxId = 0);

    // ========== 核心操作 ==========

    /**
     * @brief 存入物品
     * @param name  用户姓名
     * @param tel   电话号码
     * @param item  物品名称
     * @param code  系统生成的取件码
     */
    void storeItem(const string& name, const string& tel,
                   const string& item, const string& code);

    /**
     * @brief 取出物品，清空储物箱所有用户数据
     */
    void takeItem();

    /**
     * @brief 显示储物箱完整信息
     */
    void display() const;

    // ========== Getter 方法 ==========

    int getId()          const { return id; }
    bool getIsOccupied() const { return isOccupied; }
    string getUserName() const { return userName; }
    string getPhone()    const { return phone; }
    string getItemName() const { return itemName; }
    string getPickupCode() const { return pickupCode; }
    time_t getStoreTime() const { return storeTime; }

    // ========== Setter 方法 ==========

    void setId(int boxId) { id = boxId; }
    void setStoreTime(time_t t) { storeTime = t; }

    // ========== 工具方法 ==========

    /**
     * @brief 将时间戳转换为可读的日期时间字符串
     * @param t 时间戳
     * @return 格式化的时间字符串，如 "2024-06-15 14:30:25"
     */
    static string formatTime(time_t t);
};

#endif // LOCKER_H
