#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include "tourism.hpp"
#include "graph.hpp"

using namespace std;

Tourism::Tourism() {} // Tourism 构造函数

Tourism::~Tourism() {} // ~Tourism 析构函数

static int readInt(const string& prompt) {
    cout << prompt;
    int val;
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    return val;
} // readInt 读取整数输入，带错误处理

static string joinPath(const vector<int>& path, const Graph& g, const string& sep = " -> ") {
    if (path.empty()) return "";
    string result = g.getVex(path[0]).name;
    for (size_t i = 1; i < path.size(); i++) {
        result += sep + g.getVex(path[i]).name;
    }
    return result;
} // joinPath 将路径节点数组转换为可读字符串

int Tourism::createGraph(const string& vexPath, const string& edgePath) {
    ifstream vexFile(vexPath);
    if (!vexFile.is_open()) {
        cout << "错误：无法打开 " << vexPath << " 文件" << endl;
        return -1;
    }

    string line;
    while (getline(vexFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int num;
        string name, desc;
        ss >> num >> name;
        ss >> ws;
        getline(ss, desc);
        graph.addVex(Vex{num, name, desc});
    }
    vexFile.close();

    ifstream edgeFile(edgePath);
    if (!edgeFile.is_open()) {
        cout << "错误：无法打开 " << edgePath << " 文件" << endl;
        return -1;
    }

    while (getline(edgeFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string v1Name, v2Name;
        int weight;
        ss >> v1Name >> v2Name >> weight;

        int v1 = -1, v2 = -1;
        for (int i = 0; i < graph.getVexNum(); i++) {
            const auto& vex = graph.getVex(i);
            if (vex.name == v1Name) v1 = i;
            if (vex.name == v2Name) v2 = i;
        }

        if (v1 != -1 && v2 != -1) {
            graph.addEdge(v1, v2, weight);
            cout << "成功添加边：" << v1Name << " -> " << v2Name << ", 权重：" << weight << endl;
        } else {
            if (v1 == -1) cout << "警告：未找到顶点 " << v1Name << endl;
            if (v2 == -1) cout << "警告：未找到顶点 " << v2Name << endl;
        }
    }
    edgeFile.close();

    cout << "========== 图创建成功 ==========" << endl;
    cout << "顶点数目: " << graph.getVexNum() << endl;
    cout << "边数目: " << graph.getEdgeNum() << endl << endl;
    cout << "顶点信息:" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const auto& vex = graph.getVex(i);
        cout << "  编号: " << vex.num << ", 名称: " << vex.name << ", 描述: " << vex.desc << endl;
    }
    cout << endl;
    cout << "边信息:" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const auto& adj = graph.getAdjList(i);
        for (const auto& arc : adj) {
            if (i < arc.adjvex) {
                cout << "  " << graph.getVex(i).name << " -> "
                     << graph.getVex(arc.adjvex).name
                     << ", 权重: " << arc.weight << endl;
            }
        }
    }
    cout << "==============================" << endl;
    return 0;
} // createGraph 从文件创建图

int Tourism::getSpotInfo() {
    int spotNum = readInt("请输入景点编号：");
    if (spotNum < 0 || spotNum >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    const auto& vex = graph.getVex(spotNum);
    cout << "========== 景点信息 ==========" << endl;
    cout << "编号：" << vex.num << endl;
    cout << "名称：" << vex.name << endl;
    cout << "描述：" << vex.desc << endl << endl;
    cout << "相邻景点：" << endl;

    const auto& adj = graph.getAdjList(spotNum);
    if (adj.empty()) {
        cout << "  无相邻景点" << endl;
    } else {
        for (const auto& arc : adj) {
            cout << "  " << graph.getVex(arc.adjvex).name
                 << "（距离：" << arc.weight << "m）" << endl;
        }
    }
    cout << "==============================" << endl;
    return 0;
} // getSpotInfo 查询单个景点信息

int Tourism::showGraph() {
    if (graph.getVexNum() == 0) {
        cout << "错误：图未创建" << endl;
        return -1;
    }

    cout << "========== 图信息 ==========" << endl;
    cout << "顶点数目：" << graph.getVexNum() << endl;
    cout << "边数目：" << graph.getEdgeNum() << endl << endl;
    cout << "顶点列表：" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const auto& vex = graph.getVex(i);
        cout << "  [" << vex.num << "] " << vex.name << " - " << vex.desc << endl;
    }
    cout << endl;
    cout << "道路列表：" << endl;
    for (int i = 0; i < graph.getVexNum(); i++) {
        const auto& adj = graph.getAdjList(i);
        for (const auto& arc : adj) {
            if (i < arc.adjvex) {
                cout << "  " << graph.getVex(i).name << " <-> "
                     << graph.getVex(arc.adjvex).name
                     << "（" << arc.weight << "m）" << endl;
            }
        }
    }
    cout << "============================" << endl;
    return 0;
} // showGraph 展示图的顶点和边信息

int Tourism::TravelPath() {
    int start = readInt("请输入起始景点编号：");
    if (start < 0 || start >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    cout << "========== 旅游路径 ==========" << endl;
    cout << "从 " << graph.getVex(start).name << " 开始游览" << endl << endl;

    const auto& path = graph.DFSTraverse(start);
    if (!path.empty()) {
        cout << "推荐路线：" << endl << "  " << joinPath(path, graph) << endl;
        cout << "总距离：" << graph.calcPathWeight(path) << "m" << endl;
    } else {
        cout << "未找到游览路线" << endl;
    }
    cout << "==============================" << endl;
    return 0;
} // TravelPath 旅游路径规划（深搜遍历）

int Tourism::findShortPath() {
    int start = readInt("请输入起点编号：");
    int end = readInt("请输入终点编号：");

    if (start < 0 || start >= graph.getVexNum() || end < 0 || end >= graph.getVexNum()) {
        cout << "错误：景点编号无效" << endl;
        return -1;
    }

    const auto& path = graph.dijkstra(start, end);

    cout << "========== 最短路径 ==========" << endl;
    cout << "从 " << graph.getVex(start).name << " 到 " << graph.getVex(end).name << endl << endl;

    if (path.empty()) {
        cout << "错误：无法到达目标景点" << endl;
    } else {
        cout << "最短路线：" << endl << "  " << joinPath(path, graph) << endl;
        cout << "最短距离：" << graph.calcPathWeight(path) << "m" << endl;
    }
    cout << "==============================" << endl;
    return 0;
} // findShortPath 查找两点间最短路径

int Tourism::designMinPath() {
    if (graph.getVexNum() == 0) {
        cout << "错误：图未创建" << endl;
        return -1;
    }

    const auto& mstEdges = graph.Prim();
    int totalWeight = 0;
    for (const auto& edge : mstEdges) {
        totalWeight += edge.weight;
    }

    cout << "========== 电路铺设规划 ==========" << endl;
    cout << "最小生成树方案：" << endl << endl;

    if (mstEdges.empty()) {
        cout << "错误：无法生成电路铺设方案" << endl;
    } else {
        cout << "需要铺设电路的道路：" << endl;
        for (const auto& edge : mstEdges) {
            cout << "  " << graph.getVex(edge.vex1).name
                 << " <-> " << graph.getVex(edge.vex2).name
                 << "（长度：" << edge.weight << "m）" << endl;
        }
        cout << endl << "总铺设长度：" << totalWeight << "m" << endl;
    }
    cout << "==================================" << endl;
    return 0;
} // designMinPath 电路铺设规划（Prim最小生成树）
