# 剑指offer系列—T18树的子结构 - 右上角同学的专栏 - CSDN博客
2016年07月12日 00:23:53[右上角徐](https://me.csdn.net/u011032983)阅读数：179
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 * @author xhl
 * 树的子结构
 *	题目描述
 *输入两棵二叉树A，B，判断B是不是A的子结构。
 *（ps：我们约定空树不是任意一个树的子结构）
 *思路：先找到和要比较的子树的根节点相同的节点，再递归比较左右子树
 *
 */
public class offerT18 {
	/**
	 * @param args
	 * root1：目标树，root2：要比较的子树
	 */
	boolean hasSubTree(BinaryTreeNode root1,BinaryTreeNode root2){
		boolean has=false;
		if(root1==null||root2==null)//目标树为空时，肯定不含子树；空树不是任意数的子结构
		return false;
		if(root1.val==root2.val)//根相等则继续用函数isInclude比较左右子树
			has=isInclude(root1,root2);
		if(!has)
			has=hasSubTree(root1.left,root2);//不等，则判断是否目标树的左右树中有该子结构
		if(!has)
			has=hasSubTree(root1.right,root2);
		return has;
		
	}
	boolean isInclude(BinaryTreeNode target,BinaryTreeNode pattern){
		//boolean is=false;
		if(pattern==null)//pattern某一分支比较结束
			return true;
		if(target==null)//target已经到终点，而pattern还没比较完
			return false;
		if(target.val!=pattern.val)//某对应节点不相等
			return false;
		
		return isInclude(target.left,pattern.left)&&isInclude(target.right,pattern.right);
		
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT18 o=new offerT18();
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
		BinaryTreeNode pattern1=new BinaryTreeNode(1);
		BinaryTreeNode pattern2=new BinaryTreeNode(2);
		BinaryTreeNode pattern3=new BinaryTreeNode(3);
		pattern1.left=pattern2;
		pattern1.right=pattern3;
		System.out.print(o.hasSubTree(target1, pattern1));
		
		
	}
}
class BinaryTreeNode{
	int val;
	BinaryTreeNode left=null;
	BinaryTreeNode right=null;
	BinaryTreeNode(int val){
		this.val=val;
	}
}
```
