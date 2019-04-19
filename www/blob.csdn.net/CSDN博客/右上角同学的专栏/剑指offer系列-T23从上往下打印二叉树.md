# 剑指offer系列-T23从上往下打印二叉树 - 右上角同学的专栏 - CSDN博客
2016年07月13日 15:45:02[右上角徐](https://me.csdn.net/u011032983)阅读数：156
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序
```java
import java.util.ArrayList;
import java.util.LinkedList;
/**
 * @author xhl
 * 从上往下打印二叉树
 * 题目描述
 * 从上往下打印出二叉树的每个节点，同层节点从左至右打印.
 * 思路：层次遍历，已经在博客《剑指offer系列-T19二叉树的镜像》中简单介绍过，
 * 主要是队列的运用，每次先打印再入队，再pull出队头，打印并入队对头的左右节
 * 点，以此类推，直至队空。具体实现不再赘述。
 */
public class offerT23 {
	/**
	 * @param args
	 */
	/*按层次打印，即广度搜索顺序输出，便于观察*/
	ArrayList<Integer>  PrintFromTopToBottom(BinaryTreeNode root){
		ArrayList<Integer> list=new ArrayList<Integer>();
		LinkedList<BinaryTreeNode> queue=new LinkedList<BinaryTreeNode>();//广度搜索打印，用到队列
		if(root!=null){//打印，入队
			list.add(root.val);
			queue.offer(root);
			}
		/*队列不空时，出队一个元素，打印左节点，入队；打印右节点，入队，循环，直至队空，操作到叶子节点时只出队不打印左右节点*/
		while(queue.isEmpty()!=true){
		BinaryTreeNode root2=queue.poll();
		if(root2.left!=null){
			list.add(root2.left.val);
			queue.offer(root2.left);
			}
		if(root2.right!=null){
			list.add(root2.right.val);
			queue.offer(root2.right);
			}
		}
		return list;
		}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT23 o=new offerT23();
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
		ArrayList<Integer> list=new ArrayList<Integer>();
		list=o. PrintFromTopToBottom(target1);
		System.out.println(list);
	}
}
```
提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
            
