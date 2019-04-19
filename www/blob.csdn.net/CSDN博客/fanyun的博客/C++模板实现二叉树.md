# C++模板实现二叉树 - fanyun的博客 - CSDN博客
2017年08月27日 11:06:03[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：233
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
  二叉树是每个节点最多有两个子树的树结构。通常子树被称作“左子树”（left subtree）和“右子树”（right subtree）。二叉树常被用于实现二叉查找树和二叉堆。
      二叉树的每个结点至多只有二棵子树(不存在度大于2的结点)，二叉树的子树有左右之分，次序不能颠倒。二叉树的第i层至多有2^{i-1}个结点；深度为k的二叉树至多有2^k-1个结点；对任何一棵二叉树T，如果其终端结点数为n_0，度为2的结点数为n_2，则n_0=n_2+1。
      一棵深度为k，且有2^k-1个节点称之为满二叉树；深度为k，有n个节点的二叉树，当且仅当其每一个节点都与深度为k的满二叉树中，序号为1至n的节点对应时，称之为完全二叉树。
      下面我们使用模板实现一颗简单二叉树：
**BinTreeNode.h**
```cpp
template<typename Type> class BinaryTree;
template<typename Type> class BinTreeNode{
public:
	friend class BinaryTree<Type>;
	BinTreeNode():m_pleft(NULL),m_pright(NULL){}
	BinTreeNode(Type item,BinTreeNode<Type> *left=NULL,BinTreeNode<Type> *right=NULL)
		:m_data(item),m_pleft(left),m_pright(right){}
	Type GetData() const;		//get thd data
	BinTreeNode<Type> *GetLeft() const;		//get the left node
	BinTreeNode<Type> *GetRight() const;	//get the right node
	void SetData(const Type data);			//change the data
	void SetLeft(const BinTreeNode<Type> *left);	//change thd left node
	void SetRight(const BinTreeNode<Type> *right);	//change the right node
	void InOrder();		//inorder the tree with the root of the node
	void PreOrder();	//perorder the tree with the root of the node
	void PostOrder();	//postoder the tree with the root of the node
	
	int Size();			//get size
	int Height();		//get height
	BinTreeNode<Type> *Copy(const BinTreeNode<Type> *copy);	//copy the node
	void Destroy(){		//destroy the tree with the root of the node
		if(this!=NULL){
			this->m_pleft->Destroy();
			this->m_pright->Destroy();
			delete this;
		}
	}
	friend bool equal<Type>(const BinTreeNode<Type> *s,const BinTreeNode<Type> *t);	//is equal?
private:
	BinTreeNode<Type> *m_pleft,*m_pright;
	Type m_data;
};
template<typename Type> Type BinTreeNode<Type>::GetData() const{
	return this!=NULL?m_data:-1;
}
template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetLeft() const{
	return this!=NULL?m_pleft:NULL;
}
template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetRight() const{
	return this!=NULL?m_pright:NULL;
}
template<typename Type> void BinTreeNode<Type>::SetData(const Type data){
	if(this!=NULL){
		m_data=data;
	}
}
template<typename Type> void BinTreeNode<Type>::SetLeft(const BinTreeNode<Type> *left){
	if(this!=NULL){
		m_pleft=left;
	}
}
template<typename Type> void BinTreeNode<Type>::SetRight(const BinTreeNode<Type> *right){
	if(this!=NULL){
		m_pright=right;
	}
}
template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::Copy(const BinTreeNode<Type> *copy){
	if(copy==NULL){
		return NULL;
	}
	BinTreeNode<Type> *temp=new BinTreeNode<Type>(copy->m_data);
	temp->m_pleft=Copy(copy->m_pleft);
	temp->m_pright=Copy(copy->m_pright);
	return temp;
}
template<typename Type> bool equal(const BinTreeNode<Type> *s,const BinTreeNode<Type> *t){
	if(s==NULL&&t==NULL){
		return 1;
	}
	if(s&&t&&s->m_data==t->m_data&&equal(s->m_pleft,t->m_pleft)&&equal(s->m_pright,t->m_pright)){
		return 1;
	}
	return 0;
}
template<typename Type> void BinTreeNode<Type>::InOrder(){
	if(this!=NULL){
		this->m_pleft->InOrder();
		cout<<"--->"<<this->m_data;
		this->m_pright->InOrder();
	}
}
template<typename Type> void BinTreeNode<Type>::PreOrder(){
	if(this!=NULL){
		cout<<"--->"<<this->m_data;
		this->m_pleft->PreOrder();
		this->m_pright->PreOrder();
	}
}
template<typename Type> void BinTreeNode<Type>::PostOrder(){
	if(this!=NULL){
		this->m_pleft->PostOrder();
		this->m_pright->PostOrder();
		cout<<"--->"<<this->m_data;
	}
}
template<typename Type> int BinTreeNode<Type>::Size(){
	if(this==NULL){
		return 0;
	}
	return 1+this->m_pleft->Size()+this->m_pright->Size();
}
template<typename Type> int BinTreeNode<Type>::Height(){
	if(this==NULL){
		return -1;
	}
	int lheight,rheight;
	lheight=this->m_pleft->Height();
	rheight=this->m_pright->Height();
	return 1+(lheight>rheight?lheight:rheight);
}
```
**BinaryTree.h**
```cpp
#include "BinTreeNode.h"
template<typename Type> class BinaryTree{
public:
	BinaryTree():m_proot(NULL){}
	BinaryTree(const Type stop):m_stop(stop),m_proot(NULL){}
	BinaryTree(BinaryTree<Type>& copy);
	virtual ~BinaryTree(){
		m_proot->Destroy();
	}
	virtual bool IsEmpty(){		//is empty?
		return m_proot==NULL;
	}
	
	virtual BinTreeNode<Type> *GetLeft(BinTreeNode<Type> *current);	//get the left node
	virtual BinTreeNode<Type> *GetRight(BinTreeNode<Type> *current);//get the right node
	virtual BinTreeNode<Type> *GetParent(BinTreeNode<Type> *current);//ghe thd parent
	const BinTreeNode<Type> *GetRoot() const;	//get root
	
	virtual bool Insert(const Type item);		//insert a new node
	virtual BinTreeNode<Type> *Find(const Type item) const;	//find thd node with the data
	void InOrder();	
	void PreOrder();
	void PostOrder();
	int Size();		//get size
	int Height();	//get height
	BinaryTree<Type>& operator=(const BinaryTree<Type> copy);	//evaluate node
	friend bool operator== <Type>(const BinaryTree<Type> s,const BinaryTree<Type> t);//is equal?
	friend ostream& operator<< <Type>(ostream& ,BinaryTree<Type>&);	//output the data
	friend istream& operator>> <Type>(istream& ,BinaryTree<Type>&);	//input the data
		
private:
	Type m_stop;		//just using for input the data;
	BinTreeNode<Type> *m_proot;
	//find the parent of current in the tree with the root of start
	BinTreeNode<Type> *GetParent(BinTreeNode<Type> *start,BinTreeNode<Type> *current);
	void Print(BinTreeNode<Type> *start,int n=0);	//print the tree with the root of start
};
template<typename Type> BinaryTree<Type>::BinaryTree(BinaryTree<Type>& copy){
	if(copy.m_proot){
		this->m_stop=copy.m_stop;
	}
	m_proot=m_proot->Copy(copy.m_proot);
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetLeft(BinTreeNode<Type> *current){
	return m_proot&¤t?current->m_pleft:NULL;
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetRight(BinTreeNode<Type> *current){
	return m_proot&¤t?current->m_pright:NULL;
}
template<typename Type> const BinTreeNode<Type>* BinaryTree<Type>::GetRoot() const{
	return m_proot;
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetParent(BinTreeNode<Type> *start, BinTreeNode<Type> *current){
	if(start==NULL||current==NULL){
		return NULL;
	}
	if(start->m_pleft==current||start->m_pright==current){
		return start;
	}
	BinTreeNode<Type> *pmove;
	if((pmove=GetParent(start->m_pleft,current))!=NULL){//find the parent in the left subtree
		return pmove;
	}
	else{
		return GetParent(start->m_pright,current);	//find the parent in the right subtree
	}
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetParent(BinTreeNode<Type> *current){
	return m_proot==NULL||current==m_proot?NULL:GetParent(m_proot,current);	
}
template<typename Type> bool BinaryTree<Type>::Insert(const Type item){
	BinTreeNode<Type> *pstart=m_proot,*newnode=new BinTreeNode<Type>(item);
	if(m_proot==NULL){
		m_proot=newnode;
		return 1;
	}
	while(1){
		if(item==pstart->m_data){
			cout<<"The item "<<item<<" is exist!"<<endl;
			return 0;
		}
		if(item<pstart->m_data){
			if(pstart->m_pleft==NULL){
				pstart->m_pleft=newnode;
				return 1;
			}
			pstart=pstart->m_pleft;	//if less than the node then insert to the left subtree
		}
		else{
			if(pstart->m_pright==NULL){
				pstart->m_pright=newnode;
				return 1;
			}
			pstart=pstart->m_pright;//if more than the node then insert to the right subtree
		}
	}
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::Find(const Type item) const{
	BinTreeNode<Type> *pstart=m_proot;
	while(pstart){
		if(item==pstart->m_data){
			return pstart;
		}
		if(item<pstart->m_data){
			pstart=pstart->m_pleft;	//if less than the node then find in the left subtree
		}
		else{
			pstart=pstart->m_pright;//if more than the node then find in the right subtree
		}
	}
	return NULL;
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
template<typename Type> BinaryTree<Type>& BinaryTree<Type>::operator=(const BinaryTree<Type> copy){
	if(copy.m_proot){
		this->m_stop=copy.m_stop;
	}
	m_proot=m_proot->Copy(copy.m_proot);
    return *this;
}
template<typename Type> ostream& operator<<(ostream& os,BinaryTree<Type>& out){
	out.Print(out.m_proot);
	return os;
}
template<typename Type> istream& operator>>(istream& is,BinaryTree<Type>& in){
	Type item;
	cout<<"initialize the tree:"<<endl<<"Input data(end with "<<in.m_stop<<"!):";
	is>>item;
	while(item!=in.m_stop){	//m_stop is the end of input
		in.Insert(item);
		is>>item;
	}
	return is;
}
template<typename Type> bool operator==(const BinaryTree<Type> s,const BinaryTree<Type> t){
	return equal(s.m_proot,t.m_proot);
}
template<typename Type> void BinaryTree<Type>::InOrder(){
	this->m_proot->InOrder();
}
template<typename Type> void BinaryTree<Type>::PreOrder(){
	this->m_proot->PreOrder();
}
template<typename Type> void BinaryTree<Type>::PostOrder(){
	this->m_proot->PostOrder();
}
template<typename Type> int BinaryTree<Type>::Size(){
	return this->m_proot->Size();
}
template<typename Type> int BinaryTree<Type>::Height(){
	return this->m_proot->Height();
}
```
Main.cpp
```cpp
#include <iostream>
using namespace std;
#include "BinaryTree.h"
int main(){
	BinaryTree<int> tree(-1);
//	int init[10]={3,6,0,2,8,4,9,1,5,7};
	int init[30]={17,6,22,29,14,0,21,13,27,18,2,28,8
		,26,3,12,20,4,9,23,15,1,11,5,19,24,16,7,10,25};
	for(int i=0;i<30;i++){
		tree.Insert(init[i]);
	}
	//cin>>tree;
	cout<<tree<<endl;
	cout<<tree.GetParent(tree.Find(20))->GetData()<<endl;
	cout<<tree.Find(15)->GetRight()->GetData()<<endl;
	cout<<"size="<<tree.Size()<<endl;
	cout<<"height="<<tree.Height()<<endl;
	tree.InOrder();
	cout<<endl<<endl;
	tree.PreOrder();
	cout<<endl<<endl;
	tree.PostOrder();
	cout<<endl<<endl;
	
	BinaryTree<int> tree2=tree;
	cout<<tree2<<endl;
	cout<<tree2.GetParent(tree2.Find(20))->GetData()<<endl;
	cout<<tree2.Find(15)->GetRight()->GetData()<<endl;
	cout<<(tree==tree2)<<endl;
	return 0;
}
```
