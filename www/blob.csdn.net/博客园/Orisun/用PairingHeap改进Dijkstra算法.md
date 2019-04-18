# 用PairingHeap改进Dijkstra算法 - Orisun - 博客园







# [用PairingHeap改进Dijkstra算法](https://www.cnblogs.com/zhangchaoyang/articles/2366869.html)





先提供几个链接
- [PairingHeap算法](http://wenku.baidu.com/view/4b719d1f650e52ea55189837.html)讲得不错
- [PairingHeap的C语言实现](http://blog.csdn.net/golden_shadow/article/details/6301969)
- [BinaryHeap, FibHeap, PairHeap对改进Dijkstra的性能比较](http://d.download.csdn.net/download/zaneabc/186790)

Dijkstra的计算过程就是在维护一张表，形如：
|v|known|d|p|
|----|----|----|----|
|v1|T|0|0|
|v2|F|2|v1|
|v3|F|3|v4|
|v4|T|1|v1|
|v5|F|3|v4|
|v6|F|9|v4|
|v7|F|5|v4|

每一次循环要从d中找出最小者，于是PairHeap、FibHeap、BinaryHeap等等就派上用场了。本文我们采用PariHeap，至于为什么请看链接3。当需要更改（减小）d的值时，需要从PairHeap上找到相应的节点再执行DecreaseKey操作，于是我在链接2的基础之上为PairHeap增加了Find操作。

base.h

#ifndef _BASE_H
#define _BASE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define VERTEXNUM 7
#define FALSE (0)
#define TRUE (1)
#define MAXSIBLINGS ((VERTEXNUM)+1)

typedef int BOOL;
typedef double ValueType;

typedef struct {
	int vindex;
	ValueType dist;
} Item;

#endif


list.h

#ifndef _LIST_H
#define _LIST_H

#include"base.h"

/*单向链表节点*/
typedef struct ltnode {
	Item item;
	struct ltnode *next;
} LTNode;
/*单向链表定义*/
typedef struct linkedList {
	LTNode *head;
	int size;
} *LinkedList;

/*创建并初始化单向链表*/
BOOL Initialize_L(LinkedList * const llh);

/*确定一个单向链表是否为空*/
BOOL IsEmpty_L(const LinkedList * const llh);

/*向单向链表末尾添加一个元素*/
BOOL Insert_L(const LinkedList * const lln, const Item item);

/*释放单向链表占用的空间*/
void Release_L(LinkedList * const llh);

#endif

list.c

#include"list.h"

static LTNode *makeNode(const Item item)
{
	LTNode *newNode;
	newNode = (LTNode *) malloc(sizeof(struct ltnode));
	newNode->item = item;
	newNode->next = NULL;
	return newNode;
}

static void releaseNode(LTNode * const p)
{
	if (p != NULL) {
		releaseNode(p->next);
		free(p);
	}
}

/*创建并初始化单向链表*/
BOOL Initialize_L(LinkedList * const llh)
{
	(*llh) = (struct linkedList *) malloc(sizeof(struct linkedList));
	(*llh)->head = NULL;
	(*llh)->size = 0;
	return TRUE;
}

/*确定一个单向链表是否为空*/
BOOL IsEmpty_L(const LinkedList * const llh)
{
	if ((*llh)->size == 0)
		return TRUE;
	else
		return FALSE;
}

/*向单向链表头部添加一个元素*/
BOOL Insert_L(const LinkedList * const llh, const Item item)
{
	LTNode *newNode = makeNode(item);
	if ((*llh)->size == 0)
		(*llh)->head = newNode;
	else {
		newNode->next = (*llh)->head;
		(*llh)->head = newNode;
	}
	(*llh)->size++;
	return TRUE;
}

/*释放单向链表占用的空间*/
void Release_L(LinkedList * const llh)
{
	releaseNode((*llh)->head);
	free(*llh);
}

pairheap.h

#ifndef _PAIRHEAP_H
#define _PAIRHEAP_H

#include"base.h"

typedef struct phnode {
	Item item;
	struct phnode *left, *previous, *nextSibling;
} PHNode;

typedef struct pairingHeap {
	PHNode *root;
	int current;
} *PairingHeap;

/*创建并初始化配对堆*/
BOOL Initialize_P(PairingHeap * const pph);

/*确定一个配对堆是否为空*/
BOOL IsEmpty_P(const PairingHeap * const pph);

/*向配对堆中插入一个数据为指定数据的结点.localizer用来传递回新节点的地址*/
BOOL Insert_P(const PairingHeap * const pph, const Item item);

/*在配对堆上查找数据点*/
PHNode* Find_P(const PairingHeap * const pph, const Item item);

/*将配对堆中指定节点的数据降低delta*/
BOOL DecreaseKey_P(const PairingHeap * const pph, PHNode * const position,
		   const ValueType delta);

/*删除配对堆中数据域最小的节点，并通过pmin将其携带回调用该函数的函数*/
BOOL DeleteMin_P(const PairingHeap * const pph, Item * const pmin);

/*打印堆*/
void Print_P(const PairingHeap * const pph);

/*释放配对堆占用的空间*/
void Release_P(PairingHeap * const pph);

#endif

pairheap.c

#include"pairheap.h"

/*全局变量声明*/
static PHNode *NullNode = NULL;

/*局部函数声明*/
static PHNode *compareAndLink_P(PHNode * const first, PHNode * const second);
static PHNode *makeNode_P(const Item item);
static PHNode *combineSiblings_P(PHNode * firstSibling);
static void release_P(PHNode * const pn);
static PHNode *find(PHNode * const root, const Item item);
static void printNode(const PHNode * const root);

/*局部函数定义*/
static PHNode *compareAndLink_P(PHNode * const first, PHNode * const second)
{
	if (second == NullNode)
		return first;
	if (second->item.dist < first->item.dist) {	/*把first作为second的最左子孩子 */
		second->previous = first->previous;
		first->previous = second;
		first->nextSibling = second->left;
		first->nextSibling->previous = first;
		second->left = first;
		return second;
	} else {		/*把second作为first的最左孩子 */
		second->previous = first;
		first->nextSibling = second->nextSibling;
		first->nextSibling->previous = first;
		second->nextSibling = first->left;
		second->nextSibling->previous = second;
		first->left = second;
		return first;
	}
}

static PHNode *makeNode_P(const Item item)
{
	PHNode *newNode;
	newNode = (PHNode *) malloc(sizeof(PHNode));
	if (NULL == newNode)
		return NULL;
	newNode->item = item;
	newNode->left = newNode->nextSibling = newNode->previous = NullNode;
	return newNode;
}

static PHNode *combineSiblings_P(PHNode * firstSibling)
{
	static PHNode *treeArray[MAXSIBLINGS];	/*treeArray是个一维数组，每个元素是Node*类型。静态成员在编译时就要初始化，所以数组长度必须是已知的。给treeArray分配一个足够大的长度，再定义为静态的（全局生命周期），每次调用函数时都使用这一个treeArray，省去过多的重复初始化 */
	int i, j, numSiblings;
	/*如果只有一个孩子，则直接返回它 */
	if (firstSibling->nextSibling == NullNode)
		return firstSibling;
	/*把所有兄弟放在treeArray中 */
	for (numSiblings = 0; firstSibling != NullNode; numSiblings++) {
		treeArray[numSiblings] = firstSibling;
		/*打断双向链表中每个节点向后的指针 */
		firstSibling->previous->nextSibling = NullNode;
		firstSibling = firstSibling->nextSibling;
	}
	treeArray[numSiblings] = NullNode;	//一定要把最后一个设为NullNode，因为treeArray的总长度为MAXSIBLINGS，NullNode之前的才是有效元素
	/*从左向右两两合并子树 */
	//printf("第一趟合并： ");
	for (i = 0; i + 1 < numSiblings; i += 2){
		treeArray[i] = compareAndLink_P(treeArray[i], treeArray[i + 1]);
		//printf("一次");
		//printNode(treeArray[i]);
	}
	j = i - 2;
	if (j == numSiblings - 3){	/*兄弟有奇数个 */
		treeArray[j] = compareAndLink_P(treeArray[j], treeArray[j + 2]);
		//printf("合并最一个奇数项");
		//printNode(treeArray[j]);
	}
	/*进行第二趟合并 */
	/*从右向左逐个合并 */
	//printf("第二趟合并： ");
	while (j >= 2) {
		treeArray[j - 2] =
		    compareAndLink_P(treeArray[j - 2], treeArray[j]);
		//printf("一次");
		//printNode(treeArray[j-2]);
		j -= 2;
	}
	return treeArray[0];
}

static void release_P(PHNode * const pn)
{
	if (pn != NullNode) {
		release_P(pn->left);
		release_P(pn->nextSibling);
		free(pn);
	}
}

static PHNode *find(PHNode * const root, const Item item)
{
	//printf("开始查找vindex=%d\tdist=%d\n",root->item.vindex,(int)root->item.dist);
	if(root==NullNode)
		return NullNode;
	else if(root->item.vindex==item.vindex)
		return root;
	else if(item.dist<root->item.dist)
		return find(root->nextSibling,item);
	else{
		PHNode *rect;
		return ((rect=find(root->nextSibling,item))==NullNode)?find(root->left,item):rect;/*先搜兄弟节点;如果找不到，再搜孩子节点;如果还找不到则返回NullNode*/
	}
}

static void printNode(const PHNode * const root)
{
	if(root==NullNode){
		printf("\t");
		return;
	}
	else{
		printf("%d(%d)\t",root->item.vindex,(int)root->item.dist);
		printf("%d's next:",root->item.vindex);printNode(root->nextSibling);
		printf("%d's left:",root->item.vindex);printNode(root->left);
	}
}

/*************************************接口函数定义********************************/
BOOL Initialize_P(PairingHeap * const pph)
{
	if (NullNode == NULL) {
		NullNode = (PHNode *) malloc(sizeof(PHNode));
		if (NullNode == NULL) {
			puts("Out of space.");
			return FALSE;
		}
		*pph = (struct pairingHeap *)malloc(sizeof(struct pairingHeap));
		if (*pph == NULL) {
			puts("Out of space");
			free(NullNode);
			NullNode == NULL;
			return FALSE;
		}
		NullNode->left = NullNode->previous = NullNode->nextSibling = NullNode;
		(*pph)->root = NullNode;
		(*pph)->current = 0;
	}
	return TRUE;
}

BOOL IsEmpty_P(const PairingHeap * const pph)
{
	switch ((*pph)->current) {
	case 0:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL Insert_P(const PairingHeap * const pph, const Item item)
{
	PHNode *newNode;
	newNode = makeNode_P(item);
	if (newNode == NULL) {
		puts("out of space.");
		return FALSE;
	}
	//*localizer = newNode;
	if (IsEmpty_P(pph) == TRUE)
		(*pph)->root = newNode;
	else
		(*pph)->root = compareAndLink_P((*pph)->root, newNode);
	(*pph)->current++;
	return TRUE;
}

PHNode *Find_P(const PairingHeap * const pph, const Item item)
{
	//printf("调用Find_P\n");
	PHNode * rect=find((*pph)->root,item);
	if(rect==NullNode)
		return NULL;
	else{
		return rect;
	}
}

BOOL DecreaseKey_P(const PairingHeap * const pph, PHNode * const position,
		   const ValueType delta)
{
	if (delta <= 0)
		return FALSE;
	//printf("要把%d降低%d\n",position->item.vindex,(int)delta);
	position->item.dist -= delta;
	//printf("降低节点值以后：vindex=%d\tdist=%d\n",position->item.vindex,(int)position->item.dist);
	if (position == (*pph)->root)
		return TRUE;	/*如果减小的是根节点的值，可以直接返回 */
	/*把position从堆上(双向链表中)取下来 */
	position->nextSibling->previous = position->previous;
	if (position->previous->left == position)	/*position是最左孩子 */
		position->previous->left = position->nextSibling;
	else
		position->previous->nextSibling = position->nextSibling;
	position->nextSibling = NullNode;
	/*再把position合并到堆的根节点上去*/
	(*pph) -> root = compareAndLink_P ((*pph) -> root, position) ;
	return TRUE;
}

BOOL DeleteMin_P(const PairingHeap * pph, Item * const pmin)
{
	PHNode *newRoot;
	if (IsEmpty_P(pph))
		return FALSE;
	else {
		newRoot = NullNode;
		*pmin = (*pph)->root->item;
		if ((*pph)->root->left != NullNode)
			newRoot = combineSiblings_P((*pph)->root->left);
		free((*pph)->root);
		(*pph)->root = newRoot;
		(*pph)->current--;
		return TRUE;
	}
}

void Print_P(const PairingHeap * const pph)
{
	if((*pph)->root==NullNode)
		return;
	else
		printNode((*pph)->root);
	printf("\n");
}

void Release_P(PairingHeap * const pph)
{
	release_P((*pph)->root);
	free(*pph);
	free(NullNode);
	NullNode = NULL;
}

dijkstra.c

#include"list.h"
#include"pairheap.h"
#include<time.h>
#include<limits.h>

typedef struct  {
	int vindex;
	BOOL known;
	ValueType dist;
	int preindex;
}TableLine;

typedef struct {
	int vindex;
	LinkedList neighbours;
} Adjancent;

char *vertexName[] = { "V1", "V2", "V3", "V4", "V5", "V6", "V7" };

void InitResultTable(TableLine resultTable[],int len){
	int i;
	for(i=0;i<len;i++){
		resultTable[i].vindex=i;
		resultTable[i].known=FALSE;
		resultTable[i].dist=INT_MAX;
		resultTable[i].preindex=-1;
	}
}

/*根据最终的TableLine打印到各节点的最短路径*/
void printShortWay(TableLine resultTable[],int len){
	int i;
	for(i=0;i<len;i++){
		printf("%s: ",vertexName[resultTable[i].vindex]);
		double way=resultTable[i].dist;
		int curline=i;
		do{
			int p=resultTable[curline].preindex;
			if(p!=-1)
				printf("%s\t",vertexName[p]);
			//else
				//printf("END\t");
		}while((curline=resultTable[curline].preindex)!=-1);
		printf("总路程:%d\n",(int)way);
	}
}

/*初始化带权有向图*/
void InitGraph(Adjancent **graph)
{
	LinkedList *list0;list0=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list0);
	Item item1;item1.vindex=1;item1.dist=2;Insert_L(list0, item1);
	Item item2;item2.vindex=3;item2.dist=1;Insert_L(list0, item2);
	Adjancent *adj0;adj0=(Adjancent *)malloc(sizeof(Adjancent));adj0->vindex = 0;adj0->neighbours = *list0;graph[0]=adj0;
	
	LinkedList *list1;list1=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list1);
	Item item3;item3.vindex=3;item3.dist=3;Insert_L(list1, item3);
	Item item4;item4.vindex=4;item4.dist=10;Insert_L(list1, item4);
	Adjancent *adj1;adj1=(Adjancent *)malloc(sizeof(Adjancent));adj1->vindex = 1;adj1->neighbours = *list1;graph[1]=adj1;
	
	LinkedList *list2;list2=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list2);
	Item item5;item5.vindex=0;item5.dist=4;Insert_L(list2, item5);
	Item item6;item6.vindex=5;item6.dist=5;Insert_L(list2, item6);
	Adjancent *adj2;adj2=(Adjancent *)malloc(sizeof(Adjancent));adj2->vindex = 2;adj2->neighbours = *list2;graph[2]=adj2;
	
	LinkedList *list3;list3=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list3);
	Item item7;item7.vindex=2;item7.dist=2;Insert_L(list3, item7);
	Item item8;item8.vindex=4;item8.dist=2;Insert_L(list3, item8);
	Item item9;item9.vindex=5;item9.dist=8;Insert_L(list3, item9);
	Item item10;item10.vindex=6;item10.dist=4;Insert_L(list3, item10);
	Adjancent *adj3;adj3=(Adjancent *)malloc(sizeof(Adjancent));adj3->vindex = 3;adj3->neighbours = *list3;graph[3]=adj3;
	
	LinkedList *list4;list4=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list4);
	Item item11;item11.vindex=6;item11.dist=6;Insert_L(list4, item11);
	Adjancent *adj4;adj4=(Adjancent *)malloc(sizeof(Adjancent));adj4->vindex = 4;adj4->neighbours = *list4;graph[4]=adj4;
	
	LinkedList *list5;list5=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list5);
	Adjancent *adj5;adj5=(Adjancent *)malloc(sizeof(Adjancent));adj5->vindex = 5;adj5->neighbours = *list5;graph[5]=adj5;
	
	LinkedList *list6;list6=(LinkedList *)malloc(sizeof(LinkedList));Initialize_L(list6);
	Item item12;item12.vindex=5;item12.dist=1;Insert_L(list6, item12);
	Adjancent *adj6;adj6=(Adjancent *)malloc(sizeof(Adjancent));adj6->vindex = 6;adj6->neighbours = *list6;graph[6]=adj6;
}

/*以领接表的形式打开带权有向图*/
void printGraph(Adjancent **graph){
	int i, j;
	LTNode *neighbour;
	for (i = 0; i < VERTEXNUM; i++) {
		printf("%s\t", vertexName[graph[i]->vindex]);
		neighbour = graph[i]->neighbours->head;
		int len = graph[i]->neighbours->size;
		while (len-- > 0) {
			printf("%s(%d)\t", vertexName[neighbour->item.vindex],
			       (int)(neighbour->item.dist));
			neighbour = neighbour->next;
		}
		printf("\n");
	}
}

int main()
{
	Adjancent **graph;
	graph = (Adjancent **) malloc(sizeof(Adjancent *) * VERTEXNUM);
	InitGraph(graph);
	printGraph(graph);
	
	TableLine resultTable[VERTEXNUM];
	InitResultTable(resultTable,VERTEXNUM);
	
	PairingHeap * pph;
	pph=(PairingHeap *)malloc(sizeof(PairingHeap));
	Initialize_P(pph);
	
	int startindex=0;		/*指定起点*/
	resultTable[startindex].dist=0;
	
	int i;
	for(i=0;i<VERTEXNUM;++i){
		Item item;
		item.vindex=i;
		if(i!=startindex)
			item.dist=INT_MAX;
		else
			item.dist=0;
		Insert_P(pph,item);
	}
	//printf("初始化堆后： ");
	//Print_P(pph);
	
	while(1){
		Item *pmin;
		pmin=(Item*)malloc(sizeof(Item));
		if(DeleteMin_P(pph,pmin)==FALSE){		/*从配对堆上取下最小元素*/
			break;
		}
		//printf("取下最小元素后： ");
		//Print_P(pph);
		int index=pmin->vindex;
		resultTable[index].known=TRUE;
		double cvw=resultTable[index].dist;
		LTNode *neighbour= graph[index]->neighbours->head;
		int len = graph[index]->neighbours->size;
		while (len-- > 0) {
			int ind=neighbour->item.vindex;
			if(resultTable[ind].known==FALSE){
				double d=neighbour->item.dist;
				if(d+cvw<resultTable[ind].dist){
					Item fi;
					fi.vindex=ind;
					fi.dist=resultTable[ind].dist;
					PHNode *change=Find_P(pph,fi);
					if(change==NULL){
						fprintf(stderr,"在配对堆上找不到要找的项.vindex=%d\tdist=%d\n",fi.vindex,(int)fi.dist);
						free(change);
						exit(1);
					}
					//printf("change:vindex=%d\tdist=%d\n",change->item.vindex,(int)change->item.dist);
					DecreaseKey_P(pph,change,resultTable[ind].dist-d-cvw);
					//printf("降低元素值后： ");
					//Print_P(pph);
					resultTable[ind].dist=d+cvw;
					resultTable[ind].preindex=index;
				}
			}
			neighbour = neighbour->next;
		}
	}
	
	printShortWay(resultTable,VERTEXNUM);
	Release_P(pph);
	for(i=0;i<VERTEXNUM;++i){
		Release_L(&(graph[i]->neighbours));
	}
	return 0;
}


![](https://pic002.cnblogs.com/images/2012/103496/2012022416021943.png)












