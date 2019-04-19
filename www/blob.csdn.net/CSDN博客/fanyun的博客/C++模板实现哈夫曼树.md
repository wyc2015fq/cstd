# C++模板实现哈夫曼树 - fanyun的博客 - CSDN博客
2017年08月27日 11:21:46[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：251
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

哈夫曼树（霍夫曼树）又称为最优树.
1、路径和路径长度
在一棵树中，从一个结点往下可以达到的孩子或孙子结点之间的通路，称为路径。通路中分支的数目称为路径长度。若规定根结点的层数为1，则从根结点到第L层结点的路径长[](https://baike.baidu.com/pic/%E5%93%88%E5%A4%AB%E6%9B%BC%E6%A0%91/2305769/13211627/0e2442a7d933c8957bad904bd21373f08202002a?fr=lemma&ct=cover)
![哈夫曼树](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/c0%3Dbaike60%2C5%2C5%2C60%2C20/sign=27fe3ce93a01213fdb3e468e358e5db4/0e2442a7d933c8957bad904bd21373f08202002a.jpg)
度为L-1。
2、结点的权及带权路径长度
若将树中结点赋给一个有着某种含义的数值，则这个数值称为该结点的权。结点的带权路径长度为：从根结点到该结点之间的路径长度与该结点的权的乘积。
3、树的带权路径长度
给定n个权值作为n个叶子结点，构造一棵二叉树，若带权路径长度达到最小，称这样的二叉树为最优二叉树，也称为哈夫曼树(Huffman Tree)。哈夫曼树是带权路径长度最短的树，权值较大的结点离根较近。
##### BinTreeNode.h
```cpp
template<typename Type> class BinaryTree;
template<typename Type> void Huffman(Type *, int, BinaryTree<Type> &);
template<typename Type> class BinTreeNode{
public:
	friend class BinaryTree<Type>;
    friend void Huffman<Type>(Type *, int, BinaryTree<Type> &);
	BinTreeNode():m_pleft(NULL),m_pright(NULL){}
	BinTreeNode(Type item,BinTreeNode<Type> *left=NULL,BinTreeNode<Type> *right=NULL)
		:m_data(item),m_pleft(left),m_pright(right){}
	void Destroy(){		//destroy the tree with the root of the node
		if(this!=NULL){
			this->m_pleft->Destroy();
			this->m_pright->Destroy();
			delete this;
		}
	}
    Type GetData(){
        return m_data;
    }
    BinTreeNode<Type> *Copy(const BinTreeNode<Type> *copy);	//copy the node
private:
	BinTreeNode<Type> *m_pleft,*m_pright;
	Type m_data;
};
template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::Copy(const BinTreeNode<Type> *copy){
	if(copy==NULL){
		return NULL;
	}
	BinTreeNode<Type> *temp=new BinTreeNode<Type>(copy->m_data);
	temp->m_pleft=Copy(copy->m_pleft);
	temp->m_pright=Copy(copy->m_pright);
	return temp;
}
```
BinaryTree.h
```cpp
#include "BinTreeNode.h"
template<typename Type> void Huffman(Type *, int, BinaryTree<Type> &);
template<typename Type> class BinaryTree{
public:
    
    BinaryTree(BinaryTree<Type> &bt1, BinaryTree<Type> &bt2){
        m_proot = new BinTreeNode<Type>(bt1.m_proot->m_data 
            + bt2.m_proot->m_data, bt1.m_proot, bt2.m_proot);
    }
    BinaryTree(Type item){
        m_proot = new BinTreeNode<Type>(item);
    }
    BinaryTree(const BinaryTree<Type> ©){
        this->m_proot = copy.m_proot;
    }
    BinaryTree(){
        m_proot = NULL;
    }
    void Destroy(){
        m_proot->Destroy();
    }
    ~BinaryTree(){
//        m_proot->Destroy();
    }
    
    BinaryTree<Type>& operator=(BinaryTree<Type> copy);	//evaluate node
    friend void Huffman<Type>(Type *, int, BinaryTree<Type> &);
    friend bool operator < <Type>(BinaryTree<Type> &l, BinaryTree<Type> & r);
    friend bool operator > <Type>(BinaryTree<Type> &l, BinaryTree<Type> & r);
    friend bool operator <= <Type>(BinaryTree<Type> &l, BinaryTree<Type> & r);
    friend ostream& operator<< <Type>(ostream& ,BinaryTree<Type>&);	//output the data
private:
	BinTreeNode<Type> *m_proot;
    void Print(BinTreeNode<Type> *start,int n=0);	//print the tree with the root of start
};
template<typename Type> bool operator <(BinaryTree<Type> &l, BinaryTree<Type> &r){
    return l.m_proot->GetData() < r.m_proot->GetData();
}
template<typename Type> bool operator >(BinaryTree<Type> &l, BinaryTree<Type> &r){
    return l.m_proot->GetData() > r.m_proot->GetData();
}
template<typename Type> bool operator <=(BinaryTree<Type> &l, BinaryTree<Type> &r){
    return l.m_proot->GetData() <= r.m_proot->GetData();
}
template<typename Type> void BinaryTree<Type>::Print(BinTreeNode<Type> *start, int n){
	if(start==NULL){
		for(int i=0;i<n;i++){
			cout<<"     ";
		}
		cout<<"NULL"<<endl;
		return;
	}
	Print(start->m_pright,n+1);	//print the right subtree
	for(int i=0;i<n;i++){	//print blanks with the height of the node
		cout<<"     ";
	}
	if(n>=0){
		cout<<start->m_data<<"--->"<<endl;//print the node
	}
	Print(start->m_pleft,n+1);	//print the left subtree
}
template<typename Type> ostream& operator<<(ostream& os,BinaryTree<Type>& out){
	out.Print(out.m_proot);
	return os;
}
template<typename Type> BinaryTree<Type>& BinaryTree<Type>::operator=(BinaryTree<Type> copy){
	m_proot=m_proot->Copy(copy.m_proot);
    return *this;
}
```
##### MinHeap.h
```cpp
template<typename Type> class MinHeap{
public:
	MinHeap(Type heap[],int n);		//initialize heap by a array
	~MinHeap(){
		delete[] m_pheap;
	}
public:
    bool Insert(const Type item);
    bool DeleteMin(Type &first);
private:
	void Adjust(const int start, const int end);	//adjust the elements from start to end
private:
	const int m_nMaxSize;	
	Type *m_pheap;
	int m_ncurrentsize;
};
template<typename Type> void MinHeap<Type>::Adjust(const int start, const int end){
	int i = start,j = i*2+1;
	Type temp=m_pheap[i];
	while(j <= end){
		if(j<end && m_pheap[j]>m_pheap[j+1]){
			j++;
		}
		if(temp <= m_pheap[j]){
			break;
		}
		else{
			m_pheap[i] = m_pheap[j];
			i = j;
			j = 2*i+1;
		}
	}
	m_pheap[i] = temp;
}
template<typename Type> MinHeap<Type>::MinHeap(Type heap[], int n):m_nMaxSize(n){
	m_pheap = new Type[m_nMaxSize];
	for(int i=0; i<n; i++){
		m_pheap[i] = heap[i];
	}
	m_ncurrentsize = n;
	int pos=(n-2)/2;	//Find the last tree which has more than one element;
	while(pos>=0){
		Adjust(pos, n-1);
		pos--;
	}
}
template<typename Type> bool MinHeap<Type>::DeleteMin(Type &first){
    first = m_pheap[0];
    m_pheap[0] = m_pheap[m_ncurrentsize-1];
    m_ncurrentsize--;
    Adjust(0, m_ncurrentsize-1);
    return 1;
}
template<typename Type> bool MinHeap<Type>::Insert(const Type item){
	if(m_ncurrentsize == m_nMaxSize){
		cerr<<"Heap Full!"<<endl;
		return 0;
	}
	m_pheap[m_ncurrentsize] = item;
	int j = m_ncurrentsize, i = (j-1)/2;
	Type temp = m_pheap[j];
	while(j > 0){
		if(m_pheap[i] <= temp){
			break;
		}
		else{
			m_pheap[j] = m_pheap[i];
			j = i;
			i = (j-1)/2;
		}
	}
	m_pheap[j] = temp;
	m_ncurrentsize++;
	return 1;
}
```
Huffman.h
```cpp
#include "BinaryTree.h"
#include "MinHeap.h"
template<typename Type> void Huffman(Type *elements, int n, BinaryTree<Type> &tree){
    BinaryTree<Type> first, second;
    BinaryTree<Type> node[20];
    for (int i=0; i<n; i++){
        node[i].m_proot = new BinTreeNode<Type>(elements[i]);
    }
    MinHeap<BinaryTree<Type> > heap(node, n);
    for (int i=0; i<n-1; i++){
        heap.DeleteMin(first);
        heap.DeleteMin(second);
        
        //using the first and the second minimize element create new tree
        if (first.m_proot->GetData() == second.m_proot->GetData()){
            tree = *(new BinaryTree<Type>(second, first));
        }
        else {
            tree = *(new BinaryTree<Type>(first, second));
        }
        heap.Insert(tree);
    }
}
```
Main.cpp
```cpp
#include <iostream>
using namespace std;
#include "Huffman.h"
int main(){
    BinaryTree<int> tree;
    int init[10]={3,6,0,2,8,4,9,1,5,7};
    Huffman(init,10,tree);
    cout << tree;
    tree.Destroy();
    return 0;
}
```
