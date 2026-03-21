#include <iostream>
#include <string>
#include <vector>
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
    adjList[v1].push_back(ArcNode(v2, weight)); // 添加v1到v2的边
    adjList[v2].push_back(ArcNode(v1, weight)); // 添加v2到v1的边（无向图）
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
} // 计算路径总权重

void Graph::DFS(int v, vector<bool>& visited, vector<int>& path) {
    visited[v] = true;
    path.push_back(v);
    for (const auto& arc : adjList[v]) {
        if (!visited[arc.adjvex]) {
            DFS(arc.adjvex, visited, path);
        }
    }
} // 用于递归的DFS

vector<int> Graph::DFSTraverse(int start) {
    if (start < 0 || start >= vexs.size()) {
        return {};
    }
    vector<bool> visited(vexs.size(), false);
    vector<int> path;
    DFS(start, visited, path);
    for (size_t i = 0; i < vexs.size(); i++) {
        if (!visited[i]) {
            DFS(i, visited, path);
        }
    }
    return path;
} // 用于管理的DFS

vector<Edge> Graph::Prim() {
    int n = vexs.size();
    if (n == 0) return {};
    vector<bool> visited(n, false);
    vector<Edge> mstEdges;
    visited[0] = true;
    for (int count = 1; count < n; count++) {
        int minW = INF, u = -1, v = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) continue;
            for (const auto& arc : adjList[i]) {
                if (!visited[arc.adjvex] && arc.weight < minW) {
                    minW = arc.weight;
                    u = i;
                    v = arc.adjvex;
                }
            }
        }
        if (u == -1) break;
        mstEdges.push_back({u, v, minW});
        visited[v] = true;
    }
    return mstEdges;
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
