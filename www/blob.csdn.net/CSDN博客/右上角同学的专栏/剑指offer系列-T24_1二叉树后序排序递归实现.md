# 剑指offer系列-T24_1二叉树后序排序递归实现 - 右上角同学的专栏 - CSDN博客
2016年07月13日 16:48:19[右上角徐](https://me.csdn.net/u011032983)阅读数：253
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
	 * @param args
	 * 二叉树后序排序递归实现
	 * 思路：后序排列顺序，左右根
	 */
	void BTPost(BinaryTreeNode root){
		if(root==null)
			System.out.println("");
		if(root.left==null&&root.right==null){
			System.out.println(root.val);
		}
		else{
			if(root.left!=null)
				BTPost(root.left);
			if(root.right!=null)
				BTPost(root.right);
			System.out.println(root.val);
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT24_1 o=new offerT24_1();
		BinaryTreeNode target1=new BinaryTreeNode(1);
		BinaryTreeNode target2=new BinaryTreeNode(2);
		BinaryTreeNode target3=new BinaryTreeNode(3);
		BinaryTreeNode target4=new BinaryTreeNode(4);
		BinaryTreeNode target5=new BinaryTreeNode(5);
		BinaryTreeNode target6=new BinaryTreeNode(6);
		BinaryTreeNode target7=new BinaryTreeNode(7);
		target1.left=target2;
		target1.right=target3;
		target2.left=target4;
		target2.right=target5;
		target3.left=target6;
		target3.right=target7;
		o.BTPost(target1);
	}
}
```
