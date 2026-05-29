//
// Created by a1191 on 2026/5/27.
//
#include "Graph.h"
#include <iostream>
#include <fstream>//文件操作 头文件
#include <string>
#include <iomanip>
#include <queue>
//构造函数，初始化为INF，对角线为0
map_graph::map_graph()
{
    vertex_num=MAXSIZE;
    edge_num=0;//边 初始默认为0条
    //分配空间并全部初始化为 INF
    adjMatrix.assign(MAXSIZE,std::vector<int>(MAXSIZE,INF));
    //对角线为0
    for (int i = 0; i < MAXSIZE; ++i)
    {
        adjMatrix[i][i] = 0;
    }
    adjList.resize(MAXSIZE);
    placeNames->resize(MAXSIZE);
}

//构造map_graph函数
bool map_graph::load_map_graph(const std::string& fileName)
{
    //打开文件
    std::ifstream inFile("map.txt");
    if (!inFile.is_open())
    {
        std::cerr<<"打开失败"<<std::endl;
        return false;
    }

    //读取地点
    for (int i=0;i<MAXSIZE;i++)
    {
        std::string name;
        int id;
        inFile>>id>>name;
        placeNames[id-1]=name;
    }

    // 跳过“24条边”这一行及之前的空行
    std::string dummy;
    std::getline(inFile, dummy);  // 吃掉第15行末尾的换行符
    std::getline(inFile, dummy);  // 跳过空行
    std::getline(inFile, dummy);  // 跳过“24条边”这一行

    //读取边和权重
    int from=0;
    int to=0;
    int weight=0;
    while (inFile>>from>>to>>weight)
    {
        //构建邻接矩阵(无向)
        adjMatrix[from][to]=weight;
        adjMatrix[to][from]=weight;

        //构建邻接表(无向)
        adjList[from].push_back({to,weight});
        adjList[to].push_back({from,weight});
    }
    inFile.close();
    return true;
}

//打印邻接矩阵
void map_graph::printAdjMatrix()const
{
    const int w = 5;
    std::cout << "邻接矩阵:"<<std::endl;
    //打印列表头
    std::cout << std::setw(w) << " ";
    for (int i = 0; i < vertex_num; ++i)
    {
        std::cout << std::setw(w) << ("V"+std::to_string(i));
    }
    std::cout << std::endl;

    for (int i=0;i<vertex_num;++i)
    {
        //打印行表头
        std::cout << std::setw(w) << ("V"+std::to_string(i));
        for (int j=0;j<vertex_num;++j)
        {
            if (adjMatrix[i][j]==INF)
            {
                std::cout<<std::setw(w)<<"INF";
            }
            else
            {
                std::cout<<std::setw(w)<<adjMatrix[i][j];
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void map_graph::printAdjList()const
{
    std::cout << "邻接表:"<<std::endl;
    for (int i=0;i<vertex_num;++i)
    {
        std::cout <<"("<<placeNames[i]<<")"<<"V"<<i<<": ";//顶点,输出：V0(体育馆):
        for (int j=0;j<adjList[i].size();++j)
        {
            std::cout <<"->"<< adjList[i][j].to<<"("<<adjList[i][j].weight<<")";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}


/**
 * @brief 深度优先搜索（使用邻接矩阵）
 * @param vertex 起始搜索顶点
 * @param reset  是否需要重置visited数组
 */
void map_graph::dfs(const int vertex,bool reset)
{
    if (reset)
    {
        for (int i=0;i<vertex_num;++i)//重置visited数组为 未被访问
        {
            visited[i]=false;
        }
        reset=false;
    }


    visited[vertex]=true;//入口设为已被访问过
    std::cout << vertex+1<<"("<<placeNames[vertex]<<")"<<std::endl;//逐行打印顶点访问顺序
    for (int j=0;j<vertex_num;++j)
    {
        //两顶点之间有连接，且顶点j未被访问过
        if (adjMatrix[vertex][j]!=INF && adjMatrix[vertex][j]!=0 && visited[j]!=true)
        {
            dfs(j,false);//递归
        }
    }
}

/**
 * @brief 广度优先搜索(使用邻接矩阵)
 * @param vertex 入口顶点
 * @param reset  是否重置visited数组
 */
void map_graph::bfs(const int vertex, const bool reset)
{
    if (reset)//重置visited数组为 未被访问
    {
        for (int i=0;i<vertex_num;++i)
        {
            visited[i]=false;
        }
    }

    std::queue<int>q;//队列中存的是 未访问过的顶点
    q.push(vertex);//入队
    visited[vertex]=true;//入口设为已被访问过

    while (!q.empty())//队列为空 表明 所有顶点 已被访问
    {
        int cur=q.front();//取出队首的顶点，访问该顶点的"孩子"
        q.pop();//出队,此时队列中只有顶点的孩子
        std::cout << cur+1<<"("<<placeNames[cur]<<")"<<std::endl;//打印顶点访问

        //访问顶点(cur)的孩子
        for (int i=0;i<vertex_num;++i)
        {
            //两顶点之间有连接，且孩子i未被访问过
            if (adjMatrix[cur][i]!=INF && visited[i]!=true)
            {
                q.push(i);//入队，作为下一层while循环的顶点(父节点)
                visited[i]=true;

            }
        }
    }
}
