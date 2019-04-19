# 剑指offer系列-T27把二元查找树转变成排序的双向链表 - 右上角同学的专栏 - CSDN博客
2016年07月16日 01:27:09[右上角徐](https://me.csdn.net/u011032983)阅读数：177
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
      今晚状态不佳，以至于这道题耗费了很长时间，不过最后终于通过了牛客网全部测试用例。实现代码是在借鉴http://bylijinnan.iteye.com/blog/1343176基础上修改的，该博客代码略微繁琐，后面小编会进一步优化。
```java
/**
 * @author xhl
把二元查找树转变成排序的双向链表
题目：
输入一棵二元查找树，将该二元查找树转换成一个排序的双向链表。
要求不能创建任何新的结点，只调整指针的指向。
10
/ \
6 14
/ \ / \
4 8 12 16
转换成双向链表
4=6=8=10=12=14=1
 *
 */
import java.util.ArrayList;
import java.util.List;
public class offerT27 {
	public static void main(String[] args) {
		offerT27 bn=new offerT27();
		int[] a={10,6,14,4,8,12,16};//这些数据是按二叉查找树的层次遍历存放
		Node head=bn.creatTree(a);
		Node last=bn.toTwoWayLinkedList(head);
		bn.printTwoLinkedList(last);
		
	}
	/*
	 * 思路：中序遍历二叉树，将上次访问的节点记为previous，当前访问的节点记为current；
           对于遍历过程中的每个当前节点，让该节点的左指针指向previous(current->left = previous)，让previous的右指针
           指向当前节点(previous->right = current)，然后将previous更新为current。当中序遍历结束时，二叉搜索树也
           被转化为双链表了。
           
          具体思路可参见：http://hi.baidu.com/gropefor/blog/item/d2144f8ce0325105b31bba11.html
           
         问题：这个previous只能作为类成员才能得到正确的结果，作为局部变量的话，我得不到正解。
              我尝试过这样写： toTwoWayLinkedList(Node node,Node previous),在main函数里面调用时候，用
			toTwoWayLinkedList(head,null)，得不到正确答案
	 */
	private Node previous;
	public Node toTwoWayLinkedList1(Node node){
		if(node==null)
			return null;
		if(node!=null){
		if(node.getLeft()!=null)
			previous=toTwoWayLinkedList1(node.getLeft());
		if(previous!=null){
				previous.setRight(node);
				node.setLeft(previous);
			}
			previous=node;
			if(node.getRight()!=null)
				previous=toTwoWayLinkedList1(node.getRight());
			}
			return previous;//不是最左边的节点
			
	}
	public Node toTwoWayLinkedList(Node node){
		Node previous=toTwoWayLinkedList1(node);
		if(previous!=null){
	        while(previous.getLeft()!=null){
					previous=previous.getLeft();//find the true Head.
				}
	        }
			return previous;
	}
	
	
	
	public void printTwoLinkedList(Node node){
		if(node!=null){
			//after converting to List,head=a[0]=10,but the head is not the actually head of list.
			//the true head is 4.
			while(node!=null){
				System.out.print(node.getData()+" ");
				node=node.getRight();
			}
		}
	}
	public Node creatTree(int[] data){
		List<Node> nodeList=new ArrayList<Node>();
		for(int each:data){
			Node node=new Node(each);
			nodeList.add(node);
		}
		int lastRootIndex=data.length/2-1;
		for(int i=lastRootIndex;i>=0;i--){
			int leftIndex=i*2+1;
			Node root=nodeList.get(i);
			Node left=nodeList.get(leftIndex);
			root.setLeft(left);
			if(leftIndex+1<data.length){
				Node right=nodeList.get(leftIndex+1);
				root.setRight(right);
			}
		}
		Node head=nodeList.get(0);
		return head;
		
	}
	
	
}
class Node{
	private int data;
	private Node left;
	private Node right;
	
	public Node(int i){
		data=i;
	}
	public int getData() {
		return data;
	}
	public void setData(int data) {
		this.data = data;
	}
	public Node getLeft() {
		return left;
	}
	public void setLeft(Node left) {
		this.left = left;
	}
	public Node getRight() {
		return right;
	}
	public void setRight(Node right) {
		this.right = right;
	}
}
```
