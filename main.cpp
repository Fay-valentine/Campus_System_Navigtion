#include <iostream>
#include<fstream>//文件操作 头文件
#include<string>
#include <windows.h>
#include "Graph.h"
#include <limits>
#include <conio.h>   // _getch()
void set_utf8()
{
    //system("chcp 65001");
    //system("cls");
    // 静默将控制台输出编码设为 UTF-8
    SetConsoleOutputCP(65001);
    // 静默将控制台输入编码设为 UTF-8
    SetConsoleCP(65001);
}

void showWelcome();
void showMenu();
void refresh();
void showPlaces(const map_graph& g);

int main()
{
    set_utf8();
    map_graph g;
    g.load_map_graph("map.txt");

    showWelcome();//显示项目相关信息

    while (true)
    {
        showMenu();
        int option;
        while (true)
        {
            std::cin>>option;
            if (option>=1 && option<=7)
            {
                break;
            }
            std::cout << "输入错误，请输入数字1-7"<<std::endl;
        }

        switch(option)
        {
            case 1:
                g.printAdjMatrix();
                refresh();
                break;
            case 2:
                g.printAdjList();
                refresh();
                break;
            case 3:
                std::cout << "请输入打卡的出发点:"<<std::endl;
                showPlaces(g);
                int start_3;
                while (true)
                {
                    std::cin>>start_3;
                    if (start_3>=1 && start_3<=15)
                    {
                        break;
                    }
                    std::cout << "输入错误，应输入数字1-15"<<std::endl;
                }
                g.TSP_greedy(start_3-1);
                refresh();
                break;
            case 4:
                std::cout << "请输入下列出发点(输入对应数字):"<<std::endl;
                showPlaces(g);
                int start_4;
                while (true)
                {
                    std::cin>>start_4;
                    if (start_4>=1 && start_4<=15)
                    {
                        break;
                    }
                    std::cout << "输入错误，应输入数字1-15"<<std::endl;
                }
                g.Dijkstra_getAllPath(start_4-1);
                refresh();
                break;

            case 5:
                std::cout << "请输入出发点和终点(输入对应数字):"<<std::endl;
                showPlaces(g);
                int begin,end;
                while (true)
                {
                    std::cin>>begin>>end;
                    if ( (start_4>=1 && start_4<=15) && (end>=1 && end<=15) )
                    {
                        break;
                    }
                    std::cout << "输入错误，应输入数字1-15"<<std::endl;
                }
                g.Dijkstra_getSinglePath(begin-1,end-1);
                refresh();
                break;
            case 6:
                g.Floyd();
                refresh();
                break;
            case 7:
                std::cout << "欢迎下次使用" << std::endl;
                return 0;

            default:
                break;
        }
    }


    return 0;
}

void showWelcome()
{
    std::cout << "---校园导航系统---"<<std::endl;
    std::cout << "班级:计算机类2503"<<std::endl;
    std::cout << "小组成员:"<<std::endl;
    std::cout << "古宇乐-20251003103,负责:"<<std::endl;
    std::cout << "曾伟杰-20251002920,负责:"<<std::endl;
    std::cout << "时间:2026年6月1日至2026年6月25日"<<std::endl;
    std::cout << std::endl;
}

void showMenu()
{
    std::cout << "欢迎使用广东外语外贸大学校园导航系统"<<std::endl;
    std::cout << "输入对应数字使用下列功能:"<<std::endl;
    std::cout << "1.输出地图的邻接矩阵"<<std::endl;
    std::cout << "2.输出地图的邻接表"<<std::endl;
    std::cout << "3.从某地点出发，打卡所有点之后回到起点的经过地点序列，及所经历的路程"<<std::endl;
    std::cout << "4.输出从指定点出发，到达图中所有点的最短距离及经过的地点"<<std::endl;
    std::cout << "5.从指定的地点(起点)到达另一指定地点(终点)的最短距离，以及经过的地点"<<std::endl;
    std::cout << "6.输出图中所有两点之间的最短距离"<<std::endl;
    std::cout << "7.退出校园导航系统"<<std::endl;
}

/**
 * @brief 点击任意键继续后，清屏
 */
void refresh()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// 清空缓冲区中残留的任何内容
    std::cout << "按 Enter 键继续...";
    std::cin.get();  // 等待新的一次回车
}

void showPlaces(const map_graph& g)
{
    for (int i=0;i<g.get_vertex_num();++i)//展示可输入的地址
    {
        if (i%4==0 && i!=0)
        {
            std::cout<<std::endl;
        }
        std::cout <<std::to_string(i+1)<<"-"<<g.getPlaceName(i)<<"  ";
        if (i==g.get_vertex_num()-1)
        {
            std::cout<<std::endl;
        }
    }
}