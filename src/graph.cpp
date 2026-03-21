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

vector<int> Graph::DFS(int start, int end, vector<bool>& visited) {
    if (start < 0 || start >= vexs.size() || end < 0 || end >= vexs.size()) {
        return {}; // 顶点索引无效返回空路径
    }
    vector<int> path;
    visited[start] = true; // 标记起点已访问
    path.push_back(start);
    if (start == end) {
        return path; // 起点等于终点直接返回
    }
    for (const auto& arc : adjList[start]) {
        if (!visited[arc.adjvex]) {
            vector<int> result = DFS(arc.adjvex, end, visited); // 递归搜索
            if (!result.empty()) {
                path.insert(path.end(), result.begin(), result.end());
                return path; // 找到路径返回
            }
        }
    }
    visited[start] = false; // 回溯
    path.pop_back();
    return {}; // 未找到路径返回空
} // 深度优先搜索找两点间路径

vector<int> Graph::DFSTraverse(int start) {
    if (start < 0 || start >= vexs.size()) {
        return {}; // 起点索引无效返回空路径
    }
    vector<bool> visited(vexs.size(), false);
    vector<int> path;
    vector<int> stack = {start}; // 用栈模拟递归
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        if (visited[v]) continue; // 已访问跳过
        visited[v] = true;
        path.push_back(v);
        for (int i = adjList[v].size() - 1; i >= 0; i--) { // 逆序入栈保证顺序
            int next = adjList[v][i].adjvex;
            if (!visited[next]) {
                stack.push_back(next);
            }
        }
    }
    for (size_t i = 0; i < visited.size(); i++) { // 处理未连通顶点
        if (!visited[i]) {
            stack.push_back(i);
            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                if (visited[v]) continue;
                visited[v] = true;
                path.push_back(v);
                for (int j = adjList[v].size() - 1; j >= 0; j--) {
                    int next = adjList[v][j].adjvex;
                    if (!visited[next]) {
                        stack.push_back(next);
                    }
                }
            }
        }
    }
    return path;
} // 深度优先遍历全图

vector<Edge> Graph::Prim() {
    int n = vexs.size();
    if (n == 0) {
        return {}; // 空图返回空边集
    }
    vector<bool> visited(n, false);
    vector<Edge> mstEdges;
    visited[0] = true; // 从第一个顶点开始
    for (int count = 0; count < n - 1; count++) {
        int minWeight = INF;
        int u = -1, v = -1;
        for (int i = 0; i < n; i++) {
            if (visited[i]) { // 遍历已访问顶点的邻边
                for (const auto& arc : adjList[i]) {
                    if (!visited[arc.adjvex] && arc.weight < minWeight) {
                        minWeight = arc.weight; // 更新最小权重
                        u = i;
                        v = arc.adjvex;
                    }
                }
            }
        }
        if (u == -1) break; // 图不连通退出
        mstEdges.push_back(Edge{u, v, minWeight}); // 添加最小边
        visited[v] = true; // 标记新顶点已访问
    }
    return mstEdges;
} // Prim算法求最小生成树

vector<int> Graph::dijkstra(int start, int end) {
    int n = vexs.size();
    if (start < 0 || start >= n || end < 0 || end >= n) {
        return {}; // 顶点索引无效返回空
    }
    vector<int> dist(n, INF); // 距离数组初始化为无穷大
    vector<bool> visited(n, false); // 访问标记数组
    vector<int> prev(n, -1); // 前驱顶点数组
    dist[start] = 0; // 起点距离为0
    for (int i = 0; i < n; i++) {
        int u = -1;
        int minDist = INF;
        for (int j = 0; j < n; j++) { // 找未访问顶点中距离最小的
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break; // 所有可达顶点已处理
        visited[u] = true;
        for (const auto& arc : adjList[u]) { // 松弛操作
            if (!visited[arc.adjvex] && dist[u] + arc.weight < dist[arc.adjvex]) {
                dist[arc.adjvex] = dist[u] + arc.weight;
                prev[arc.adjvex] = u; // 更新前驱
            }
        }
    }
    if (dist[end] == INF) {
        return {}; // 终点不可达返回空
    }
    vector<int> path;
    for (int current = end; current != -1; current = prev[current]) {
        path.insert(path.begin(), current); // 头插法构建路径
    }
    return path;
} // Dijkstra算法求两点间最短路径
