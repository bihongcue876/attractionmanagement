#ifndef TOURISM_H
#define TOURISM_H

#include "graph.hpp"


class Tourism{
private:
    Graph graph; // 类对象
public:
    Tourism(); //构造函数
    ~Tourism(); //析构函数
    int createGraph(const std::string &vexPath, const std::string &edgePath); //建图
    int designMinPath(); //规划最短路径（最小生成树）
    int findShortPath(); //寻找单源最短路径
    int getSpotInfo(); //获取单点信息
    int showGraph(); //展示图
    int TravelPath(); //旅游路径（深搜）
}; // 旅游类

#endif
