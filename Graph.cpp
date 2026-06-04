//
// Created by a1191 on 2026/5/27.
//
#include "Graph.h"
#include <iostream>
#include <fstream>//文件操作 头文件
#include <string>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>



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
        visited[i]=false;
    }
    adjList.resize(MAXSIZE);
}

int map_graph::get_vertex_num() const
{
    return vertex_num;
}

std::string map_graph::getPlaceName(int id) const
{
    if (id >= 0 && id < MAXSIZE)
    {
        return placeNames[id];
    }
    else
    {
        return "";   // 无效编号时返回空字符串
    }
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
        std::cout <<"("<<placeNames[i]<<")"<<i+1<<": ";//顶点,输出：V0(体育馆):
        for (int j=0;j<adjList[i].size();++j)
        {
            std::cout <<"->"<< (adjList[i][j].to)+1<<"-"<<placeNames[adjList[i][j].to]<<"("<<adjList[i][j].weight<<")";
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
        q.pop();//出队,此后队列中只有顶点的孩子
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

void map_graph::prim(const int startVertex)
{
    std::vector<int>weight(vertex_num,INF);//存放候选边的权重
    std::vector<int>parent(vertex_num,-1);//值表示父节点，索引表示子节点
    std::vector<bool>inMST(vertex_num,false);//是否在最小生成树中

    //通过weight数组和parent数组，来表示：在可选路径中，父节点(parent[i])到子节点(i)的权重为weight[i]
    //weight的下标也对应子顶点

    //先将起始顶点的权重存入weight，即添加起始顶点的可选路径
    for (int i=0;i<vertex_num;++i)
    {
        weight[i]=adjMatrix[startVertex][i];
        parent[i]=startVertex;//所有顶点的父节点是起始节点
    }
    //先初始化startVertex
    weight[startVertex]=0;//自己到自己的权重为0
    parent[startVertex]=-1;//起始节点无父节点
    inMST[startVertex]=true;//起始节点存入最小生成树
    std::cout << "加入顶点"<<startVertex+1<<"("<<placeNames[startVertex]<<")"<<std::endl;

    for (int count=1;count<vertex_num;++count)//每次循环增加一个 最小权重的顶点
    {
        int minWeight=INF;
        int newVertex=-1;
        //寻找权重最小 的可选路径
        for (int i=0;i<vertex_num;++i)
        {
            if (!inMST[i] && weight[i]<minWeight)//子节点不在最小生成树，且权重小于minWeight
            {
                minWeight=weight[i];
                newVertex=i;//得到新顶点
            }
        }

        if (newVertex==-1)//如果没有新顶点，说明本次循环的顶点是孤点，该图不联通
        {
            std::cout << "该图不联通"<<std::endl;
            return;
        }

        //将newVertex加入到最小生成树中
        inMST[newVertex]=true;
        std::cout << "加入顶点"<<newVertex+1<<"("<<placeNames[newVertex]<<")"<<std::endl;

        //将新顶点所连接的路径 更新到可选路径中,即更新parent数组;并更新对应权重，即同步更新weight数组
        for (int i=0;i<vertex_num;++i)//循环中的i代表所连接的子顶点
        {
            if (!inMST[i] && adjMatrix[newVertex][i] != INF && adjMatrix[newVertex][i] != 0)
            {
                if (adjMatrix[newVertex][i]<weight[i])
                {
                    parent[i]=newVertex;//更新 顶点newVertex到顶点parent[i]这一条可选路径
                    weight[i]=adjMatrix[newVertex][i];//更新对应路径的权重
                }
            }
        }
    }
}

void Kruskal_swap(Kruskal_edge edge[], int i, int j)
{
    int temp = edge[i].begin;
    edge[i].begin = edge[j].begin;
    edge[j].begin = temp;

    temp = edge[i].end;
    edge[i].end = edge[j].end;
    edge[j].end = temp;

    temp = edge[i].weight;
    edge[i].weight = edge[j].weight;
    edge[j].weight = temp;
}

void Kruskal_sort(Kruskal_edge edge[],int edgeNum)
{
    for (int i=0;i<edgeNum;++i)
    {
        for (int j=i+1;j<edgeNum;++j)
        {
            if (edge[i].weight>edge[j].weight)
            {
                Kruskal_swap(edge,i,j);
            }
        }

    }
}

//find函数的作用就是找到顶点index的老大是谁，parent[index]的值就是顶点index的上级
int Kruskal_find(int parent[],int index)
{
    while (parent[index]>0)//不断寻找上级，直到找到老大
    {
        index=parent[index];
    }
    return index;
}

void map_graph::Kruskal()
{
    edge_num=MAXEDGE;
    Kruskal_edge edge[edge_num];
    int k=0;
    //通过邻接矩阵获取每一条边
    for (int i=0;i<vertex_num;++i)
    {
        for (int j=i+1;j<vertex_num;++j)
        {
            if (adjMatrix[i][j]!=INF)
            {
                edge[k].begin=i;
                edge[k].end=j;
                edge[k].weight=adjMatrix[i][j];
                k++;
            }
        }
    }
    //排序边，实现贪心选择的效果，从权重最小的开始连接
    Kruskal_sort(edge,k);
    //创建并初始化parent数组为0
    int parent[vertex_num];
    for (int i=0;i<vertex_num;++i)
    {
        parent[i]=0;//一开始每个人的老大是自己
    }

    int n;
    int m;
    for (int i=0;i<k;++i)
    {
        n=Kruskal_find(parent,edge[i].begin);//找顶点—edge[i].begin的老大
        m=Kruskal_find(parent,edge[i].end);//找顶点-edge[i].end的老大
        if (n!=m)//判断老大是否相同
        {
            parent[n]=m;//老大不同，则让begin的老大归顺于end的老大，合并帮派，使得只有一个老大（根）
            //此时begin和end有了共同的老大，再把它们联通，就把两个帮派构成了一个新的联通帮派
            std::cout << edge[i].begin<<"->"<<edge[i].end<<":"<<edge[i].weight<<std::endl;//输出连接的路径
        }//如果老大相同，已经在同一个帮派 → 再连边就会形成环 → 跳过这条边。
    }//最后使得有一个共同的老大，即最小生成树的根
    //总之：一开始两个顶点一定是不联通的，
    //当两个帮派（或者两个顶点）的老大不一样时（两帮派没有联通），合并出一个新的老大（确认共同的根），
    //再连接，这样就获得了一个有 唯一老大的联通帮派
    //保证了联通后的帮派一定有共同的老大
    //当两顶点有共同老大时，说明它们一定是联通的，所以不需要再进行连接
}

/**
 * @brief 找到未访问且距离起始点最短的顶点
 */
int map_graph::Dijkstra_choose(const std::vector<int>& distance, const std::vector<bool>& found)
{
    int min=INF;
    int minPos=-1;//-1在path数组中表示没有上一个顶点，即起始顶点
    for (int i=0;i<vertex_num;i++)
    {
        if (!found[i] && distance[i]<min)
        {
            min=distance[i];//更新最短距离
            minPos=i;//更新顶点
        }
    }
    return minPos;
}

/**
 * @brief 得到distance数组，；path数组
 * @param begin 起点
 * @param distance 存放起点到各点之间的最短距离
 * @param path path[i]表示点i的下一个点，不断访问i=path[i]获取点i->起点的路径(反向的)
 */
void map_graph::Dijkstra(int begin,std::vector<int>& distance, std::vector<int>& path)
{
    std::vector<bool> found(vertex_num);
    distance.assign(vertex_num,INF);
    path.assign(vertex_num,-1);
    //初始化
    for (int i=0;i<vertex_num;++i)
    {
        found[i]=false;//全部点默认为未找到
        distance[i]=adjMatrix[begin][i];//传入起始顶点 所能访问的顶点 及其距离
        if (distance[i] != INF && i != begin)//初始化与起始顶点直接相连的顶点的path
        {
            path[i]=begin;
        }
        else
        {
            path[i]=-1;//表示无上一顶点
        }
    }

    found[begin]=true;//已访问起始顶点
    distance[begin]=0;//起始顶点到自身的距离为0

    for (int i=1;i<vertex_num;++i)
    {
        //找到下一个要访问的顶点，该函数已经更新了顶点i的distance和path
        int next=Dijkstra_choose(distance,found);
        if (next == -1) break;   // 剩余顶点不可达，提前退出
        found[next]=true;//已访问该顶点
        //根据当前访问的顶点，更新 起始顶点 到 与当前顶点所连接的顶点 的距离和路径
        for (int j=0;j<vertex_num;++j)
        {
            if (!found[j] && adjMatrix[next][j] != INF)//如果顶点j是未被访问的,且与当前顶点有连接
            {
                //如果 当前顶点到起始顶点的距离+当前顶点到顶点j的距离 < 起始顶点到顶点j的距离
                if (distance[next]+adjMatrix[next][j] < distance[j])
                {
                    distance[j]=distance[next]+adjMatrix[next][j];//则更新出一段更小的路径
                    path[j]=next;//更新顶点j的上一顶点为 当前顶点(next)
                }
            }

        }
    }
}

void map_graph::Dijkstra_getAllPath(int begin)
{
    std::vector<int>distance;
    std::vector<int>path;
    Dijkstra(begin,distance,path);

    //输出起始顶点 到 每一个顶点 的最短路径和距离
    for (int i=0;i<vertex_num;++i)
    {
        std::cout <<begin+1<<"("<<placeNames[begin]<<")"<<"->"<<i+1<<"("<<placeNames[i]<<")---";
        std::cout << "distance:"<<distance[i]<<std::endl;
        std::cout << "path:"<<std::endl;
        //把路径改为正向
        std::vector<int>pathArr;
        for (int v=i;v!=-1;v=path[v])
        {
            pathArr.push_back(v);//反向压入
        }

        std::reverse(pathArr.begin(),pathArr.end());//反转为正向
        for (int k=0;k<pathArr.size();++k)
        {
            if (k > 0) std::cout << " -> ";
            std::cout << pathArr[k]+1 << "(" << placeNames[pathArr[k]] << ")";
        }
        std::cout << std::endl << std::endl;
    }
}

void map_graph::Dijkstra_getSinglePath(int begin,int end)
{
    std::vector<int>distance;
    std::vector<int>path;
    Dijkstra(begin,distance,path);
    std::cout <<begin+1<<"("<<placeNames[begin]<<")"<<"->"<<end+1<<"("<<placeNames[end]<<")---";
    std::cout << "distance:"<<distance[end]<<std::endl;
    std::cout << "path:"<<std::endl;
    //把路径改为正向
    std::vector<int>pathArr;
    for (int v=end;v!=-1;v=path[v])
    {
        pathArr.push_back(v);//反向压入
    }
    std::reverse(pathArr.begin(),pathArr.end());//反转为正向

    //输出路径
    for (int k=0;k<pathArr.size();++k)
    {
        if (k > 0) std::cout << " -> ";
        std::cout << pathArr[k]+1 << "(" << placeNames[pathArr[k]] << ")";
    }
    std::cout << std::endl << std::endl;
}

void map_graph::Floyd()
{
    std::vector<std::vector<int>>distance(vertex_num,std::vector<int>(vertex_num));
    std::vector<std::vector<int>>path(vertex_num,std::vector<int>(vertex_num));
    for (int i=0;i<vertex_num;++i)
    {
        for (int j=0;j<vertex_num;++j)
        {
            distance[i][j]=adjMatrix[i][j];
            if (i!=j && adjMatrix[i][j] != INF)
            {
                path[i][j]=j;// 初始时 i->j 的直接后继是 j
            }
            else
            {
                path[i][j]=-1;// 不可达或自身
            }
        }
    }

    //三层for循环:k:中转顶点  i:起点  j:终点
    for (int k=0;k<vertex_num;++k)
    {
        for (int i=0;i<vertex_num;++i)
        {
            for (int j=0;j<vertex_num;++j)
            {
                //如果起点可通过中转顶点到达终点,且起点通过中转顶点到达终点的距离>起点到达终点的距离
                if ( (distance[i][k]!=INF&&distance[k][j]!=INF) &&
                    (distance[i][j]>distance[i][k]+distance[k][j]) )
                {
                    distance[i][j]=distance[i][k]+distance[k][j];//更新距离
                    path[i][j]=k;//更新中转点
                }
            }
        }
    }

    const int w = 5;
    std::cout << "所有点对最短距离矩阵:" << std::endl;
    // 打印列头
    std::cout << std::setw(w) << " ";
    for (int i = 0; i < vertex_num; ++i)
        std::cout << std::setw(w) << ("V" + std::to_string(i));
    std::cout << std::endl;

    for (int i = 0; i < vertex_num; ++i)
    {
        // 打印行头
        std::cout << std::setw(w) << ("V" + std::to_string(i));
        for (int j = 0; j < vertex_num; ++j)
        {
            if (distance[i][j] == INF)
                std::cout << std::setw(w) << "INF";
            else
                std::cout << std::setw(w) << distance[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief 使用贪心算法打卡每一个点，然后使用Dijkstra算法从终点返回起点
 */
void map_graph::TSP_greedy(int start)
{
    //1.贪心算法部分
    std::vector<bool>visited(vertex_num,false);//存放每个顶点是否访问过
    std::vector<int>path_checkIn;//存放打卡的路径(不包含返回的)

    visited[start]=true;//起点被访问
    path_checkIn.push_back(start);//把start添加到路径

    int totalDist=0;//总路程
    int current=start;//当前所在顶点，起始为start

    for (int i=1;i<vertex_num;++i)//i对应访问的每一个未访问顶点
    {
        //每次循环都使用一次Dijkstra获取current的最短未访问顶点，防止current没有直连的顶点
        std::vector<int>temp_dist;
        std::vector<int>temp_path;

        Dijkstra(current,temp_dist,temp_path);

        int minDist=INF;//离顶点i最短的路径
        int next=-1;//下一个要访问的顶点

        for (int j=0;j<vertex_num;++j)//j对应在i点时，找出的离i最近的点
        {
            if (!visited[j] && temp_dist[j]<minDist)//未被访问过，且是离i最近的点j
            {
                minDist=temp_dist[j];//更新最小距离
                next=j;//下一个顶点访问j
            }
        }
        if (next==-1)
        {
            std::cout << "图不联通"<<std::endl;
            return;
        }

        //构建current到next的路径
        std::vector<int>curTonext;
        for (int k=next;k!=current;k=temp_path[k])//获取next到current的路径
        {
            curTonext.push_back(k);
        }
        std::reverse(curTonext.begin(),curTonext.end());//反转变为current到next的路径

        //加入到打卡路径中
        for (int k=0;k<curTonext.size();++k)
        {
            path_checkIn.push_back(curTonext[k]);
            visited[curTonext[k]]=true;
        }

        totalDist+=minDist;//更新总路程
        current=next;//更新当前顶点
    }

    //2.终点回到起点，使用Dijkstra
    std::vector<int>distance;
    std::vector<int>path_Dijkstra;
    Dijkstra(current,distance,path_Dijkstra);//计算从 current 到所有点的最短路径，把current当成起点

    //判断图是否联通
    if (distance[start] == INF)
    {
        std::cout << "无法从最后一个点返回起点，图不连通！" << std::endl;
        return;
    }

    totalDist+=distance[start];//加上从终点返回起点的距离

    std::cout << "total distance:"<<totalDist<<std::endl;//输出路程
    std::cout << "path:"<<std::endl;
    //把路径改为正向
    std::vector<int>path_back;
    for (int v=start;v!=current;v=path_Dijkstra[v])//这里把start当作返回路径中的终点
    {
        path_back.push_back(v);//反向压入
    }
    path_back.push_back(current);//加入终点current
    std::reverse(path_back.begin(),path_back.end());//反转为正向

    //3.输出路径
    //起点到终点（打卡路径）
    std::cout << "打卡路径:"<<std::endl;
    for (int k=0;k<path_checkIn.size();++k)
    {
        if (k!=0 && k%5==0)
        {
            std::cout<<std::endl;
        }
        if (k > 0) std::cout << " -> ";
        std::cout << path_checkIn[k]+1 << "(" << placeNames[path_checkIn[k]] << ")";
    }
    std::cout << std::endl;
    //终点到起点（返回路径）
    std::cout << "返回路径:"<<std::endl;
    for (int k=0;k<path_back.size();++k)
    {
        if (k!=0 && k%5==0)
        {
            std::cout<<std::endl;
        }
        if (k > 0) std::cout << " -> ";
        std::cout << path_back[k]+1 << "(" << placeNames[path_back[k]] << ")";
    }
    std::cout << std::endl << std::endl;
}


