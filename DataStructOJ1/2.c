//Your code begin.
//示例仅供参考，你也可以自行修改设计。
#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
	int coef;	//系数
	int exp;		//指数
	int layer;		//该栈当前所处的地址（层数），建议从0开始，layer<5。用于判断栈满情况
	struct node* next;
}node, * pstack; 		//node用于声明结点，pstack用于声明栈指针

void create_node(pstack new_node, float coef, int exp); //创建新结点
pstack push(pstack top, pstack new_node); //入栈
pstack pop(pstack top); //出栈

int main(void)
{
	//52 1 15 4 20 5 2 8 ; 22 2 6 3 43 6 30 10
	//读取数据
	pstack s1 = NULL;
	for(int i=0;i<6;i++){
		int coef = 0;
		int exp = 0;
		scanf("%d %d", &coef, &exp);
		pstack new_node = (pstack)malloc(sizeof(node));
		create_node(new_node, coef, exp);
		s1 = push(s1, new_node);

		//查看接下来是否有数字
		char c = getchar();
		while(c==' '){
			c = getchar();
		}
		if(c==';'){
			//遇到分号, 结束
			break;
		}else{
			ungetc(c, stdin);
		}
	}

	//读取数据
	pstack s2 = NULL;
	for(int i=0;i<6;i++){
		int coef = 0;
		int exp = 0;
		scanf("%d %d", &coef, &exp);
		pstack new_node = (pstack)malloc(sizeof(node));
		create_node(new_node, coef, exp);
		s2 = push(s2, new_node);

		//查看接下来是否有数字
		char c = getchar();
		while(c==' '){
			c = getchar();
		}
		if(c=='\n' || c==EOF){
			break;
		}else{
			ungetc(c, stdin);
		}
	}

	//合并
	while(s1!=NULL && s2!=NULL){
		if(s1->exp > s2->exp){
			printf("%d %d ", s1->coef, s1->exp);
			s1 = s1->next;
		}else if(s1->exp < s2->exp){
			printf("%d %d ", s2->coef, s2->exp);
			s2 = s2->next;
		}
	}
	//剩余的直接打印
	while(s1!=NULL){
		printf("%d %d ", s1->coef, s1->exp);
		s1 = s1->next;
	}
	while(s2!=NULL){
		printf("%d %d ", s2->coef, s2->exp);
		s2 = s2->next;
	}

    return 0;
}

pstack push(pstack top, pstack new_node)
{
	if(!top){
		top = new_node;
		top->next = NULL;
		top->layer = 0;
		return top;
	}else if (top->layer < 4){
		new_node->layer = top->layer+1;
		new_node->next = top;
		top = new_node;
		return top;
	}else{
		printf("Full!");
		exit(0);
	}
}
void create_node(pstack new_node, float coef, int exp)
{
	new_node->coef = coef;
	new_node->exp = exp;
	new_node->next = NULL;
	new_node->layer = 0;
}
pstack pop(pstack top)
{
	if(top){
		top = top->next;
		top->layer--;
	}
	return top;
}