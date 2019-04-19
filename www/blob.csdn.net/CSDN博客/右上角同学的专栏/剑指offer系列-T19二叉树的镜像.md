# 剑指offer系列-T19二叉树的镜像 - 右上角同学的专栏 - CSDN博客
2016年07月12日 13:36:12[右上角徐](https://me.csdn.net/u011032983)阅读数：201
       本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
import java.util.LinkedList;
/**
 * @author xhl
 * 二叉树的镜像
 * 题目描述
 * 操作给定的二叉树，将其变换为源二叉树的镜像。
 * 思路：因为每一步都是交换左右子树，并对子树做相同操作，所以可用递归实现，
 * 思路不复杂，不过要考虑到递归结束条件和空树情况，另外，顺便复习了一下层次遍历。
 * 
 *
 */
public class offerT19 {
	/**
	 * @param args
	 */
	public void Mirror(BinaryTreeNode root) {
		BinaryTreeNode root2=MirrorTree(root);
	}
	BinaryTreeNode MirrorTree(BinaryTreeNode root){
		if(root==null){
			return null;
		}
		if(root.left==null&&root.right==null)
			return root;//叶子节点的mirrortree是本身
		if(root.left!=null&&root.right!=null){
			//先交换左右子树，再对交换后的左右子树做递归求mirrortree操作和下面注释掉的代码效果一样
			BinaryTreeNode temp=root.right;
			root.right=root.left;
			root.left=temp;
			root.right=MirrorTree(root.right);
			root.left=MirrorTree(root.left);
			//有左右子树时，先递归求左右子树的mirrortree,再交换左右树
			//BinaryTreeNode right=MirrorTree(root.left);
			//BinaryTreeNode left=MirrorTree(root.right);
			//root.right=right;
			//root.left=left;
			}
		else if(root.left!=null){
			BinaryTreeNode right=MirrorTree(root.left);
			root.right=right;
			root.left=null;
		}
		else{
			BinaryTreeNode left=MirrorTree(root.right);
			root.left=left;
			root.right=null;
		}
		return root;
		
	}
	
	/*按层次打印，即广度搜索顺序输出，便于观察*/
	void print(BinaryTreeNode root){
		LinkedList<BinaryTreeNode> queue=new LinkedList<BinaryTreeNode>();//广度搜索打印，用到队列
		if(root!=null){//打印，入队
			System.out.println(root.val);
			queue.offer(root);
			}
		/*队列不空时，出队一个元素，打印左节点，入队；打印右节点，入队，循环，直至队空，操作到叶子节点时只出队不打印左右节点*/
		while(queue.isEmpty()!=true){
		BinaryTreeNode root2=queue.poll();
		if(root2.left!=null){
			System.out.println(root2.left.val);
			queue.offer(root2.left);
			}
		if(root2.right!=null){
			System.out.println(root2.right.val);
			queue.offer(root2.right);
			}
		}
		}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT19 o=new offerT19();
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
		BinaryTreeNode root=o.MirrorTree(target1);
		o.print(root);
		
	}
}
```
