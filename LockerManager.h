/**
 * @file LockerManager.h
 * @brief 储物箱管理类声明
 *
 * 智能储物箱管理系统 —— 管理类
 * 负责储物箱的增删改查、数据持久化、业务逻辑
 */

#ifndef LOCKERMANAGER_H
#define LOCKERMANAGER_H

#include <vector>
#include <string>
#include "Locker.h"

using namespace std;

class LockerManager {
private:
    vector<Locker> lockers;     // 储物箱列表
    int nextId;                 // 下一个可用箱号（自增）
    static const string FILENAME; // 数据文件路径

public:
    // ========== 构造与析构 ==========

    /**
     * @brief 构造函数：自动从文件加载数据
     */
    LockerManager();

    /**
     * @brief 析构函数：自动保存数据到文件
     */
    ~LockerManager();

    // ========== 数据持久化 ==========

    /**
     * @brief 从 locker.txt 加载储物箱数据
     */
    void loadData();

    /**
     * @brief 将储物箱数据保存到 locker.txt
     */
    void saveData() const;

    // ========== 核心业务 ==========

    /**
     * @brief 存物：查找空闲箱 → 生成取件码 → 存入物品
     * @param name  用户姓名
     * @param phone 电话号码
     * @param item  物品名称
     * @return 分配的箱号，若无空闲箱返回 -1
     */
    int storeItem(const string& name, const string& phone, const string& item);

    /**
     * @brief 取物：验证箱号与取件码 → 取出物品
     * @param id   箱号
     * @param code 取件码
     * @return true=成功, false=验证失败
     */
    bool takeItem(int id, const string& code);

    // ========== 查询功能 ==========

    /**
     * @brief 按箱号查询并显示
     * @param id 箱号
     * @return 是否找到
     */
    bool searchById(int id) const;

    /**
     * @brief 按姓名查询并显示
     * @param name 用户姓名
     * @return 是否找到
     */
    bool searchByName(const string& name) const;

    /**
     * @brief 按电话查询并显示
     * @param phone 电话号码
     * @return 是否找到
     */
    bool searchByPhone(const string& phone) const;

    // ========== 查看功能 ==========

    /// 显示所有储物箱
    void showAll() const;

    /// 显示空闲储物箱
    void showEmpty() const;

    /// 显示已占用储物箱
    void showOccupied() const;

    // ========== 管理员功能 ==========

    /**
     * @brief 添加一个新的储物箱
     */
    void addLocker();

    /**
     * @brief 删除指定箱号的空闲储物箱
     * @param id 箱号
     * @return true=删除成功, false=不存在或被占用
     */
    bool deleteLocker(int id);

    /**
     * @brief 显示统计信息（总数、占用、空闲、使用率）
     */
    void showStatistics() const;

    // ========== 辅助查询 ==========

    /**
     * @brief 按箱号查找储物箱，返回指针便于取物等操作
     * @param id 箱号
     * @return 找到返回指针，否则返回 nullptr
     */
    Locker* findLockerById(int id);

private:
    // ========== 内部工具 ==========

    /**
     * @brief 生成一个4位随机取件码（1000-9999）
     * @return 4位数字字符串
     */
    string generateCode() const;

    /**
     * @brief 查找第一个空闲储物箱
     * @return 找到返回下标，否则返回 -1
     */
    int findEmptyIndex() const;
};

#endif // LOCKERMANAGER_H
