# 树的前序遍历（递归和非递归java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月13日 16:13:39[QuJack](https://me.csdn.net/A784586)阅读数：966







** 树的前序遍历（递归和非递归实现）**

树的基本遍历是解决树相关问题的基础，所以要很熟悉，理解透彻！



二叉树的结点定义：

```java
class BinaryTree{  
public int value;  
public BinaryTree leftNode;  
public BinaryTree rightNode;  
BinaryTree(int x) { value = x; }  
}
```





前序遍历：根左右的原则遍历访问树的所有结点；

```java
void preOrder(BinaryTree root){
	if(root !=null){
	System.out.println(root.value);
	preOrder(root.left);
	preOrder(root.right);
	}
}
```





应用：输出一个颗树的所有叶子节点。

```java
void Displayleaf(BinaryTree root){
	if(root !=null){
		if(root.left!=null && root.right!=null)
				System.out.println(root.value);
		Displayleaf(root.left);
		Displayleaf(root.right);
	}
}
```



这样叶子节点是从左到右边输出的，如果想叶子节点从右往左输出，只需要稍微调整即可。

```java
void Displayleaf(BinaryTree root){
	if(root !=null){
		if(root.left!=null && root.right!=null)
				System.out.println(root.value);
		Displayleaf(root.right);
		Displayleaf(root.left);
	}
}
```




//求指定元素在一棵树中的第几层：

```java
int level(BinaryTree root ,int m ){
	int lh,rh;
	if(root==null) return 0;
	if(root.value==m) return 1;


	lh=level(root.left,  m);
	if(lh!=0)   return (lh+1);
	else{
		rh=level(root.right,  m);
		if(rh!=0) return (rh+1);
		else return 0;
		}
}
```





//判断两棵树是否相似：（节点心态相同，节点值不论）

```java
int Like(BinaryTree root1 ,BinaryTree root2 ){
int like1,like2;
if(root1==null && root2==null) return 1;
else if(root1==null || root2==null) return 0;
else{
like1=Like(root1.left,  root2.left );
like2=Like(root1.right,  root2.right);
return (like1 && like2);
}
}
```



//输出一个二叉树的给定节点的所有祖先：

```java
int ancestor(BinaryTree root ,int m)
{
if(root==null)  return 0;
if(root.value==m) return 1;
if(ancestor(root.left,  m)  ||  ancestor(root.right,  m)){
	System.out.println(root.value);
	return 1;
	}
}
```





//先序遍历的非递归算法实现：【java实现】

```java
package com.mytest.mymain;
import java.util.Stack;

class BTree{
	public int value;  //public static int value;  那么最终输出都为8个8
	public BTree left;  
	public BTree right;  
	BTree(int x) { value = x; }  
}
public class PreOrderwithStack {
	public static void main(String[] args) {
		BTree root=new BTree(1);
		BTree Node2=new BTree(2);
		BTree Node3=new BTree(3);
		BTree Node4=new BTree(4);
		BTree Node5=new BTree(5);
		BTree Node6=new BTree(6);
		BTree Node7=new BTree(7);
		BTree Node8=new BTree(8);
		root.left=Node2;
		root.right=Node3;
		
		Node2.left=Node4;
		Node2.right=Node5;
		
		Node3.left=Node6;
		Node3.right=Node7;
		
		Node4.left=Node8;
		
		preorderfun(root);
	}
	public static void preorderfun(BTree root){
		Stack<BTree> stack =new Stack<BTree>();
		BTree bTree=null;
		if(root!=null){
			stack.push(root);
			while(!stack.isEmpty()){
				bTree=stack.pop();
				System.out.print(bTree.value+"   ");
				if(bTree.right!=null){stack.push(bTree.right);}
				if(bTree.left!=null){stack.push(bTree.left);}
			}
		}
		
	}


}
```








//注意入栈顺序：

```java
if(bTree.right!=null){stack.push(bTree.right);}
   if(bTree.left!=null){stack.push(bTree.left);}
```





