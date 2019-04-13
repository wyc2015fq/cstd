
# link 链表反转 - 阳光岛主 - CSDN博客

2014年07月26日 22:02:36[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3726个人分类：[Algorithm																](https://blog.csdn.net/sunboy_2050/article/category/694821)



链表反转，示例如下：
偶数个输入：a->b->c->d->e->f
偶数个输出：e->f->c->d->a->b
or
奇数个输入：a->b->c->d->e->f->g
偶数个输出：g->e->f->c->d->a->b

```python
#include <stdio.h>
#include <malloc.h>  
#include <stdlib.h>
/************** start of stack *************/
#define STACK_SIZE 1024
char stack[STACK_SIZE];
int top = 0;
void push(char ch){
    stack[top] = ch;
    top++;
}
char pop(){
    top--;
    return stack[top];
}
int isempty(){
    return 0 == top;
}
void test_stack(){
    push('a');
    push('b');
    push('c');
    push('d');
    while(!isempty()){
        printf("pop ch: %c\n", pop());
    }
}
/************** end of stack *************/


struct _node{
    char data;
    struct _node *next;
};
typedef struct _node node, *plink;

plink init_link(){
    plink pl;
    pl = (plink)malloc(sizeof(node));
    
    // check malloc success or not
    if(NULL == pl) {
        printf("malloc memory fail...");
        return NULL;
    }
    
    // init link head
    pl->data = '\0';
    pl->next = NULL;
    
    return pl;
}
void input_data(plink pl, char data){
    plink p = pl;
    
    while(p->next){
        p = p->next;
    }
    
    plink node = NULL;
    node = (plink)malloc(sizeof(node));     // malloc a new node
    
    // add data
    if(NULL != node){
        node->data = data;
        node->next = p->next;        // last next is NULL
        p->next = node;
        p = node;                    // p point last node
    }
}
void output_link(plink pl){
    if(NULL == pl){
        printf("plink is null");
        return;
    }
    
    plink p = pl->next;    // already check pl is NULL, so here is ok
    while(NULL != p){
        printf("%c -> ", p->data);
        p = p->next;
    } 
    printf("\n\n");        
}

// push and pop stack
plink revert_link2(plink pl){
    plink p = pl;
    while(p->next){
//        printf("p->data: %c\n", p->next->data);
        if(p->next->next){
            push(p->next->next->data);
            push(p->next->data);
            p = p->next->next;
        } else {
            push(p->next->data);
            p = p->next;
        }
    }
    while(!isempty()){
        printf("%c -> ", pop());
    }
    
    printf("\n\n");
    return NULL;
}

plink revert_link(plink pl){
    if(NULL == pl){    // check link is NULL
        return NULL;
    }
    
    int link_len = 0;
    plink tmp_pl = pl->next;
    
    while(tmp_pl){    // count link count
        link_len++;
        tmp_pl = tmp_pl->next;
    }
    
    // link length is no more than two node(s)
    if(link_len <= 2){
        return pl;
    }
    
    // link length is more than two nodes
    return revert_link2(pl);
}


int main(){
    plink pl = NULL;
    
    pl = init_link();         // init link head
    
    input_data(pl, 'a');      // add data
    input_data(pl, 'b');
    input_data(pl, 'c');
    input_data(pl, 'd');
    input_data(pl, 'e');
    input_data(pl, 'f');
    input_data(pl, 'g');
    
    output_link(pl);
    
    plink pl2 = revert_link(pl);
    
    output_link(pl2);
    return 0;
}

/****
revert_link.c

linux gcc compile
gcc revert_link.c -o revert_link && ./revert_link

output result:
a -> b -> c -> d -> e -> f -> g 
g -> e -> f -> c -> d -> a -> b
or
a -> b -> c -> d -> e -> f
e -> f -> c -> d -> a -> b

****/
```
**间隔螺旋反转：**
输入： a -> b -> c -> d -> e -> f
输出： b -> a -> d -> c -> f -> e

```python
plink revert_link3(plink pl){
    if(NULL == pl){
        printf("plink is null");
        return NULL;	
    }	
    plink p = pl;
    plink first = p->next;
    while(NULL != first){
        plink second = first->next;
        if(NULL != second){
            first->next = second->next;     // third node
            second->next = first;           // revert two nodes
            first = first->next;
            p->next = second;
            p = second->next;
        }
    }
    return pl;
}
```



