#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char name[10]; // 站名
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
proute CreateRoute(int num); //创建线路
void PrintAllRoute(proute* p); //打印线路

int main(){
    scanf("%d", &RouteCnt);
    proute* route = CreateRoutes(RouteCnt);
    //printf("RouteCnt: %d\n", RouteCnt);
    PrintAllRoute(route);
    return 0;
}

void PrintAllRoute(proute* p){
    int num = RouteCnt;
    for(int i = 0; i < num; i++){
        printf("%d ", p[i]->num);
        pnode temp = p[i]->head;
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
        printf("\n");
    }
}


proute CreateRoute(int num){
    proute p = (proute)malloc(sizeof(struct route));
    p->num = num;
    p->head = NULL;
    p->tail = NULL;
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
            scanf("%s", p->name);
            scanf("%lf", &distance);
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