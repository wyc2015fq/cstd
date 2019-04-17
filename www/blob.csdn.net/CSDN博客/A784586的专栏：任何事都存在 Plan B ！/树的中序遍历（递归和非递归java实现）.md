# 树的中序遍历（递归和非递归java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月13日 17:08:47[QuJack](https://me.csdn.net/A784586)阅读数：438








**树的中序遍历（递归和非递归实现）**


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



中序遍历的递归实现代码：【访问次序：左根右】



```java

```
void InOrder(BinaryTree root){
	if(root !=null){
	InOrder(root.left);
	System.out.println(root.value);
	InOrder(root.right);
	}
}





中序遍历的非递归算法【java实现】：



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
		System.out.println();
		inorderfun(root);
	}
	
public static void inorderfun(BTree root){
		Stack<BTree> stack =new Stack<BTree>();
		BTree bTree=null;
		if(root!=null){
			bTree=root;
			while(!stack.isEmpty() || bTree!=null){
				while(bTree!=null){//处理所有左结点，进栈
					stack.push(bTree);
					bTree=bTree.left;
				}
				if(!stack.isEmpty()){//执行到这里，栈顶元素没有左孩子或者左子树都被访问过
					bTree=stack.pop();
					System.out.print(bTree.value+"  ");
					bTree=bTree.right;
				}
		}
		
	}
}
```







