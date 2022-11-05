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
proute* CreateRoutes(int num); //创建线路
proute CreateRoute(int num); //创建线路
proute AddNode(proute p, double len_pre, double len_next, char name_pre[10], char name[10]); //添加节点
proute DeleteNode(proute p, char *name); //删除节点
int FromNumGetPos(proute* p, int num); //根据线路号获取线路
void Operate(proute* p); //操作
void PrintAllRoute(proute* p); //打印线路
void PrintRoute(proute p); //打印线路

void PrintRouteAndLen(proute p); //打印线路和距离
void InsertVertex(char *name); //插入节点, 如果存在则不插入
void InsertEdge(char *start, char* end, double dis); //插入边
int FindVertex(char *name); //寻找节点, 返回节点位置
proute GetNeartest(char *start, char *end); //获取最近的站点
void PrintGraph(); //打印邻接表

void CreateQueue();
void QueuePush(Vertex *v);
Vertex* QueuePop();
int QueueIsEmpty();

int main(){
    scanf("%d", &RouteCnt);
    //读入图
    //printf("%d",RouteCnt);
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
    //PrintGraph();
    char *start = malloc(sizeof(char)*NameSize);
    char *end = malloc(sizeof(char)*NameSize);
    scanf("%s %s",start, end);
    proute result = GetNeartest(start, end);
    PrintRouteAndLen(result);
    return 0;
}

void PrintAllRoute(proute* p){
    int num = RouteCnt;
    for(int i = 0; i < num; i++){
        PrintRoute(p[i]);
        printf("\n");
    }
}
void PrintRoute(proute p){
    printf("%d ", p->num);
    pnode temp = p->head;
    while(temp != NULL){
        printf("%s", temp->name);
        if(temp->next != NULL){
            printf(" ");
        }
        if(temp->len != 0){
            printf("%.2lf ", temp->len);
        }
        temp = temp->next;
    }
}
void Operate(proute* p){
    char *op = (char*)malloc(sizeof(char) * 20);
    scanf("%s", op);
    if(op[0]=='a'){
        //add 操作
        int num;
        scanf("%d", &num);
        num = FromNumGetPos(p, num);

        double len_pre, len_next;
        char *name_pre = (char*)malloc(sizeof(char) * 20), *name = (char*)malloc(sizeof(char) * 20);
        scanf("%lf", &len_pre);
        if(len_pre == 0){
            scanf("%lf %s", &len_next, name);
            p[num] = AddNode(p[num], 0, len_next, NULL, name);
        }else{
            scanf("%lf %s %s", &len_next, name_pre, name);
            p[num] = AddNode(p[num], len_pre, len_next, name_pre, name);
        }

    }else if(op[0] = 'd'){
        // delete 操作
        int num;
        scanf("%d", &num);
        num = FromNumGetPos(p, num);

        char *name = (char*)malloc(sizeof(char) * 20);
        scanf("%s", name);
        p[num] = DeleteNode(p[num], name);
    }
    
}
proute CreateRoute(int num){
    proute p = (proute)malloc(sizeof(struct route));
    p->num = num;
    p->head = NULL;
    p->tail = NULL;
    return p;
}
int FromNumGetPos(proute* p, int _num){
    int cnt = RouteCnt;
    for(int i = 0; i < cnt; i++){
        if(p[i]->num == _num){
            return i;
        }
    }
    return -1;
}
proute DeleteNode(proute p, char *name){
    pnode temp = p->head;
    while(temp != NULL){
        if(strcmp(temp->name, name) == 0){
            //printf("Delete s\n", temp->name);
            if(temp->pre != NULL){
                temp->pre->next = temp->next;
                temp->pre->len = temp->pre->len + temp->len;
            }else{
                p->head = temp->next;
            }

            if(temp->next != NULL){
                temp->next->pre = temp->pre;
            }else{
                p->tail = temp->pre;
            }
            free(temp);
            PrintRoute(p);
            return p;
        }
        temp = temp->next;
    }
    if(temp == NULL){
        printf("删除失败，没有同名站点\n");
    }
    return p;
}
proute AddNode(proute p, double len_pre, double len_next, char *name_pre, char *name){
    pnode new_node = (pnode)malloc(sizeof(struct node));
    new_node->name = (char*)malloc(sizeof(char) * 20);
    strcpy(new_node->name, name);
    new_node->len = len_next;
    new_node->next = NULL;
    new_node->pre = NULL;
    //插头
    if(len_pre == 0){
        new_node->next = p->head;
        p->head->pre = new_node;
        p->head = new_node;
        PrintRoute(p);
        return p;
    }
    if(p->head == NULL){
        p->head = new_node;
        p->tail = new_node;
    }else{
        pnode temp_pre = NULL;
        pnode temp = p->head;
        while(temp != NULL){
            if(strcmp(temp->name, name) == 0){
                printf("增加失败，已有同名站点");
                return p;
            }

            if(strcmp(temp->name, name_pre) == 0 && temp_pre == NULL){
                temp_pre = temp;
            }
            temp = temp->next;
        }
        if(temp_pre == NULL){
            printf("增加失败，没有与输入的增加位置前一站点同名的站点");
            return p;
        }
        //考虑边界情况, 插到最后一站点
        if(temp_pre->next != NULL){
            //temp_pre->new_node->temp
            temp = temp_pre->next;
            temp->pre->next = new_node;
            new_node->pre = temp_pre;
            new_node->next = temp;
            temp->pre = new_node;
            temp_pre->len = len_pre;
        }else{
            //temp_pre->new_node
            temp_pre->next = new_node;
            new_node->pre = temp_pre;
            temp_pre->len = len_pre;
            p->tail = new_node;
        }

    }
    PrintRoute(p);
    return p;
}
proute* CreateRoutes(int num){
    proute* route = (proute*)malloc(sizeof(proute)*num);

    for(int i = 0; i < num; i++){
        int number;
        scanf("%d", &number);
        route[i] = CreateRoute(number);

        //读入站点
        double distance = 1;
        //读取站点
        while(distance != 0){
            pnode p = (pnode)malloc(sizeof(struct node));
            p->name = (char*)malloc(sizeof(char) * 20);
            char *name = (char*)malloc(sizeof(char) * 20);
            scanf("%s", name);
            scanf("%lf", &distance);
            strcpy(p->name, name);
            p->len = distance;
            if(route[i]->head == NULL){
                route[i]->head = p;
                route[i]->tail = p;
                p->next = NULL;
                p->pre = NULL;
            }else{
                route[i]->tail->next = p;
                p->pre = route[i]->tail;
                route[i]->tail = p;
                p->next = NULL;
            }
        }
    }
    return route;
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
//队列存储:
//左闭右开区间
//fornt指示队头元素
//rear指示队尾元素的下一个
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