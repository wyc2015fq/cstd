# 树的后序遍历（递归和非递归java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月13日 19:17:16[QuJack](https://me.csdn.net/A784586)阅读数：877







** 树的后序遍历（递归和非递归java实现）**

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





后序遍历：左右根的原则遍历访问树的所有结点；

```java
void postOrder(BinaryTree root){
	if(root !=null){	
	postOrder(root.left);	
	postOrder(root.right);
	System.out.println(root.value);
	}
}
```



后序遍历的非递归实现【java实现】



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
		System.out.println();
		postorderfun(root);
	}
	public static void postorderfun(BTree root){
		Stack<BTree> stack =new Stack<BTree>();
		BTree proot;//标记栈顶元素前一个被访问的元素
		int flag;//root的左孩子未被访问；
		if(root!=null){
			do{
				while(root!=null){//将root所有左孩子全部入栈
					stack.push(root);
					root=root.left;
				  }
				
				//执行到此处，栈顶元素没有左孩子或者左子树已经被访问过；
				proot=null;//标记栈顶元素前一个被访问的元素，或者此时为最左下边，该元素前一个被访问的元素肯定为空。
				flag=1;//root的左孩子已经被访问；或者root为null
				
				while(!stack.isEmpty() && flag==1){
					root=stack.peek();       //取到栈顶元素，但是不出栈；
					if(root.right==proot){
						root=stack.pop();
						System.out.print(root.value+"  ");
						proot=root;
					}else{
						root=root.right;
						flag=0;//root左边孩子未被访问；
					}
				}
			}while(!stack.isEmpty());
		}
		
		
	}
```









