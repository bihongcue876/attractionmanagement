#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "graph.hpp"

using namespace std;

Graph::Graph() : edgeNum(0) {} // 构造函数

Graph::~Graph() {} // 析构函数

void Graph::addVex(const Vex& vex) {
    vexs.push_back(vex); // 添加顶点到顶点数组
    adjList.emplace_back(); // 为新顶点创建邻接表
} // 添加顶点

Vex Graph::getVex(int index) const {
    if (index < 0 || index >= vexs.size()) {
        return Vex{-1, "", ""}; // 索引无效返回空顶点
    }
    return vexs[index];
} // 获取指定索引的顶点

void Graph::addEdge(int v1, int v2, int weight) {
    adjList[v1].push_back(ArcNode(v2, weight)); // 添加 v1 到 v2 的边
    adjList[v2].push_back(ArcNode(v1, weight)); // 添加 v2 到 v1 的边（无向图）
    edgeNum++;
} // 添加边（无向图双向添加）

int Graph::getWeight(int v1, int v2) const {
    if (v1 < 0 || v1 >= vexs.size() || v2 < 0 || v2 >= vexs.size()) {
        return INF; // 顶点索引无效返回无穷大
    }
    for (const auto& arc : adjList[v1]) {
        if (arc.adjvex == v2) {
            return arc.weight; // 找到边返回权重
        }
    }
    return INF; // 未找到边返回无穷大
} // 获取两顶点间边的权重

vector<ArcNode>& Graph::getAdjList(int v) {
    static vector<ArcNode> emptyList; // 静态空列表用于错误情况
    if (v < 0 || v >= vexs.size()) {
        return emptyList;
    }
    return adjList[v];
} // 获取顶点邻接表

const vector<ArcNode>& Graph::getAdjList(int v) const {
    static const vector<ArcNode> emptyList; // 静态空列表用于错误情况
    if (v < 0 || v >= vexs.size()) {
        return emptyList;
    }
    return adjList[v];
} // 获取顶点邻接表（常量版本）

int Graph::calcPathWeight(const vector<int>& path) const {
    int weight = 0;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int w = getWeight(path[i], path[i + 1]);
        if (w == INF) return INF; // 路径不可达返回无穷大
        weight += w;
    }
    return weight;
}

void Graph::DFS(int v, vector<bool>& visited, vector<int>& path) {
    visited[v] = true;              // 标记当前顶点 v 为已访问
    path.push_back(v);              // 将顶点 v 加入当前路径

    if (path.size() == vexs.size()) {   // 路径长度等于顶点总数，找到一条完整路径
        allPaths.push_back(path);       // 将完整路径保存到所有路径集合
    } else {
        vector<ArcNode> sortedAdj = adjList[v]; // 获取顶点 v 的邻接边列表
        sort(sortedAdj.begin(), sortedAdj.end(), [](const ArcNode& a, const ArcNode& b) {
            return a.weight < b.weight;         // 按边权从小到大排序，优先访问权重小的边
        });
        for (const auto& arc : sortedAdj) {     // 遍历所有邻接边
            if (!visited[arc.adjvex]) {         // 邻接顶点未访问过
                DFS(arc.adjvex, visited, path); // 递归访问邻接顶点
            }
        }
    }

    visited[v] = false;             // 回溯：取消顶点 v 的访问标记
    path.pop_back();                // 回溯：从路径中移除顶点 v
} // DFS 递归找所有路径（哈密顿路径）

vector<vector<int>> Graph::DFSTraverse(int start) {
    if (start < 0 || start >= vexs.size()) {    // 起点索引无效
        return {};                              // 返回空结果
    }
    allPaths.clear();                           // 清空之前的路径记录
    vector<bool> visited(vexs.size(), false);   // 初始化所有顶点为未访问
    vector<int> path;                           // 当前路径初始化为空
    DFS(start, visited, path);                  // 从起点开始深度优先搜索
    return allPaths;                            // 返回所有找到的路径
}

vector<Edge> Graph::Prim() {
    int n = vexs.size();                        // 获取顶点总数
    if (n == 0) return {};                      // 空图直接返回空结果
    vector<bool> visited(n, false);             // visited[i]=true 表示顶点 i 已加入 MST
    vector<Edge> mstEdges;                      // 存储最小生成树的边集
    visited[0] = true;                          // 从顶点 0 开始，标记为已访问
    for (int count = 1; count < n; count++) {   // 循环 n-1 次，每次添加一条边到 MST
        int minW = INF, u = -1, v = -1;         // minW 记录最小边权，u,v 记录边的两端点
        for (int i = 0; i < n; i++) {           // 遍历所有顶点
            if (!visited[i]) continue;          // 只考虑已加入 MST 的顶点
            for (const auto& arc : adjList[i]) {// 遍历顶点 i 的所有邻接边
                if (!visited[arc.adjvex] && arc.weight < minW) { // 找连接未访问顶点的最小权边
                    minW = arc.weight;          // 更新最小边权
                    u = i;                      // 记录边的起点（已在 MST 中）
                    v = arc.adjvex;             // 记录边的终点（待加入 MST）
                }
            }
        }
        if (u == -1) break;                     // 找不到有效边，说明图不连通，提前退出
        mstEdges.push_back({u, v, minW});       // 将最小边加入 MST 边集
        visited[v] = true;                      // 将新顶点 v 标记为已访问
    }
    return mstEdges;                            // 返回最小生成树的所有边
}

vector<int> Graph::dijkstra(int start, int end) {
    int n = vexs.size();
    if (start < 0 || start >= n || end < 0 || end >= n) return {};

    vector<int> dist(n, INF), prev(n, -1);
    vector<bool> visited(n, false);
    dist[start] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1, minD = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minD) {
                minD = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = true;
        for (const auto& arc : adjList[u]) {
            if (!visited[arc.adjvex] && dist[u] + arc.weight < dist[arc.adjvex]) {
                dist[arc.adjvex] = dist[u] + arc.weight;
                prev[arc.adjvex] = u;
            }
        }
    }

    if (dist[end] == INF) return {};
    vector<int> path;
    for (int cur = end; cur != -1; cur = prev[cur]) {
        path.insert(path.begin(), cur);
    }
    return path;
}
