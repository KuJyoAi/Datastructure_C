#include<stdio.h>
#include<stdlib.h>

typedef struct term
{
    float coef;
    int exp;
    struct term *next;
}term, * pterm; //定义结构体类型和指针类型

pterm push(pterm head, float coef, int exp); //按幂降序插入, 返回头指针
pterm add(pterm left, pterm right); //计算, 返回相加后的头指针
void print(pterm result); //打印结果

int main(){
    //1 0 1 1 - 1 1
    //1 0
    pterm left = NULL;
    // 输入左边的多项式
    while(1){
        float coef = 0;
        int exp = 0;
        if(!scanf("%f %d", &coef, &exp)){
            break;
        }
        left = push(left, coef, exp);

        //试探是否到结束
        char c = getchar();
        while(c==' '){
            c = getchar();
        }
        if(c=='+' || c=='-'){
            //遇到符号, 如果后面是空格, 则说明到了结尾
            char c2 = getchar();
            if(c2==' '){
                //结尾, 放回符号
                ungetc(c, stdin);
                break;
            }else{
                //否则, 放回去
                ungetc(c2, stdin);
                ungetc(c, stdin);
            }
        }else{
            ungetc(c, stdin);
        }
    }

    int sign; //符号, 0为+, 1为-
    sign = getchar();
    while(sign == ' '){
        sign = getchar();
    }
    if(sign=='+'){
        sign = 0;
    }else{
        sign = 1;
    }
    
    // 输入右边的多项式
    pterm right = NULL;
    while(1){
        float coef = 0;
        int exp = 0;
        if(scanf("%f %d", &coef, &exp)){
            //直接识别前一步读出的符号, 修改系数
            if(sign == 0){   
                right = push(right, coef, exp);
            }else{
                right = push(right, -coef, exp);
            }
        }
        
        char c = getchar();
        while(c==' '){
            c = getchar();
        }
        if(c=='\n' || c==EOF){
            //遇到结尾, 结束
            break;
        }else{
            ungetc(c, stdin);
        }
    }
    // 计算
    pterm result = add(left, right);
    
    //输出结果
    print(result);
}
pterm push(pterm head, float coef, int exp){
    //系数为0, 不插入
    if(coef == 0){
        return head;
    }

    pterm q = (pterm)malloc(sizeof(term));
    q->coef = coef;
    q->exp = exp;
    q->next = NULL;
    //空链表, 直接插入
    if(head == NULL){
        head = q;
        return head;
    }
    //最大, 插入到头部
    if(exp > head->exp){
        q->next = head;
        head = q;
        return head;
    }
    //寻找合适的位置, 保证为递增
    pterm p = head;
    while(p->next != NULL && p->next->exp >= exp){
        p = p->next;
    }
    q->next = p->next;
    p->next = q;
    return head;
}
void print(pterm result){
    //输出结果
    //特殊处理
    if(result == NULL){
        printf("0 0");
        return;
    }
    //输出
    while(result != NULL){
        if(result->coef == (int)result->coef){
            //整数, 去掉.0
            printf("%d %d ", (int)result->coef, result->exp);
        }else{
            //小数, 保留一位小数
            printf("%.1f %d ", result->coef, result->exp);
        }
        result = result->next;
    }
}
pterm add(pterm left, pterm right){
    // 计算
    pterm result = NULL;
    while(left!=NULL && right!=NULL){
        if(left->exp == right->exp){
            if(left->coef + right->coef != 0){
                //如果系数为0, 则直接删除项
                result = push(result, left->coef + right->coef, left->exp);
            }
            left = left->next;
            right = right->next;       
        }else if(left->exp > right->exp){
            result = push(result, left->coef, left->exp);
            left = left->next;
        }else{
            result = push(result, right->coef, right->exp);
            right = right->next;
        }
    }
    //处理剩余的, 直接加到结果中
    while(left != NULL){
        result = push(result, left->coef, left->exp);
        left = left->next;
    }
    while(right != NULL){
        result = push(result, right->coef, right->exp);
        right = right->next;
    }
    return result;
}