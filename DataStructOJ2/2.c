#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}*proute;
int RouteCnt = 0; //线路数
proute* CreateRoutes(int num); //创建线路
proute CreateRoute(int num); //读入并创建线路
proute AddNode(proute p, double len_pre, double len_next, char name_pre[10], char name[10]); //添加节点
proute DeleteNode(proute p, char *name); //删除节点
proute GetNeartest(char *s1, char *s2); //获取最近站点
int FromNumGetPos(proute* p, int num); //根据线路号获取线路
void Operate(proute* p); //操作
void PrintAllRoute(proute* p); //打印线路
void PrintRoute(proute p); //打印线路

int main(){
    scanf("%d", &RouteCnt);
    proute* route = CreateRoutes(RouteCnt);
    //printf("RouteCnt: %d\n", RouteCnt);
    PrintAllRoute(route);
    
    Operate(route);

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