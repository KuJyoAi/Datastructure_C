#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QueueSize 100
#define NameSize 20

typedef struct node {
    char *name; // 站名
    struct node * next;  //指向下一站
    struct node * pre;  //指向上一站
    double len; //到下一站的距离
}* pnode;
typedef struct route {//线路
    pnode head;
    pnode tail;
    int num; //线路号s
    double len; //线路长度
}*proute;

typedef struct Edge {
    char *name; //被指向的顶点
    double dis; //边权值
    struct Edge* next; //下一条边
}*pEdge;

typedef struct Vertex {
    char *name;
    pEdge next;
}Vertex; //顶点
Vertex *Vertexs[50];

struct Queue{
    Vertex *v[QueueSize];
    int front;
    int rear;
}Queue; //搜索队列
struct Queue q;

int VertexNum = 0; //节点数
int RouteCnt = 0; //线路数

void CreateGraph(); //读入图并创建
void PrintRouteAndLen(proute p); //打印线路和距离
void InsertVertex(char *name); //插入节点, 如果存在则不插入
void InsertEdge(char *start, char* end, double dis); //插入边
int FindVertex(char *name); //寻找节点, 返回节点位置
proute GetNeartest(char *start, char *end); //获取最近的站点, 使用Dijkstra算法
void PrintGraph(); //打印邻接表, 调试用

//队列存储:
//左闭右开区间
//fornt指示队头元素
//rear指示队尾元素的下一个
void CreateQueue();
void QueuePush(Vertex *v);
Vertex* QueuePop();
int QueueIsEmpty();

int main(){
    scanf("%d", &RouteCnt);
    CreateGraph();
    //PrintGraph();

    char *start = malloc(sizeof(char)*NameSize);
    char *end = malloc(sizeof(char)*NameSize);
    scanf("%s %s",start, end);

    proute result = GetNeartest(start, end);
    PrintRouteAndLen(result);
    return 0;
}
void CreateGraph(){
    //读入图
    for(int p=0;p<RouteCnt;p++){
        int RouteNum;
        scanf("%d",&RouteNum);
        char *StartName = malloc(sizeof(char)*NameSize);
        char *EndName;
        double Dis;
        scanf("%s", StartName);
        InsertVertex(StartName);
        while(1){
            scanf("%lf", &Dis);
            if(Dis!=0){
                EndName = malloc(sizeof(char)*NameSize);
                scanf("%s",EndName);
                InsertVertex(EndName);
                InsertEdge(StartName, EndName, Dis);
                StartName = EndName;
            }else{
                break;
            }
        }
    }
}

void InsertVertex(char *name){
    if(FindVertex(name)==-1){
        Vertex *v = malloc(sizeof(Vertex));
        v->name = name;
        v->next = NULL;
        Vertexs[VertexNum] = v;
        VertexNum++;
    }
}
int FindVertex(char *name){
    for(int i=VertexNum-1;i>=0;i--){
        //printf("%s, %s\n", Vertexs[i]->name, name);
        if(strcmp(Vertexs[i]->name,name)==0){
            return i;
        }
    }
    return -1;
}
void InsertEdge(char *start, char* end, double dis){
    int StartPos = FindVertex(start);
    pEdge e = malloc(sizeof(struct Edge));
    e->dis = dis;
    e->name = end;
    e->next = Vertexs[StartPos]->next;
    Vertexs[StartPos]->next = e;
}

void PrintGraph(){
    for(int i=0;i<VertexNum;i++){
        printf("%s",Vertexs[i]->name);
        pEdge e = Vertexs[i]->next;
        while(e!=NULL){
            printf("-%.2lf->%s",e->dis, e->name);
            e = e->next;
        }
        printf("-->NULL\n");
    }
}

proute GetNeartest(char *start, char* end){
    //dijkstra算法
    int StartPos = FindVertex(start);
    int EndPos = FindVertex(end);
    int visited[VertexNum]; //记录是否访问过
    double dis[VertexNum]; //记录到起点的距离
    int path[VertexNum]; //记录前驱节点
    //初始化
    for(int i=0;i<VertexNum;i++){
        visited[i] = 0;
        dis[i] = 1000000;
        path[i] = -1;
    }
    visited[StartPos] = 1;
    //初始化起点附近的节点
    pEdge e = Vertexs[StartPos]->next;
    while(e!=NULL){
        int pos = FindVertex(e->name);
        dis[pos] = e->dis;
        path[pos] = StartPos;
        e = e->next;
    }
    dis[StartPos] = 0;
    path[StartPos] = StartPos;
    //开始遍历
    for(int i=0;i<VertexNum;i++){
        int min = 100000000;
        int minPos = -1;
        //找到距离最近的节点
        for(int j=0;j<VertexNum;j++){
            if(visited[j]==0 && dis[j]<min){
                min = dis[j];
                minPos = j;
            }
        }
        //没找到
        if(minPos == -1){
            break;
        }
        visited[minPos] = 1;
        //以minPos为中间节点，更新其他节点的距离
        pEdge e = Vertexs[minPos]->next;
        while(e!=NULL){
            int pos = FindVertex(e->name);
            if(visited[pos]==0 && dis[minPos]+e->dis<dis[pos]){
                dis[pos] = dis[minPos]+e->dis;
                path[pos] = minPos;
            }
            e = e->next;
        }
    }
    //输出路径
    proute p = (proute)malloc(sizeof(struct route));
    p->head = NULL;
    p->tail = NULL;
    p->len = dis[EndPos];
    int pos = EndPos;
    //在开始节点之前插入
    while(pos!=StartPos){
        pnode node = (pnode)malloc(sizeof(struct node));
        node->name = Vertexs[pos]->name;
        node->next = p->head;
        node->len = dis[pos] - dis[path[pos]]; //距离为前驱节点到当前节点的距离
        p->head = node;
        pos = path[pos];
    }
    //插入开始节点
    pnode node = (pnode)malloc(sizeof(struct node));
    node->name = Vertexs[StartPos]->name;
    node->next = p->head;
    node->len = dis[StartPos];
    p->head = node;
    return p;
}

void QueuePush(Vertex *v){
    if(q.front == q.rear){
        printf("QUEUE FULL!");
        return;
    }
    q.v[q.rear] = v;
    q.rear = (q.rear+1)%QueueSize;
}

Vertex* QueuePop(){
    if(q.front == q.rear){
        printf("QUEUE EMPTY!");
        return NULL;
    }
    Vertex *v = q.v[q.front];
    q.front = (q.front+1)%QueueSize;
    return v;
}

void CreateQueue(){ 
    q.front = 0;
    q.rear = 1;
}

int QueueIsEmpty(){
    return q.front == q.rear;
}

void PrintRouteAndLen(proute p){
    //13.00 青年路 4.00 中山公园 2.00 循礼门 2.00 江汉路 5.00 积玉桥
    pnode node = p->head;
    printf("%.2lf ", p->len); //总长度
    printf("%s ", node->name); //第一个节点
    node = node->next;
    while(node!=NULL){
        printf("%.2lf ", node->len); //中间节点的距离
        printf("%s ", node->name); //中间节点的名字
        node = node->next;
    }
}