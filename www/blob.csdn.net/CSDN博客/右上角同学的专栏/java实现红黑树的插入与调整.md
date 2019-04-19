# java实现红黑树的插入与调整 - 右上角同学的专栏 - CSDN博客
2016年07月27日 14:59:01[右上角徐](https://me.csdn.net/u011032983)阅读数：223
个人分类：[数据结构](https://blog.csdn.net/u011032983/article/category/6328409)
  不得不说，红黑树把小编整得心累。起初看大神July的博客(http://blog.csdn.net/v_JULY_v/article/details/6105630)，虽然解说详细，但小编还是迷迷糊糊，后来又听其他大神建议看维基百科，发现维基文字描述搭配优雅的c代码，的确更容易消化。
       本代码是在维基百科红黑树原理说明的基础上实现的，亲测无误。介于网上的介绍已经非常详细具体，所以代码仅注释仅强调易错处。
       维基百科目前已被屏蔽，内容参考http://www.doc88.com/p-9778237347760.html
传一张拙劣的手稿。前方有障碍，敬请绕行。。。
![](https://img-blog.csdn.net/20160727155636370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
/**
 * @author xhl
 * 红黑树的插入操作
 *	
 */
public class RBtree {
	/**
	 * @param args
	 */
	Node root = null;
	/* 插入新节点（过程类比二叉排序树插入） */
	public void rbInsert(Node node) {
		Node temp = root;
		node.color = Color.RED;
		/*为空树时*/
		if (root == null) {
			root = node;
		} else {/*为非空树时，注意排序树的节点新插入后都是终端节点，所以每次遍历到null，但需要先保留上个节点*/
			Node previous = null;
			while (temp != null) {
				previous = temp;// 每次记住插入地点的终端节点，即插入点的前一个节点
				if (node.value == temp.value) {
					System.out.print("error:存在相等值");
					return;
				} else {
					if (node.value < temp.value)
						temp = temp.left;
					else
						temp = temp.right;
				}
			}
			node.parent = previous;// 比排序树多了parent项
			if (node.value < previous.value) {
				previous.left = node;
			} else
				previous.right = node;
		}
		rb_Insert_Fixup(node);
	}
	void rb_Insert_Fixup(Node node) {
		if (node.parent == null)
			node.color = Color.BLACK;// insert_case 1
		else {
			if (node.parent.color == Color.BLACK)
				return;// insert_case2
			else
				insert_case3(node);
		}
	}
	void insert_case3(Node node) {
		if (node.uncle() != null && node.uncle().color == Color.RED) {
			node.uncle().color = Color.BLACK;
			node.parent.color = Color.BLACK;
			node.grandparent().color = Color.RED;
			rb_Insert_Fixup(node.grandparent());
		} else
			insert_case4(node);
	}
	void insert_case4(Node node) {
		if (node == node.parent.right && node.parent == node.grandparent().left) {
			rotate_left(node.parent);
			node = node.left;
		} else {
			if (node == node.parent.left && node.parent == node.grandparent().right) {
				rotate_right(node.parent);
				node = node.right;
			}
		}
		insert_case5(node);
	}
	void insert_case5(Node node) {
		node.parent.color = Color.BLACK;
		node.grandparent().color = Color.RED;
		if (node == node.parent.left && node.parent == node.grandparent().left) {
			rotate_right(node.grandparent());
		} else {
			rotate_left(node.grandparent());
		}
	}
	/* 左旋转 */
	void rotate_left(Node node) {
		if (node == null || node.right == null) {
			System.out.println("rotate_left failed");
			return;
		}
		node.right = node.right.left;
		if (node.right.left != null) {
			node.right.left.parent = node;
		}
		Node temp = node.right;
		node.right.left = node;
		node.parent = temp;
		// node=temp;//指向新的根节点
	}
	/* 右旋转 */
	void rotate_right(Node node) {
		if (node == null || node.left == null) {
			System.out.println("rotate_right failed");
			return;
		}
		node.left = node.left.right;
		if (node.left.right != null) {
			node.left.right.parent = node;
		}
		Node temp = node.left;
		node.left.right = node;
		node.parent = temp;
		// node=temp;//指向新的根节点
	}
	void printTree() {
		inOrderTraverse(root);
	}
//中序遍历
	void inOrderTraverse(Node node) {
		if (node != null) {
			inOrderTraverse(node.left);
			System.out.println(" 节点：" + node.value + "的颜色为：" + node.color);
			inOrderTraverse(node.right);
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Node node11 = new Node(11);
		Node node2 = new Node(2);
		Node node14 = new Node(14);
		Node node1 = new Node(1);
		Node node7 = new Node(7);
		Node node15 = new Node(15);
		Node node5 = new Node(5);
		Node node8 = new Node(8);
	
		RBtree tree = new RBtree();
		tree.rbInsert(node11);
		tree.rbInsert(node2);
		tree.rbInsert(node14);
		tree.rbInsert(node1);
		tree.rbInsert(node7);
		tree.rbInsert(node15);
		tree.rbInsert(node5);
		tree.rbInsert(node8);
	
		tree.printTree();
	}
}
/*红黑树节点定义*/
class Node {
	Node left = null;
	Node right = null;
	Node parent = null;
	Color color;
	int value;
	public Node(int value) {
		this.value = value;
	}
	Node grandparent() {
		return this.parent.parent;
	}
	Node uncle() {
		if (this.parent == this.grandparent().left)
			return this.grandparent().right;
		else
			return this.grandparent().left;
	}
}
/*枚举类，里面的枚举值都是类静态常量，通过Color c=Color.RED得到一个实例*/
enum Color {
	RED, BLACK
}
```
