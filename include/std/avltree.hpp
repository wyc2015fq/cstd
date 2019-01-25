#include<stack>
#include<utility>
#include<allocators>
#include<functional>
using std::pair;
using std::allocator;
using std::less;
using std::stack;
#ifndef AVLTreeH
#define AVLTreeH
enum {RH=-1,EH=0,LH=1};
template<typename T>struct AVLTreeNode
{
	T data;
	int BF;
	AVLTreeNode<T> *Left,*Right,*Parent;
};
template<typename T,typename Reference,typename Pointer>
struct AVLTreeIterator
{
	typedef AVLTreeIterator<T,Reference,Pointer> iter;
	typedef AVLTreeIterator<T,T&,T*> Iter;
	AVLTreeIterator():node(0) {}
	AVLTreeIterator(AVLTreeNode<T>* p):node(p) {}
	AVLTreeIterator(const Iter& r):node(r.node) {}
	Reference operator*()const {return node->data;}
	Pointer operator->()const {return &(operator*());}
	iter& operator++()
	{
		if(node->Right)
		{
			node=node->Right;
			while(node->Left) node=node->Left;
		}
		else
		{
			AVLTreeNode<T>* y=node->Parent;
			while(y->Right==node&&y->Left!=node) node=y,y=y->Parent;
			node=y;
		}
		return *this;
	}
	iter operator++(int) {iter temp=*this;++*this;return temp;}
	iter& operator--()
	{
		if(node->Left)
		{
			node=node->Left;
			while(node->Right) node=node->Right;
		}
		else
		{
			AVLTreeNode<T>* y=node->Parent;
			while(y->Left==node) node=y,y=y->Parent;
			node=y;
		}
		return *this;
	}
	iter operator--(int) {iter temp=*this;--*this;return temp;}
	bool operator==(const iter& x)const {return node==x.node;}
	bool operator!=(const iter& x)const {return node!=x.node;}
	AVLTreeNode<T>* node;
};
template<typename T,typename Reference,typename Pointer>
struct AVLTreeReverseIterator
{
	typedef AVLTreeIterator<T,Reference,Pointer> iterator;
	typedef AVLTreeReverseIterator<T,Reference,Pointer> iter;
	AVLTreeReverseIterator():node(0) {}
	AVLTreeReverseIterator(const iterator& r):node(r.node) {}
	AVLTreeReverseIterator(const iter& r):node(r.node) {}
	template<typename K,typename U,typename V>
	AVLTreeReverseIterator(const AVLTreeReverseIterator<K,U,V>& r):node(r.node) {}
	iterator base()const {return node;}
	Reference operator*()const
	{
		iter temp=*this;
		++temp;
		return temp.node->data;
	}
	Pointer operator->()const {return &(operator*());}
	iter& operator++()
	{
		if(node->Left)
		{
			node=node->Left;
			while(node->Right) node=node->Right;
		}
		else
		{
			AVLTreeNode<T>* y=node->Parent;
			while(y->Left==node) node=y,y=y->Parent;
			node=y;
		}
		return *this;
	}
	iter operator++(int) {iter temp=*this;++*this;return temp;}
	iter& operator--()
	{
		if(node->Right)
		{
			node=node->Right;
			while(node->Left) node=node->Left;
		}
		else
		{
			AVLTreeNode<T>* y=node->Parent;
			while(y->Right==node&&y->Left!=node) node=y,y=y->Parent;
			node=y;
		}
		return *this;
	}
	iter operator--(int) {iter temp=*this;--*this;return temp;}
	bool operator==(const iter& x)const {return node==x.node;}
	bool operator!=(const iter& x)const {return node!=x.node;}
	AVLTreeNode<T>* node;
};
template<typename T>
inline void RightRotate(AVLTreeNode<T>* &p,AVLTreeNode<T>* header)
{
	AVLTreeNode<T>* Lc=p->Left;
	p->Left=Lc->Right;
	if(Lc->Right) Lc->Right->Parent=p;
	Lc->Parent=p->Parent;
	if(p==header->Left) header->Left=Lc;
	else if(p==p->Parent->Left) p->Parent->Left=Lc;
	else p->Parent->Right=Lc;
	Lc->Right=p;
	p->Parent=Lc;
	p=Lc;
}
template<typename T>
inline void LeftRotate(AVLTreeNode<T>* &p,AVLTreeNode<T>* header)
{
	AVLTreeNode<T>* Rc=p->Right;
	p->Right=Rc->Left;
	if(Rc->Left) Rc->Left->Parent=p;
	Rc->Parent=p->Parent;
	if(p==header->Left) header->Left=Rc;
	else if(p==p->Parent->Left) p->Parent->Left=Rc;
	else p->Parent->Right=Rc;
	Rc->Left=p;
	p->Parent=Rc;
	p=Rc;
}
template<typename T>
void AVLInsertBalance(AVLTreeNode<T>* q,AVLTreeNode<T>* p,AVLTreeNode<T>* header)
{
	bool Up=true;
	while(Up&&p!=header->Left)
	{
		if(p==q->Left)
		{
			if(q->BF==EH) q->BF=LH,Up=true;
			else if(q->BF==RH) q->BF=EH,Up=false;
			else
			{
				if(p->BF==LH) q->BF=p->BF=EH;
				else
				{
					if(p->Right->BF==LH) q->BF=RH,p->BF=EH;
					if(p->Right->BF==EH) q->BF=EH,p->BF=EH;
					if(p->Right->BF==RH) q->BF=EH,p->BF=LH;
					p->Right->BF=EH;
					LeftRotate(p,header);
				}
				RightRotate(q,header);
				return;
			}
		}
		else
		{
			if(q->BF==EH) q->BF=RH,Up=true;
			else if(q->BF==LH) q->BF=EH,Up=false;
			else
			{
				if(p->BF==RH) q->BF=p->BF=EH;
				else
				{
					if(p->Left->BF==LH) q->BF=EH,p->BF=RH;
					if(p->Left->BF==EH) q->BF=EH,p->BF=EH;
					if(p->Left->BF==RH) q->BF=LH,p->BF=EH;
					p->Left->BF=EH;
					RightRotate(p,header);
				}
				LeftRotate(q,header);
				return;
			}
		}
		p=q;
		q=q->Parent;
	}
}
template<typename T>
void AVLEraseBalance(AVLTreeNode<T>* q,AVLTreeNode<T>* p,AVLTreeNode<T>* header)
{
	bool Lo=true;
	while(Lo&&q!=header)
	{
		if(p==q->Left)
		{
			if(q->BF==LH) q->BF=EH,Lo=true;
			else if(q->BF==EH) q->BF=RH,Lo=false;
			else
			{
				if(q->Right==0) q->BF=EH,Lo=true;
				else
				{
					AVLTreeNode<T>* Rc=q->Right;
					if(Rc->BF==EH) q->BF=RH,Rc->BF=LH,Lo=false;
					else if(Rc->BF==RH) q->BF=EH,Rc->BF=EH,Lo=true;
					else
					{
						AVLTreeNode<T>* Ld=Rc->Left;
						if(Ld->BF==LH) q->BF=EH,Rc->BF=RH;
						if(Ld->BF==EH) q->BF=EH,Rc->BF=EH;
						if(Ld->BF==RH) q->BF=LH,Rc->BF=EH;
						Ld->BF=EH;
						RightRotate(Rc,header);
						Lo=true;
					}
					LeftRotate(q,header);
				}
			}
		}
		else
		{
			if(q->BF==RH) q->BF=EH,Lo=true;
			else if(q->BF==EH) q->BF=LH,Lo=false;
			else
			{
				if(q->Left==0) q->BF=EH,Lo=true;
				else
				{
					AVLTreeNode<T>* Lc=q->Left;
					if(Lc->BF==EH) q->BF=LH,Lc->BF=RH,Lo=false;
					else if(Lc->BF==LH) q->BF=EH,Lc->BF=EH,Lo=true;
					else
					{
						AVLTreeNode<T>* Rd=Lc->Right;
						if(Rd->BF==EH) q->BF=EH,Lc->BF=EH;
						if(Rd->BF==RH) q->BF=EH,Lc->BF=LH;
						if(Rd->BF==LH) q->BF=RH,Lc->BF=EH;
						Rd->BF=EH;
						LeftRotate(Lc,header);
						Lo=true;
					}
					RightRotate(q,header);
				}
			}
		}
		p=q;
		q=q->Parent;
	}
}
template<typename T,typename Compare=less<T>,template<typename T>class Alloc=allocator>
class AVLTree
{
public:
	typedef AVLTreeNode<T> Node;
	typedef unsigned sizetype;
	typedef AVLTreeIterator<T,T&,T*> iterator;
	typedef AVLTreeIterator<T,const T&,const T*> constiterator;
	typedef AVLTreeReverseIterator<T,T&,T*> reverseiterator;
	typedef AVLTreeReverseIterator<T,const T&,const T*> constreverseiterator;
	AVLTree():nodecount(0)
	{
		AVLTreeInitialize();
	}
	template<typename Input>AVLTree(Input beg,Input end):nodecount(0)
	{
		AVLTreeInitialize();
		for(;beg!=end;++beg) insert(*beg);
	}
	AVLTree(const AVLTree<T,Compare,Alloc>& r):nodecount(r.nodecount),comp(r.comp)
	{
		AVLTreeInitialize();
		if(!r.empty()) ZCopyTree(r.header,header);
	}
	AVLTree<T,Compare,Alloc>& operator=(const AVLTree<T,Compare,Alloc>& r)
	{
		if(this!=&r)
		{
			clear();
			if(!r.empty()) ZCopyTree(r.header,header);
			nodecount=r.nodecount;
			comp=r.comp;
		}
		return *this;
	}
	~AVLTree()
	{
		clear();
		Destroy(header);
	}
	sizetype size()const {return nodecount;}
	bool empty()const {return header->Left==0;}
	sizetype maxsize()const {return sizetype(-1);}
	void swap(AVLTree<T,Compare,Alloc>& r)
	{
		Zswap(header,r.header);
		Zswap(nodecount,r.nodecount);
		Zswap(comp,r.comp);
	}
	iterator begin() {return header->Parent;}
	constiterator begin()const {return header->Parent;}
	iterator end() {return header;}
	constiterator end()const {return header;}
	reverseiterator rbegin() {return reverseiterator(end());}
	constreverseiterator rbegin()const {return constreverseiterator(end());}
	reverseiterator rend() {return reverseiterator(begin());}
	constreverseiterator rend()const {return constreverseiterator(begin());}
	pair<iterator,bool> insert(const T& v)
	{
		Node *q=header,*p=header->Left;
		while(p!=0)
		{
			q=p;
			if(comp(v,p->data)) p=p->Left;
			else if(comp(p->data,v)) p=p->Right;
			else return pair<iterator,bool>(header,false);
		}
		p=Construct(v);
		p->BF=EH;
		p->Left=p->Right=0;
		p->Parent=q;
		if(header->Left==0) header->Left=header->Parent=header->Right=p;
		else if(comp(v,q->data))
		{
			q->Left=p;
			if(header->Parent==q) header->Parent=p;
		}
		else
		{
			q->Right=p;
			if(header->Right==q) header->Right=p;
		}
		++nodecount;
		AVLInsertBalance(q,p,header);
		return pair<iterator,bool>(p,true);
	}
	template<typename Input>void insert(Input beg,Input end)
	{
		for(;beg!=end;++beg) insert(*beg);
	}
	sizetype erase(const T& v)
	{
		Node* p=header->Left;
		while(p!=0&&p->data!=v)
			p=comp(v,p->data)? p->Left:p->Right;
		if(p==0) return 0;
		Zerase(p);
		return 1;
	}
	void erase(iterator pos) {Zerase(pos.node);}
	void erase(iterator beg,iterator end)
	{
		if(beg.node==header->Parent&&end.node==header) clear();
		else while(beg!=end) erase(beg++);
	}
	void clear()
	{
		Node *pre=header,*p=header->Left;
		stack<Node*> s;
		s.push(0);
		while(p!=0||!s.empty())
		{
			while(p!=0)
			{
				if(p->Right) s.push(p->Right);
				pre=p;
				p=p->Left;
				Destroy(pre);
			}
			p=s.top(),s.pop();
		}
		nodecount=0;
		header->Left=0;
		header->Parent=header->Right=header;
	}
	iterator find(const T& v)
	{
		Node* p=header->Left;
		while(p!=0&&!(p->data==v))
			p=comp(v,p->data)? p->Left:p->Right;
		if(p!=0) return p;
		return end();
	}
	constiterator find(const T& v)const
	{
		Node* p=header->Left;
		while(p!=0&&!(p->data==v))
			p=comp(v,p->data)? p->Left:p->Right;
		if(p!=0) return p;
		return end();
	}
protected:
	void AVLTreeInitialize()
	{
		header=Construct();
		header->Left=0;
		header->Right=header->Parent=header;
	}
	Node* ZCopyNode(Node* p)
	{
		Node* q=Construct(p->data);
		q->Left=q->Right=0;
		q->BF=p->BF;
		return q;
	}
	void ZCopyTree(Node* header,Node* header2)
	{
		Node* p=header->Left;
		Node *pre=header2,*q=header2->Left;
		stack<Node*> s,s2;
		s.push(0),s2.push(0);
		while(p!=0||!s.empty())
		{
			if(p!=0)
			{
				q=ZCopyNode(p);
				pre->Left=q;
				q->Parent=pre;
				pre=q;
				if(p->Right) s.push(p),s2.push(q);
				p=p->Left;
			}
			else
			{
				p=s.top(),s.pop();
				pre=s2.top(),s2.pop();
				if(p)
				{
					p=p->Right;
					q=ZCopyNode(p);
					pre->Right=q;
					q->Parent=pre;
					pre=q;
					if(p->Right) s.push(p),s2.push(q);
					p=p->Left;
				}
			}
		}
		q=p=header2->Left;
		q=p=header2->Left;
		header2->Parent=minimum(p);
		header2->Right=maximum(q);
	}
	template<typename U>void Zswap(U& x,U& y)
	{
		U tmp=x;
		x=y;
		y=tmp;
	}
	void Zerase(Node* z)
	{
		Node* y=z;
		Node* x=0;
		Node* xparent=0;
		if(y->Left==0) x=y->Right;
		else if(y->Right==0) x=y->Left;
		else
		{
			y=y->Right;
			while(y->Left) y=y->Left;
			x=y->Right;
		}
		if(z->Left==0||z->Right==0)
		{
			xparent=y->Parent;
			if(x) x->Parent=y->Parent;
			if(header->Left==z) header->Left=x;
			else if(z->Parent->Left==z) z->Parent->Left=x;
			else z->Parent->Right=x;
			if(header->Parent==z)
				if(z->Right==0) header->Parent=z->Parent;
				else header->Parent=minimum(x);
			if(header->Right==z)
				if(z->Left==0) header->Right=z->Parent;
				else header->Right=maximum(x);
		}
		else
		{
			z->Left->Parent=y;
			y->Left=z->Left;
			if(y!=z->Right)
			{
				xparent=y->Parent;
				if(x) x->Parent=y->Parent;
				y->Parent->Left=x;
				y->Right=z->Right;
				z->Right->Parent=y;
			}
			else xparent=y;
			if(header->Left==z) header->Left=y;
			else if(z->Parent->Left==z) z->Parent->Left=y;
			else z->Parent->Right=y;
			y->Parent=z->Parent;
			y->BF=z->BF;
			y=z;
		}
		nodecount-=1;
		AVLEraseBalance(xparent,x,header);
		Destroy(y);
	}
	static Node* minimum(Node* p)
	{
		while(p->Left) p=p->Left;
		return p;
	}
	static Node* maximum(Node* p)
	{
		while(p->Right) p=p->Right;
		return p;
	}
private:
	Node* header;
	sizetype nodecount;
	Compare comp;
	typename Alloc<T>::rebind<Node>::other alloc;
	Node* Construct()
	{
		Node* p=alloc.allocate(1);
		return p;
	}
	Node* Construct(const T& v)
	{
		Node* p=alloc.allocate(1);
		new (p) T(v);
		return p;
	}
	void Destroy(Node* p)
	{
		(&p->data)->~T();
		alloc.deallocate(p,1);
	}
};
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator==(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	typedef typename AVLTree<T,Compare,Alloc>::constiterator constiterator;
	constiterator b1=x.begin(),b2=y.begin(),e1=x.end(),e2=y.end();
	while(b1!=e1&&b2!=e2&&*b1==*b2) ++b1,++b2;
	return b1==e1&&b2==e2;
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator!=(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	return !(x==y);
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator<(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	typedef typename AVLTree<T,Compare,Alloc>::constiterator constiterator;
	constiterator b1=x.begin(),b2=y.begin(),e1=x.end(),e2=y.end();
	for(;b1!=e1&&b2!=e2;++b1,++b2)
	{
		if(*b1<*b2) return true;
		if(*b2<*b1) return false;
	}
	return b1==e1&&b2!=e2;
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator<=(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	return !(y<x);
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator>(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	return y<x;
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline bool operator>=(const AVLTree<T,Compare,Alloc>& x,const AVLTree<T,Compare,Alloc>& y)
{
	return !(x<y);
}
template<typename T,typename Compare,template<typename T>class Alloc>
inline void swap(AVLTree<T,Compare,Alloc>& x,AVLTree<T,Compare,Alloc>& y)
{
	x.swap(y);
}
#endif
