#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>

const int INF = 999999;         // 无穷大值，表示无边连接

// 顶点
typedef struct Vex {
    int num;                    // 景点编号
    std::string name;           // 景点名称
    std::string desc;           // 景点描述
} Vex;

// 边
typedef struct Edge {
    int vex1;                   // 顶点1编号
    int vex2;                   // 顶点2编号
    int weight;                 // 边权重
} Edge;

// 邻接表节点
typedef struct ArcNode {
    int adjvex;                 // 邻接顶点下标
    int weight;                 // 边权重
    ArcNode(int v = -1, int w = INF) : adjvex(v), weight(w) {}
} ArcNode;

// 图类（使用vector的邻接表）
class Graph {
private:
    std::vector<Vex> vexs;                    // 顶点数组
    std::vector<std::vector<ArcNode>> adjList; // 邻接表
    int edgeNum;                              // 当前边数量

public:
    Graph(); // 构造函数
    ~Graph(); // 析构函数
    int getVexNum() const;// 获取顶点数量
    int getEdgeNum() const; // 获取边数量
    Vex getVex(int index) const;// 获取指定索引的顶点信息
    void addVex(const Vex& vex);// 添加顶点
    bool removeVex(int v);// 删除指定顶点
    Edge getEdge(int v1, int v2) const; // 获取边信息
    bool setVexNum(int v, int num); // 设置顶点编号
    const std::vector<ArcNode>& getAdjList(int v) const;// 获取顶点的邻接表
    void addEdge(int v1, int v2, int weight);// 添加边
    bool removeEdge(int v1, int v2);// 删除边
    int getWeight(int v1, int v2) const;// 获取两顶点间的权重
    // 算法
    std::pair<std::vector<int>, int> DFS(int start, int end, std::vector<bool>& visited); // DFS算法，返回路径和权重
    std::pair<std::vector<int>, int> DFSTraverse(int start); // DFS遍历所有顶点，返回路径和总权重
    void DFSAll(int start, std::vector<bool>& visited, std::vector<int>& path, int& totalWeight); // DFS辅助函数
    void DFSTraverseAll(int v0); // DFS算法综合，验证连通性
    std::pair<std::vector<Edge>, int> Prim(); // Prim最小生成树算法，返回边数组和总权重
    std::pair<std::vector<int>, std::vector<std::vector<int>>> dijkstra(int start); // Dijkstra算法，返回距离和路径
    
};

#endif
