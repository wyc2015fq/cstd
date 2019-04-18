# 【算法学习】B-Tree编程实现（C++模板类封装） - 江南烟雨 - CSDN博客
2013年03月20日 15:05:19[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3069
B-Tree模拟编程实现。采用C++模板类封装。参考《算法导论（第二版）》第18章 B树。
实现了B树的搜索、插入和删除的重要操作。
欢迎交流和讨论，如有错误，还请指出~（E-Mali：xiajunhust@gmail.com）
BTree.h：
```cpp
//B-树编程实现
//《算法导论（第二版）》第18章 B树
//Author：江南烟雨
//E-Mail：xiajunhust@gmail.com
#include <iostream>
//注意：内存的释放操作
//B-Tree节点数据结构定义
template <class KeyT>
struct BTreeNode{
	int n;//当前节点中存储的关键字数
	KeyT *keys;//指向所有关键字的指针
	bool isLeaf;//标识当前节点是否是叶子
	struct BTreeNode **childs;//指向所有指向子女的指针
	struct BTreeNode *parent;//指向父节点的指针
};
//B-Tree类封装
template <class KeyT>
class BTree{
public:
	
private:
	typedef struct BTreeNode<KeyT>* BTreeNodeLink;//指向节点类型的指针类型定义
	BTreeNodeLink T;//根节点
	//每个非根节点至少有t-1个关键字，至多2t-1个关键字
	int t;//B树的最小度数
public:
	BTree(int tVal = 2);
	~BTree();
	BTreeNodeLink searchBTree(BTreeNodeLink T,KeyT k,int &index);//在B树中搜索某关键字
	BTreeNodeLink getRoot();//返回当前B树的根节点
	void insertBTreeNode(KeyT k);//向B树中插入关键字
	void deleteBTreeKey(BTreeNodeLink T,KeyT k);//从B树中删除特定关键字
	void createBTree(KeyT *keyValues);//创建B树
	void printBTreeBFS(BTreeNodeLink t);//层次遍历输出以t为根的子树B树
	void splitChild(BTreeNodeLink x,int i,BTreeNodeLink y);//分裂满的子节点
	void insertBTreeNonFull(BTreeNodeLink x,KeyT k);//将关键字插入到以该非满节点为根的树中
	BTreeNodeLink __allocateNode();//产生一个新的节点
	void deleteNode(BTreeNodeLink node);//释放一个节点所占的空间（不包括其子女节点所占空间）
	void deleteTree(BTreeNodeLink t);//删除一棵B树所占空间
};
//构造函数
template <class KeyT>
BTree<KeyT>::BTree(int tVal = 2)
{
	t = tVal;
	typename BTree<KeyT>::BTreeNodeLink x = __allocateNode();
	x->isLeaf = true;
	x->n = 0;
	T = x;
}
//析构函数
template <class KeyT>
BTree<KeyT>::~BTree()
{
	deleteTree(T);
	T = NULL;
}
//函数：依据一组关键字值，创建一棵B树
template <class KeyT>
void BTree<KeyT>::createBTree(KeyT *keyValues)
{
	//递归插入关键字实现
}
//函数：在B树中搜索特定关键字
//参数解释：
//T：要搜索的子树的根
//k：要搜索的关键字
//index：保存关键字所在节点中关键字序列中的索引
//返回值：关键字所在的节点
template <class KeyT>
typename BTree<KeyT>::BTreeNodeLink BTree<KeyT>::searchBTree(BTreeNodeLink T,KeyT k,int &index)
{
	if(NULL == T)
	{
		index = -1;
		return NULL;
	}
	int i = 0;
	//搜索根节点
	while(i < T->n && k > T->keys[i])
		++i;
	//在根节点中已搜索到相应的关键字
	if(i < T->n && k == T->keys[i])
	{
		index = i;
		//cout << "OK : search the key " << k << " successfully ! " << endl;
		//cout << "The index : " << index << endl;
		return T;
	}
	//否则，在子树中递归搜索
	if(T->isLeaf)//当前子树根节点已经是叶子，则搜索失败
	{
		//cout << "Warnning : search the key " << k << "  failed ! " << endl;
		index = -1;
		return NULL;
	}
	else
	{
		return searchBTree(T->childs[i],k,index);
	}
}
//函数：返回当前B树的根节点
template <class KeyT>
typename BTree<KeyT>::BTreeNodeLink BTree<KeyT>::getRoot()
{
	return T;
}
//函数：向B树中插入关键字
template <class KeyT>
void BTree<KeyT>::insertBTreeNode(KeyT k)
{
	typename BTree<KeyT>::BTreeNodeLink r = T;
	if (2 * t - 1 == r->n)//根节点满
	{
		typename BTree<KeyT>::BTreeNodeLink s = __allocateNode();
		T = s;//新根节点
		s->isLeaf = false;
		s->n = 0;
		s->childs[0] = r;
		splitChild(s,0,r);//分裂原根节点
		insertBTreeNonFull(s,k);
	}
	else
		insertBTreeNonFull(r,k);
}
//函数：删除特定关键字
//参数解释：
//K：要删除的关键字
//TSubTree：要删除的关键字所在节点指针
template <class KeyT>
void BTree<KeyT>::deleteBTreeKey(BTreeNodeLink TSubTree,KeyT k)
{
	if(NULL == TSubTree)
		return;
	//判断需要删除的关键字是否存在这棵B树中
	int searchedIndex;
	BTreeNodeLink searchedNode = searchBTree(TSubTree,k,searchedIndex);
	if(NULL == searchedNode)
	{
		cout << "The keyword to be deleted not exist in this B-tree!" << endl;
		return;
	}
	//判断要被删除的关键字是否在当前节点TSubTree中
	int keyIndex = -1;
	for(int j = 0;j < TSubTree->n;++j)
	{
		if(k == TSubTree->keys[j])
		{
			keyIndex = j;
			break;
		}
	}
	//如果要被删除的关键字存在当前节点中且当前节点是叶节点
	if(keyIndex != -1 && TSubTree->isLeaf == true)
	{
		//直接删除
		for(int j = keyIndex;j < TSubTree->n - 1;++j)
			TSubTree->keys[j] = TSubTree->keys[j + 1];
		TSubTree->n = TSubTree->n - 1;
		return;
	}
	//如果要被删除的关键字存在当前节点中且当前节点不是叶节点
	else if (keyIndex != -1 && TSubTree->isLeaf == false)
	{
		//被删除关键字所在节点是内节点，进行如下操作
		//寻找前驱
		BTreeNodeLink predecessorNode = TSubTree->childs[keyIndex];
		//寻找后继
		BTreeNodeLink succeedNode = TSubTree->childs[keyIndex + 1];
		//如果前驱节点中关键字数目大于t-1，则以前驱y中最大关键字替代被删除的关键字，然后删除前驱中最大关键字
		if(predecessorNode->n > t - 1)
		{
			KeyT predecessorKey = predecessorNode->keys[predecessorNode->n - 1];
			TSubTree->keys[keyIndex] = predecessorKey;
			deleteBTreeKey(predecessorNode,predecessorKey);
		}
		//如果后继节点中关键字数目大于t-1，则以后继z中最小关键字替代被删除的关键字，然后删除后继中最小关键字
		else if (succeedNode->n > t - 1)
		{
			KeyT succeedKey = succeedNode->keys[0];
			TSubTree->keys[keyIndex] = succeedKey;
			deleteBTreeKey(succeedNode,succeedKey);
		}
		//前驱y和后继z节点中关键字数目都不满足要求
		else
		{
			//需要将被删除的关键字和z合并进y，使得TSubTree失去k以及指向z的指针，然后从y中删除k
			predecessorNode->keys[predecessorNode->n] = k;
			for(int j = 0;j < t - 1;++j)
				predecessorNode->keys[t + j] = succeedNode->keys[j];
			predecessorNode->n = 2 * t - 1;
			//修改节点TSubTree
			for(int j = keyIndex;j < TSubTree->n - 1;++j)
				TSubTree->keys[j] = TSubTree->keys[j + 1];
			for(int j = keyIndex + 1;j < TSubTree->n;++j)
				TSubTree->childs[j] = TSubTree->childs[j + 1];
			TSubTree->n = TSubTree->n - 1;
			//如果合并节点后TSubTree中关键字数小于t - 1，只可能是TSubTree是根节点
			if(0 == TSubTree->n)//根节点中关键字数为0，树高度下降
				T = TSubTree->childs[keyIndex];
			//释放节点z所占空间
			deleteNode(succeedNode);
			deleteBTreeKey(TSubTree->childs[keyIndex],k);
		}
	}
	//如果要被删除的关键字不存在当前节点中
	else
	{
		//首先确定包含要被删除的关键字的子树的根
		int subTreeIndex;
		BTreeNodeLink searchedNode;
		BTreeNodeLink deletedKeySubT;//包含要被删除的关键字的子树的根
		for (int j = 0;j < TSubTree->n + 1;++j)
		{
			searchedNode = searchBTree(TSubTree->childs[j],k,subTreeIndex);
			if(searchedNode != NULL)
			{
				deletedKeySubT = TSubTree->childs[j];
				break;
			}
		}
		//包含要被删除的关键字的子树的根节点关键字数少于t，则需要进行调整
		//以保证我们降至一个包含至少关键字数为t的节点，然后递归进行删除操作
		if (deletedKeySubT->n < t)
		{
			int index;//当前子树根节点指针在父节点指针序列中的索引
			for (int i = 0;i < TSubTree->n + 1;++i)
			{
				if(TSubTree->childs[i] == deletedKeySubT)
				{
					index = i;
					break;
				}
			}
			//如果有一个相邻兄弟包含至少t个关键字
			BTreeNodeLink leftBrotherNode = TSubTree->childs[index - 1];//左兄弟节点
			BTreeNodeLink rightBrotherNode = TSubTree->childs[index + 1];//右兄弟节点
			//如果左兄弟节点中有多余的关键字，进行如下操作
			//将左兄弟节点中最大关键字上移至双亲节点，而将双亲节点中大于该上移关键字的关键字下移至被删除关键字所在节点中
			if(index >= 1 && leftBrotherNode->n > t - 1)
			{
				//双亲节点中关键字下移
				for(int j = deletedKeySubT->n - 1;j >= 0 ;--j)
					deletedKeySubT->keys[j + 1] = deletedKeySubT->keys[j];
				deletedKeySubT->keys[0] = TSubTree->keys[index - 1];
				deletedKeySubT->n = deletedKeySubT->n + 1;
				//左兄弟节点中关键字上移
				TSubTree->keys[index - 1] = leftBrotherNode->keys[leftBrotherNode->n - 1];
				leftBrotherNode->n = leftBrotherNode->n - 1;
			}
			//如果右兄弟节点中有多余的关键字，进行类似的操作
			else if(index < TSubTree->n + 1 && rightBrotherNode->n > t - 1)
			{
				//双亲节点中关键字下移
				deletedKeySubT->keys[deletedKeySubT->n] = TSubTree->keys[index];
				deletedKeySubT->n = deletedKeySubT->n + 1;
				//右兄弟节点中关键字上移
				TSubTree->keys[index] = rightBrotherNode->keys[0];
				for(int j = 0;j < rightBrotherNode->n - 1;++j)
					rightBrotherNode->keys[j] = rightBrotherNode->keys[j + 1];
				rightBrotherNode->n = rightBrotherNode->n - 1;
			}
			//两个兄弟节点的关键字数都不满足要求，需要进行合并操作
			else
			{
				//将当前节点、父节点中的一个关键字合并到一个兄弟节点（注意有可能没有左兄弟节点或者有兄弟节点的情况）
				if (index >= 1)//有左兄弟，则合并至左兄弟
				{
					leftBrotherNode->keys[t - 1] = TSubTree->keys[index - 1];//父节点中的关键字
					for(int j = 0;j < t - 1;++j)
						leftBrotherNode->keys[t + j] = deletedKeySubT->keys[j];
					leftBrotherNode->n = 2 * t - 1;//合并之后左兄弟节点关键字数为2t-1
					for(int j = 0;j < t;++j)
						leftBrotherNode->childs[t + j] = deletedKeySubT->childs[j];
					//更新父节点关键字及子女指针序列
					for(int j = index - 1;j < TSubTree->n - 1;++j)
						TSubTree->keys[j] = TSubTree->keys[j + 1];
					TSubTree->n = TSubTree->n - 1;
					for(int j = index;j < TSubTree->n;++j)
						TSubTree->childs[j] = TSubTree->childs[j + 1];
					deleteNode(deletedKeySubT);
					deletedKeySubT = leftBrotherNode;//递归删除操作降至一棵子树
					////递归删除原节点左兄弟节点中的关键字k
					//deleteBTreeKey(leftBrotherNode,k);
				}
				else//否则，合并至右兄弟
				{
					deletedKeySubT->keys[t - 1] = TSubTree->keys[index - 1];//父节点中的关键字
					for(int j = 0;j < t - 1;++j)
						deletedKeySubT->keys[t + j] = rightBrotherNode->keys[j];
					deletedKeySubT->n = 2 * t - 1;//合并之后左兄弟节点关键字数为2t-1
					for(int j = 0;j < t;++j)
						deletedKeySubT->childs[t + j] = rightBrotherNode->childs[j];
					//更新父节点关键字及子女指针序列
					for(int j = index;j < TSubTree->n - 1;++j)
						TSubTree->keys[j] = TSubTree->keys[j + 1];
					TSubTree->n = TSubTree->n - 1;
					for(int j = index + 1;j < TSubTree->n;++j)
						TSubTree->childs[j] = TSubTree->childs[j + 1];
					deleteNode(rightBrotherNode);
					////递归删除原节点左兄弟节点中的关键字k
					//deleteBTreeKey(TSubTree,k);
				}
			}
		}
		
		//递归在子树中进行删除操作
		deleteBTreeKey(deletedKeySubT,k);
	}
}
//函数：层次遍历输出以t为根的子树
template <class KeyT>
void BTree<KeyT>::printBTreeBFS(typename BTree<KeyT>::BTreeNodeLink t)
{
	if(NULL == t)
		return;
	//输出当前节点所有关键字
	cout << "[";
	for(int i = 0;i < t->n;++i)
	{
		cout << t->keys[i];
		if(t->n - 1 != i)
			cout << " ";
	}
	cout << "]" << endl;
	//递归输出所有子树
	for(int i = 0;i <= t->n;++i)
		printBTreeBFS(t->childs[i]);
}
//函数：分裂满的子节点
//参数解释：
//x：一个非满的内节点
//y：x的一个满子节点
//i；y的下标
template <class KeyT>
void BTree<KeyT>::splitChild(typename BTree<KeyT>::BTreeNodeLink x, int index,typename BTree<KeyT>::BTreeNodeLink y)
{
	typename BTree<KeyT>::BTreeNodeLink z = __allocateNode();//分裂产生的新节点
	z->isLeaf = y->isLeaf;
	z->n = t - 1;
	//关键字赋值
	for(int i = 0;i < t - 1;++i)
		z->keys[i] = y->keys[t + i];
	//非叶节点，子女节点指针赋值
	if(!y->isLeaf)
	{
		for(int i = 0;i < t;++i)
			z->childs[i] = y->childs[t + i];
	}
	z->parent = x;
	//调整原节点的参数
	y->n = t - 1;
	//向后移动子女指针，便于增加一个子女节点指针
	for(int i = x->n;i >= index + 1;--i)
		x->childs[i + 1] = x->childs[i];
	x->childs[index + 1] = z;//插入新子女指针，指向增加的节点z
	//向后移动关键字，便于将子女的一个关键字上升至父节点
	for(int i = x->n - 1;i >= index;--i)
		x->keys[i + 1] = x->keys[i];
	x->keys[index] = y->keys[t - 1];//关键字上移
	x->n = x->n + 1;
}
//函数：将关键字插入到根节点非满的子树中
template <class KeyT>
void BTree<KeyT>::insertBTreeNonFull(typename BTree<KeyT>::BTreeNodeLink x, KeyT k)
{
	int i = x->n;
	//要插入的节点是叶子节点，直接插入
	if (x->isLeaf)
	{
		//空节点
		if(!i)
		{
			x->keys[0] = k;
			x->n = x->n + 1;
			return;
		}
		//搜寻插入位置,并将关键字向后移
		while(i >= 0 && k < x->keys[i - 1])
		{
			x->keys[i] = x->keys[i - 1];
			--i;
		}
		x->keys[i] = k;
		x->n = x->n + 1;
	}
	else{
		//要插入的节点是非叶节点，需要向下递归到子树，将其插入到子树中适当的叶节点中去
		//寻找要插入的子树
		while(i > 0 && k < x->keys[i - 1])
			--i;
		//判断要下降的子节点是否满
		if (2 * t - 1 == x->childs[i]->n)
		{
			//如果要下降的子树满，则分裂
			splitChild(x,i,x->childs[i]);
			//判断需要下降至哪个子树上
			if(k > x->keys[i])
				++i;//需要下降至右子树上
		}
		insertBTreeNonFull(x->childs[i],k);
	}
}
//函数：生成一个新的节点
//返回值：返回指向新节点的指针
template <class KeyT>
typename BTree<KeyT>::BTreeNodeLink BTree<KeyT>::__allocateNode()
{
	typename BTree<KeyT>::BTreeNodeLink newNode = new struct BTreeNode<KeyT>;
	newNode->n = 0;
	newNode->keys = new KeyT[2 * t - 1];//一次性分配2 * t - 1的空间
	newNode->isLeaf = true;
	newNode->childs = new typename BTree<KeyT>::BTreeNodeLink[2 * t];
	newNode->parent = NULL;
	//子女指针初始化
	for(int i = 0;i < 2 * t;++i)
		newNode->childs[i] = NULL;
	return newNode;
}
//函数：释放一个节点所占空间
template <class KeyT>
void BTree<KeyT>::deleteNode(typename BTree<KeyT>::BTreeNodeLink node)
{
	delete[] node->keys;
}
//函数：释放一棵B树所占空间
template <class KeyT>
void BTree<KeyT>::deleteTree(typename BTree<KeyT>::BTreeNodeLink t)
{
	if(NULL == t)
		return;
	//是叶节点，直接删除空间
	if(t->isLeaf)
		delete[] t->keys;
	else
	{
		//递归删除子树
		for(int i = 0;i < t->n;++i)
			deleteTree(t->childs[i]);
		delete[] t->childs;
	}
}
```
BTreeTest.cpp：
```cpp
#include "BTree.h"
#include <iostream>
using namespace std;
int main()
{
	typedef char KeyType;
	const int NUM = 20;
	KeyType keyVals[NUM] = {'a','b','f','g','k','d','h','m','j','e','s','i','r','x','c','l','n','t','u','p'};
	BTree<KeyType> *BTreeObj = new BTree<KeyType>(3);
	BTreeObj->insertBTreeNode(keyVals[0]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[1]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[2]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[3]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[4]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[5]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[6]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[7]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[8]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[9]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[10]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[11]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[12]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[13]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[14]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[15]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[16]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[17]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[18]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	BTreeObj->insertBTreeNode(keyVals[19]);
	cout << "Current B-Tree------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	int searchedIndex;
	struct BTreeNode<KeyType> *searchedNode = BTreeObj->searchBTree(BTreeObj->getRoot(),'j',searchedIndex);
	KeyType deletedKey = 'm';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'j';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'g';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'i';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'k';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'l';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'r';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'h';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'n';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'e';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'f';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 's';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'u';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'a';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'b';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'p';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'd';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'c';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 't';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	//delete BTreeObj;
	deletedKey = 'x';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	deletedKey = 'x';
	BTreeObj->deleteBTreeKey(BTreeObj->getRoot(),deletedKey);
	cout << "Current B-Tree after delete " << deletedKey << "------------------------------------ : " << endl;
	BTreeObj->printBTreeBFS(BTreeObj->getRoot());
	return 0;
}
```
运行结果截图：
![](https://img-my.csdn.net/uploads/201303/20/1363763366_9072.jpg)
![](https://img-my.csdn.net/uploads/201303/20/1363763376_6656.jpg)
![](https://img-my.csdn.net/uploads/201303/20/1363763402_5954.jpg)

