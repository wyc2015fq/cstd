# 剑指offer所有的题目总结JAVA



## 零、小结

1、<<      :     左移运算符，num << 1,相当于num乘以2
      \>>      :     右移运算符，num >> 1,相当于num除以2
      \>>>    :     无符号右移，忽略符号位，空位都以0补齐
2、//与1位与得1就是奇数，1只有最后一位是1

## **一、位运算**
### 1、二进制中1的个数
输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
思路：最简单的思路，整数n每次进行无符号右移一位，同1做&运算，可以判断最后以为是否为1。
通常的剑指offer的思路，n&(n-1) 操作相当于把二进制表示中最右边的1变成0。所以只需要看看进行了多少次这样的操作即可。看看什么时候n为0.
```java
public class erjinzhi {
    public int NumberOf1(int n) {
        /*int count = 0;  //自己的思路，主要就是n与2 4 8 16分别与，来判断
    	long temp = 1;
    	for(int i = 1; i <= 32;i++){
    		if((n&temp) > 0)
    			count++;
    		temp = temp * 2;
    	}
        return count;*/
/*    	//简单的思路
    	int res = 0;
    	while (n!=0) {
	    res = res + n&1;
	    n>>>=1;
		}
    	return res;*/
        int count = 0;
        while(n!=0)
        {
            n = n&(n-1);
            count++;
        }
        return count;
    }
}
```
### 2、判断二进制中0的个数
思路：每次右移一位，判断是否是0即可。暂时没有找到别的好思路。

```java
public static int findZero(int n) {
	int count = 0;
	while(n != 0) {
		if((n&1)!=1)
			count++;
		n>>>=1;
	}
    return count;
}
```
### 3、二进制高位连续0的个数
思路：每次与最高位为1的二进制进行&操作。0x80000000的二进制是1000 0000 0000 0000 ...共32位，最高位为1.
参考https://blog.csdn.net/u013190513/article/details/70216730
https://www.cnblogs.com/hongten/p/hongten_java_integer_toBinaryString.html
https://blog.csdn.net/lpjishu/article/details/51323722

```java
public static int numberOfLeadingZeros0(int i){
    if(i == 0)
        return 32;
    int n = 0;
    int mask = 0x80000000;
    int j = i & mask;
    while(j == 0){
        n++;
        i <<= 1;
        j = i & mask;
    }
    return n;
}
```
JDK中源码解决思路.
```java
public static int numberOfLeadingZeros(int i) {
    // HD, Figure 5-6
    if (i == 0)
        return 32;
    int n = 1;
    if (i >>> 16 == 0) { n += 16; i <<= 16; }
    if (i >>> 24 == 0) { n +=  8; i <<=  8; }
    if (i >>> 28 == 0) { n +=  4; i <<=  4; }
    if (i >>> 30 == 0) { n +=  2; i <<=  2; }
    n -= i >>> 31;
    return n;
}
```
## 二、二叉树
### 1、二叉搜索树第k个结点
给定一颗二叉搜索树，请找出其中的第k小的结点。例如， 5 / \ 3 7 /\ /\ 2 4 6 8 中，按结点数值大小顺序第三个结点的值为4。
思路：递归的方式：二叉搜索树的中序遍历就是排序的，所以用中序遍历，每一次中间的时候判断是否等于k即可。
?         非递归的方式：运用栈进行操作。相当于用栈实现了中序遍历，在中间进行了个数的判断
![img](https://img-blog.csdn.net/20180701221656634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
int count = 0;
TreeNode KthNode(TreeNode pRoot, int k)
{
    if(pRoot != null) {
    	TreeNode leftNode = KthNode(pRoot.left, k);
    	if(leftNode != null)
    		return leftNode;
    	count++;
    	if(count == k)
    		return pRoot;
    	TreeNode rightNode = KthNode(pRoot.right, k);
    	if(rightNode != null)
    		return rightNode;
    }
    return null;
}
```
//栈的方式
```java
TreeNode KthNode(TreeNode pRoot, int k)
{
    Stack<TreeNode> stack = new Stack<TreeNode>();
    if(pRoot==null||k==0) return null;
    int t=0;
    while(pRoot!=null ||stack.size()>0){
        while(pRoot!=null){
            stack.push(pRoot);
            pRoot = pRoot.left;
        }
        if(stack.size()>0){
            pRoot= stack.pop();
            t++;
            if(t==k) return pRoot;
            pRoot= pRoot.right;
        }
    }
   return null;   
}
```
### 2.0、从上往下打印二叉树
从上往下打印出二叉树的每个节点，同层节点从左至右打印。
思路：
![img](https://img-blog.csdn.net/20180917102943318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
import java.util.ArrayList;
import java.util.LinkedList;
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
   /**
	 * 算法思路，（剑指offer图片）
	 * 1.根节点放到队列里面，队列不空，就打印队列头，打印这个节点，马上把这个节点的左右子节点放到队列中。
	 * 2.再要访问一个节点，把这个节点的左右放入，此时队头是同层的，对位是打印出来的左右。依次先入先出就可以得到结果。
	 * @param root
	 * @return
	 */
	public ArrayList<Integer> PrintFromTopToBottom(TreeNode root) {
		ArrayList<Integer> layerList = new ArrayList<Integer>();
		if (root == null)
			return layerList;
		LinkedList<TreeNode> queue = new LinkedList<TreeNode>();
		queue.add(root);
		while (!queue.isEmpty()) {
			TreeNode node = queue.poll();
			layerList.add(node.val);
			if (node.left != null)
				queue.addLast(node.left);
			if (node.right != null)
				queue.addLast(node.right);
		}
		return layerList;
	}
}
```
### 2.1、二叉树打印成多行
从上到下按层打印二叉树，同一层结点从左至右输出。每一层输出一行。（注意是一行一行输出）
思路：主要采用左程云的思路，
![img](https://img-blog.csdn.net/20180702172930724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![img](https://img-blog.csdn.net/20180702173054745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
static ArrayList<ArrayList<Integer> > Print(TreeNode pRoot) {
 	ArrayList<ArrayList<Integer>> res = new ArrayList<>();
     if(pRoot == null)
     	return res;
     Queue<TreeNode> queue = new LinkedList<TreeNode>();
     TreeNode last = pRoot;
     TreeNode nlast =null;
     queue.offer(pRoot);
     ArrayList<Integer> tmp = new ArrayList<>();
     while (!queue.isEmpty()) {
		pRoot = queue.poll();
		tmp.add(pRoot.val);//出队列，就把他左右孩子入队列，
		                    //此时，下一层的最右要跟着更新
		if (pRoot.left!=null) {
			queue.offer(pRoot.left);
			nlast = pRoot.left;
		} 
		if (pRoot.right!=null) {
			queue.offer(pRoot.right);
			nlast = pRoot.right;
		}
		//如果到了本层的最右，就把这一层结果放入。注意最后一层时，isempty不成立，
		//最后一层的结果要单独放入。
		if (pRoot == last && !queue.isEmpty()) {
			res.add(new ArrayList<>(tmp));
			last = nlast;
			tmp.clear();
		}
	}
     res.add(new ArrayList<>(tmp));
     return res;
 }
```
### 2.2、按之字形顺序打印二叉树
题目描述

请实现一个函数按照之字形打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右至左的顺序打印，第三行按照从左到右的顺序打印，其他行以此类推。
利用两个栈的辅助空间分别存储奇数偶数层的节点，然后打印输出。或使用链表的辅助空间来实现，利用链表的反向迭实现逆序输出。
```java
import java.util.ArrayList;
import java.util.Stack;
/*
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    //利用两个栈的辅助空间分别存储奇数偶数层的节点，然后打印输出。或使用链表的辅助空间来实现，利用链表的反向迭实现逆序输出。
    public ArrayList<ArrayList<Integer> > Print(TreeNode pRoot) {
        ArrayList<ArrayList<Integer>> res = new ArrayList<>();
        if(pRoot == null)
        	return res;
        Stack<TreeNode> s1 = new Stack<>();
        Stack<TreeNode> s2 = new Stack<>();
        s1.push(pRoot);
        int level = 1;
        while (!s1.empty()||!s2.empty()) {
			if (level %2 != 0) {
				ArrayList<Integer> list = new ArrayList<>();
				while (!s1.empty()) {
					TreeNode node = s1.pop();
					if (node!= null) {
						list.add(node.val);
						s2.push(node.left);//因为偶数层，先右后左，所以要先放左子树，栈
						s2.push(node.right);
					}
				}
				  if (!list.isEmpty()) {
		                res.add(list);
		                level++;
		            }
			}
			else {
				ArrayList<Integer> list = new ArrayList<>();
				while (!s2.empty()) {
					TreeNode node = s2.pop();
					if (node!= null) {
						list.add(node.val);
						s1.push(node.right);
						s1.push(node.left);
					}
				}
				  if (!list.isEmpty()) {
		                res.add(list);
		                level++;
		            }
			}
		}
        return res;
    }
}
```
### 3、数据流中位数
如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么中位数就是所有数值排序之后位于中间的数值。如果从数据流中读出偶数个数值，那么中位数就是所有数值排序之后中间两个数的平均值。
思路：主要是博客的代码，参考了左的部分分析。
创建优先级队列维护大顶堆和小顶堆两个堆，并且小顶堆的值都大于大顶堆的值。比如6，1，3，0，9，8，7，2则较小的部分大根堆是0，1，2，3 较大的部分小根堆是6，7，8，9.
具体思路：
1.本代码为了保证两个堆的尺寸差距最大为1，采用奇数个时候插到大根堆，偶数个插到小根堆。
2.当数据总数为偶数时，新加入的元素，应当进入小根堆（注意不是直接进入小根堆，而是经大根堆筛选后取大根堆中最大元素进入小根堆），要保证小根堆里面所有数都比大根堆的大。
3.当数据为奇数个时，按照相应的调整进入大根堆。
4.如果个数位奇数个，则大根堆堆顶为中位数，否则就是两个堆顶除以2.比如新加入三个，那么第一个在大，第二个在小，第三个可能在大。所以就是大根堆的堆顶。
\* 插入有两种思路： 左采用第一种，本代码采用第二种
\* 1：直接插入大堆中，之后若两堆尺寸之差大于1(也就是2)，则从大堆中弹出堆顶元素并插入到小堆中
\* 若两队之差不大于1，则直接插入大堆中即可。
\* 2：奇数个数插入到大堆中，偶数个数插入到小堆中，
\* 但是 可能会出现当前待插入的数比小堆堆顶元素大，此时需要将元素先插入到小堆，然后将小堆堆顶元素弹出并插入到大堆中
\* 对于偶数时插入小堆的情况，一样的道理。why?
\* 因为要保证最大堆的元素要比最小堆的元素都要小。
```java
import java.util.Comparator;
import java.util.PriorityQueue;
public class Shujuliumedian {
	int count = 0;
	PriorityQueue<Integer> minheap = new PriorityQueue<>();
	PriorityQueue<Integer> maxheap = new PriorityQueue<>(11, new Comparator<Integer>() {
		@Override
		public int compare(Integer o1, Integer o2) {
			// TODO Auto-generated method stub
			return o2.compareTo(o1);//o2大于o1返回1 ，否则返回-1
		}
	});
    public void Insert(Integer num) {
        count++;
        if (count % 2 ==0) {//偶数进入小根堆,这个其实无所谓，定了一个平均分配的规则
        	//保证进入小根堆的元素要比大根堆最大的大，所以如果小调整
        	if (!maxheap.isEmpty() && num < maxheap.peek()) {
				maxheap.offer(num);
				num = maxheap.poll();
			}
			minheap.offer(num);
		}
        else {//奇数进入大根堆
        	if (!minheap.isEmpty() && num > minheap.peek()) {
				minheap.offer(num);
				num = minheap.poll();
			}
        	maxheap.offer(num);
        }
    }
    public Double GetMedian() {
        double median = 0;
        if (count % 2 ==1) {
			median = maxheap.peek();
		}
        else
        	median = (minheap.peek()+maxheap.peek())/2.0;
        return median;
    }
}
```
### 4、二叉树中和为某一值的路径
输入一颗二叉树的跟节点和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的根结点开始往下一直到叶结点所经过的结点形成一条路径。(注意: 在返回值的list中，数组长度大的数组靠前)
思路： 
代码步骤：一个链表记录路径，一个存放这个链表的链表记录最终的结果。
1.首先将根节点放入链表，target减去这个根节点
2.判断是否target同时是叶子节点，如果是就将当前的链表放在结果连表里
3.如果不是，就递归去访问左右子节点。
4.无论是找到没有，都要回退一步、

```java
import java.util.ArrayList;
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
private ArrayList<ArrayList<Integer>> listAll = new ArrayList<ArrayList<Integer>>();  
    private ArrayList<Integer> list =new ArrayList<Integer>();
    private ArrayList<ArrayList<Integer>> resultList = new ArrayList<ArrayList<Integer>>();
    /**
     * 剑指offer思路
     * 代码步骤：一个链表记录路径，一个存放这个链表的链表记录最终的结果。前序遍历去访问。先访问根，在递归在左右子树找。注意回退
     * 1.首先将根节点放入链表，target减去这个根节点
     * 2.判断是否target同时是叶子节点，如果是就将当前的链表放在结果连表里
     * 3.如果不是，就递归去访问左右子节点。
     * 4.无论是找到没有，都要回退一步、
     * @param root
     * @param target
     * @return
     */
    public ArrayList<ArrayList<Integer>> FindPath(TreeNode root,int target) {
        if(root == null)
        	return resultList;
        list.add(root.val);
        target = target - root.val;
        if(target == 0 && root.left == null && root.right == null){
        	resultList.add(new ArrayList<Integer>(list));
        }
        else {
			FindPath(root.left, target);
			FindPath(root.right, target);
		}
        // 在返回父节点之前，在路径上删除该结点
        list.remove(list.size()-1);
        return resultList;
    }
}
```
### 5、重建二叉树
输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
思路：剑指
![img](https://img-blog.csdn.net/20180712221140802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180712221239116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180712221218145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180712221301607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
/**
 * Definition for binary tree
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
import java.util.Arrays;
public class Solution {
	public TreeNode reConstructBinaryTree(int[] pre, int[] in) {
		if (pre == null || in == null) {
			return null;
		}
		if (pre.length == 0 || in.length == 0) {
			return null;
		}
		if (pre.length != in.length) {
			return null;
		}
		TreeNode root = new TreeNode(pre[0]);//第一个
		for (int i = 0; i < in.length; i++) {
			if (pre[0] == in[i]) {
				//pre的0往后数i个是左子树的，copyofrange包含前面的下标，不包含后面的下标
				//in的i往前数i个是左子树的。
				root.left = reConstructBinaryTree(Arrays.copyOfRange(pre, 1, i + 1), Arrays.copyOfRange(in, 0, i));
				//注意in是从i+1开始，因为i是现在的根，i+1开始才是右子树
				root.right = reConstructBinaryTree(Arrays.copyOfRange(pre, i + 1, pre.length),
						Arrays.copyOfRange(in, i + 1, in.length));
			}
		}
		return root;
	}
}
```
### 6、树的子结构
输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）
思路：   //先从根开始再把左作为根，再把右作为根由本函数决定。把一个为根的时候的具体比对过程是第二个函数决定。

```java
    //从根可以认为是一颗树，从左子树开始又可以认为是另外一颗树，从右子树开始又是另外一棵树。
    //本函数就是判断这一整颗树包不包含树2，如果从根开始的不包含，就从左子树作为根节点开始判断，
    //再不包含从右子树作为根节点开始判断。
    //是整体算法递归流程控制。


/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
   //先从根开始再把左作为根，再把右作为根由本函数决定。把一个为根的时候的具体比对过程是第二个函数决定。
    //从根可以认为是一颗树，从左子树开始又可以认为是另外一颗树，从右子树开始又是另外一棵树。
    //本函数就是判断这一整颗树包不包含树2，如果从根开始的不包含，就从左子树作为根节点开始判断，
    //再不包含从右子树作为根节点开始判断。
    //是整体算法递归流程控制。
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        boolean res = false;
        if (root1 != null && root2 != null) {
			if(root1.val == root2.val){
				res = doesTree1haveTree2(root1,root2);
			}
			if(!res)
			{
				res = HasSubtree(root1.left, root2);
			}
			if(!res)
			{
				res = HasSubtree(root1.right, root2);
			}
		}
        return res;
    }
	//本函数，判断从当前的节点 ，开始两个树能不能对应上，是具体的比对过程
    public boolean doesTree1haveTree2(TreeNode root1,TreeNode root2) {
		if(root2 == null)
			return true;
		if(root1 == null)
			return false;
		if(root1.val != root2.val){
			return false;
		}
		//如果根节点可以对应上，那么就去分别比对左子树和右子树是否对应上
		return doesTree1haveTree2(root1.left, root2.left) && doesTree1haveTree2(root1.right, root2.right);
	}
}
```
### 7、二叉树的镜像
操作给定的二叉树，将其变换为源二叉树的镜像。
```html
二叉树的镜像定义：源二叉树 
    	    8
    	   /  \
    	  6   10
    	 / \  / \
    	5  7 9 11
    	镜像二叉树
    	    8
    	   /  \
    	  10   6
    	 / \  / \
    	11 9 7  5
```
```java
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
import java.util.Stack;
public class Solution {
/**
     * 算法步骤
     * 1.节点为空直接返回
     * 2.如果这个节点的左右子树不为空，就交换。
     * 3.递归对这个节点的左子树进行求镜像。对这个节点的右子树求镜像。
     * @param root
     */
    public void Mirror(TreeNode root){
        if (root == null) {
			return;
		}
    	if(root.left != null || root.right != null) {
			TreeNode temp = root.left;
			root.left = root.right;
			root.right = temp;
			Mirror(root.left);
			Mirror(root.right);
		}
    }
        /*public void Mirror(TreeNode root) {
        if (root == null) {
			return;
		}
        Stack<TreeNode> stack = new Stack<TreeNode>();
        stack.push(root);
        while (!stack.isEmpty()) {
        	TreeNode node = stack.pop();
        	if (node.left != null || node.right != null) {
				TreeNode temp = node.left;
				node.left = node.right;
				node.right = temp;
			}
        	if(node.left != null)
        		stack.push(node.left);
        	if(node.right != null)
        		stack.push(node.right);
		}
    }*/
}
```
### 8、二叉搜素树的后序遍历序列 
输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。
```java
public class Solution {
   /**二叉搜索树的性质：
     * 所有左子树的节点小于根节点，所有右子树的节点值大于根节点的值。
     * 算法步骤：
     * 1.后序遍历的最后一个值为root，在前面的数组中找到第一个大于root值的位置。
     * 2.这个位置的前面是root的左子树，右边是右子树。然后左右子树分别进行这个递归操作。
     * 3.其中，如果右边子树中有比root值小的直接返回false
     * @param sequence
     * @return
     */
    public boolean VerifySquenceOfBST(int [] sequence) {
    	if (sequence == null || sequence.length == 0) 
			return false;
    	return IsBST(sequence, 0, sequence.length -1);
    }
    public boolean IsBST(int [] sequence, int start, int end) {
    	if(start >= end) //注意这个条件的添加// 如果对应要处理的数据只有一个或者已经没
            //有数据要处理（start>end）就返回true
    		return true;
    	int index = start;
    	for (; index < end; index++) {//寻找大于root的第一个节点，然后再分左右两部分
			if(sequence[index] > sequence[end])
				break;
		}
        for (int i = index; i < end; i++) {//若右子树有小于根节点的值，直接返回false
			if (sequence[i] < sequence[end]) {
				return false;
			}
		}
        return IsBST(sequence, start, index-1) && IsBST(sequence, index, end-1);
	}
}/*当案例为{4,6,7,5}的时候就可以看到： 
（此时start为0，end为3） 
一开始index处的值为1，左边4的是start为0，index-1为0，下一次递归的start和end是一样的，true！ 
右边，start为1，end-1为2，是{6,7}元素，下一轮递归是： 
————7为root，index的值指向7， 
————所以左边为6，start和index-1都指向6，返回true。 
————右边index指向7，end-1指向6，这时候end > start！如果这部分还不返回true，下面的数组肯定超了    */
```
### 9、二叉搜索树与双向链表
题目描述

输入一棵二叉搜索树，将该二叉搜索树转换成一个排序的双向链表。要求不能创建任何新的结点，只能调整树中结点指针的指向。
```java
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
 /**二叉搜索树的中序遍历就是递增的排序，所以就运用中序遍历方法来做。
     * 算法思想：
     * 中序遍历的步骤，只不过在递归的中间部分不是输出节点值，而是调整指针指向。
     *     10
     *     /\
     *    5 12
     *   /\
     *  4 7
     *  步骤记住就行，第一次执行，到4的时候，head和resulthead都指向这个
     *  指针调整的其中一步：4是head 5是pRootOfTree 然后调整head右指向5，5左指向4，然后5变成head就行了。
     * @param pRootOfTree
     * @return
     */
    TreeNode head = null;
    TreeNode resultHead = null; //保存生成链表的头结点，便于程序返回
    public TreeNode Convert(TreeNode pRootOfTree) {
        ConvertSub(pRootOfTree);
        return resultHead;
    }
    public void ConvertSub(TreeNode pRootOfTree) {
    	if(pRootOfTree == null)
    		return;
    	ConvertSub(pRootOfTree.left);
		if(head == null){
			head = pRootOfTree;
			resultHead = pRootOfTree;
		}
		else {
			head.right = pRootOfTree;
			pRootOfTree.left = head;
			head = pRootOfTree;
		}
		ConvertSub(pRootOfTree.right);		
	}
}
```
### 10、二叉树的深度
题目描述
输入一棵二叉树，求该树的深度。从根结点到叶结点依次经过的结点（含根、叶结点）形成树的一条路径，最长路径的长度为树的深度。
```java
/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
	// 注意最后加1，因为左右子树的深度大的+根节点的深度1 
    public int TreeDepth(TreeNode root) {
        if(root == null)
        	return 0;
        int left = TreeDepth(root.left);
        int right = TreeDepth(root.right);
        return left > right? left +1:right+1;
    }
}
```
### 11、平衡二叉树
输入一棵二叉树，判断该二叉树是否是平衡二叉树
描述：如果某二叉树中任意节点的左右子树的深度相差不超过1，那么它就是一棵平衡二叉树。
```java
public class Solution {
    // 注意使用全局变量  
    boolean isBalance = true;  
    public boolean IsBalanced_Solution(TreeNode root) {  
        lengthOfTree(root);  
        return isBalance;  
    }  
    private int lengthOfTree(TreeNode root) {  
        if (root == null)  
            return 0;  
        int left = lengthOfTree(root.left);  
        int right = lengthOfTree(root.right);  
        if (Math.abs(left - right) > 1)  
            isBalance = false;  
        return Math.max(left, right) + 1;  
    } 
}
```
第二种**Better思路**：从底向上判断，这样可以记录下一层的深度
```java
public class Solution {
  public boolean IsBalanced(TreeNode root) {
        int depth = 0;
        return IsBalanced(root, depth);
    }
    public boolean IsBalanced(TreeNode root, int depth) {
        if (root == null) {
            depth = 0;
            return true;
        }
        int left = 0, right = 0;
        if (IsBalanced(root.left, left) && IsBalanced(root.right, right)) {
            int diff = left - right;
            if (diff <= 1 && diff >= -1) {
                depth = 1 + (left > right ? left : right);
                return true;
            }
        }
        return false;
    }
}
```
### 12、二叉树的下一个节点
题目描述
给定一个二叉树和其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。
```java
/*
public class TreeLinkNode {
    int val;
    TreeLinkNode left = null;
    TreeLinkNode right = null;
    TreeLinkNode next = null;
    TreeLinkNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    /**参考左程云和有详解博客的思路，
     * 主要分三种：
     * 1.如果有右孩子，后继节点就是最左边的
     * 2.如果没有右孩子，判断是否是父节点的左孩子，是的话，返回，不是继续网上找
     * 3.找不到就是null
     * @param pNode
     * @return
     */
    public TreeLinkNode GetNext(TreeLinkNode pNode)
    {
        if(pNode == null)
        	return null;
     // 如果有右子树，则找右子树的最左节点  
        if (pNode.right != null) {
        	pNode = pNode.right;
        	// 如果此时pNode没有左子树，那么它就是下一个结点 ，就是最左边的了
        	//如果有左子树，那就在左子树找最左边的
        	while(pNode.left != null)
        		pNode = pNode.left;
        	return pNode;
		}
        //// 非跟结点，并且没有右子树
        while(pNode.next != null) {
        	// 找到一个结点是该其父亲的左孩子  ,找到就是返回父节点作为后记
        	if (pNode.next.left == pNode) 
				return pNode.next;
        	//找不到这个左孩子的，就继续往上，next其实是parent
			pNode = pNode.next;
        }
        return null;	
    }
}
```
###  13、对称的二叉树
请实现一个函数，用来判断一颗二叉树是不是对称的。注意，如果一个二叉树同此二叉树的镜像是同样的，定义其为对称的。
```java
/*
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    //利用递归进行判断，
    //若左子树的左孩子等于右子树的右孩子且左子树的右孩子等于右子树的左孩子，
    //并且左右子树节点的值相等，则是对称的。
    boolean isSymmetrical(TreeNode pRoot)
    {
        if (pRoot == null) 
			return true;
		return isCommon(pRoot.left,pRoot.right);
    }
    public boolean isCommon(TreeNode leftNode, TreeNode rightNode) {
		if (leftNode == null && rightNode == null)
			return true;
		if (leftNode != null && rightNode != null) {
			return leftNode.val == rightNode.val && 
					isCommon(leftNode.left, rightNode.right) &&
					isCommon(leftNode.right, rightNode.left);
		}
		return false;
	}
}
```
### 14、序列化二叉树
请实现两个函数，分别用来序列化和反序列化二叉树 
 ![img](https://img-blog.csdn.net/20180916142138748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![img](https://img-blog.csdn.net/20180916142203503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180921195704281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180921195728790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180916142417848?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180916142440437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这段代码是按照先序遍历的方法来做的： 
```java
/*
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
*/
import java.util.LinkedList;
import java.util.Queue;
public class Solution {
    //主要运用左程云的编程思想的方式来实现
    String Serialize(TreeNode root) {
        if(root == null)
        	return "#!";
        String res = root.val+"!";
        res = res + Serialize(root.left);
        res = res + Serialize(root.right);
        return res;
   }
    TreeNode Deserialize(String str) {
       String [] values = str.split("!");
       Queue<String> queue = new LinkedList<String>();
       for (int i = 0; i < values.length; i++) {
		queue.offer(values[i]);
	}
       return reconPre(queue);
  }
    TreeNode reconPre(Queue<String> queue) {
    	String value = queue.poll();
    	if(value.equals("#"))
    		return null;
    	TreeNode head = new TreeNode(Integer.valueOf(value));
    	head.left = reconPre(queue);
    	head.right = reconPre(queue);
    	return head;
    }
}
```
##  
## 三、字符串
### 1、正则表达式的匹配
请实现一个函数用来匹配包括'.'和'*'的正则表达式。模式中的字符'.'表示任意一个字符，而'*'表示它前面的字符可以出现任意次（包含0次）。 在本题中，匹配是指字符串的所有字符匹配整个模式。例如，字符串"aaa"与模式"a.a"和"ab*ac*a"匹配，但是与"aa.a"和"ab*a"均不匹配。
思路：参考网上的思路，主要就是分程序中的两种情况来讨论。第二位是不是*
```java
/*
当模式中的第二个字符不是“*”时：
  1、如果字符串第一个字符和模式中的第一个字符相匹配，
  那么字符串和模式都后移一个字符，然后匹配剩余的。 
  2、如果字符串第一个字符和模式中的第一个字符相不匹配，直接返回false。
  而当模式中的第二个字符是“*”时：
  如果字符串第一个字符跟模式第一个字符不匹配，则模式后移2个字符，继续匹配。
  如果字符串第一个字符跟模式第一个字符匹配，可以有3种匹配方式： 
  1、模式后移2字符，相当于x*被忽略； 
  2、字符串后移1字符，模式后移2字符； 相当于x*算一次
  3、字符串后移1字符，模式不变，即继续匹配字符下一位，因为*可以匹配多位，相当于算多次
  这里需要注意的是：Java里，要时刻检验数组是否越界。*/
public class Zhengze {
	public boolean match(char[] str, char[] pattern) {
		if (str == null || pattern == null) {
			return false;
		}
		int strIndex = 0;
		int patternIndex = 0;
		return matchCore(str, strIndex, pattern, patternIndex);
	}
	public boolean matchCore(char[] str, int strIndex, char[] pattern, int patternIndex) {
		// 有效性检验：str到尾，pattern到尾，匹配成功
		if (strIndex == str.length && patternIndex == pattern.length)
			return true;
		// pattern先到尾，匹配失败
		if (strIndex != str.length && patternIndex == pattern.length)
			return false;
		// 模式第2个是*，且字符串第1个跟模式第1个匹配,分3种匹配模式；如不匹配，模式后移2位
		if (patternIndex + 1 < pattern.length && pattern[patternIndex + 1] == '*') {
			if ((strIndex != str.length && pattern[patternIndex] == str[strIndex])
					|| (pattern[patternIndex] == '.' && strIndex != str.length)) {
				return // 模式后移2，视为x*匹配0个字符
				matchCore(str, strIndex, pattern, patternIndex + 2)
						// 视为模式匹配1个字符
						|| matchCore(str, strIndex + 1, pattern, patternIndex + 2)
						// *匹配1个，再匹配str中的下一个
						|| matchCore(str, strIndex + 1, pattern, patternIndex);
			} else {
				return matchCore(str, strIndex, pattern, patternIndex + 2);
			}
		} // 模式第2个不是*，且字符串第1个跟模式第1个匹配，则都后移1位，否则直接返回false
		if ((strIndex != str.length && pattern[patternIndex] == str[strIndex])
				|| (pattern[patternIndex] == '.' && strIndex != str.length)) {
			return matchCore(str, strIndex + 1, pattern, patternIndex + 1);
		}
		return false;
	}
}
```
### 2、表示数值的字符串
请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串"+100","5e2","-123","3.1416"和"-1E-16"都表示数值。 但是"12e","1a3.14","1.2.3","+-5"和"12e+4.3"都不是。
思路:按照一定的规则，如果第一位是+或-，就后移一位。
如果是数字，索引后移，数字表示1.
如果是点，要判断至此点的数量和e的数量是否已经有了，因为java 中e要求后面为整数，如果有了肯定false。索引后移，dotnum增加。
如果是e，判断是否重复e，或者前面没有数字返回false。enum++， 索引++，此时还要判断最后一位是不是e或者+或者-，如果是false。
```java
public class StrexpressNum {
	/*例子：
	 * 110   1a1   1.1.1  2.2  12e 
	 * 
	 * */
	public  static boolean isNumeric(char[] str) {
		if(str == null)
			return false;
		int length = str.length;
		int dotNum = 0;//记录点的数量
		int index = 0;//索引
		int eNum = 0;//记录e的数量
		int num = 0;//记录数字的数量
		if (str[0] == '+' || str[0] == '-') {
			index++;
		}
		while (index < length) {
			if(str[index]>='0' && str[index]<='9') {
				index++;
	            num = 1;
	         // .前面可以没有数字,所以不需要判断num是否为0
			}else if(str[index]=='.') {
				// e后面不能有.,e的个数不能大于1.java科学计数要求aeb，b为整数
				if(dotNum >0 || eNum >0)
					return false;
				dotNum++;
				index++;
			}else if(str[index] == 'e' || str[index] == 'E') {
				// 重复e或者e前面没有数字
				if(eNum > 0 || num ==0)
					return false;
				eNum++;
				index++;
				// 符号不能在最后一位
				if(index < length &&(str[index]=='+'||str[index]=='-'))
					index++;
				// 表示e或者符号在最后一位
				if(index == length)
					return false;
			}else {
				return false;
			}
		}
		return true;
	}
public static void main(String[] args) {
	char [] str = {'1','2','e'};
	System.out.println(isNumeric(str));
}
}
```
或者用正则表达式来匹配：
[+-]? 表示+或者-出现0次或1次。[0-9]{0,}表示0到9出现0次或者更多次。()表示这个分组作为一个整体。 \\.?表示.出现0次或1次。[0-9]{1,}表示0到9出现1次或者多次。()表示一个分组。如果把两个分组去掉进行判断是不准确的。100匹配到[0-9]{1,}出错。
```java
    public boolean isNumeric(char[] str) {
        String res = String.valueOf(str);
		return res.matches("[+-]?[0-9]{0,}(\\.?[0-9]{1,})?([Ee][+-]?[0-9]{1,})?");
    }
```
### 3.0、第一个只出现一次的字符
题目描述
在一个字符串(0<=字符串长度<=10000，全部由字母组成)中找到第一个只出现一次的字符,并返回它的位置, 如果没有则返回 -1（需要区分大小写）.
```java
import java.util.LinkedHashMap;
public class Solution {
public int FirstNotRepeatingChar(String str) {
    	//这个hashmap有序，所以用这个
        LinkedHashMap<Character, Integer> map= new LinkedHashMap<>();
        //遍历字符串，第一次设为1 否则就加
        for (int i = 0; i < str.length(); i++) {
			if (!map.containsKey(str.charAt(i))) {
				map.put(str.charAt(i), 1);
			}
			else
				map.put(str.charAt(i),map.get(str.charAt(i))+1);
		}
        //找出现次数为1的
        for (int i = 0; i < str.length(); i++) {
			if (map.get(str.charAt(i)) == 1) {
				return i;
			}
		}
        return -1;
    }
}
```
### 3.1、字符流中第一个不重复的字符
请实现一个函数用来找出字符流中第一个只出现一次的字符。例如，当从字符流中只读出前两个字符"go"时，第一个只出现一次的字符是"g"。当从该字符流中读出前六个字符“google"时，第一个只出现一次的字符是"l"。如果当前字符流没有存在出现一次的字符，返回#字符
思路：参考两个博客一种用hashmap来做，一种用字符数组来做。
hashmap方法：
```java
    HashMap<Character, Integer> map = new HashMap<>();//记录字符出现次数
    ArrayList<Character> list = new ArrayList<>();//记录当前的所有的字符
    //Insert one char from stringstream
    public void Insert(char ch)
    {
        if(map.containsKey(ch))
        	map.put(ch, map.get(ch)+1);
        else
        	map.put(ch,1);
        list.add(ch);
    }
  //return the first appearence once char in current stringstream
    public char FirstAppearingOnce()
    {
        for(char c:list) {
        	if(map.get(c)==1)
        		return c;
        }
        return '#';
    }
```
字符数组的方法：
char类型和int类型数值在 0-255之内的可以通用
默认初始值是ASCII的0（int）;char类型会自动转换成(int型)ASCII进行算术运算
```java
   char [] chars = new char[256];//ascii字符共128，其他字符非中文认为256个，
                                //为每个字符预留空间。默认每个存的ascii值为0 
   StringBuffer sb = new StringBuffer();//记录当前的所有字符
    //Insert one char from stringstream
    public void Insert(char ch)
    {
        sb.append(ch);
        chars[ch]++;//如果字符是1，那么就是在字符1对应的下标的地方
                    //也就是49的下标处，ascii加1.此时如果输出chars[ch],里面存ascii值
                    //为1，所以是一个不可显示的字符。
    }
  //return the first appearence once char in current stringstream
    public char FirstAppearingOnce()
    {
         char [] str = sb.toString().toCharArray();
         for(char c:str) {
        	 if(chars[c] == 1)//判断这个字符数组中在这个字符下标处值是否为1.
        		 return c;
         }
         return '#';
    }
```
### 4、翻转字符串
牛客最近来了一个新员工Fish，每天早晨总是会拿着一本英文杂志，写些句子在本子上。同事Cat对Fish写的内容颇感兴趣，有一天他向Fish借来翻看，但却读不懂它的意思。例如，“student. a am I”。后来才意识到，这家伙原来把句子单词的顺序翻转了，正确的句子应该是“I am a student.”。Cat对一一的翻转这些单词顺序可不在行，你能帮助他么？
思路：两个思路，一个比较简单的就是用空格切分出来，student.  a am I。然后从后往前添加到stringbuffer里面。
另一个思路就是基本思路剑指offer，先将整个字符串翻转，然后将每个单词翻转。i love you  反转就是  uoy evol i,然后再每个单词进行反转。
```java
public class Fanzhuan {
	//第一种方法，用空格将字符串切分，
	//倒着往stringbuffer里面插入。
	public String ReverseSentence1(String str) {
		if (str == null || str.trim().length() == 0) {
			return str;
		}
		String[] strs =str.split(" ");//str = "i love you"则strs[0]=i strs[1]=love
		StringBuffer sb = new StringBuffer();
		for (int i = strs.length -1; i >= 0; i--) {
			sb.append(strs[i]);
			if (i>0) {//最后一个不添加空格
				sb.append(" ");
			}
		}
		return sb.toString();
	}
	//第二种思路，先将整个字符串反转，再逐个单词反转
	public String ReverseSentence(String str) {
	    if (str == null || str.length() == 0)
	        return str;
	    if (str.trim().length() == 0)
	        return str;
	    StringBuilder sb = new StringBuilder();
	    String re = reverse(str);
	    String[] s = re.split(" ");
	    for (int i = 0; i < s.length - 1; i++) {
	        sb.append(reverse(s[i]) + " ");
	    }
	    sb.append(reverse(s[s.length-1]));
	    return String.valueOf(sb);
	}
	public String reverse(String str) {
	    StringBuilder sb = new StringBuilder();
	    for (int i = str.length() - 1; i >= 0 ; i--) {
	        sb.append(str.charAt(i));
	    }
	    return String.valueOf(sb);
	}
}
```
### 5、左旋转字符串
汇编语言中有一种移位指令叫做循环左移（ROL），现在有个简单的任务，就是用字符串模拟这个指令的运算结果。对于一个给定的字符序列S，请你把其循环左移K位后的序列输出。例如，字符序列S=”abcXYZdef”,要求输出循环左移3位后的结果，即“XYZdefabc”。是不是很简单？OK，搞定它！
思路：前n位反转，后几位反转，最后总的反转
先反转前n位，再反转后几位，变为了cbafedZYX，再整体反转变为XYZdefabc
```java
 public String LeftRotateString(String str,int n) {
        if (str == null || str.trim().length() == 0) {
			return str;
		}
        int len = str.length();
        n = n % len;// 当len=3，n=4，其实相当于左旋转1位，所以需要取余
        char[] charstr = str.toCharArray();
        //先旋转前面的
        reverse(charstr, 0, n-1);
        //再旋转后面的字符串
        reverse(charstr, n, len -1);
        //最后整体反转
        reverse(charstr, 0, len-1);
        return String.valueOf(charstr);
    }
    //实现的是charstrs从i到j的反转，也可以使用上题中stringbuffer的反转方式
	private void reverse(char[] charStrs, int i, int j) { 
		while(i<j) {
			char temp = charStrs[i];
			charStrs[i] =charStrs[j];
			charStrs[j] = temp;
			i++;
			j--;
		}
	}
```
### 5、把字符串转换为整数
将一个字符串转换成一个整数，要求不能使用字符串转换整数的库函数。 数值为0或者字符串不是一个合法的数值则返回0
思路：若为负数，则输出负数，字符0对应48,9对应57，不在范围内则返回false。
```java
public class Strtoint {
	public int StrToInt(String str) {
		if (str == null || str.length() == 0)
			return 0;
		int mark = 0;
		int number = 0;
		char[] chars = str.toCharArray();
		if (chars[0] == '-')
			mark = 1;//第一位如果是-号，则从第二位开始循环
		for (int i = mark; i < chars.length; i++) {
             if(chars[i] == '+')
            	 continue;
             if(chars[i]<48 || chars[i]>57)
            	 return 0;
             number = number * 10+chars[i] - 48;
		}
		return mark==0?number:-number;//最后根据mark标记的正负号来决定数字正负
	}
}
```
### 6、字符串的排列 
题目描述
输入一个字符串,按字典序打印出该字符串中字符的所有排列。例如输入字符串abc,则打印出由字符a,b,c所能排列出来的所有字符串abc,acb,bac,bca,cab和cba。
输入描述:

```html
输入一个字符串,长度不超过9(可能有字符重复),字符只包括大小写字母
```
![img](https://img-blog.csdn.net/20180607204021153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
public class Solution {
//解法来源：牛客网评论。基于回溯的递归实现。
    /**剑指offer的思路解析。
	 * 步骤：
	 * 
	 * @param str
	 * @return
	 */
	public ArrayList<String> Permutation(String str) {
		List <String> res = new ArrayList<String>();
	    if(str != null && str.length() >0){
	    	PermutationHelp(str.toCharArray(),0,res);
	    	Collections.sort(res); //按字典序 输出字符串数组。
	    }
		return (ArrayList)res;
	}
	public void PermutationHelp(char[] chars, int index, List<String> list) {
		if(index == chars.length -1){ //当递归交换到最后一个位置的时候，就看看list有么有这个字符串，没有的话就放进去。
			String val = String.valueOf(chars);
			if (!list.contains(val)) {//如果最后list没有这个string，因为可能交换后有重复的
				list.add(val);
			}
		}
		else {
		    for (int i = index; i < chars.length; i++) { //循环来执行交换操作，先交换，然后固定这个，下一个交换。最后要交换回来不要影响执行
				swap(chars, index, i);
				PermutationHelp(chars, index+1, list);//依次固定一个
				swap(chars, index, i);
			}
		}
	}
	public void swap(char[] chars,int i, int j) {//交换数组中的两个位置中的值
		char temp =chars[i];
		chars[i] = chars[j];
		chars[j] = temp;
	}
}
```
## 四、数组
### 1、数组中重复的数字
在一个长度为n的数组里的所有数字都在0到n-1的范围内。 数组中某些数字是重复的，但不知道有几个数字是重复的。也不知道每个数字重复几次。请找出数组中任意一个重复的数字。 例如，如果输入长度为7的数组{2,3,1,0,2,5,3}，那么对应的输出是第一个重复的数字2
思路：比较好的思路的分析为，数组中的数字为0到n-1的范围内。如果这个数组中没有重复的数字，则对应的i位置的数据也为i。可以重排此数组，

```
现在让我们重排这个数组。从头到尾依次扫描这个数组中的每个数字。当扫描到下标为i的数字时，首先比较这个数字（用m表示）是不是等于i。如果是，接着扫描下一个数字。如果不是，再拿它和第m个数字进行比较。如果它和第m个数字相等，就找到了一个重复的数字（该数字在下标为i和m的位置都出现了）。如果它和第m个数字不相等，就把第i个数字和第m个数字交换，把m放到属于它的位置。接下来再重复这个比较、交换的过程，直到我们发现一个重复的数字。

以数组{2，3，1，0，2，5，3}为例来分析找到重复数字的步骤。数组的第0个数字（从0开始计数，和数组的下标保持一致）是2，与它的下标不相等，于是把它和下标为2的数字1交换。交换之后的数组是{1，3，2，0，2，5，3}。
此时第0个数字是1，仍然与它的下标不相等，继续把它和下标为1的数字3交换，得到数组{3，1，2，0，2，5，3）。接下来继续交换第0个数字3和第3个数字0，得到数组{0，1，2，3，2，5，3}。此时第0数字的数值为0，接着扫描下一个数字。在接下来的几个数字中，下标为1、2、3的三个数字分别为1、2、3，它们的下标和数值都分别相等，因此不需要做任何操作。接下来扫描到下标为4的数字2。由于它的数值与它的下标不相等，再比较它和下标为2的数字。注意到此时数组中下标为2的数字也是2，也就是数字2在下标为2和下标为4的两个位置都出现了，因此找到一个重复的数字。
```

![img](https://img-blog.csdn.net/20180704165101875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
public class chongfushuzi {
    // 使用排序的方式
    public boolean duplicate(int numbers[],int length,int [] duplication) {
        if(numbers == null || numbers.length ==0) {
        	duplication[0] = -1;
        	return false;
        }
        Arrays.sort(numbers);
        for (int i = 0; i < length -1; i++) {//注意这个i的范围可能越界
			if(numbers[i] == numbers[i+1]) {
				duplication[0] = numbers[i];
				return true;
			}
		}
        return false;
    }
    //使用额外空间的方法。
    public boolean duplicate2(int numbers[],int length,int [] duplication) {
        if(numbers == null || numbers.length ==0) {
        	duplication[0] = -1;
        	return false;
        }
        ArrayList<Integer> list = new ArrayList<>();
        for (int i = 0; i < length; i++) {
			if(list.contains(numbers[i])) {
				duplication[0] = numbers[i];
				return true;
			}
			list.add(numbers[i]);
		}
        return false;
    }
    //使用额外空间的方法。
    public boolean duplicate4(int numbers[],int length,int [] duplication) {
    	 if(length < 2||numbers==null){
             return false;
         }
         Set<Integer> ss = new HashSet<Integer>();
         for (int i = 0; i < numbers.length; i++) {
             if (ss.contains(numbers[i])) {
                 duplication[0] = numbers[i];
                 return true;
             } else {
                 ss.add(numbers[i]);
             }
         }
         return false;
     }
    //比较好的解决方式,时间复杂度O(n),空间复杂度O(1)
    //数组中的数字为0到n-1的范围内。
    //如果这个数组中没有重复的数字，则对应的i位置的数据也为i。可以重排此数组
    public boolean duplicate3(int numbers[],int length,int [] duplication) {
    	  if(numbers == null || numbers.length ==0) {
          	duplication[0] = -1;
          	return false;
          }
    	  for (int i = 0; i < length; i++) {
  			if (numbers[i] < 0 || numbers[i] > length - 1) {
  				duplication[0] = -1;
  				return false;
  			}
         }
    	  for (int i = 0; i < length; i++) {
			while(numbers[i] != i) {
				if(numbers[i] == numbers[numbers[i]]) {
					duplication[0] = numbers[i];
					return true;
				}
				else {
					int tmp = numbers[i];
					numbers[i] = numbers[tmp];
					numbers[tmp] = tmp;
				}
			}
		}
    	 return false; 
    }
}
```
### 2、构建乘积数组
给定一个数组A[0,1,...,n-1],请构建一个数组B[0,1,...,n-1],其中B中的元素B[i]=A[0]*A[1]*...*A[i-1]*A[i+1]*...*A[n-1]。不能使用除法。
思路：用矩阵的方式，先计算左下三角，再计算右上三角。根据图来分析即可。
![img](https://img-blog.csdn.net/20180704165540314?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
public class MultiArray {
	// 新建一个新数组B， 对A数组i项左侧自上往下累乘，
	// 对A数组i项右侧自下往上累乘 时间复杂度O(n)
    public int[] multiply(int[] A) {
    	// 将B拆分为A[0] *...* A[i-1]和A[n-1]*...*A[i+1] 两部分
    	if(A == null || A.length ==0)
        	return A;
        int length = A.length;
        int [] B = new int[length];
        B[0] = 1;
     // 先计算左下三角形，此时B[0]只有一个元素，舍为1，
     		// B[0]不包括A[0]
        for (int i = 1; i < length; i++) {
			B[i] = B[i-1]*A[i-1];
		}
        int tmp =1;
        //计算右上三角形
        for (int i = length -1; i >=0; i--) {
			//最终的B[i]是之前乘好的再乘以右边的
        	B[i] *=tmp;
		tmp *= A[i];
		}
        return B;
    }
}
```
复杂度为O(n^2)的解法如下：(有个对比就行)
```java
public int[] multiplyWithFor(int[] A) {
		int len = A.length;
		// 定义一个结果对象
		int[] result = new int[len];
		// 定义一个基本的量
		int rst = 1;
		for (int i = 0; i < len; i++) {
			// 如果相同，就路过继续
			for (int j = 0; j < len; j++) {
				if (i == j) {
					continue;
				}
				// 如果不同，就相乘
				rst *= A[j];
			}
			result[i] = rst;
			rst = 1; // 还原基本的量
		}
		return result;
	}
	// 
```
### 3、二维数组的查找
在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
![img](https://img-blog.csdn.net/20180704174130306?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
思路：
 从右上角开始，若小，向下走，删除一行，若大，向左走，删除一列
```
/*
利用二维数组由上到下，由左到右递增的规律，
那么选取右上角的元素a[row] [col]与target进行比较，
当target小于元素a[row] [col]时，那么target必定在元素a所在行的左边,
即col--；
当target大于元素a[row][col]时，那么target必定在元素a所在列的下边,
即row++；
*
```
```java
    public boolean Find(int target, int [][] array) {
        int row = 0;
        int col = array[0].length -1;
        while(row<array.length && col>= 0) {
        	if (array[row][col]==target) {
				return true;
			}
        	else if (array[row][col]<target) {
				row++;
			}
        	else {
				col--;
			}
        }
        return false;
    }
```
### 4、数组中只出现一次的数字
一个整型数组里除了两个数字之外，其他的数字都出现了两次。请写程序找出这两个只出现一次的数字。
思路：主要参考左神的思路，辅之以剑指offer思路。主要如下：
![img](https://img-blog.csdn.net/2018070422231047?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考的部分思路：当只有一个数出现一次时，我们把数组中所有的数，依次异或运算，最后剩下的就是落单的数，因为成对儿出现的都抵消了。 
 依照这个思路，我们来看两个数（我们假设是AB）出现一次的数组。我们首先还是先异或，剩下的数字肯定是A、B异或的结果，这个结果的二进制中的1，表现的是A和B的不同的位。我们就取第一个1所在的位数，假设是第3位，接着把原数组分成两组，分组标准是第3位是否为1。如此，相同的数肯定在一个组，因为相同数字所有位都相同，而不同的数，肯定不在一组。然后把这两个组按照最开始的思路，依次异或，剩余的两个结果就是这两个只出现一次的数字。
所以，只于k位上是1的异或，就把其中一组的落单的那个异或出来了。此外a^b=d  则a^d=b满足结果的互换。知道异或的结果和其中一个，可以求得另一个。
```java
    public void FindNumsAppearOnce(int [] array,int num1[] , int num2[]) {
        int eO = 0,eOne = 0;
        for(int num:array)
        	eO ^=num;
        int firstOne = eO &(~eO +1);//求得二进制中第一位1，比如101和011得到010
        for(int cur:array)
        	if ((cur&firstOne) !=0) {//把第k位是1的一组找出来进行异或
				eOne ^=cur; 
			}//最终结果就是第k位是1且落单的那个
       num1[0] = eOne;
       num2[0] = eOne^eO;//异或结果的运算规则。
    }
```
### 5、和为S的两个数
输入一个递增排序的数组和一个数字S，在数组中查找两个数，使得他们的和正好是S，如果有多对数字的和等于S，输出两个数的乘积最小的。
思路：![img](https://img-blog.csdn.net/20180706101202353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180706101223726?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
// 数列满足递增，设两个头尾两个指针i和j，*
// 若ai + aj == sum，就是答案（和一定，两个数字相差越远乘积越小）*
// 若ai + aj > sum，aj肯定不是答案之一，j -= 1*
// 若ai + aj < sum，ai肯定不是答案之一，i += 1*
// O(n)*
// 已经排好序，运用数学上的夹*

    public ArrayList<Integer> FindNumbersWithSum(int [] array,int sum) {
        ArrayList<Integer> list = new ArrayList<>();
        if(array == null || array.length <2)
        	return list;
        int low = 0;
        int high = array.length -1;
        while(low < high) {
        	int small = array[low];
        	int big = array[high];
        	if(small + big == sum) {
        		list.add(small);
        		list.add(big);
        		break;
        	}
        	else if (small+big < sum) 
				low++;
        	else 
				high--;
        }
        return list;
    }
```
### 6.和为S的连续正数序列
小明很喜欢数学,有一天他在做数学作业时,要求计算出9~16的和,他马上就写出了正确答案是100。但是他并不满足于此,他在想究竟有多少种连续的正数序列的和为100(至少包括两个数)。没多久,他就得到另一组连续正数和为100的序列:18,19,20,21,22。现在把问题交给你,你能不能也很快的找出所有和为S的连续正数序列? Good Luck!
思路：剑指offer
![img](https://img-blog.csdn.net/20180706101834631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
	/*
	*初始化small=1，big=2;
	*small到big序列和小于sum，big++;大于sum，small++;
	*当small增加到(1+sum)/2是停止
	*/
    public ArrayList<ArrayList<Integer> > FindContinuousSequence(int sum) {
        ArrayList<ArrayList<Integer>> res = new ArrayList<>();
        ArrayList<Integer> list = new ArrayList<>();
        if(sum < 3) //不重复，最少有两个数字
        	return res;
        int small = 1;
        int big = 2;
      //因为是两个数，假设small是这个
        //，big假设也是这个，和为sum+1，所以到此就可以停了，后面肯定更大
        while(small !=(sum+1)/2) { 
        	int cursum = SumOfList(small, big);
        	if(cursum == sum) {
        		for (int i = small; i <= big; i++) {
					list.add(i);
				}
        		res.add(new ArrayList<>(list));
        		list.clear();//清理掉
        		big++;//找到一组，继续big++，找下一组满足要求的。
        	}
        	else if (cursum > sum) {
				small++;
			}
        	else {
				big++;
			}
        }
        return res;
    }
    //计算list内的数据的和
    public int SumOfList(int small, int big) {
		int sum = 0;
		for (int i = small; i <= big; i++) {
			sum +=i;
		}
		return sum;
	}
```
### 7、调整数组顺序使奇数位于偶数前面 
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。
```java
import java.util.ArrayList;
//import java.util.Arrays;
public class Solution {
    public void reOrderArray(int [] array) {
        ArrayList<Integer> jlist = new ArrayList<Integer>();
        ArrayList<Integer> olist = new ArrayList<Integer>();
        for (int i = 0; i < array.length; i++) {
			if (array[i]%2 == 0) 
				olist.add(Integer.valueOf(array[i]));
			else
				jlist.add(Integer.valueOf(array[i]));//valueOf 是将int转换为Integer
                                                     //intValue是反过来
		}
        jlist.addAll(olist);
        int temp = 0;
        for (int i = 0; i < jlist.size(); i++) {  
            //System.out.println(jlist.get(i));  
        	array[temp] = jlist.get(i).intValue();
        	temp++;
        } 
    }
    //前一种方法的执行时间低，但是需要额外的空间，后一种方法执行时间高一点，但是不需要额外的空间
    /*public void reOrderArray(int[] array) {  
        int temp = 0;
        for (int i = 0; i < array.length - 1; i++) {  
            for (int j = 0; j < array.length - 1 - i; j++) {  
                // 前偶后奇数就交换  
                if ((array[j] & 1) == 0 && (array[j + 1] & 1) == 1) {  //与1位与得1就是奇数，1只有最后一位是1
                    temp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = temp;
                }  
            }  
        }  
    } */
}
```
### 8、数组中出现次数超过一半的数字
数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组{1,2,3,2,2,2,5,4,2}。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。如果不存在则输出0。
```java
public class Solution {
    /**自己的空间复杂度的思路，哈希表记录。
     * 此外：博客，基于快排、基于阵地。此处采用基于防守阵地的方式来进行。
     * 左的思路。
     * 一次再数组中删除两个不同的数，最后剩下的数   有可能    是超过一半的。所以要检验一下。 一个数出现次数大于一半，他肯定会被剩下来，但是剩下来的缺不一定满足。
     * 算法步骤：
     * 如果times为0，就把候选设为当前值。
     * 如果下个数和候选一样，times就++。
     * 如果下个数和候选不一样，times就--。相当于对子，同归于尽。因为超过一半的数肯定超过剩下的所有数。所以和这个数对，这个数肯定会剩下来。
     * 但是剩下的数不一定是，比如 1 2 3 剩下3 比如 1 2 1 3 3 3 2 2 也是剩下3.所以要余外的判断，看是否这个数真的超过。
     * @param array
     * @return
     */
    public int MoreThanHalfNum_Solution(int [] array) {
        int cand = 0;
        int times = 0;
        for (int i = 0; i < array.length; i++) {
			if(times == 0){
				cand = array[i];
				times = 1;
			}
			else if (array[i] == cand) {
				times++;
			}
			else {
				times--;
			}
		}
        times = 0;
        for (int i = 0; i < array.length; i++) {
			if(array[i] == cand)
				times++;
		}
        if (times*2 > array.length) {
			return cand;
		}
        else
           return 0;
    }
}
```
### 9、连续子数组的最大和
题目描述
HZ偶尔会拿些专业问题来忽悠那些非计算机专业的同学。今天测试组开完会后,他又发话了:在古老的一维模式识别中,常常需要计算连续子向量的最大和,当向量全为正数的时候,问题很好解决。但是,如果向量中包含负数,是否应该包含某个负数,并期望旁边的正数会弥补它呢？例如:{6,-3,-2,7,-15,1,2,2},连续子向量的最大和为8(从第0个开始,到第3个为止)。给一个数组，返回它的最大连续子序列的和，你会不会被他忽悠住？(子向量的长度至少是1)
/* 算法时间复杂度O（n） 用total记录累计值，maxSum记录和最大 基于思想：对于一个数A，若是A的左边累计数非负，那么加上A能使得值不小于A，认为累计值对 整体和是有贡献的。如果前几项累计值负数，则认为有害于总和，total记录当前值。 此时 若和大于maxSum 则用maxSum记录下来 */
```java
public class Solution {
/**
	 * 不是看当前的值，而是看当前的累计值，如果当前累计为负数，那么加上现在这个数，
	 * 肯定和就小了，所以继续从当前开始累计，如果为正，那就继续加，
	 * 但是要时刻保存下最大值来，因为后面的累计有可能小。
	 * @param array
	 * @return
	 */
	public int FindGreatestSumOfSubArray(int[] array) {  
		if(array.length == 0)
			return 0;
		int total = array[0];//当前的前面的和累计
		int maxsum = array[0];//记录可能的最大值
		for (int i = 1; i < array.length; i++) {
			if(total >= 0)//
				total = total + array[i];
			else
				total = array[i];
			if(total>maxsum)
				maxsum = total;
		}
		return maxsum;
	}
}
```
### 10、把数组排成最小的数 
题目描述
输入一个正整数数组，把数组里所有数字拼接起来排成一个数，打印能拼接出的所有数字中最小的一个。例如输入数组{3，32，321}，则打印出这三个数字能排成的最小数字为321323。
/* 解题思路： * 考虑到大数问题，先将整型数组转换成String数组，然后将String数组排序，最后将排好序的字符串数组拼接出来。关键就是制定排序规则。 * 排序规则如下： * 若ab > ba 则 a > b， * 若ab < ba 则 a < b， * 若ab = ba 则 a = b； * 解释说明： * 比如 "3" < "31"但是 "331" > "313"，所以要将二者拼接起来进行比较 */
```java
import java.util.Arrays;
import java.util.Comparator;
public class Solution {
	    /**
	     * 主要就是规则的制定，将两个数字转换为字符串，防止溢出
	     * 假设两个数字m和n，拼接有mn和nm
	     * 如果mn>nm, 我们打印nm，此时定义n小于m。（此处小于是我们定义的）
	     * 不是m和n的大小关系，
	     * 而是看拼了之后的大小，来决定m和n的大小。
	     * 如果mn<nm 那么就是m小于n。
	     * 
	     * @param numbers
	     * @return
	     */
	    public String PrintMinNumber(int [] numbers) {
             if(numbers == null || numbers.length == 0)
            	 return "";
             int len = numbers.length;
             String[] str = new String[len];
             StringBuffer sb = new StringBuffer();
             //将数字型的放在字符串数组中。
             for (int i = 0; i < len; i++) {
				str[i] = String.valueOf(numbers[i]);
			}
             //根据定义的规则重新堆str进行升序排序
            Arrays.sort(str, new Comparator<String>() {
				@Override
				public int compare(String s1, String s2) {
					// TODO Auto-generated method stub
					String c1 = s1 + s2;
					String c2 = s2 + s1;
					return c1.compareTo(c2);
				}
			});
            //根据规则排好序，将结果依次放入stringbuffer中就行了
            for (int i = 0; i < len; i++) {
				sb.append(str[i]);
			}
            return sb.toString();
	    }
}
```
### 11、数组中的逆序对
题目描述
在数组中的两个数字，如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组,求出这个数组中的逆序对的总数P。并将P对1000000007取模的结果输出。 即输出P%1000000007
输入描述:
题目保证输入的数组中没有的相同的数字
数据范围：
对于%50的数据,size<=10^4
对于%75的数据,size<=10^5
对于%100的数据,size<=2*10^5
示例1

输入1,2,3,4,5,6,7,0

输出7

```java
public class Solution {
    /**主要是剑指offer的思想，用了归并排序，用count来记录数量
     * 所不同的是用的--而不是++
     * 每一次归并之前，记录当前有的个数，
     * @param array
     * @return
     */
	int count = 0;
    public int InversePairs(int [] array) {
    	if(array == null || array.length ==0)
    		return 0;
    	mergeSort(array, 0, array.length -1);
    	return count;
    }
    public void mergeSort(int []array, int start, int end) {
		if(start < end) {
			int mid = (start + end)/2;
			mergeSort(array, start, mid);
			mergeSort(array, mid + 1, end);
			merge(array, start, mid, mid+ 1, end);
		}
	}
    public void merge(int []array,int start1,int end1, int start2, int end2) {
		int i = end1;
		int j = end2;
		int k = end2 - start1 ;
		int [] temp = new int[end2- start1 +1];
		while(i >= start1 && j >=start2) {
			if(array[i] > array[j]) { 
				//假设此时两个归并的是17 19 22 || 16 18 21
				//那么22大于21，所以可以看出对应22
			    //有三个，22 16 22 18 22 21
				temp[k--] = array[i--];
				count = count + j - start2 +1;
				count %= 1000000007;
			}
			else
				temp[k--] = array[j--];
		}
		while(i >= start1)
			temp[k--] = array[i--];
		while(j >= start2)
			temp[k--] = array[j--];
        int m = start1;
        for(int element:temp)
        	array[m++] = element;
	}
}
```
### 12、数字在排序数组中出现的次数
统计一个数字在排序数组中出现的次数。
思路：![img](https://img-blog.csdn.net/20180915165919801?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![img](https://img-blog.csdn.net/2018091517000458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180915170146981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```java
public class Solution {
    /**
     * 博客上的解题思路，也就是剑指offer的思路
     * 首先用递归二分法找到第一个k和最后一个k，然后得到个数
     * @param array
     * @param k
     * @return
     */
    public int GetNumberOfK(int [] array , int k) {
        int num = 0;
        if (array != null && array.length >0) {
			int firstk = getFirstK(array, k,0, array.length-1);
			int lastk = getLastK(array, k,0, array.length-1);
			if (firstk > -1 && lastk > -1) 
				num = lastk -firstk +1;
		}
        return num;
    }
    /*
     * 找到第一个出现的数字的下标
     */
    public int getFirstK(int [] array, int k,int start, int end) {
    	if(start > end)
    		return -1;
		int midindex = (start + end)/2;
		int middata = array[midindex];
		if (middata == k) {
			//判断是不是第一个K，前一个不等于K，就是第一个K
			if(midindex > 0 && array[midindex - 1]!=k||midindex == 0)
				return midindex;
			else
				end = midindex -1;//如果不是第一个k，那么肯定是在前面找，所以end要往前放
		}
		else if (middata > k) {
			end = midindex -1; //二分，如果这个大于k，所以要在前面找
		}
		else
			start = midindex + 1;// 如果小于k，说明要往后找
		return getFirstK(array,k, start, end);
	}
    /*
   * 找到最后一个出现的数字的下标
   */
    public int getLastK(int [] array, int k,int start, int end) {
		if(start > end)
			return -1;
		int midindex = (start + end)/2;
		int middata = array[midindex];
		if(middata == k) {
			 //判断是不是最后一个K，后一个不等于K，就是最后一个K
			if(midindex < array.length-1 && array[midindex + 1]!= k||midindex ==array.length -1)
		            return midindex;
			else
				start = midindex + 1;
		}
		else if (middata > k) {
			end = midindex - 1;
		}
		else 
			start = midindex +1;
		return getLastK(array, k,start, end);
	}
}
```
## 五、发散思维能力

### 1、求1+2+3+...+n
求1+2+3+...+n，要求不能使用乘除法、for、while、if、else、switch、case等关键字及条件判断语句（A?B:C）。
思路：巧用递归，用了短路，使递归结束。
```java
// &&两侧的表达式结果必须为boolean型，
// 所有&&右侧要用一个无关变量a判断是否与result相等，
// 让右侧的表达式返回boolean型。不管返回的是true还是false，
// 我们的目的仅仅是让&&右侧的表达式执行。
// &&连接的表达式，必须要将最终的boolean结果赋给变量，否则编译报错！
//一直执行递归，当n=0时，短路，不执行，开始执行result+n，一直回溯就把n给加上来。

    public int Sum_Solution(int n) {
        int result =0;
        int temp = 0;
        boolean flag = (n>0) && temp == (result = Sum_Solution(n-1));
        result += n;
        return result;
    }
```
### 2、不用加减乘除做加法
写一个函数，求两个整数之和，要求在函数体内不得使用+、-、*、/四则运算符号。
思路：
首先看十进制是如何做的： 5+7=12，三步走
第一步：相加各位的值，不算进位，得到2。
第二步：计算进位值，得到10. 如果这一步的进位值为0，那么第一步得到的值就是最终结果。
第三步：重复上述两步，只是相加的值变成上述两步的得到的结果2和10，得到12。
同样我们可以用三步走的方式计算二进制值相加： 5-101，7-111 第一步：相加各位的值，**不算进位**，得到010，**二进制每位相加就相当于各位做异或操作**，101^111。
第二步：计算**进位值**，得到1010，**相当于各位做与操作得到101，再向左移一位**得到1010，(101&111)<<1。
第三步**重复上述两步**， 各位相加 010^1010=1000，进位值为100=(010&1010)<<1。 
继续重复上述两步：1000^100 = 1100，进位值为0，跳出循环，1100为最终结果。
```java
    public int Add(int num1,int num2) {
		int sum = num1;//保证当num2为0时候返回num1
		int carry = 0;
		while(num2 != 0) {
			sum = num1 ^ num2;
			carry = (num1 & num2) << 1;
			num1 = sum;
			num2 = carry;
		} 
		return sum;
    }
```
使用递归的方式实现：
```java
	public int Add(int num1, int num2) {
		if (num2 == 0)
			return num1;
		return Add(num1 ^ num2, (num1 & num2) << 1);
	}
```
### 3、旋转数组的最小数字
把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 输入一个非减排序的数组的一个旋转，输出旋转数组的最小元素。 例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 NOTE：给出的所有元素都大于0，若数组大小为0，请返回0 
思路：**思路**：利用二分法，找到中间的数，然后和最左边的值进行比较，若大于最左边的数，则最左边从mid开始，若小于最右边值，则最右边从mid开始。若左中右三值相等，则取mid前后值中较小的数。
```java
import java.util.ArrayList;
public class Solution {
    public int minNumberInRotateArray(int [] array) {
        int front = 0;
        int rear = array.length-1;
        int mid = (front + rear)/2;
        while(front < rear){
            if(rear - front == 1){
                break;
            }
            mid = (front + rear)/2;
            if(array[mid] >= array[front]){
                front = mid;
            }
            else{
                rear = mid;
            }
        }
        return array[rear];
    }
}
```
##  
## 六、其他
### 1、整数中1出现的次数（从1到n中的整数中1出现的次数）
求出1~13的整数中1出现的次数,并算出100~1300的整数中1出现的次数？为此他特别数了一下1~13中包含1的数字有1、10、11、12、13因此共出现6次,但是对于后面问题他就没辙了。ACMer希望你们帮帮他,并把问题更加普遍化,可以很快的求出任意非负整数区间中1出现的次数。
思路：主要是根据编程之美博客的思路，左程云的思路类似，但是感觉代码稍微麻烦一点。
编程之美上给出的规律： 
1、如果第i位（自右至左，从1开始标号）上的数字为0（小于x），则第i位可能出现1的次数由更高位决定（若没有高位，视高位为0），等于更高位数字 * 当前位数的权重10^(i-1)。
2、如果第i位上的数字为1(等于x)，则第i位上可能出现1的次数不仅受更高位影响，还受低位影响（若没有低位，视低位为0），等于更高位数字 * 当前位数的权重10^(i-1)+（低位数字+1）。
3、如果第i位上的数字大于1(大于x)，则第i位上可能出现1的次数仅由更高位决定（若没有高位，视高位为0），等于（更高位数字+1） * 当前位数的权重10^(i-1)。
这个思路中，如果不是求1出现的次数，而是求其他1-9任意一个次数，就是比较大于、等于、小于这个数就行了，分别对应上边的三条规则。0的出现次数需要余外计算。
**X的数目** 
这里的 X∈[1,9] ，因为 X=0 不符合下列规律，需要单独计算。
首先要知道以下的规律： 
从 1 至 10，在它们的个位数中，任意的 X 都出现了 1 次。 
从 1 至 100，在它们的十位数中，任意的 X 都出现了 10 次。 
从 1 至 1000，在它们的百位数中，任意的 X 都出现了 100 次。
依此类推，从 1 至 10^ i ，在它们的左数第二位（右数第 i 位）中，任意的 X 都出现了 10^(i-1) 次。
这个规律很容易验证，这里不再多做说明。 
接下来以 n=2593,X=5 为例来解释如何得到数学公式。从 1 至 2593 中，数字 5 总计出现了 813 次，其中有 259 次出现在个位，260 次出现在十位，294 次出现在百位，0 次出现在千位。
> 现在依次分析这些数据， 
> **首先是个位**。从 1 至 2590 中，包含了 259 个 10，因此任意的 X 都出现了 259 次。最后剩余的三个数 2591, 2592 和 2593，因为它们最大的个位数字 3 < X，因此不会包含任何 5。（**也可以这么看，3 < X，则个位上可能出现的X的次数仅由更高位决定，等于更高位数字（259）\*10^(1-1)=259**）。
>
> **然后是十位**。从 1 至 2500 中，包含了 25 个 100，因此任意的 X 都出现了 25×10=250 次。剩下的数字是从 2501 至 2593，它们最大的十位数字 9 > X，因此会包含全部 10 个 5。最后总计 250 + 10 = 260。（**也可以这么看，9>X，则十位上可能出现的X的次数仅由更高位决定，等于更高位数字（25+1）\*10^(2-1)=260**）。
>
> **接下来是百位**。从 1 至 2000 中，包含了 2 个 1000，因此任意的 X 都出现了 2×100=200 次。剩下的数字是从 2001 至 2593，它们最大的百位数字 5 == X，这时情况就略微复杂，它们的百位肯定是包含 5 的，但不会包含全部 100 个。如果把百位是 5 的数字列出来，是从 2500 至 2593，数字的个数与百位和十位数字相关，是 93+1 = 94。最后总计 200 + 94 = 294。（**也可以这么看，5==X，则百位上可能出现X的次数不仅受更高位影响，还受低位影响，等于更高位数字（2）\*10^(3-1)+（93+1）=294**）。
>
> **最后是千位**。现在已经没有更高位，因此直接看最大的千位数字2< X，所以不会包含任何 5。（**也可以这么看，2< X，则千位上可能出现的X的次数仅由更高位决定，等于更高位数字（0）\*10^(4-1)=0**）。 
> 到此为止，已经计算出全部数字 5 的出现次数。
总结一下就是：
求x出现的次数，分别拆分，然后根据编程之美判断这一位与x的大小关系，分情况写出出现的次数。
```java
  public int NumberOf1Between1AndN_Solution(int n) {
        int low,cur,temp,i=1;
        int high = n;//记录高位数
        int total = 0; //总的1的数量
        if(n < 1)
        	return 0;
        while(high!=0) {
        	//记忆2593 此时i=2，依次拆分25 9 3
        	high = n/powerBaseof10(i);//// 获取第i位的高位
        	temp = n%powerBaseof10(i);
        	cur = temp/powerBaseof10(i-1);// 获取第i位
        	low = temp%powerBaseof10(i-1);// 获取第i位的低位
        	if(cur ==1) {
        		total += high * powerBaseof10(i-1) + low +1;
        	}
        	else if (cur > 1) {
				total += (high + 1) * powerBaseof10(i -1);
			}
        	else {
				total += high * powerBaseof10(i-1);
			}
        	i++;
        }
        return total;
    }
    //就是求10^base次方
    public int powerBaseof10(int base) {
    	return (int)Math.pow(10, base);
    }
```
### 2、扑克牌顺子
LL今天心情特别好,因为他去买了一副扑克牌,发现里面居然有2个大王,2个小王(一副牌原本是54张^_^)...他随机从中抽出了5张牌,想测测自己的手气,看看能不能抽到顺子,如果抽到的话,他决定去买体育彩票,嘿嘿！！“红心A,黑桃3,小王,大王,方片5”,“Oh My God!”不是顺子.....LL不高兴了,他想了想,决定大\小 王可以看成任何数字,并且A看作1,J为11,Q为12,K为13。上面的5张牌就可以变成“1,2,3,4,5”(大小王分别看作2和4),“So Lucky!”。LL决定去买体育彩票啦。 现在,要求你使用这幅牌模拟上面的过程,然后告诉我们LL的运气如何。为了方便起见,你可以认为大小王是0。
思路：剑指和博客
![img](https://img-blog.csdn.net/20180706102419460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
思路:用数组记录五张扑克牌，将数组调整为有序的，若0出现的次数>=顺子的差值，即为顺子。
```java
    public boolean isContinuous(int [] numbers) {
        if(numbers == null || numbers.length ==0)
        	return false;
        int zero = 0;//记录0的个数
        int diff = 0;//记录空缺的数
        Arrays.sort(numbers);
        for (int i = 0; i < numbers.length -1; i++) {
			if(numbers[i] == 0) {
				zero++; //找不到非0的就继续下一次循环
				continue;
			}
			if (numbers[i] != numbers[i+1]) {
				diff += numbers[i+1] - numbers[i] -1 ;//4 和 8，中间空缺3
			}
			else
				return false;//说明有对子，肯定不是顺子
		}
        if(diff<= zero)
        	return true;//如果diff小于zero，那么zero放在最后就行，因为任意值
        return false;
    }
```
### 3、孩子们的游戏（圆圈中剩下的数）
每年六一儿童节,牛客都会准备一些小礼物去看望孤儿院的小朋友,今年亦是如此。HF作为牛客的资深元老,自然也准备了一些小游戏。其中,有个游戏是这样的:首先,让小朋友们围成一个大圈。然后,他随机指定一个数m,让编号为0的小朋友开始报数。每次喊到m-1的那个小朋友要出列唱首歌,然后可以在礼品箱中任意的挑选礼物,并且不再回到圈中,从他的下一个小朋友开始,继续0...m-1报数....这样下去....直到剩下最后一个小朋友,可以不用表演,并且拿到牛客名贵的“名侦探柯南”典藏版(名额有限哦!!^_^)。请你试着想下,哪个小朋友会得到这份礼品呢？(注：小朋友的编号是从0到n-1)
思路：使用递推或者是循环链表的方式。
循环链表的方式：
![img](https://img-blog.csdn.net/20180706110132617?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
bt = (bt + m -1)%list.size();解析：
![img](https://img-blog.csdn.net/20180706112115582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
假设有9个人，m=5，那么第一次是去掉4，然后从5开始数，下次刚好是0.
```java
   public int LastRemaining_Solution(int n, int m) {
        LinkedList<Integer> list = new LinkedList<>();
        for (int i = 0; i < n; i++) {
			list.add(i);
		}
        int bt = 0;
        while(list.size() >1) {
        	//// 删除报数为m-1的人（从0开始）
        	//解析看前面
        	bt = (bt + m -1)%list.size();
        	list.remove(bt);
        }
        return list.size()==1?list.get(0):-1;
    }
```
递推公式的：主要剑指offer上有，没有细看。
```java
// 第一种方法使用递推公式
	public int LastRemaining_Solution(int n, int m) {
		if (m < 1 || n < 1)
			return -1;
		int last = 0;
		// i代表有目前有个人
		for (int i = 2; i <= n; i++)
			last = (last + m) % i;
		return last;
	}
```
### 4、替换空格
在一个二维数组中（每个一维数组的长度相同），每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
思路：新定义一个往后追加。
```java
public class Solution {
    public String replaceSpace(StringBuffer str) {
    	StringBuffer newstr = new StringBuffer();
        for(int i = 0;i<str.length();i++)
        {
            if(str.charAt(i) != ' ')
            {
                newstr.append(str.charAt(i));
            }
            else
            {
                newstr.append("%20");
            }
        }
        return newstr.toString();
    }
}
```
### 5、斐波那契数列 
大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。
n<=39
递归简洁，但是效率不高。循环O（N）
![img](https://img-blog.csdn.net/20180915141343154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![img](https://img-blog.csdn.net/20180915141423659?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
public class Solution {
    public int Fibonacci(int n) {
	    	/*int pretwo = 0;
	    	int preone = 1;
	    	int res = 0;
	    	if(n==0)
	    		return 0;
	    	if(n==1)
	    		return 1;
	    	for(int i = 2;i <= n;i++)
	    	{
	    		res = preone + pretwo;
	    		pretwo = preone;
	    		preone = res;
	    	}
	    	return res;*/
        int res = 0;
	        if(n==1 || n==2)
	        	return 1;
	        else if(n == 0)
	        	return 0;
	        else {
		       res = Fibonacci(n-1) + Fibonacci(n-2);		
			}
	        return res;
    }
}
```
### 6、跳台阶
一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。
思路：斐波拉契数序列，初始条件n=1:只能一种方法，n=2:两种 
对于第n个台阶来说，只能从n-1或者n-2的台阶跳上来，所以 
F(n) = F(n-1) + F(n-2)
```java
public class Solution {
    public int JumpFloor(int target) {
        int res = 0;
        if(target == 1)
            return 1;
        else if(target == 2)
            return 2;
        else
            res = JumpFloor(target-1) + JumpFloor(target-2);
        return res;
    }
}
```
###  7、变态跳台阶
一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
思路：
![img](https://img-blog.csdn.net/20170320141710814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWluZV9zb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
public class Solution {
    public int JumpFloorII(int target) {
        int res = 1;
        for(int i =1;i <=target-1;i++){
            res = res * 2;
        }
        return res;
    }
}
```
### 8、矩形覆盖
我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？
![img](https://img-blog.csdn.net/20180915143747616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
public class Solution {
    public int RectCover(int target) {
	        int res = 0;
            if(target == 0)
                return 0;
	        else if(target == 2)
	            return 2;
            else if(target ==1)
                return 1;
	      	else
	            res = RectCover(target - 1) + RectCover(target-2);
	        return res;
         /*int result = 0;  
            if (target > 0) {  
                if (target <= 2)  
                    return target;  
                else  
                    return result = RectCover(target - 1) + RectCover(target - 2);  
            }  
            return result;  */
    }
}
```
###  9、数值的整数次方
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方
```java
public class Solution {
    public double Power(double base, int exponent) {
		//本题注意的是exponent小于0 或者 等于 0 的情况 还有base为0的情况
        double result = 1;
	    if(exponent < 0){
	    	base = 1/base;
	    	exponent = (-1) * exponent;
	    	for (int i = 1; i <= exponent; i++) {
				result = result *base;
			}
	    }
	    else{
		    for (int i = 1; i <= exponent; i++) {
				result = result *base;
			}
	    }
		return result;
  }
}
    /*{
            double res = 0;
    if (exponent == 0) {
        return 1.0;
    }
    if (exponent > 0) {
        res = mutiply(base,exponent);
    }else {
        res = mutiply(1/base,-exponent);
    }
    return res;
}
public double mutiply(double base, int e) {
    double sum = 1;
    for (int i = 0; i < e; i++) {
        sum = sum * base;
    }
    return sum;
}
}*/
/*博客
public class Solution {
	//时间复杂度O(n)
	public double Power(double base, int exponent) {
		int n = Math.abs(exponent);
		if (n == 0)
			return 1;
		if (n == 1)
			return base;
		//以上两个if判断可省。for循环中判断
		double result = 
1.0
;
		for (int i = 0; i < n; i++) {
			result *= base;
		}
		if (exponent < 0) {
			result = 1 / result;
		}
		return result;
	}
}
*/
```
### 10、顺时针打印矩阵
 输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下4 X 4矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10.
思路：左神的
```java
import java.util.ArrayList;
public class Solution {
	/**
	 * 基本思路：
	 * 1.左上角的坐标和右下角的坐标固定了一圈矩阵，先打印这一圈矩阵。其中，对矩阵只有一行或者一列分别打印。
	 * 2.打印完了这一圈，左上角坐标都+1 ，右下角的都减一，到更内一圈。
      *比如：4*4的矩阵，(0,0)--(3,3)然后，(1,1)--(2,2)
	 * 3.当左上角跑到右下角下面就结束了。
     * —---------|
       |         |
       |         |
       |----------
	 * @param matrix
	 * @return
	 */
	public  ArrayList<Integer> printMatrix(int [][] matrix){
		ArrayList<Integer> list = new ArrayList<Integer>();
		int topRow = 0;
		int topCol = 0;
		int downRow = matrix.length - 1;
		int downCol = matrix[0].length - 1;
		while (topRow <= downRow && topCol <= downCol) { //当满足左上角的小于等于右下角就可以循环
			printCircle(list, matrix, topRow++, topCol++, downRow--, downCol--);
		}
		return list;
	}
	public  void printCircle(ArrayList<Integer> list, int [][] matrix, int topRow, int topCol, int downRow, int downCol) {
		if (topRow == downRow) { //子矩阵只有一行的时候
			for (int i = topCol; i <= downCol; i++) {//注意循环开始的条件，是从这一列开始，不是从零
				list.add(matrix[topRow][i]);
			}
		}
		else if (topCol == downCol) {//子矩阵只有一列的时候
			for (int i = topRow; i <= downRow; i++) {//
				list.add(matrix[i][topCol]);
			}
		}
		else { //其他的情况下
			int currentRow = topRow;
			int currentCol = topCol;
			while (currentCol != downCol) {//左到右 本行最后一个不访问，在下个循环里面。如图
				list.add(matrix[topRow][currentCol]);
			    currentCol++;
			}
			while (currentRow != downRow) {//上到下0
				list.add(matrix[currentRow][downCol]);
				currentRow++;
			}
			while (currentCol != topCol) {//右到左
				list.add(matrix[downRow][currentCol]);
				currentCol--;
			}
			while (currentRow != topRow) {//下到上
				list.add(matrix[currentRow][topCol]);
				currentRow--;
			}
		}
	}
}
```
### 11、最小的k个数
题目描述
输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4,。
```java
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Arrays;
public class Solution {
public  ArrayList<Integer> GetLeastNumbers_Solution(int[] input, int k) {
		ArrayList<Integer> list = new ArrayList<>();
		if (input == null || k <= 0 || k > input.length) {
			return list;
		}
		int[] kArray = Arrays.copyOfRange(input, 0, k);
        //1.构建大顶堆
        for(int i=kArray.length / 2 - 1;i>=0;i--){
            //从第一个非叶子节点开始，下次第二个，依次调整构建大根堆
            adjustHeap(kArray,i,kArray.length);
        }
        //2.依次来判断，有小于堆顶的那就替换掉继续调整一个堆
		for (int i = k; i < input.length; i++) {
			if (input[i] < kArray[0]) {
				kArray[0] = input[i];
				adjustHeap(kArray, 0, kArray.length);
			}
		}
        //最后把堆里的元素读出来
		for (int i = kArray.length - 1; i >= 0; i--) {
			list.add(kArray[i]);
		}
		return list;
	}
    public static void adjustHeap(int []arr,int i,int length){ //从i节点开始调整，
        int temp = arr[i];//先取出当前元素i
        for(int k = i*2+1;k<length;k = k*2+1){//从i结点的左子结点开始，也就是2i+1处开始
            if(k+1<length && arr[k]<arr[k+1]){//如果左子结点小于右子结点，k指向右子结点
                k++;
            }
            if(arr[k] >temp){//如果子节点大于父节点，将子节点值赋给父节点（不用进行交换）
                arr[i] = arr[k];
                i = k;
            }else{
                break;
            }
        }
        arr[i] = temp;//将temp值放到最终的位置
    }
        /**自己的思路：先鲁棒性判断是否k超了长度或者k为0
     * 基本步骤就是：
     * 1.先把k个数放到list里面，
     * 2.把list排序，从k后面一个开始遍历，如果比list里面最大的值小，就替换掉。重新排序一次。循环执行
     * @param input
     * @param k
     * @return
     */
    /*public  ArrayList<Integer> GetLeastNumbers_Solution(int [] input, int k) {
    	List<Integer> list = new ArrayList<Integer>();
    	if (k>input.length || k==0) {
    		return (ArrayList)list; 
		}
    	for (int i = 0; i < k; i++) {
			list.add(input[i]);
		}
    	for (int i = k; i < input.length; i++) {
    		Collections.sort(list);
			if(input[i] < list.get(k-1)){
				list.set(k-1, input[i]);
			}
		}
    	return (ArrayList)list;
    }
    //public static void main(String[] args) {
	//	int [] input = {4,5,1,6,2,7,3,8};
	//	System.out.println(GetLeastNumbers_Solution(input,4).toString());
	//}
    //}
    */
}
```
### 12、丑数
题目描述
把只包含质因子2、3和5的数称作丑数（Ugly Number）。例如6、8都是丑数，但14不是，因为它包含质因子7。 习惯上我们把1当做是第一个丑数。求按从小到大的顺序的第N个丑数。
![img](https://img-blog.csdn.net/20180915163206535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![img](https://img-blog.csdn.net/20180915163243618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
public class Solution {
  /**
     * 剑指offer的思路：
     * 取一个数组来放丑数，下一个丑数必定是这个数组里面的数乘以2或者3
     * 或者5，这三种中最小的那一个。依次，判断下去
     * @param index
     * @return
     */
    public int GetUglyNumber_Solution(int index) {
    	if(index <= 0)
    		return 0;
    	int [] res = new int[index];
    	res[0] = 1;//先把1放入
    	int m2 = 0;//控制乘以2的位置，假设得到一个丑数是乘以2得到的，
    	            //那么下一次就是数组中的下一个丑数可能达到。
    	int m3 = 0;
    	int m5 = 0;
    	for (int i = 1; i < index; i++) {
			int min = Math.min(res[m2]*2, Math.min(res[m3]*3, res[m5]*5));
			res[i] = min;//最小的那个作为当前的丑数
			//判断是由谁乘以得到的
			if(res[m2] *2 == min)//假设res[1]是乘以2得到的丑数，那么下一次就要判断
				              //是否是res[2]乘以2可能得到丑数，所以就要++
				m2++;
			if(res[m3]*3 == min)
				m3++;
			if(res[m5]*5 == min)
				m5++;
		}
        return res[index - 1];
    }
}
```
### 七、栈和队列
### 1、滑动窗口的最大值
给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值。例如，如果输入数组{2,3,4,2,6,2,5,1}及滑动窗口的大小3，那么一共存在6个滑动窗口，他们的最大值分别为{4,4,6,6,6,5}； 针对数组{2,3,4,2,6,2,5,1}的滑动窗口有以下6个： {[2,3,4],2,6,2,5,1}， {2,[3,4,2],6,2,5,1}， {2,3,[4,2,6],2,5,1}， {2,3,4,[2,6,2],5,1}， {2,3,4,2,[6,2,5],1}， {2,3,4,2,6,[2,5,1]}。
思路：左程云的代码和思路，博客的代码其实可以用左的思路来说明。
![img](https://img-blog.csdn.net/20180706192018308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180706192035173?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![img](https://img-blog.csdn.net/20180706192050192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
	public static ArrayList<Integer> maxInWindows(int[] num, int size) {
		ArrayList<Integer> maxWindows = new ArrayList<>();
		if (num == null || size == 0 || num.length == 0 || num.length < size)
			return maxWindows;
		Deque<Integer> dq = new LinkedList<>();
		for (int i = 0; i < size; i++) {
```
```java
   // 如果已有数字小于待存入的数据，      
// 如果已有数字小于待存入的数据，      
          // 这些数字已经不可能是滑动窗口的最大值        
// 这些数字已经不可能是滑动窗口的最大值        
        // 因此它们将会依次地从队尾删除
			while (!dq.isEmpty() && num[i] > num[dq.getLast()])
				dq.removeLast();
			dq.addLast(i);
		}
		//System.out.println(dq);
		for (int i = size; i < num.length; i++) {
			maxWindows.add(num[dq.getFirst()]);
			while (!dq.isEmpty() && num[i] >= num[dq.getLast()])
				dq.removeLast();
			if (!dq.isEmpty() && dq.getFirst() <= i - size)
				dq.removeFirst();
			dq.addLast(i);
			System.out.println(i + "--" + dq);
		}
		maxWindows.add(num[dq.getFirst()]);
		return maxWindows;
	}
// 因此它们将会依次地从队尾删除
			while (!dq.isEmpty() && num[i] > num[dq.getLast()])
				dq.removeLast();
			dq.addLast(i);
		}
		//System.out.println(dq);
		for (int i = size; i < num.length; i++) {
			maxWindows.add(num[dq.getFirst()]);
			while (!dq.isEmpty() && num[i] >= num[dq.getLast()])
				dq.removeLast();
			if (!dq.isEmpty() && dq.getFirst() <= i - size)
				dq.removeFirst();
			dq.addLast(i);
			System.out.println(i + "--" + dq);
		}
		maxWindows.add(num[dq.getFirst()]);
		return maxWindows;
	}
```
左程云的代码：
思路：其实当有新的数来的时候，无论这个数是大于队尾还是小于队尾都要保存。只不过如果新来的大于队尾，说明队尾的数永远不会是大的数了，因为来了更大的，所以直接弹出就好了。如果新来的数小于队尾，说明队中的仍是大值，但是这个大的数值可能会过期，所以新来的小数仍然要保存。如下两个例子
a. 5 4 3 2 窗口是3，所以[5 4 3] 2第一个是5，如果新数4 和 3 不保存，当第二个窗口的时候，[4 3 2]就把4没有了
b.5 6 7 2窗口是3，要依次队尾中添加，最后剩7，前面的小，去掉就好了。
```java
import java.util.ArrayList;
import java.util.LinkedList;
public class HuadongWindows {
	public static void main(String[] args) {
		int num[] = { 2, 3, 4, 2, 6, 2, 5, 1 };
		System.out.println(maxInWindows(num, 3));
	}
	//因为函数形式是这样的，其实因为返回的窗口最大值的数组一共就是n-w+1,不需要arraylist
	public static  ArrayList<Integer> maxInWindows(int [] num, int size){
		ArrayList<Integer> res = new ArrayList<>();
		if(num == null || size <=0 || size > num.length)
			return res;
		LinkedList<Integer> qmax = new LinkedList<>();//记录窗口
		for (int i = 0; i < num.length; i++) {
			//如果新值大于队尾的，之前的那个队尾永远不是最大了，就直接弹出来就好了
			while(!qmax.isEmpty() && num[qmax.peekLast()]<=num[i])
				qmax.pollLast();
			qmax.addLast(i);
			if(qmax.peekFirst() == i -size)//此时，下标已经过期，说明此时的窗口其实
				                           //没有包含这个下标了
				qmax.pollFirst(); 
			if(i >= size-1)//保证一开始的不存入，假设3 2 1，
				                        //只有下标大于窗口时候才判断加入此时的对头
				res.add(num[qmax.peekFirst()]);
		}
		return res;
	}
}
```
### 2、用 两个栈实现队列
题目描述：用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。
思路：入栈给stack1，出栈时，若stack2不为空，则出栈，若为空，把stack1的内容全都放入stack2，然后再出栈
```java
import java.util.Stack;
public class Solution {
    Stack<Integer> stack1 = new Stack<Integer>();
    Stack<Integer> stack2 = new Stack<Integer>();
    public void push(int node) {
        stack1.push(node);
    }
    public int pop() {
        if(stack2.isEmpty()){
            while(!stack1.isEmpty()){
                stack2.push(stack1.pop());
            }
        }
        return stack2.pop();
    }
}
```
###  3、包含min函数的栈
定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。
思路：
```java
import java.util.Stack;
public class Solution {
 /* 
* 思路：用一个栈stack保存数据，用另外一个栈min保存依次入栈最小的数  
* 比如，stack中依次入栈，5, 4, 3, 8, 10,11,12,1  
* 则min依次入栈，5, 4, 3, 3, 3, 3, 3, 1 
* 每次入栈的时候，如果入栈的元素比min中的栈顶元素小或等于则入栈，否则入stack的栈顶元素。  
* 保持stack中和min中保持相同个数的元素 ，同时保持min的栈顶是此时原栈的最小值。
*/
    Stack<Integer> stackData = new Stack<>(); //声明时候的异同
	Stack<Integer> stackMin = new Stack<Integer>();
    public void push(int node) {
        stackData.push(node);
     // 如果min为空或者node比min栈中的元素小，则入min栈  
        if(stackMin.size() == 0 || stackMin.peek() > node)
        	stackMin.push(node);
        else // 否则把min栈中的顶部元素重复入栈
			stackMin.push(stackMin.peek());
    }
    public void pop() {//因为时刻保持两个栈的高度相同，所以两个都pop，时刻保持min的栈顶是原栈的最小值。
        //如果返回应该是返回原栈的。
        if(!stackData.isEmpty()){
        	stackData.pop();
        	stackMin.pop();
        }
    }
    public int top() {
        return stackData.peek();
    }
    public int min() {
        return stackMin.peek();
    }
}
```
### 4、栈的压入、弹出序列
输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否可能为该栈的弹出顺序。假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。（注意：这两个序列的长度是相等的）
```java
import java.util.ArrayList;
import java.util.Stack;
public class Solution {
	/**借用一个辅助的栈，遍历压栈顺序，先讲第一个放入栈中，这里是1，
    然后判断栈顶元素是不是出栈顺序的第一个元素，这里是4，很显然1≠4，所以我们继续压栈，
    直到相等以后开始出栈，出栈一个元素，则将出栈顺序向后移动一位，直到不相等，
    这样循环等压栈顺序遍历完成，如果辅助栈还不为空，说明弹出序列不是该栈的弹出顺序。
    举例： 
    入栈1,2,3,4,5 
   出栈4,5,3,2,1 
   首先1入辅助栈，此时栈顶1≠4，继续入栈2 
   此时栈顶2≠4，继续入栈3 
   此时栈顶3≠4，继续入栈4 
   此时栈顶4＝4，出栈4，弹出序列向后一位，此时为5，,辅助栈里面是1,2,3 
   此时栈顶3≠5，继续入栈5 
   此时栈顶5=5，出栈5,弹出序列向后一位，此时为3，,辅助栈里面是1,2,3 
  …. 
  依次执行，最后辅助栈为空。如果不为空说明弹出序列不是该栈的弹出顺序。
	 * @param pushA
	 * @param popA
	 * @return
	 */
	public boolean IsPopOrder(int[] pushA, int[] popA) {
		if (pushA == null || popA == null || pushA.length == 0
				|| popA.length == 0)
			return false;
		int index = 0; //作为弹出序列的一个索引
		Stack<Integer> stack = new Stack<Integer>();
        for (int i = 0; i < pushA.length; i++) {
        	stack.push(pushA[i]); 
            while (!stack.isEmpty() && stack.peek() == popA[index]) {// 当栈不为空且栈顶元
                //素等于弹出序列元素时候，就弹出一个，同时让弹出序列后移一个
                stack.pop();
                index++;
            }
		}
		return stack.isEmpty();//如果最后，栈不为空，相当于没有按照给定的弹出popA弹出完毕，
        //就说明不能按照popA，返回false
	}
}
```
###  
## 八、回溯法
### 1、矩阵中的路径
请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一个格子开始，每一步可以在矩阵中向左，向右，向上，向下移动一个格子。如果一条路径经过了矩阵中的某一个格子，则之后不能再次进入这个格子。 例如 a b c e s f c s a d e e 这样的3 X 4 矩阵中包含一条字符串"bcced"的路径，但是矩阵中不包含"abcb"路径，因为字符串的第一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入该格子。
思路：回溯算法也叫试探法，它是一种系统地搜索问题的解的方法。回溯算法的基本思想是：从一条路往前走，能进则进，不能进则退回来，换一条路再试。首先，在矩阵中任选一个格子作为路径的二七店。假设矩阵中某个格子的字符为ch并且这个格子将对应于路径上的第i个字符。如果路径上的第i个字符不是ch，那么这个格子不可能处在路径上的第i个位置。如果路径上的第i个字符正好是ch，那么朝相邻的格子寻找路径上的第i+1个字符。除在边界上的格子之外，其他格子都有4个相邻的格子。重复这个过程直到路径上的所有字符都在矩阵中找到相应的位置。
由于回溯法的递归特性，路径可以看成一个栈。当矩阵中定位了路径中的前n个字符的位置之后，在与第n个字符对应的格子的周围都没有找到第n+1个字，需要在路径上会退到第n-1个字符，重新定位第n个字符。由于路径不能重复进入矩阵的格子，还需要定义和矩阵大小一样的布尔值矩阵，用来标识路径是否已经进入了每个格子。
```java
public class Solution {
    /**
     * 判断字符矩阵是否包含某一个字符序列
     * @param matrix    
     * @param rows  矩阵行数
     * @param cols  矩阵列数
     * @param str   目标字符序列
     * @return
     */
    public boolean hasPath(char[] matrix, int rows, int cols, char[] str) {
        boolean visitFlags[] = new boolean[matrix.length];
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (hasPathCore(matrix, rows, cols, row, col, str, 0, visitFlags))
                    return true;
            }
        }
        return false;
    }
    /**
     * 回溯法递归实现判断
     * @param matrix    字符矩阵
     * @param rows  矩阵行数
     * @param cols  矩阵列数
     * @param row   当前行索引
     * @param col   当前列索引
     * @param str   目标字符序列
     * @param k 目标字符序列中当前字符索引
     * @param visitFlags    字符矩阵是否被访问过标记
     * @return
     */
    boolean hasPathCore(char[] matrix, int rows, int cols, int row, int col, char[] str,  int k, boolean[] visitFlags) {
        int index = row * cols + col;
        // 行列索引超限、当前字符已经被访问过、当前字符不等于目标字符序列的当前字符，直接返回false
        if (row < 0 || col < 0 || row >= rows || col >= cols || 
                visitFlags[index] || matrix[index] != str[k])
            return false;
        visitFlags[index] = true;   // 设置访问标记
        if (k == str.length - 1)    // 递归结束条件，k已经到达目标字符序列的最后一个字符
            return true;
        k++;    // 匹配目标字符序列的下一个字符
        // 在当前字符的上、下、左、右的元素搜索下一个目标字符，递归
        if (hasPathCore(matrix, rows, cols, row + 1, col, str, k, visitFlags) || 
                hasPathCore(matrix, rows, cols, row - 1, col, str, k, visitFlags) || 
                hasPathCore(matrix, rows, cols, row, col + 1, str, k, visitFlags) || 
                hasPathCore(matrix, rows, cols, row, col - 1, str, k, visitFlags))
            return true;
        // // 在当前字符的上、下、左、右的元素没有搜索到下一个目标字符，将访问标记重置为false，返回false；
        visitFlags[index] = false;
        return false;
    }
}
```
### 2、机器人运动范围
地上有一个m行和n列的方格。一个机器人从坐标0,0的格子开始移动，每一次只能向左，右，上，下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于k的格子。 例如，当k为18时，机器人能够进入方格（35,37），因为3+5+3+7 = 18。但是，它不能进入方格（35,38），因为3+5+3+8 = 19。请问该机器人能够达到多少个格子？
思路：利用递归实现，每次只能走上下左右四个点，进行判断点的位置是否越界，点数之和是否大于K，是否已经走过了。
```java
public int movingCount(int threshold, int rows, int cols) {
    int flag[][] = new int[rows][cols]; //记录是否已经走过
    return helper(0, 0, rows, cols, flag, threshold);
}
private int helper(int i, int j, int rows, int cols, int[][] flag, int threshold) {
    if (i < 0 || i >= rows || j < 0 || j >= cols ||
            numSum(i) + numSum(j) > threshold || flag[i][j] == 1)
        return 0;
    flag[i][j] = 1;
    return helper(i - 1, j, rows, cols, flag, threshold)
            + helper(i + 1, j, rows, cols, flag, threshold)
            + helper(i, j - 1, rows, cols, flag, threshold)
            + helper(i, j + 1, rows, cols, flag, threshold) + 1;
}
private int numSum(int i) {
    int sum = 0;
    while (i > 0) {
        sum += i % 10;
        i = i / 10;
    }
    return sum;
}
```
## 九、链表
### 1、从尾到头打印链表
输入一个链表，按链表值从尾到头的顺序返回一个ArrayList。
也可以用栈：
```java
public class Solution {
    public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
        if(listNode == null){
            ArrayList list = new ArrayList();
            return list;
        }
        Stack<Integer> stk = new Stack<Integer>();
        while(listNode != null){
            stk.push(listNode.val);
            listNode = listNode.next;
        }
        ArrayList<Integer> arr = new ArrayList<Integer>();
        while(!stk.isEmpty()){
            arr.add(stk.pop());
        }
        return arr;
    }
}
```
```java
/**
*    public class ListNode {
*        int val;
*        ListNode next = null;
*
*        ListNode(int val) {
*            this.val = val;
*        }
*    }
*
*/
import java.util.ArrayList;
import java.util.Collections;
public class Solution {
    public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
    ArrayList<Integer> addressArrayList = new ArrayList<Integer>();
        if(listNode == null){
        ArrayList first = new ArrayList();
            return first;
        }
        while (listNode != null) {
            addressArrayList.add(listNode.val);
          listNode = listNode.next;
       }
        Collections.reverse(addressArrayList); 
         return addressArrayList;
      }
    }
```
### 2、链表中倒数第k个结点
输入一个链表，输出该链表中倒数第k个结点。
```java
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
public class Solution {
    public ListNode FindKthToTail(ListNode head,int k) {
    	//一种思路是先遍历一遍求长度，然后输出倒数k个
    	//正常的思路是，设置两个游标,让快的领先k个
    	ListNode slow = head;
    	ListNode fast = head;
        if (head == null || k <= 0) {
            return null;
        }
    	for (int i = 1; i < k; i++) { //快的先走k-1步，倒数第三个，其实应该快的指到第三个，只需要走两步即可。
			if(fast.next == null) //这个是k与链表长度的关系，如果，链表长度小于k，肯定在走到k之前就出现
                //null，直接返回null即可
				return null;
			else 
			   fast = fast.next;
		}
    	while(fast.next != null){ //快的从第k个，慢的从第1个，同时开始走。
    		slow = slow.next;
    		fast = fast.next;
    	}
    	return slow;
    }
}
```
###  3、反转链表
输入一个链表，反转链表后，输出新链表的表头。
```java
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
public class Solution {
    public ListNode ReverseList(ListNode head) {
		if (head == null) 
			return null;
		ListNode pre = null;
		ListNode next = null;
		while(head != null){ //注意这个地方的写法，如果写head.next将会丢失最后一个节点
			next = head.next;
			head.next = pre;
			pre = head;
			head = next;
		}
		return pre;
    }
}
/*
 //反转链表
	public ListNode ReverseList1(ListNode head) {
		if (head == null) 
			return null; // head为当前节点，如果当前节点为空的话，那就什么也不做，直接返回null； 
		ListNode pre = null;
		ListNode next = null;
		// 当前节点是head，pre为当前节点的前一节点，next为当前节点的下一节点  
        // 需要pre和next的目的是让当前节点从pre->head->next1->next2变成pre<-head next1->next2  
        // 即pre让节点可以反转所指方向，但反转之后如果不用next节点保存next1节点的话，此单链表就此断开了  
        // 所以需要用到pre和next两个节点  
        // 1->2->3->4->5  
        // 1<-2<-3->4->5  
		while(head != null){ //注意这个地方的写法，如果写head.next将会丢失最后一个节点
			// 做循环，如果当前节点不为空的话，始终执行此循环，此循环的目的就是让当前节点从指向next到指向pre   
            // 如此就可以做到反转链表的效果  
            // 先用next保存head的下一个节点的信息，保证单链表不会因为失去head节点的原next节点而就此断裂
			next = head.next; //先让head.next指向的节点，即第二个节点叫next
			head.next = pre; //将head.next指向pre，也就是说断开head节点与后面的连接
			pre = head;//pre,head依次向后移动一个节点，进行下一次反转
			head = next;
		}
		// 如果head为null的时候，pre就为最后一个节点了，但是链表已经反转完毕，pre就是反转后链表的第一个节点 
		return pre;
	}
	//合并两个递增的链表并且保证最终的链表也是单调不减的。
*/
```
### 4、合并两个排序的链表
题目描述
输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
 思路：输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
    比较两个链表的首结点，哪个小的的结点则合并到第三个链表尾结点，并向前移动一个结点。
    步骤一结果会有一个链表先遍历结束，或者没有
    第三个链表尾结点指向剩余未遍历结束的链表
    返回第三个链表首结点 
```java
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
public class Solution {
    public ListNode Merge(ListNode list1,ListNode list2) {
        	    if (list1 == null) {
			return list2;
		}
	    if (list2 == null) {
			return list1;
		}
	    //新建一个用于存放融合之后的链表
	    //因为，融合的过程中链表是一直移动的，所以要把链表的头保存下来，才能返回正确的一条链
	    ListNode res = new ListNode(0);  //此处或者res = null，这样就不是res.next 而是res
	    ListNode merlistNode = res;
	    while (list1 != null && list2 != null) { //依次比较，将较小的节点连到融合节点上
			if(list1.val < list2.val){   
				merlistNode.next = list1;      //连上小的list1
			    list1 = list1.next;         //list1 可以往后移动一个，下次用移动后的和list2比较
			    merlistNode = merlistNode.next;// merlistNode也往后移动一个
			}
			else {
				merlistNode.next = list2;
			    list2 = list2.next;
			    merlistNode = merlistNode.next;
			}
		}
	    //把未结束的链表连接到合并后的链表尾部 
	    if(list1 != null)
	    	merlistNode.next = list1;
	    if(list2 != null)
	    	merlistNode.next = list2;
	    return res.next;
    }
    //递归的方式
     /*public ListNode Merge(ListNode list1,ListNode list2) {  
            if (list1 == null) {  
                return list2;  
            }  
            if (list2 == null) {  
                return list1;  
            }  
            ListNode newHead = null;  
            if (list1.val <= list2.val) {  
                newHead = list1;  
                newHead.next = Merge(list1.next,list2);  
            }else {  
                newHead = list2;  
                newHead.next = Merge(list1,list2.next);  
            }  
            return newHead;  
        }  */
}
```
### 5、复杂链表的复制
题目描述
输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意一个节点），返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则判题程序会直接返回空）
```java
/*
public class RandomListNode {
    int label;
    RandomListNode next = null;
    RandomListNode random = null;
    RandomListNode(int label) {
        this.label = label;
    }
}
*/
import java.util.HashMap;
public class Solution {
 /**左程云思路：除了这个还有不用链表的思路。
     * 算法步骤：遍历两遍链表，第一遍将仅仅将数赋值给map中的值，第二遍将指针指向赋值。注意保存头指针的位置。
     * 1.第一遍遍历，key是第一个链表中的节点，value是复制后的链表节点，但是指针都指向null。
     * 2.第二遍遍历，将相对应的next和random均复制。
     * @param pHead
     * @return
     */
    public RandomListNode Clone(RandomListNode pHead)
    {
        HashMap<RandomListNode, RandomListNode> map =new HashMap<RandomListNode, RandomListNode>();
        RandomListNode current = pHead; //保存头结点
        while (current != null) {//第一遍遍历
			map.put(current,new RandomListNode(current.label));// hashmap里面，key放的是之前的链表节点，value现在只放值
			current = current.next;
		}
        current = pHead;
        while (current != null) {//第二遍遍历
        	//现在map中是1--1'  2--2'。为了让1'指向2'  要给1'的next赋值， 要找1'就得get(1)。值是2',要找2'就是get(1.next)
			map.get(current).next = map.get(current.next); 
			map.get(current).random = map.get(current.random);
			current = current.next;
		}
        return map.get(pHead);
    }
}
```
### 6、两个链表的第一个公共结点
题目描述
输入两个链表，找出它们的第一个公共结点
![img](https://img-blog.csdn.net/2018091516513423?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzenkxMzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```java
/*
public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}*/
public class Solution {
    /**采用了左程云的代码思想：
     * 首先，如果相交，那么尾节点一定是一样的。
     * 接下来，谁长谁就先走一定的步数，然后一起走，肯定是同时到达相交的点。
     * @param pHead1
     * @param pHead2
     * @return
     */
    public ListNode FindFirstCommonNode(ListNode pHead1, ListNode pHead2) {
    	 if (pHead1 == null || pHead2 == null)
    		 return null;
    	 ListNode  cur1 = pHead1;
    	 ListNode  cur2 = pHead2;
    	 int n = 0;
    	 while(cur1.next != null) {
    		 n++; //记录长度
    		 cur1 = cur1.next;
    	 }
    	 while(cur2.next != null) {
    		 n--;
    		 cur2 = cur2.next;
    	 }
    	 if(cur1 != cur2)
    		 return null;
    	 cur1 = n > 0 ? pHead1:pHead2;// n大于0  说明cur1要先走一部分。
    	 cur2 = cur1 == pHead1 ?pHead2:pHead1;//cur2 等于另一个
    	 n= Math.abs(n);
    	 while(n !=0 ) {
    		 n--;    //先cur1走完这部分
    		 cur1 = cur1.next;
    	 }
    	 while(cur1 != cur2) {
    		 cur1 = cur1.next;
    		 cur2 = cur2.next;
    	 }
    	return cur1;	 
    }
}
```
### 7、链表中环的入口节点
给一个链表，若其中包含环，请找出该链表的环的入口结点，否则，输出null。
**思路：**
1.   第一步，找环中相汇点。分别用p1，p2指向链表头部， 
2.  \* p1每次走一步，p2每次走二步，直到p1==p2找到在环中的相汇点。 
通过141题，我们知道可以通过快慢指针来判断是否有环，现在我们假设两个指针相遇在z点，如图
![img](https://img-blog.csdn.net/20150907161516402)
那么我们可以知道fast指针走过a+b+c+b
slow指针走过a+b
那么2*(a+b) = a+b+c+b
所以a = c
那么此时让slow回到起点，fast依然停在z，两个同时开始走，一次走一步
那么它们最终会相遇在y点，正是环的起始点
```java
/*
 public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
	/**
	 * 主要思路就是一快 一慢两个指针，如果有环，最终快的肯定能追上慢的，
	 * 找环的入口的思路见博客。
	 * @param pHead
	 * @return
	 */
    public ListNode EntryNodeOfLoop(ListNode pHead)
    {
        if (pHead == null || pHead.next == null) {
			return null;
		}
        ListNode fast = pHead;
        ListNode slow = pHead;
        while(fast != null && fast.next != null) {//因为fast每次要走两步，所有需要判断fast的下一个是否为空  
        	slow = slow.next;
        	fast = fast.next.next;//一个走一步 一个走两步
        	if(slow == fast) {
        		fast = pHead;
        		while(slow != fast) {
        			slow = slow.next;
        			fast = fast.next;
        		}
        		return slow;
        	}
        }
        return null;
    }
}
```
### 8、删除链表中重复的节点
题目描述
在一个排序的链表中，存在重复的结点，请删除该链表中重复的结点，重复的结点不保留，返回链表头指针。 例如，链表1->2->3->3->4->4->5 处理后为 1->2->5
重点是第一个也可能是重复的点，因此新建一个preNode节点保存前一个节点
```java
/*
 public class ListNode {
    int val;
    ListNode next = null;
    ListNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
     /**
     * 主要参考博客的解题思路
     * @param pHead
     * @return
     */
    public ListNode deleteDuplication(ListNode pHead)
    {
            if(pHead == null)
            	return null;
            // 新建一个节点，防止头结点被删除
            ListNode firstNode = new ListNode(-1);
            firstNode.next = pHead;
            ListNode p = pHead;
            // 指向前一个节点
            ListNode preNode = firstNode;
            while (p!=null &&p.next !=null) {//注意条件的顺序，否则不对 因为如果p为null，p.next肯定异常
				if(p.val == p.next.val) {
					int val = p.val;
					 // 向后重复查找
					while (p != null&&p.val == val) {
						p = p.next;
					}
					// 上个非重复值指向下一个非重复值：即删除重复值
					preNode.next = p;
				}
				else {
					 // 如果当前节点和下一个节点值不等，则向后移动一位
					preNode = p;
					p = p.next;
				}
			}
            return firstNode.next;
    }
}
```
9、链表回文结构--左神
## 十、非剑指offer
### 1、左神的
给定一个字符串str，str表示一个公式，公式里可能有整数，加减乘除符号和左右括号，返回公式计算的结果。例如，str = 48×((70-65)-43)+8×1。返回-1816。
**【说明】**

1. 可以认定给定的字符串一定是正确的公式，不需要对str做公式有效性检查。
2. 如果是负数就需要有括号括起来，比如4*(-3)。但如果负数作为公式的开头或者括号部分的开头，则可以没有括号，比如-3*4和（-3*4）都是合法的。
3. 不需要考虑溢出问题
```java
import java.util.LinkedList;
/**
 * 算法顺序大致如下：
 * 没有括号的时候： 1、执行value，碰到加减乘除，则执行addNum将其中原有的乘除法运算完，把结果放到队列中，跳出此方法。然后把符号放进去。
 *                     保证其中没有加减乘除导致无法知道优先级。
 *                  2、全部遍历完毕，则执行addNum把乘除法计算完毕，然后把最后一个数字放入。
 *                  3、执行getNum 计算队列中的加减法，没有优先级问题，可以直接进行运算即可。
 * 有括号的情况下：1、执行value，按上述第一步执行，碰到括号，则进入递归，去按上述所有的步骤计算括号里面的。
 *                  2、剩下的按照上边的算法继续计算。
 * Created by shixi_shengzhi on 2018/9/9
 */
public class Main {
    public static int getValue(String str) {
        return value(str.toCharArray(), 0)[0];
    }
    public static int[] value(char[] str, int i) {
        LinkedList<String> que = new LinkedList<String>();
        int pre = 0;
        int[] bra = null;
        while (i < str.length && str[i] != ')') {
            if (str[i] >= '0' && str[i] <= '9') {
                pre = pre * 10 + str[i++] - '0';
            } else if (str[i] != '(') { //+ - * / ，每当碰到运算符号就先把里面的乘除法运算完，保证队列里面都是加减法
                addNum(que, pre);// 计算完乘除法，将结果放到队列中 没有运算把之前的数放入
                que.addLast(String.valueOf(str[i++]));
                pre = 0;
            } else { // 碰到符号（
                bra = value(str, i + 1); //重新递归上述过程。返回括号中的结果以及位置
                pre = bra[0];
                i = bra[1] + 1;
            }
        }
        //公式执行完或者碰到 )
        addNum(que, pre); //计算乘除法
        return new int[]{getNum(que), i}; //计算公式中还保留的加减法
    }
    /**
     * 计算乘除法，把乘除法的结果还有加减号在放在队列中。
     *
     * @param que
     * @param num
     */
    public static void addNum(LinkedList<String> que, int num) {
        if (!que.isEmpty()) {
            int cur = 0;
            String top = que.pollLast();
            if (top.equals("+") || top.equals("-")) {
                que.addLast(top);
            } else {
                cur = Integer.valueOf(que.pollLast());
                num = top.equals("*") ? (cur * num) : (cur / num);
            }
        }
        que.addLast(String.valueOf(num));
    }
    /**
     * 计算队列中的加减法
     *
     * @param que
     * @return
     */
    public static int getNum(LinkedList<String> que) {
        int res = 0;
        boolean add = true;
        String cur = null;
        int num = 0;
        while (!que.isEmpty()) {
            cur = que.pollFirst();
            if (cur.equals("+")) {
                add = true;
            } else if (cur.equals("-")) {
                add = false;
            } else {
                num = Integer.valueOf(cur);
                res += add ? num : (-num);
            }
        }
        return res;
    }
    public static void main(String[] args) {
        String exp = "48*((70-65)-43)+8*1";
        exp = "3+5*6";
        exp = "3+4*5+6";
        System.out.println(getValue(exp));
//        System.out.println(getValue(exp));
//
//        exp = "4*(6+78)+53-9/2+45*8";
//        System.out.println(getValue(exp));
//
//        exp = "10-5*3";
//        System.out.println(getValue(exp));
//
//        exp = "-3*4";
//        System.out.println(getValue(exp));
//
//        exp = "3+1*4";
//        System.out.println(getValue(exp));
    }
}
```
