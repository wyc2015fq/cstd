# C语言二叉树之二叉链表 - Koma Hub - CSDN博客
2017年12月05日 22:56:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：289
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/* 二叉链表 */
typedef struct BTNode{
    char ch;
    struct BTNode *lchild, *rchild;
}BTNode, *BinTree;
BTNode *newBTNode(char ch)
{
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    node->ch = ch;
    node->lchild = NULL;
    node->rchild = NULL;
}
void addLchild(BinTree *bt, char ch)
{
    BTNode *node = newBTNode(ch);
    (*bt)->lchild = node;
}
void addRchild(BinTree *bt, char ch)
{
    BTNode *node = newBTNode(ch);
    (*bt)->rchild = node;
}
void priOrder(BinTree bt)
{
    if(bt){
        printf("%c",bt->ch);
        priOrder(bt->lchild);
        priOrder(bt->rchild);
    }
}
void midOrder(BinTree bt)
{
    if(bt){
        midOrder(bt->lchild);
        printf("%c",bt->ch);
        midOrder(bt->rchild);
    }
}
void postOrder(BinTree bt)
{
    if(bt){
        postOrder(bt->lchild);
        postOrder(bt->rchild);
        printf("%c",bt->ch);
    }
}
int main(int argc, char*argv[])
{
    char ch[11] = {'a','b','c','d','e','f','g','h','i','j','q'};
    BinTree bt = (BTNode*)malloc(sizeof(BTNode));
    bt->ch = ch[0];
    addLchild(&bt, ch[1]);
    addRchild(&bt, ch[2]);
    addLchild(&(bt->lchild), ch[3]);
    addRchild(&(bt->lchild), ch[4]);
    addLchild(&(bt->rchild), ch[5]);
    addRchild(&(bt->rchild), ch[6]);
    printf(" Prior : ");priOrder(bt);printf("\n");
    printf("Middle : ");midOrder(bt);printf("\n");
    printf("  Post : ");postOrder(bt);printf("\n");
    
	return 0;
}
```
结果：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
 Prior : abdecfg
Middle : dbeafcg
  Post : debfgca
-bash-4.1$
```
2.二叉链表树的输入（前序、中序、后序）：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/* 二叉链表 */
typedef struct BTNode{
    char ch;
    struct BTNode *lchild, *rchild;
}BTNode, *BinTree;
BTNode *newBTNode(char ch)
{
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    node->ch = ch;
    node->lchild = NULL;
    node->rchild = NULL;
}
void addLchild(BinTree *bt, char ch)
{
    BTNode *node = newBTNode(ch);
    (*bt)->lchild = node;
}
void addRchild(BinTree *bt, char ch)
{
    BTNode *node = newBTNode(ch);
    (*bt)->rchild = node;
}
void priOrder(BinTree bt)
{
    if(bt){
        printf("%c",bt->ch);
        priOrder(bt->lchild);
        priOrder(bt->rchild);
    }
}
void midOrder(BinTree bt)
{
    if(bt){
        midOrder(bt->lchild);
        printf("%c",bt->ch);
        midOrder(bt->rchild);
    }
}
void postOrder(BinTree bt)
{
    if(bt){
        postOrder(bt->lchild);
        postOrder(bt->rchild);
        printf("%c",bt->ch);
    }
}
/* 销毁二叉树T */
void destoryBinTree(BinTree *bt)
{
    if(*bt)
    {
        if((*bt)->lchild)
        {
            destoryBinTree(&(*bt)->lchild);
        }
        if((*bt)->rchild)
        {
            destoryBinTree(&(*bt)->rchild);
        }
        free(*bt);
        *bt = NULL;
    }
}
/* 按前序输入二叉树中结点的值（一个字符） */
void priorInputBinTree(BinTree *bt)
{
    char c;
    scanf("%c",&c);
    if(c == 'q')
    {
        *bt = NULL;   
        
    } else {
        *bt = (BinTree)malloc(sizeof(BTNode));
        (*bt)->ch = c;
        priorInputBinTree(&(*bt)->lchild);
        priorInputBinTree(&(*bt)->rchild);
    }
}
/* 按middle序输入二叉树中结点的值（一个字符） */
void midInputBinTree(BinTree *bt)
{
    char c;
    scanf("%c",&c);
    if(c == 'q')
    {
        *bt = NULL;   
        
    } else {
        *bt = (BinTree)malloc(sizeof(BTNode));
        priorInputBinTree(&(*bt)->lchild);
        (*bt)->ch = c;
        priorInputBinTree(&(*bt)->rchild);
    }
}
/* 按post序输入二叉树中结点的值（一个字符） */
void postInputBinTree(BinTree *bt)
{
    char c;
    scanf("%c",&c);
    if(c == 'q')
    {
        *bt = NULL;   
        
    } else {
        *bt = (BinTree)malloc(sizeof(BTNode));
        priorInputBinTree(&(*bt)->lchild);
        priorInputBinTree(&(*bt)->rchild);
        (*bt)->ch = c;
    }
}
/* 初始条件: 二叉树T存在 */
/* 操作结果: 若T为空二叉树,则返回TRUE,否则FALSE */
char* BinTreeEmpty(BinTree bt)
{ 
	if(bt)
		return "Binary tree isn't empty!";
	else
		return "Binary tree is empty!";
}
/* 初始条件: 二叉树T存在。操作结果: 返回T的深度 */
int depthOfBinTree(BinTree bt)
{
    int i, j;
    if(bt->lchild)
    {
        i = depthOfBinTree(bt->lchild);
    } 
    else 
        i = 0;
    if(bt->rchild)
    {
        j = depthOfBinTree(bt->rchild);
    } 
    else 
        j = 0;
    return i>j?++i:++j;
}
int main(int argc, char*argv[])
{
    char ch[11] = {'a','b','c','d','e','f','g','h','i','j','q'};
    BinTree bt = (BTNode*)malloc(sizeof(BTNode));
    bt->ch = ch[0];
    addLchild(&bt, ch[1]);
    addRchild(&bt, ch[2]);
    addLchild(&(bt->lchild), ch[3]);
    addRchild(&(bt->lchild), ch[4]);
    addLchild(&(bt->rchild), ch[5]);
    addRchild(&(bt->rchild), ch[6]);
    printf(" Prior : ");priOrder(bt);printf("\n");
    printf("Middle : ");midOrder(bt);printf("\n");
    printf("  Post : ");postOrder(bt);printf("\n");
    
    destoryBinTree(&bt);
    printf("%s\n",BinTreeEmpty(bt));
    //bt = (BTNode*)malloc(sizeof(BTNode));
    //bt->ch = ch[0];
    //addLchild(&bt, ch[1]);
    //addRchild(&bt, ch[2]);
    priorInputBinTree(&bt);
    //midInputBinTree(&bt);
    printf("%s\n",BinTreeEmpty(bt));
    printf("The depth of binary tree is %d\n",depthOfBinTree(bt));
    printf(" Prior : ");priOrder(bt);printf("\n");
    printf("Middle : ");midOrder(bt);printf("\n");
    printf("  Post : ");postOrder(bt);printf("\n");
	return 0;
}
```
结果为：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
 Prior : abdecfg
Middle : dbeafcg
  Post : debfgca
Binary tree is empty!
abdqqeqqcfqqgqq
Binary tree isn't empty!
The depth of binary tree is 3
 Prior : abdecfg
Middle : dbeafcg
  Post : debfgca
-bash-4.1$
```
值得注意的是二叉树的输入！！！！！！！
