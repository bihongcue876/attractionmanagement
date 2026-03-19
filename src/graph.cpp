#include <iostream>
#include <string>
#include <vector>
#include "graph.hpp"

using namespace std;


// 构造函数
Graph::Graph(){
    edgeNum = 0; // 初始化边数量
}

// 析构函数
Graph::~Graph(){
}

// 获取顶点数量
int Graph::getVexNum() const{
    return vexs.size();
}

// 获取边的数量
int Graph::getEdgeNum() const{
    return edgeNum;
}

// 获取指定索引的顶点
Vex Graph::getVex(int index) const{
    if (index < 0 || index >= static_cast<int>(vexs.size())) {
        return Vex{-1, "", ""}; // 返回无效顶点
    }
    return vexs[index];
}

// 添加顶点
void Graph::addVex(const Vex& vex){
    vexs.push_back(vex);
    adjList.push_back(vector<ArcNode>());
    edgeNum = 0; // 重置边数量
}

// 删除指定顶点
bool Graph::removeVex(int v){
    if (v < 0 || v >= static_cast<int>(vexs.size())) {
        return false;
    }
    vexs.erase(vexs.begin() + v);
    adjList.erase(adjList.begin() + v);
    edgeNum = 0; // 重置边数量
    return true;
}

// 获取两个顶点之间的边
Edge Graph::getEdge(int v1, int v2) const{
    if (v1 < 0 || v1 >= static_cast<int>(vexs.size()) ||
        v2 < 0 || v2 >= static_cast<int>(vexs.size())) {
        return Edge{-1, -1, INF}; // 返回无效边
    }
    
    for (const auto& arc : adjList[v1]) {
        if (arc.adjvex == v2) {
            return Edge{v1, v2, arc.weight}; // 返回找到的边
        }
    }
    
    return Edge{-1, -1, INF}; // 边不存在
}

// 设置顶点数量
bool Graph::setVexNum(int v, int num){
    if (v < 0 || v >= static_cast<int>(vexs.size())) {
        return false;
    }
    vexs[v].num = num;
    return true;
}

// 获取指定顶点的邻接表
const vector<ArcNode>& Graph::getAdjList(int v) const{
    if (v < 0 || v >= static_cast<int>(vexs.size())) {
        static const vector<ArcNode> emptyList;
        return emptyList; // 返回空列表
    }
    return adjList[v];
}

// 添加边
void Graph::addEdge(int v1, int v2, int weight){
    adjList[v1].push_back(ArcNode(v2, weight));
    adjList[v2].push_back(ArcNode(v1, weight));
    edgeNum++; // 增加边数量
}

// 删除边
bool Graph::removeEdge(int v1, int v2){
    if (v1 < 0 || v1 >= static_cast<int>(vexs.size()) ||
        v2 < 0 || v2 >= static_cast<int>(vexs.size())) {
        return false;
    }
    
    bool found = false;
    // 删除v1到v2的边
    for (auto it = adjList[v1].begin(); it != adjList[v1].end(); ++it) {
        if (it->adjvex == v2) {
            adjList[v1].erase(it);
            found = true;
            break;
        }
    }
    
    // 删除v2到v1的边
    for (auto it = adjList[v2].begin(); it != adjList[v2].end(); ++it) {
        if (it->adjvex == v1) {
            adjList[v2].erase(it);
            break;
        }
    }
    
    if (found) {
        edgeNum--;
        return true;
    }
    
    return false;
}

// 获取边的权重
int Graph::getWeight(int v1, int v2) const{
    if (v1 < 0 || v1 >= static_cast<int>(vexs.size()) ||
        v2 < 0 || v2 >= static_cast<int>(vexs.size())) {
        return INF; // 返回无穷大表示无效
    }
    
    for (const auto& arc : adjList[v1]) {
        if (arc.adjvex == v2) {
            return arc.weight;
        }
    }
    
    return INF; // 边不存在
}

// 深度优先搜索，返回路径和总权重
pair<vector<int>, int> Graph::DFS(int start, int end, vector<bool>& visited){
    int n = vexs.size(); // 获取顶点数量
    if (start < 0 || start >= n || end < 0 || end >= n) { // 检查起点和终点是否有效
        vector<int> emptyPath; // 空路径
        return make_pair(emptyPath, -1); // 返回空结果
    }
    
    vector<int> path; // 存储路径
    int totalWeight = 0; // 总权重
    
    visited[start] = true; // 标记起点已访问
    path.push_back(start); // 将起点加入路径
    
    if (start == end) { // 如果起点就是终点
        return make_pair(path, totalWeight); // 返回路径和权重
    }
    
    // 遍历起点的所有邻接顶点
    for (int i = 0; i < (int)adjList[start].size(); i++) {
        int v = adjList[start][i].adjvex; // 邻接顶点编号
        int weight = adjList[start][i].weight; // 边的权重
        
        if (!visited[v]) { // 如果邻接顶点未访问
            pair<vector<int>, int> result = DFS(v, end, visited); // 递归搜索
            if (!result.first.empty()) { // 如果找到路径
                totalWeight = weight + result.second; // 计算总权重
                path.insert(path.end(), result.first.begin(), result.first.end()); // 合并路径
                return make_pair(path, totalWeight); // 返回路径和权重
            }
        }
    }
    
    visited[start] = false; // 回溯，取消访问标记
    path.pop_back(); // 移除起点
    vector<int> emptyPath; // 空路径
    return make_pair(emptyPath, -1); // 未找到路径
} // DFS函数结束

// 深度优先遍历，验证连通性
void Graph::DFSTraverse(int v0){
    int n = vexs.size(); // 获取顶点数量
    if (v0 < 0 || v0 >= n) { // 检查起始顶点是否有效
        return; // 无效顶点，直接返回
    }
    
    vector<bool> visited(n, false); // 访问标记数组，初始化为未访问
    vector<int> path; // 存储遍历路径
    
    // 从v0开始深度优先遍历
    for (int i = v0; i < n; i++) {
        if (!visited[i]) { // 如果顶点i未访问
            // 调用DFS进行搜索
            pair<vector<int>, int> result = DFS(i, i, visited);
            if (!result.first.empty()) { // 如果找到路径
                // 可以在这里处理遍历结果
            }
        }
    }
}

// Prim算法求最小生成树，返回边数组和总权重
pair<vector<Edge>, int> Graph::Prim(){
    int n = vexs.size(); // 获取顶点数量
    if (n == 0) { // 如果图没有顶点
        return make_pair(vector<Edge>(), 0); // 返回空结果
    }
    
    vector<bool> visited(n, false); // 访问标记数组
    vector<Edge> mstEdges; // 最小生成树的边数组
    int totalWeight = 0; // 总权重
    
    visited[0] = true; // 从第一个顶点开始
    
    // 循环n-1次，每次添加一条边
    for (int count = 0; count < n - 1; count++) {
        int minWeight = INF; // 最小权重
        int u = -1; // 已访问顶点
        int v = -1; // 未访问顶点
        
        // 遍历所有已访问顶点的邻接边
        for (int i = 0; i < n; i++) {
            if (visited[i]) { // 如果顶点i已访问
                for (int j = 0; j < (int)adjList[i].size(); j++) {
                    int adjVex = adjList[i][j].adjvex; // 邻接顶点
                    int weight = adjList[i][j].weight; // 边权重
                    
                    // 如果邻接顶点未访问且权重更小
                    if (!visited[adjVex] && weight < minWeight) {
                        minWeight = weight;
                        u = i;
                        v = adjVex;
                    }
                }
            }
        }
        
        if (u == -1) break; // 找不到边，图不连通
        
        mstEdges.push_back(Edge{u, v, minWeight}); // 添加边
        totalWeight += minWeight; // 累加权重
        visited[v] = true; // 标记新顶点
    }
    
    return make_pair(mstEdges, totalWeight); // 返回结果
}

// Dijkstra算法，返回距离和路径
pair<vector<int>, vector<vector<int>>> Graph::dijkstra(int start){
    int n = vexs.size(); // 获取顶点数量
    if (start < 0 || start >= n) { // 检查起点是否有效
        vector<int> emptyDist; // 空距离数组
        vector<vector<int>> emptyPaths; // 空路径数组
        return make_pair(emptyDist, emptyPaths); // 返回空结果
    }
    
    vector<int> dist(n, INF); // 距离数组，初始化为无穷大
    vector<bool> visited(n, false); // 访问标记数组，初始化为未访问
    vector<int> prev(n, -1); // 前驱节点数组，用于记录路径
    
    dist[start] = 0; // 起点到自己的距离为0
    
    // 循环n次，每次确定一个顶点的最短路径
    for (int i = 0; i < n; i++) {
        int u = -1; // 当前要处理的顶点
        int minDist = INF; // 最小距离
        
        // 在未访问的顶点中找到距离最小的顶点
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) { // 如果未访问且距离更小
                minDist = dist[j]; // 更新最小距离
                u = j; // 记录顶点编号
            }
        }
        
        if (u == -1) { // 如果找不到可访问的顶点
            break; // 提前结束
        }
        
        visited[u] = true; // 标记顶点u已访问
        
        // 遍历顶点u的所有邻接顶点
        for (int k = 0; k < (int)adjList[u].size(); k++) {
            int v = adjList[u][k].adjvex; // 邻接顶点编号
            int weight = adjList[u][k].weight; // 边的权重
            
            // 如果邻接顶点未访问，且通过u到达v的距离更短
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight; // 更新最短距离
                prev[v] = u; // 更新前驱节点
            }
        }
    }
    
    // 根据前驱数组重建路径
    vector<vector<int>> paths(n); // 路径数组
    for (int i = 0; i < n; i++) {
        if (dist[i] != INF) { // 如果顶点i可达
            vector<int> path; // 存储路径
            int current = i; // 从终点开始回溯
            while (current != -1) { // 回溯到起点
                path.insert(path.begin(), current); // 在路径开头插入顶点
                current = prev[current]; // 移动到前驱节点
            }
            paths[i] = path; // 保存路径
        }
    }
    
    return make_pair(dist, paths); // 返回距离数组和路径数组
}
