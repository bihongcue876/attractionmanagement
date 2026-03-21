#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <vector>

using namespace std;
constexpr int INF = 999999; 

struct Vex {
    int num; // 景点编号
    string name; // 景点名称
    string desc; // 景点描述
};

struct Edge {
    int vex1; // 边的起点
    int vex2; // 边的终点
    int weight; // 边的权重
};

struct ArcNode {
    int adjvex; // 邻接顶点下标
    int weight; // 边的权重
    ArcNode(int v = -1, int w = INF) : adjvex(v), weight(w) {} // 构造函数
};

class Graph {
private:
    vector<Vex> vexs; // 顶点数组
    vector<vector<ArcNode>> adjList; // 邻接表
    int edgeNum; // 边的数量
    int traverseDist; // DFS遍历总距离
public:
    Graph(); // 构造函数
    ~Graph(); // 析构函数
    int getVexNum() const { return vexs.size(); } // 获取顶点数量
    int getEdgeNum() const { return edgeNum; } // 获取边数量
    int getTraverseDist() const { return traverseDist; } // 获取DFS遍历总距离
    void addVex(const Vex& vex); // 添加顶点
    Vex getVex(int index) const; // 获取顶点信息
    void addEdge(int v1, int v2, int weight); // 添加边
    int getWeight(int v1, int v2) const; // 获取边权重
    vector<ArcNode>& getAdjList(int v); // 获取邻接表
    const vector<ArcNode>& getAdjList(int v) const; // 获取邻接表(常量版本)
    int calcPathWeight(const vector<int>& path) const; // 计算路径权重
    void DFS(int v, vector<bool>& visited, vector<int>& path, int& ttlDist); // 深度优先搜索
    vector<int> DFSTraverse(int start); // 深度优先遍历
    vector<Edge> Prim(); // Prim最小生成树算法
    vector<int> dijkstra(int start, int end); // Dijkstra最短路径算法
};

#endif