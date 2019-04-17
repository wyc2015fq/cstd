# [算法] - Moris遍历 - Snoopy_Dream - CSDN博客





2019年01月27日 15:41:24[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：53








**目录**

[1. 封神的地方](#%E7%89%9B%E9%80%BC%E7%9A%84%E5%9C%B0%E6%96%B9)

[2. 实质](#%E5%AE%9E%E8%B4%A8)

[3. 具体细节：](#%E5%85%B7%E4%BD%93%E7%BB%86%E8%8A%82%EF%BC%9A)

[4. morris遍历时间复杂度的证明](#morris%E9%81%8D%E5%8E%86%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6%E7%9A%84%E8%AF%81%E6%98%8E)

[5. 先序，中序，后序](#%E5%85%88%E5%BA%8F%EF%BC%8C%E4%B8%AD%E5%BA%8F%EF%BC%8C%E5%90%8E%E5%BA%8F)

[6. 代码](#6.%20%E4%BB%A3%E7%A0%81)

Morris遍历 ，一种遍历二叉树的方式

时间复杂度O(N)，额外空间复杂度O(1) 

![](https://img-blog.csdnimg.cn/20190127153928146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 1. 牛逼的地方

**普通遍历，**因为本身树没有往回指的指针，所以需要递归**用递归栈或者自己压栈的方式，遍历**树结构，额外空间复杂度为O(h)，h为二叉树的高度。

这里通过**利用原树中大量空闲指针的方式**，达到节省空间的目的。

### 2. 实质

建立一种机制，对**于没有左子树的节点只到达一次，对于有左子树的节点会到达两次**

### 3. 具体细节：

**当前节点，最右节点，上一节点，下一节点，想到这些的时候记得重新弄个变量！！**

Morris遍历细节 假设来到当前节点cur，开始时cur来到头节点位置

1）如果cur没有左孩子，cur向右移动(cur = cur.right)

2）如果cur有左孩子，找到左子树上最右的（**为空或者指向自己终止**）节点mostRight：     

```cpp
while (mostRight.right != null && mostRight.right != cur) {
                    mostRight = mostRight.right;
                }
```

      a.如果mostRight的右指针指向空，让其指向cur，     然后cur向左移动(cur = cur.left)     

      b.如果mostRight的右指针指向cur，让其指向null，     然后cur向右移动(cur = cur.right)

3）cur为空时遍历停止

### 4. morris遍历时间复杂度的证明

可以看出主要是找到左子树上最右的节点，这里涉及一个while循环，其他的肯定是O（N）的。

而且很容易判别这里的while最多也就是趋近于O（N），所以总体还是O（N）的算法。

看下图，所有左子树的右边界上的点都遍历两次，一次是null，然后指向自己，再左移，一次是指向自己，然后null，再右移。

![](https://img-blog.csdnimg.cn/20190127152954734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 5. 先序，中序，后序

![](https://img-blog.csdnimg.cn/20190127153928146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)**moris序+先序+中序 示意图**

先序、中序可以由morris遍历加工得到。

**先序：**能回到自己的两次的节点，**第一次打印**，回到自己一次的直接打印。

**中序：**能回到自己的两次的节点，**第二次打印**，回到自己一次的直接打印。

**后序：**后序遍历也可由morris遍历加工得到，但是把处理时机放在，能够达到两次的节点并且是第二次到达的时候。

具体就是第二次到的时候，打印左子树的右边界，最后记得打印整颗树的右边界。



![](https://img-blog.csdnimg.cn/20190127152733572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



### 6. 代码

C++版本

```cpp
// Moris遍历

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x):val(x),left(NULL),right(NULL){}// 这里没有分号
};
// 有左孩子的到达了两次！！，没有左孩子的到达一次


void Moris(TreeNode* root){// 第一次出现的时候打印
    if(root==NULL) return root;
    TreeNode* cur=root;// 当前节点
    //TreeNode* left=cur->left;
    TreeNode* MostRight=NULL; // 左孩子的最右边界点
    while(cur!=NULL){
        MostRight =cur->left;//初始化为左孩子，找左孩子的右边界
        if (MostRight!=NULL){
            while( MostRight->right!=NULL && MostRight->right!=cur)
            {
                MostRight = MostRight->right;
            }                                                                                                                                           
            if(MostRight->right==NULL){// 第一次出现
                MostRight->right = MostRight;// 如果为空就指向自己
                cur = cur->left;// 当前节点左移动
            }
            else// 第二次出现
            {
                MostRight->right = NULL;// 如果不为空，就指向空
                cur = cur->right;// 当前节点右移动
            }   
        }
        else//没有左孩子的到达一次
        {
            cur=cur->right;//如果没有左节点，当前节点右移动

        }
    }
}


void MorisPre(TreeNode* root){// 第一次出现的时候打印
    if(root==NULL) return;
    TreeNode* cur=root;// 当前节点
    //TreeNode* left=cur->left;
    TreeNode* MostRight=NULL; // 左孩子的最右边界点
    while(cur!=NULL){
        MostRight =cur->left;//初始化为左孩子，找左孩子的右边界
        if (MostRight!=NULL){
            while( MostRight->right!=NULL && MostRight->right!=cur)
            {
                MostRight = MostRight->right;
            }                                                                                                                                           
            if(MostRight->right==NULL){// 第一次出现
                MostRight->right = MostRight;// 如果为空就指向自己
                cout << cur->val<<" ";
                cur = cur->left;// 当前节点左移动
            }
            else// 第二次出现
            {
                MostRight->right = NULL;// 如果不为空，就指向空
                cur = cur->right;// 当前节点右移动
            }   
        }
        else//没有左孩子的到达一次
        {
            cout << cur->val<<" ";
            cur=cur->right;//如果没有左节点，当前节点右移动
        }
    }
}


void MorisIn(TreeNode* root){// 第二出现的时候打印
    if(root==NULL) return;
    TreeNode* cur=root;// 当前节点
    TreeNode* MostRight=NULL; // 最右边界点
    while(cur!=NULL){
        MostRight = cur->left;
        if (MostRight!=NULL){
            while( MostRight->right!=NULL && MostRight->right!=cur)
            {
                MostRight = MostRight->right;
            }                                                                                                                                           
            if(MostRight->right==NULL){// 第一次出现
                MostRight->right = MostRight;// 如果为空就指向自己        
                cur = cur->left;// 当前节点左移动
            }
            else// 第二次出现
            {
                MostRight->right = NULL;// 如果不为空，就指向空
                cout << cur->val<<" ";
                cur = cur->right;// 当前节点右移动
            }   
        }
        else
        {
            cout << cur->val<<" "; 
            cur=cur->right;//如果没有左节点，当前节点右移动

        }
    }
}



void MorisPos(TreeNode* root){// 第二出现的时候，逆序打印左子树的右边界
    if(root==NULL) return;
    TreeNode* cur=root;// 当前节点
    TreeNode* MostRight=NULL; // 最右边界点
    while(cur!=NULL){
        MostRight = cur->left;
        if (MostRight!=NULL){   
            while( MostRight->right!=NULL || MostRight->right!=left)
            {
                MostRight = MostRight->right;
            }                                                                                                                                           
            if(MostRight->right==NULL){// 第一次出现
                MostRight->right = MostRight;// 如果为空就指向自己        
                cur = cur->left;// 当前节点左移动
            }
            else// 第二次出现
            { 
                MostRight->right = NULL;// 如果不为空，就指向空
                printReverse(cur->left);// 逆序打印该节点左孩子的右边界
                cur = cur->right;// 当前节点右移动
            }   
        }
        else
        {
            cur=cur->right;//如果没有左节点，当前节点右移动
        }
    }
    printReverse(root);// 打印根节点对应的最右边界
}


void printReverse(TreeNode* root)// 逆序打印该节点左孩子的右边界
{
    // 能第二次到的，说明肯定有左节点，因此不需要判断左节点是否为空
    TreeNode* tail = ReverseEdge(root);// 逆序他的右边界
    TreeNode* cur =tail;
    while(cur->right!=nullptr)
    {
        cout << cur->val<<" ";
        cur=cur->right;//来到左子树右边界
    }
    ReverseEdge(tail);// 打印完后还原

TreeNode* ReverseEdge(TreeNode* root)
{
    TreeNode* pre=NULL;
    TreeNode* next=NULL;
    TreeNode* cur =root;
    while(cur!=NULL){
        next = cur->right;
        cur->right = pre;
        pre = cur;
        cur = next;
    }
    return pre;
}
```

```java
package class05;

public class Code01_MorrisTraversal {
	
	public static class Node {
		public int value;
		Node left;
		Node right;

		public Node(int data) {
			this.value = data;
		}
	}
	
	public static void process(Node head) {
		if(head == null) {
			return;
		}
		// 1
		process(head.left);
		// 2
		process(head.right);
		// 3
	}
	
	
	public static void morris(Node head) {
		if (head == null) {
			return;
		}
		Node cur = head;
		Node mostRight = null;
		while (cur != null) {
			mostRight = cur.left; // mostRight是cur左孩子
			if (mostRight != null) {
				while (mostRight.right != null && mostRight.right != cur) {
					mostRight = mostRight.right;
				}
				// mostRight变成了cur左子树上，最右的节点
				if (mostRight.right == null) { // 这是第一次来到cur
					mostRight.right = cur;
					cur = cur.left;
					continue;
				} else { // 这是第二次来到cur
					mostRight.right = null;
				}
			}
			cur = cur.right;
		}
	}
	
	
	
	public static void morrisPre(Node head) {
		if (head == null) {
			return;
		}
		Node cur = head;
		Node mostRight = null;
		while (cur != null) {
			mostRight = cur.left; // mostRight是cur左孩子
			if (mostRight != null) {
				while (mostRight.right != null && mostRight.right != cur) {
					mostRight = mostRight.right;
				}
				// mostRight变成了cur左子树上，最右的节点
				if (mostRight.right == null) { // 这是第一次来到cur
					System.out.println(cur.value);
					mostRight.right = cur;
					cur = cur.left;
					continue;
				} else { // 这是第二次来到cur
					mostRight.right = null;
				}
			} else { // 当前cur，只能来到一次
				System.out.println(cur.value);
			}
			cur = cur.right;
		}
	}
	
	public static void morrisIn(Node head) {
		if (head == null) {
			return;
		}
		Node cur = head;
		Node mostRight = null;
		while (cur != null) {
			mostRight = cur.left; // mostRight是cur左孩子
			if (mostRight != null) {
				while (mostRight.right != null && mostRight.right != cur) {
					mostRight = mostRight.right;
				}
				// mostRight变成了cur左子树上，最右的节点
				if (mostRight.right == null) { // 这是第一次来到cur
					mostRight.right = cur;
					cur = cur.left;
					continue;
				} else { // 这是第二次来到cur
					mostRight.right = null;
				}
			}
			System.out.println(cur.value);
			cur = cur.right;
		}
	}
	



	public static void morrisPos(Node head) {
		if (head == null) {
			return;
		}
		Node cur1 = head;
		Node cur2 = null;
		while (cur1 != null) {
			cur2 = cur1.left;
			if (cur2 != null) {
				while (cur2.right != null && cur2.right != cur1) {
					cur2 = cur2.right;
				}
				if (cur2.right == null) {
					cur2.right = cur1;
					cur1 = cur1.left;
					continue;
				} else {
					cur2.right = null;
					printEdge(cur1.left);
				}
			}
			cur1 = cur1.right;
		}
		printEdge(head);
		System.out.println();
	}

	public static void printEdge(Node head) {
		Node tail = reverseEdge(head);
		Node cur = tail;
		while (cur != null) {
			System.out.print(cur.value + " ");
			cur = cur.right;
		}
		reverseEdge(tail); //记得再还原
	}

	public static Node reverseEdge(Node from) {
		Node pre = null;
		Node next = null;
		while (from != null) {
			next = from.right;
			from.right = pre;
			pre = from;
			from = next;
		}
		return pre;
	}

	// for test -- print tree
	public static void printTree(Node head) {
		System.out.println("Binary Tree:");
		printInOrder(head, 0, "H", 17);
		System.out.println();
	}

	public static void printInOrder(Node head, int height, String to, int len) {
		if (head == null) {
			return;
		}
		printInOrder(head.right, height + 1, "v", len);
		String val = to + head.value + to;
		int lenM = val.length();
		int lenL = (len - lenM) / 2;
		int lenR = len - lenM - lenL;
		val = getSpace(lenL) + val + getSpace(lenR);
		System.out.println(getSpace(height * len) + val);
		printInOrder(head.left, height + 1, "^", len);
	}

	public static String getSpace(int num) {
		String space = " ";
		StringBuffer buf = new StringBuffer("");
		for (int i = 0; i < num; i++) {
			buf.append(space);
		}
		return buf.toString();
	}

	public static void main(String[] args) {
		Node head = new Node(4);
		head.left = new Node(2);
		head.right = new Node(6);
		head.left.left = new Node(1);
		head.left.right = new Node(3);
		head.right.left = new Node(5);
		head.right.right = new Node(7);
		printTree(head);
		morrisIn(head);
		morrisPre(head);
		morrisPos(head);
		printTree(head);

	}

}
```





