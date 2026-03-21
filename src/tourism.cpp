#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include "tourism.hpp"
#include "graph.hpp"


using namespace std;

// 构造函数
Tourism::Tourism(){
}

// 析构函数
Tourism::~Tourism(){
}

// 建图
int Tourism::createGraph(const string& vexPath, const string& edgePath){
    ifstream vexFile(vexPath.c_str()); // 打开景点信息文件
    if (!vexFile.is_open()) {
        cout << "错误：无法打开 " << vexPath << " 文件" << endl;
        return -1;
    }
    
    string line;
    while (getline(vexFile, line)) { // 逐行读取景点信息
        if (line.empty()) continue; // 跳过空行
        
        stringstream ss(line); // 使用字符串流解析
        int num;
        string name, desc;
        
        ss >> num >> name; // 读取编号和名称
        ss >> ws; // 跳过空白字符
        getline(ss, desc); // 读取描述（可能包含空格）
        
        Vex vex{num, name, desc}; // 创建顶点对象
        graph.addVex(vex); // 添加顶点到图中
    }
    vexFile.close(); // 关闭文件
    
    ifstream edgeFile(edgePath.c_str()); // 打开道路信息文件
    if (!edgeFile.is_open()) {
        cout << "错误：无法打开 " << edgePath << " 文件" << endl;
        return -1;
    }
    
    while (getline(edgeFile, line)) { // 逐行读取道路信息
        if (line.empty()) continue; // 跳过空行
        
        stringstream ss(line); // 使用字符串流解析
        string v1Name, v2Name;
        int weight;
        
        ss >> v1Name >> v2Name >> weight; // 读取起点、终点和权重
        
        // 根据名称查找顶点编号
        int v1 = -1, v2 = -1;
        for (int i = 0; i < graph.getVexNum(); i++) {
            Vex vex = graph.getVex(i);
            if (vex.name == v1Name) v1 = i; // 找到起点编号
            if (vex.name == v2Name) v2 = i; // 找到终点编号
        }
        
        if (v1 != -1 && v2 != -1) { // 如果两个顶点都找到
            graph.addEdge(v1, v2, weight); // 添加边到图中
            cout << "成功添加边：" << v1Name << " -> " << v2Name << ", 权重：" << weight << endl;
        } else {
            if (v1 == -1) {
                cout << "警告：未找到顶点 " << v1Name << "，跳过边 " << v1Name << " -> " << v2Name << endl;
            }
            if (v2 == -1) {
                cout << "警告：未找到顶点 " << v2Name << "，跳过边 " << v1Name << " -> " << v2Name << endl;
            }
        }
    }
    edgeFile.close(); // 关闭文件
    
    // 输出图的信息
    cout << "========== 图创建成功 ==========" << endl;
    cout << "顶点数目: " << graph.getVexNum() << endl;
    cout << "边数目: " << graph.getEdgeNum() << endl << endl;
    
    cout << "顶点信息:" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        Vex vex = graph.getVex(i);
        cout << "  编号: " << vex.num << ", 名称: " << vex.name << ", 描述: " << vex.desc << endl;
    }
    cout << endl;
    
    cout << "边信息:" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const vector<ArcNode>& adjList = graph.getAdjList(i);
        for (int j = 0; j < (int)adjList.size(); j++) {
            if (i < adjList[j].adjvex) { // 避免重复输出（无向图）
                cout << "  " << graph.getVex(i).name << " -> " 
                     << graph.getVex(adjList[j].adjvex).name 
                     << ", 权重: " << adjList[j].weight << endl;
            }
        }
    }
    cout << "==============================" << endl;
    
    return 0;
}

// 获取单点信息
int Tourism::getSpotInfo(){
    int spotNum;
    cout << "请输入景点编号：";
    cin >> spotNum;

    // 检查输入是否有效
    if (cin.fail()) {
        cin.clear(); // 清除错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    if (spotNum < 0 || spotNum >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    Vex vex = graph.getVex(spotNum); // 调用算法层获取顶点信息
    if (vex.num == -1) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }
    cout << "========== 景点信息 ==========" << endl;
    cout << "编号：" << vex.num << endl;
    cout << "名称：" << vex.name << endl;
    cout << "描述：" << vex.desc << endl;
    cout << endl;

    cout << "相邻景点：" << endl;
    const vector<ArcNode>& adjList = graph.getAdjList(spotNum); // 调用算法层获取邻接表
    if (adjList.empty()) {
        cout << "  无相邻景点" << endl;
    } else {
        for (int i = 0; i < (int)adjList.size(); i++) {
            Vex adjVex = graph.getVex(adjList[i].adjvex);
            cout << "  " << adjVex.name << "（距离：" << adjList[i].weight << "m）" << endl;
        }
    }
    cout << "==============================" << endl;
    
    return 0;
}

// 展示图
int Tourism::showGraph(){
    if (graph.getVexNum() == 0) {
        cout << "错误：图未创建" << endl;
        return -1;
    }
    
    cout << "========== 图信息 ==========" << endl;
    cout << "顶点数目：" << graph.getVexNum() << endl;
    cout << "边数目：" << graph.getEdgeNum() << endl << endl;
    
    cout << "顶点列表：" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        Vex vex = graph.getVex(i);
        cout << "  [" << vex.num << "] " << vex.name << " - " << vex.desc << endl;
    }
    cout << endl;
    
    cout << "道路列表：" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const vector<ArcNode>& adjList = graph.getAdjList(i);
        for (int j = 0; j < (int)adjList.size(); j++) {
            if (i < adjList[j].adjvex) {
                cout << "  " << graph.getVex(i).name << " <-> " 
                     << graph.getVex(adjList[j].adjvex).name 
                     << "（" << adjList[j].weight << "m）" << endl;
            }
        }
    }
    cout << "============================" << endl;
    
    return 0;
}

// 旅游路径（深搜）
int Tourism::TravelPath(){
    int start;
    cout << "请输入起始景点编号：";
    cin >> start;
    
    // 检查输入是否有效
    if (cin.fail()) {
        cin.clear(); // 清除错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
        cout << "错误：景点编号无效" << endl;
        return -1;
    }
    
    if (start < 0 || start >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    cout << "========== 旅游路径 ==========" << endl;
    cout << "从 " << graph.getVex(start).name << " 开始游览" << endl << endl;

    // 调用算法层的DFS遍历所有顶点
    pair<vector<int>, int> result = graph.DFSTraverse(start);

    if (!result.first.empty()) {
        cout << "推荐路线：" << endl;
        for (int i = 0; i < (int)result.first.size(); i++) {
            cout << "  " << graph.getVex(result.first[i]).name;
            if (i < (int)result.first.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        cout << "总距离：" << result.second << "m" << endl;
    } else {
        cout << "未找到游览路线" << endl;
    }
    cout << "==============================" << endl;
    
    return 0;
}

// 寻找单源最短路径
int Tourism::findShortPath(){
    int start, end;
    cout << "请输入起点编号：";
    cin >> start;
    
    // 检查起点输入是否有效
    if (cin.fail()) {
        cin.clear(); // 清除错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
        cout << "错误：景点编号无效" << endl;
        return -1;
    }
    
    cout << "请输入终点编号：";
    cin >> end;
    
    // 检查终点输入是否有效
    if (cin.fail()) {
        cin.clear(); // 清除错误状态
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误输入
        cout << "错误：景点编号无效" << endl;
        return -1;
    }
    
    if (start < 0 || start >= graph.getVexNum() || 
        end < 0 || end >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }
    
    // 调用算法层的Dijkstra算法
    pair<vector<int>, vector<vector<int>>> result = graph.dijkstra(start);
    vector<int> distances = result.first;
    vector<vector<int>> paths = result.second;
    
    cout << "========== 最短路径 ==========" << endl;
    cout << "从 " << graph.getVex(start).name 
         << " 到 " << graph.getVex(end).name << endl << endl;
    
    if (distances[end] == INF) {
        cout << "错误：无法到达目标景点" << endl;
    } else {
        cout << "最短路线：" << endl;
        vector<int> path = paths[end];
        for (int i = 0; i < (int)path.size(); i++) {
            cout << "  " << graph.getVex(path[i]).name;
            if (i < (int)path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
        cout << "最短距离：" << distances[end] << "m" << endl;
    }
    cout << "==============================" << endl;
    
    return 0;
}

// 规划最短路径（最小生成树）
int Tourism::designMinPath(){
    if (graph.getVexNum() == 0) {
        cout << "错误：图未创建" << endl;
        return -1;
    }
    
    // 调用算法层的Prim算法
    pair<vector<Edge>, int> result = graph.Prim();
    vector<Edge> mstEdges = result.first;
    int totalWeight = result.second;
    
    cout << "========== 电路铺设规划 ==========" << endl;
    cout << "最小生成树方案：" << endl << endl;
    
    if (mstEdges.empty()) {
        cout << "错误：无法生成电路铺设方案" << endl;
    } else {
        cout << "需要铺设电路的道路：" << endl;
        for (int i = 0; i < (int)mstEdges.size(); i++) {
            cout << "  " << graph.getVex(mstEdges[i].vex1).name 
                 << " <-> " << graph.getVex(mstEdges[i].vex2).name 
                 << "（长度：" << mstEdges[i].weight << "m）" << endl;
        }
        cout << endl;
        cout << "总铺设长度：" << totalWeight << "m" << endl;
    }
    cout << "==================================" << endl;
    
    return 0;
} 
