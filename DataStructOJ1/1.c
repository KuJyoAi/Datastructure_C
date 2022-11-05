#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 8

typedef struct node {
	int val;
	struct node * next;

}queue, * pnode;//队列结点

typedef struct queue {
	pnode front;
	pnode rear;
	int size;
	int idx; //队首index
}*pqueue;//队列

pqueue create(int idx); //创建队列
pqueue push(pqueue p, int val); //入队, 返回队首index, 如果队列满, 不会入队
pqueue pop(pqueue p); //弹出最后一个元素, 返回队首indexs

int main(){
	int n, i, k;//读入个数 队首下标 输出个数
	pqueue q;
	scanf("%d %d %d", &n, &i, &k);

	q = create(i);
	//读入数据
	for (int j = 0; j < n; j++){
		int val;
		scanf("%d", &val);
		q = push(q, val);
	}

	//处理
	if(k>MAXSIZE){
		printf("Error");
		return 0;
	}else if(k>n){
		printf("Fault");
		return 0;
	}

	if(n>MAXSIZE){
		printf("Full ");
	}

	for(int j=0;j<k;j++){
		printf("%d ", q->rear->val);
		q = pop(q);
	}
	return 0;
}
pqueue pop(pqueue p) {
	if (p->size == 0) {
		printf("Fault");
		return p;
	}
	pnode temp = p->rear;
	p->rear = p->rear->next;
	free(temp);
	return p;
}
pqueue push(pqueue p, int val){
    if(p->front==NULL)
	{
		p->front=(pnode)malloc(sizeof(queue));
		p->front->val=val;
		p->front->next=NULL;
		p->rear=p->front;
		p->size++;
	}else if(p->size<MAXSIZE){
		pnode temp=(pnode)malloc(sizeof(queue));
		temp->val=val;
		temp->next=NULL;
		p->front->next = temp;
		p->front = temp;
		p->size++;
	}
	return p;
}

pqueue create(int idx){
	pqueue p = (pqueue)malloc(sizeof(queue));
	p->front = NULL;
	p->rear = NULL;
	p->size = 0;
	p->idx = idx;
	return p;
}