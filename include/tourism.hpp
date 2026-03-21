#ifndef TOURISM_HPP
#define TOURISM_HPP
#include "graph.hpp"

class Tourism {
private:
    Graph graph; // 图对象
public:
    Tourism(); // 构造函数
    ~Tourism(); // 析构函数
    int createGraph(const std::string& vexPath, const std::string& edgePath); // 创建图
    int getSpotInfo(); // 获取景点信息
    int showGraph(); // 显示图
    int TravelPath(); // 旅游路径
    int findShortPath(); // 查找最短路径
    int designMinPath(); // 设计最小路径
};

#endif