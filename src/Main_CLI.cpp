#include <iostream>
#include <string>
#include "tourism.hpp"

using namespace std;

// 显示主菜单
void showMenu() {
    cout << "====================================" << endl;
    cout << "      景区信息管理系统" << endl;
    cout << "====================================" << endl;
    cout << "  1. 创建图" << endl;
    cout << "  2. 查询景点信息" << endl;
    cout << "  3. 展示图" << endl;
    cout << "  4. 旅游路径导航" << endl;
    cout << "  5. 搜索最短路径" << endl;
    cout << "  6. 铺设电路规划" << endl;
    cout << "  0. 退出系统" << endl;
    cout << "====================================" << endl;
    cout << "请选择操作（0-6）：";
}

int main(void) {
    system("chcp 65001"); // 设置控制台为 UTF-8 编码
    
    Tourism tourism; // 创建旅游管理对象
    int choice; // 用户选择
    
    // 无限循环，直到用户选择退出
    while (true) {
        showMenu(); // 显示菜单
        cin >> choice; // 读取用户输入
        
        cout << endl;
        
        // 使用 switch case 处理用户选择
        switch (choice) {
            case 1: { // 创建图
                cout << "========== 创建景区图示 ==========" << endl;
                
                string vexPath, edgePath;
                cout << "请输入景点信息文件路径（直接回车使用默认路径 ./data/Vex.txt）：";
                cin.ignore(); // 清除输入缓冲区
                getline(cin, vexPath);
                
                cout << "请输入道路信息文件路径（直接回车使用默认路径 ./data/Edge.txt）：";
                getline(cin, edgePath);
                
                // 如果用户直接回车，使用默认路径
                if (vexPath.empty()) {
                    vexPath = "./data/Vex.txt";
                }
                if (edgePath.empty()) {
                    edgePath = "./data/Edge.txt";
                }
                
                cout << endl;
                cout << "正在读取文件..." << endl;
                cout << "景点文件：" << vexPath << endl;
                cout << "道路文件：" << edgePath << endl;
                cout << endl;
                
                tourism.createGraph(vexPath, edgePath);
                break;
            }
                
            case 2: { // 查询景点信息
                cout << "========== 查询景点信息 ==========" << endl;
                tourism.getSpotInfo();
                break;
            }
                
            case 3: { // 展示图
                cout << "========== 展示图信息 ==========" << endl;
                tourism.showGraph();
                break;
            }
                
            case 4: { // 旅游路径导航
                cout << "========== 旅游路径导航 ==========" << endl;
                tourism.TravelPath();
                break;
            }
                
            case 5: { // 搜索最短路径
                cout << "========== 搜索最短路径 ==========" << endl;
                tourism.findShortPath();
                break;
            }
                
            case 6: { // 铺设电路规划
                cout << "========== 铺设电路规划 ==========" << endl;
                tourism.designMinPath();
                break;
            }
                
            case 0: // 退出系统
                cout << "========== 退出系统 ==========" << endl;
                cout << "感谢使用景区信息管理系统！" << endl;
                return 0;
                
            default: // 无效输入
                cout << "========== 错误提示 ==========" << endl;
                cout << "错误：无效的选择，请重新输入（0-6）" << endl;
                break;
        }
        
        cout << endl;
        system("pause"); // 暂停，等待用户按键
    }
    
    return 0;
}
