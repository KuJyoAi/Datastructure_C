#include<stdio.h>
#include<stdlib.h>

typedef struct term
{
    float coef;
    int exp;
    struct term *next;
}term, * pterm;
pterm push(pterm head, float coef, int exp); //按幂降序插入, 返回头指针
pterm read(pterm head, int _sign); // 处理输入, 读入系数和指数, 并插入链表, _sign是总符号计数, 为偶数代表正
void print(pterm p); // 打印多项式
pterm add(pterm left, pterm right); //计算, 返回相加后的头指针

int main(){
    pterm left = NULL;
    // 输入左边的多项式
    char c = getchar(); //c = "("
    while(1){
        left = read(left, 0);
        c = getchar();
        if(c == ')'){
            //读到')', 结束
            break;
        }else{
            ungetc(c, stdin);
        }
        
    }
    //读取符号
    c = getchar(); //c = "+" or "-"
    int sign = 0;
    if(c == '-'){
        sign = 1;
    }
    // 输入右边的多项式
    pterm right = NULL;
    c = getchar(); //c = "("
    while(1){
        right = read(right, sign);
        c = getchar();
        if(c == ')'){
            break;
        }else{
            ungetc(c, stdin);
        }
    }

    // 计算
    pterm result = NULL;
    result = add(left, right);

    //输出结果
    print(result);
}
void print(pterm result){
    if(result == NULL){
        printf("0");
        return;
    }

    while(result != NULL){
        //打印系数
        if(result->coef != 1 && result->coef != -1){
            if(result->coef == (int)result->coef){
                //处理整数, 去掉.0
                printf("%d", (int)result->coef);
            }else{
                //处理小数, 保留一位小数
                printf("%.1f", result->coef);
            }
        }else if(result->coef == -1){
            //处理系数为-1的情况, 省略1
            printf("-");
        }else if(result->exp == 0){
            //指数为0, 且系数为1, 直接打印"1
            printf("1");
        }

        //打印x
        if(result->exp != 0){
            printf("x");
        }

        //打印指数
        if (result->exp != 1 && result->exp != 0){
            printf("^%d", result->exp);
        }

        result = result->next;

        //后面还有项, 打印"+", "-"包含在下一次的系数中
        if(result != NULL){
            if(result->coef > 0){
                printf("+");
            }
        }
    }
}
pterm read(pterm head, int sign){
    //(-x+4.4x^2+6x^-3-1.2x^9)-(5.4x^2-6x^-3+7.8x^15)
    float coef;
    int exp;
    char c = getchar();
    
    if(c=='0' || c ==')'){
        //结束
        if(c==')'){
            //放回右括号以便下一步处理
            ungetc(c, stdin);
        }
        return head;
    }

    //符号测试
    if(c=='-'){
        sign+=1;
    }else if(c != '+'){
        //读到了系数
        ungetc(c, stdin);
    }
    
    //系数测试
    c = getchar();
    if(c=='x'){
        //系数为+-1
        coef = 1;
        //放回x以便下一步操作
        ungetc(c, stdin);
    }else{
        //有系数
        ungetc(c, stdin);
        scanf("%f", &coef);
    }

    c = getchar();
    if(c=='x'){
        c = getchar();
        if(c=='^'){
            //有指数
            scanf("%d", &exp);
        }else{
            //指数为1
            exp = 1;
            //放回x以便下一步操作
            ungetc(c, stdin);
        }
    }else{
        //没有x, 指数为0
        exp = 0;
        //放回c以便下一步操作
        ungetc(c, stdin);
    }

    //处理系数符号
    if(sign%2==1){
        coef = -coef;
    }

    head = push(head, coef, exp);
    return head;
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

    // 空链表
    if(head == NULL){
        head = q;
        return head;
    }

    //头部处理
    if(exp > head->exp){
        q->next = head;
        head = q;
        return head;
    }else if(exp == head->exp){
        head->coef += coef;
        if(head->coef == 0){
            pterm temp = head;
            head = head->next;
            free(temp);
        }
        return head;
    }
    
    //插入到中间或尾部
    pterm p = head;
    while(p->next != NULL && p->next->exp > exp){
        p = p->next;
    }

    // 相等的情况
    if(p->next != NULL && p->next->exp == exp){
        p->next->coef += coef;
        if(p->next->coef == 0){
            pterm temp = p->next;
            p->next = p->next->next;
            free(temp);
        }
        return head;
    }
    q->next = p->next;
    p->next = q;
    return head;
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