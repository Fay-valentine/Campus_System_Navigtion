#include <iostream>
#include<fstream>//文件操作 头文件
#include<string>
#include <windows.h>
#include "Graph.h"

void set_utf8()
{
    //system("chcp 65001");
    //system("cls");
    // 静默将控制台输出编码设为 UTF-8
    SetConsoleOutputCP(65001);
    // 静默将控制台输入编码设为 UTF-8
    SetConsoleCP(65001);
}


int main()
{
    set_utf8();
    map_graph g;

    g.load_map_graph("map.txt");

    g.printAdjMatrix();

    g.printAdjList();
    return 0;
}