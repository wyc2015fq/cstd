# 红黑树ADT学习研究 - 仙路看近行远 - CSDN博客












2019年03月25日 00:16:20[为道为僧_何必区分](https://me.csdn.net/qq_32250495)阅读数：20








一、封装ADT

    此类为本人参考算法导论用java编写，供讨论研究。

```java
package tree;

import java.util.concurrent.LinkedBlockingQueue;

/**
 * 红黑树ADT(Abstract Data Struct)
 * 本类纯属虚构，如有雷同纯属巧合。本类为个人学习研究编写，如有问题可指正。
 * 
 * 
 * 红黑树是avl树的一种，一般avl树使用树高和旋转操作来保持平衡。类似的红黑树使用颜色和旋转来保持平衡。
 * 
 * 红黑树的插入比较简单，在按照搜索树插入一个节点后，将其染成红色，由于可能与父节点形成连续红色，所以需要简单调整。将当前校对的节点称为基准节点。
 * 1. 如果其叔节点为空或者叔节点为红色，则进行简单的变色，就可以将基准节点上移。
 * 2. 如果其叔节点为黑色则可进行一次单旋，或者双旋，相间改变颜色即可
 * 
 * 红黑树的删除比较复杂，首先按照搜索树删除一个节点(此过程本身已经很复杂)。然后将所有情况转换成同一种最终情况，稍微变换颜色即可
 * 如果最终删除的节点是个红节点，则不会破坏性质。相反，则基准节点路径将少一个黑色节点。需要通过旋转操作移过来
 * 1. 如果基准节点是红节点，则直接染黑即可。
 * 2. 如果基准节点是黑节点，则看其兄弟结点。如果兄弟结点是红色，则进行单旋移动一个节点去基准节点路径上并保持原来的性质
 * 3. 如果兄弟结点是黑色。则看兄弟结点的子结点。 分以下几种情况
 * 4. 如果子结点都不存在，或者都为黑色，或者只存在一个且为黑色：直接将兄弟结点染成红色，并将基准节点上移。此时基准节点子树路径黑节点数量一致
 * 5. 对于兄弟结点为右子树的情况(对称同理)。有如下两种情况，两种情况需顺序判断，情况1可以转换为情况2
 * 情况1. 如果兄弟结点的左子节点存在且为红色，则进行一次单旋，变换颜色可达到情况2。
 * 情况2. 然后如果右结点存在且为红色，则进行一次单旋变换颜色即可完成。
 * 
 * 
 * @author Jane
 * @e-mail 752793520@qq.com
 */
public class RedBlackTtree {
	
	// 树根
	private static Node root = null;

	
	/**
	 * 开放API
	 */
	
	
	// 插入值
	public boolean addVal(int val) {
		if(root == null) {
			root = new Node(val);
			root.color=Color.black;
			return true;
		}
		try {
			addValue(root,val);
		} catch(Exception e) {
			System.out.println("an exception occur while insert value: " + e.getStackTrace());
			return false;
		}
		return true;
	}
	
	// 删除值
	public boolean delVal(int val) {
		try {
			if(root == null)
				throw new RuntimeException("tree root node is null");
			this.deleteValue(root, val);
		} catch (Exception e) {
			return false;
		}
		
		return true;
	}
	
	// 打印序列
	public void printTree() {
		this.printSortedArray(root);
		System.out.println();
	}
	
	// 层次打印
	public void printTreeByLayer() {
		if(root == null)
			return;
		printTreeByLayer(root);
	}
	
	public boolean validTree() {
		if(isValidRedBlackTree(root)) {
			System.out.println("正确的红黑树");
			return true;
		}else {
			System.out.println("错误的红黑树");
			return false;
		}
	}
	
	
	/**
	 * 内部实现
	 */
	
	
	/**
	 * 先按照二叉搜索树插入节点，然后从插入节点开始调整使树维护红黑树的性质
	 * @param node 节点
	 * @param val  值
	 */
	private void addValue(Node node, int val) {
		
		if(val<node.val && node.left!=null)
		  addValue(node.left, val);
		else if (val<node.val && node.left==null) {
			Node leaf = new Node(val);
			leaf.parent=node;
			node.left = leaf;
			leaf.color = Color.red;
			fixRedBlack(leaf);
			return;
		}
		
		if(val>=node.val && node.right!=null)
		  addValue(node.right, val);
		else if(val>=node.val && node.right == null) {
			Node leaf = new Node(val);
			leaf.parent=node;
			node.right = leaf;
			leaf.color = Color.red;
			fixRedBlack(leaf);
			return;
		}
		
	}
	
	/**
	 * 调整节点和颜色，使得树保持红黑树性质
	 * @param leaf 叶子节点
	 */
	private void fixRedBlack(Node leaf) {
		// 终止条件：找到根节点，或者父节点是黑色
		while(leaf.parent !=null && leaf.parent.color==Color.red) {
			Node p = leaf.parent;
			Node pp = p.parent;
			
			// 父节点为祖节点左子树
			if(p == pp.left) {
				Node sr = pp.right;  // 获得叔节点
				if(sr == null) {     // 叔节点为空，变化颜色继续循环
					p.color = Color.black;
					pp.color = Color.red;
				} else if(sr.color ==Color.red) { // 叔节点为红色，变化颜色继续循环
					pp.color = Color.red;
					sr.color = Color.black;
					p.color = Color.black;
				} else {					// 叔节点为黑色，做最终调整
					Node ppp=pp.parent;	
					if(leaf == p.left) {	// 直线则单旋
						if(ppp == null) {
							turnRight(p);
							break;
						}
						else if(pp == ppp.left)
							ppp.left = turnRight(p);
						else
							ppp.right = turnRight(p);
						p.color=Color.black;
						pp.color=Color.red;
					} else {				// 非直线则双旋
						if(ppp == null) {
							turnLeftThenRight(p);
							break;
						}
						if( pp ==ppp.left)
							ppp.left = turnLeftThenRight(p);
						else 
							ppp.right = turnLeftThenRight(p);
						leaf.color = Color.black;
						pp.color = Color.red;
					}
					break;
				}
				// 叶子节点上移
				leaf = pp; 
			} else {		// 对称同理：父节点为祖节点右子树
				Node sl =pp.left;
				if(sl == null) {
					pp.color = Color.red;
					p.color = Color.black;
				} else if (sl.color == Color.red) {
					sl.color = Color.black;
					p.color = Color.black;
					pp.color = Color.red;
				} else {
					Node ppp = pp.parent;
					if(leaf == p.left) {
						if(ppp == null) {
							turnRightThenLeft(p);
						} else if(pp == ppp.left) {
							ppp.left = turnRightThenLeft(p);
						} else {
							ppp.right = turnRightThenLeft(p);;
						}
						
						leaf.color = Color.black;
						pp.color = Color.red;
					} else {
						if(ppp == null) {
							turnRight(p);
						} else if (pp ==ppp.left) {
							ppp.left = turnRight(p);
						} else {
							ppp.right = turnRight(p);
						}
						p.color = Color.black;
						pp.color = Color.red;
					}
					break;
				}
				leaf = pp;
			}
		}
		
		// 保持根节点为黑色
		if(leaf.parent == null)
			leaf.color=Color.black;
	}
	
	
	/**
	 * 先按照二叉搜索书删除节点，然后从相关节点开始调整使树保持红黑树性质
	 * @param val
	 */
	private void deleteValue(Node t, int val) {
		if(t == null) {
			return;
		}
		
		// 删除该节点
		if(t.val == val) {
			Node p = t.parent;
			Node leaf;
			Color originColor = t.color;  // 记录被删除节点的颜色
			if(t.left == null && t.right == null) {
				// 叶子节点直接删除
				transplant(t, null);  
				return;
			}
			if(t.left == null && t.right !=null) {
				leaf = t.right;
				transplant(t, t.right);
			} else if(t.left !=null && t.right == null) {
				leaf = t.left;
				transplant(t, t.left);
			} else {
				// 找该节点右子树一个最小结点替换要删除的节点
				Node sub = findMinNode(t.right);
				originColor = sub.color;
				leaf = sub.right;
				sub.parent.left = leaf;
				if(leaf !=null)
					leaf.parent = sub.parent;
				transplant(t, sub);
				sub.left = t.left;
				sub.color = t.color; 
				sub.right = t.right;
				t.right.parent = sub;
				sub.left.parent = sub;
				t = null; // help gc
			}
			
			// 如果leaf结点为null则往后不再有路径，自然也无需调整
			// 如果被删除的是红节点则性质必然继续保持(红结点的父节点必为黑色，删除红节点不影响路径黑节点数量)
			if(leaf != null && originColor != Color.red)
				fixRedBlackWhenDel(leaf);
			
			return;
		}
		
		if(val<t.val)
			deleteValue(t.left, val);
		else
			deleteValue(t.right, val);
	}
	
	/**
	 * 移植替换要删除的节点
	 * @param u 待删除的节点
	 * @param v 被移植的节点
	 */
	private void transplant(Node u, Node v) {
		if(u.parent == null) {
			root = v;
		} else if(u == u.parent.left) {
			u.parent.left=v;
		} else
			u.parent.right = v;
		
		if(v != null) {
			v.parent = u.parent;
		}
	}
	
	private void fixRedBlackWhenDel(Node leaf) {
		// 如果leaf是红节点，只需将其颜色变黑即可保持性质
		// 如果是黑节点，则包含leaf的路径都将少一个黑节点，需要通过旋转移一个黑节点过来，并保持其他路径黑节点数量不变
		while(leaf.color == Color.black && leaf.parent !=null) {
			Node p = leaf.parent;
			if(leaf == p.left) {
				Node sr = p.right;
				if(sr == null) {// 单路径直接往上
					leaf = p;
				}else if(sr.color == Color.red) {
					Node pp = p.parent;
					if(pp == null) {  // 当前父节点是跟节点 
						turnLeft(p.right);
					}else if(p == pp.left) {
						pp.left = turnLeft(p.right);
					} else {
						pp.right = turnLeft(p.right);
					}
					p.color = Color.red;
					sr.color = Color.black;
				} else {
					Node wl = sr.left;
					Node wr = sr.right;
					if((wl==null && wr == null)||(wl==null && wr.color==Color.black)||(wr==null && wl.color == Color.black)||(wl.color==Color.black && wr.color == Color.black)) {
						sr.color = Color.red;
						leaf = p;  // 包含p节点的路径 黑节点数量都少1个，所以将leaf上移
					} else if(wr !=null && wr.color == Color.red) {
						Node pp = p.parent;
						if(pp == null) {
							turnLeft(sr);
						}else if( p == pp.left) {
							pp.left=turnLeft(sr);
						}else {
							pp.right=turnLeft(sr);
						}
						p.color = Color.black;
						sr.color = Color.red;
						wr.color = Color.black;
						break;
					} else if(wl !=null && wl.color == Color.red) {
						p.right=turnRight(sr.left);
						p.right.color = Color.black;
						sr.color = Color.red;
					}
				}
			} else {
				Node sl = p.left;
				Node pp = p.parent;
				if (sl == null)
					leaf = p;
				else if(sl.color == Color.red) {
					if(pp == null) {
						turnRight(p.left);
					}else if(p==pp.left) {
						pp.left = turnRight(p.left);
					} else
						pp.right = turnLeft(p.left);
						sl.color =Color.black;
						p.color = Color.red;
				} else {
					Node wl = sl.left;
					Node wr = sl.right;
					if((wl==null && wr == null)||(wl==null && wr.color==Color.black)||(wr==null && wl.color == Color.black)||(wl.color==Color.black && wr.color == Color.black)) {
						sl.color = Color.red;
						leaf = p;
					} else if(wl != null && wl.color == Color.red) {
						if(pp==null) {
							turnRight(p.left);
						}else if(p == pp.left)
							pp.left = turnRight(sl);
						else
							pp.right = turnRight(sl);
						
						p.color = Color.black;
						sl.color = Color.red;
						wl.color = Color.black;
						break;
					} else if(wr != null && wr.color == Color.red) {
						p.left=turnLeft(sl.right);
						p.left.color= Color.black;
						sl.color = Color.red;
					}
				}
			}
		}
		
		leaf.color = Color.black;
	}
	
	private Node findMinNode(Node node) {
		if(node.left == null)
			return node;
		return findMinNode(node.left);
	}
	
	
	// 右旋
	private Node turnRight(Node t) {
		Node p = t.parent;
		p.left = t.right;
		t.right = p;
		return t;
	}
	
	// 左旋
	private Node turnLeft(Node t) {
		Node p = t.parent;
		p.right = t.left;
		t.left = p;
		return t;
	}
	
	// 先左旋再右旋
	private Node turnLeftThenRight(Node t) {
		Node p = t.parent;
		p.left = turnLeft(t.right);
		return turnRight(p.left);
	}
	
	// 先右旋转再左旋
	private Node turnRightThenLeft(Node t) {
		Node p = t.parent;
		p.right = turnRight(t.left);
		return turnLeft(p.right);
	}
	

	/**
	 * 输出树的值
	 * 
	 */
	
	// 中序遍历  按从小到大的顺序打印
	private void printSortedArray(Node node) {
		if(node ==null)
			return;
		printSortedArray(node.left);
		System.out.print(node.val + " ");
		printSortedArray(node.right);
		
	}
	
	/**
	 * 按层次打印树并输出颜色
	 * @param node
	 */
	private void printTreeByLayer(Node node) {

		int count = 0;
		int layerLen = 0;
		int nextLayerLen = 0;
		LinkedBlockingQueue<Node> queue = new LinkedBlockingQueue<>();
		queue.offer(node);
		layerLen++;
		while(!queue.isEmpty()) {
			Node t = queue.remove();
			count++;
			
			if(t.left != null) {
				queue.offer(t.left);
				nextLayerLen++;
			}
			if(t.right != null) {
				queue.offer(t.right);
				nextLayerLen++;
			}
			
			System.out.print(t.val + "-" + t.color.name());
			if(layerLen == count) {
				System.out.println();
				layerLen = nextLayerLen;
				nextLayerLen = 0;
				count=0;
			} else {
				System.out.print(" ");
			}
			
			
		}
	}
	
	
	/**
	 * 校验红黑树是否正确
	 */
	
	private boolean isValidRedBlackTree(Node root) {
		if(root == null)
			return true;
		
		// 根节点校验
		if(root.color != Color.black) {
			return false;
		}
		
		// 连续红节点校验
		if(!validColor(root, false))
			return false;
		
		// 路径黑节点数量校验
		if(!validBlackCount(root))
			return false;
		// 值顺序校验
		if(!validSortValue(root))
			return false;
		
		// 满足所有必要条件，则充分证明是棵发育良好的红黑树
		return true;
	}
	
	private boolean validBlackCount(Node root) {
		
		if(countBlackNodes(root)<0)
			return false;
		return true;
	}
	
	private int countBlackNodes(Node root) {
		int cur = 0;
		
		if(root.color == Color.black)
			cur++;
		
		int countLeft=0;
		int countRight=0;
		
		if(root.left == null && root.right == null)
			return cur; 
		if(root.left != null)
		 countLeft = countBlackNodes(root.left);
		
		if(root.right != null)
		 countRight = countBlackNodes(root.right);
		
		if(root.left == null && root.right !=null)
			return countRight + cur;
		if(root.left != null && root.right == null)
			return countLeft + cur;
		
		if(countLeft == -1 || countRight == -1 || countLeft != countRight)
			return -1;
		
		return countLeft + cur;
	}
	
	private boolean validSortValue(Node root) {
		boolean left = true;
		boolean right = true;
		if(root.left!=null&& root.left.val<root.val) {
			left = validSortValue(root.left);
		} else if( root.left!=null && root.left.val >= root.val)
			return false;
		
		if(root.right!=null && root.right.val >= root.val)
		    right = validSortValue(root.right);
		else if(root.right!=null && root.right.val < root.val)
			return false;
		
		return left && right;
	}
	
	private boolean validColor(Node root, boolean parentRed) {
		if(root.color == Color.red && parentRed) // 路径连续红节点
			return false;
		else if(root.color == Color.red)
			parentRed = true;
		else if(root.color == Color.black)
			parentRed = false;
		
		boolean left=true;
		boolean right=true;
		if(root.left!=null)
			left=validColor(root.left, parentRed);
		if(root.right!=null)
			right=validColor(root.right, parentRed);
		
		return left && right;
	}
	
	/**
	 * 节点设计
	 * 包含一个带有父节点的引用
	 * @author 75279
	 *
	 */
	class Node{
		Node left;
		Node right;
		Color color;
		Node parent;
		int val;
		
		Node(int val){
			this.val=val;
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

		public Color getColor() {
			return color;
		}

		public void setColor(Color color) {
			this.color = color;
		}

		public Node getParent() {
			return parent;
		}

		public void setParent(Node parent) {
			this.parent = parent;
		}

		public int getVal() {
			return val;
		}

		public void setVal(int val) {
			this.val = val;
		}

	}
	
	// 颜色枚举
	enum Color{
		red(1),
		black(0);
		Color(int val){
			value =val;
		}
		int value;
		public int getColor(){
			return value;
		}
	}
}
```

**二、测试主类:**

```java
import tree.RedBlackTtree;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		RedBlackTtree redBlackTtree = new RedBlackTtree();
		redBlackTtree.addVal(13);
		redBlackTtree.addVal(51);
		redBlackTtree.addVal(1);
		redBlackTtree.validTree();
		redBlackTtree.addVal(33);
		redBlackTtree.addVal(150);
		redBlackTtree.addVal(6);
		redBlackTtree.validTree();
		redBlackTtree.addVal(12);
		redBlackTtree.addVal(15);
		redBlackTtree.addVal(11);
		redBlackTtree.validTree();
		redBlackTtree.addVal(25);
		redBlackTtree.validTree();
		
		redBlackTtree.printTree();
		redBlackTtree.printTreeByLayer();
		
		// 删除一个节点 并校验 并打印
		redBlackTtree.delVal(150);
		redBlackTtree.validTree();
		
		redBlackTtree.delVal(12);
		redBlackTtree.validTree();
		
		redBlackTtree.delVal(33);
		redBlackTtree.validTree();
		
		redBlackTtree.delVal(1);
		redBlackTtree.validTree();
		
		redBlackTtree.delVal(51);
		redBlackTtree.validTree();
		
		redBlackTtree.printTree();
		redBlackTtree.printTreeByLayer();
		
	}

}
```






