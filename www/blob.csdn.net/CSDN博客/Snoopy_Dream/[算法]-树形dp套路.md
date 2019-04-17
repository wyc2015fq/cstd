# [算法] - 树形dp套路 - Snoopy_Dream - CSDN博客





2019年03月20日 16:04:26[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：67








树形dp套路 树形dp套路使用前提： 如果题目求解目标是S规则，则求解流程可以定成以每一个节点为头节点的子树在S规则下的每一个答案，并且最终答案一定在其中

[https://www.cnblogs.com/mhpp/p/6628548.html](https://www.cnblogs.com/mhpp/p/6628548.html) 这其中是一些其他的例子，抽空可以看看。

### 1. 树形dp套路

【站在 **左树也能要信息，右树也能要信息，考虑当前头****，**的角度】

**树形dp套路第一步：**

以某个节点X为头节点的子树中，分析答案有哪些可能性，并且这种分析是以X的左子树、X的右子树和X整棵树的角度来考虑可能性的

**树形dp套路第二步：**

根据第一步的可能性分析，列出所有需要的信息

**树形dp套路第三步：**

合并第二步的信息，对左树和右树提出同样的要求，并写出信息结构

**树形dp套路第四步：**

设计递归函数，递归函数是处理以X为头节点的情况下的答案。

包括设计递归的basecase，默认直接得到左树和右树的所有信息，以及把可能性做整合，并且要返回第三步的信息结构这**四个小步骤**



### 题目一  ：二叉树节点间的最大距离问题

从二叉树的节点a出发，可以向上或者向下走，但沿途的节点只能经过一次，到达节点b时路径上的节点个数叫作a到b的距离，那么二叉树任何两个节点之间都有距离，**求整棵树上的最大距离**。



1. 以X的左子树、X的右子树和X整棵树的角度来考虑可能性的

1) 和x有关，那就是左子树上最长的 到x 然后 到右子树

2) 和x无关

    A. 只与x的左子树有关 [ 如下图 ]

    B. 只与x的右子树有关

![](https://img-blog.csdnimg.cn/20190127171013144.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

2. 根据第一步的可能性分析，列出所有需要的信息

**左远到右远**

左树上的最大距离，左远对应的左树的高度。

右树上的最大距离，右远对应的右树的高度。

![](https://img-blog.csdnimg.cn/20190127171102157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

3.合并第二步的信息，对左树和右树提出同样的要求，并写出信息结构【求并集】

如左树要最小，右树要最大，那么信息结构就是两者都要求。![](https://img-blog.csdnimg.cn/20190127171055252.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

4.设计递归函数，递归函数是处理以X为头节点的情况下的答案。

包括设计递归的basecase，默认直接得到左树和右树的所有信息，以及把可能性做整合，并且要返回第三步的信息结构这四个小步骤

```java
public class Code02_MaxDistanceInTree {

	public static class Node {
		public int value;
		public Node left;
		public Node right;

		public Node(int data) {
			this.value = data;
		}
	}
	
	// 主函数，得到最大距离，输入一个头结点
	public static int maxDistance(Node head) {
		return process(head).maxDistance;
	}
	
	public static class Info{
		public int maxDistance;
		public int height;
		public Info(int dis, int h) {
			maxDistance = dis;
			height  = h;
		}
	}
	
	
	public static Info process(Node x) {//通过proess函数得到info
        // 1. basecase
		if(x == null) {
			return new Info(0,0);//构造新的info返回
		}
        // 2. 默认直接得到左树和右树的所有信息
		Info leftInfo = process(x.left);// 用了黑盒
		Info rightInfo = process(x.right);
		// info 拆解黑盒，拆出该函数的意义 ，这里是得到info【maxDistance，height】
        // 3. 可能性做整合 拆解黑盒
		int p1 = leftInfo.maxDistance;//情况一
		int p2 = rightInfo.maxDistance;//情况二
		int p3 = leftInfo.height + 1 + rightInfo.height;//情况三
		int maxDistance = Math.max(p3, Math.max(p1, p2));
		int height = Math.max(leftInfo.height, rightInfo.height) + 1 ;
        // 4. 返回第三步的信息结构
		return new Info(maxDistance, height);
	}
}
```

### 题目二 派对的最大快乐值  

员工信息的定义如下:

class Employee {    

              public int happy; // 这名员工可以带来的快乐值    

              List<Employee> subordinates; // 这名员工有哪些直接下级 }

公司的每个员工都符合 Employee 类的描述。整个公司的人员结构可以看作是一棵标准的、 没有环的多叉树。树的头节点是公司唯一的老板。除老板之外的每个员工都有唯一的直接上级。 叶节点是没有任何下属的基层员工(subordinates列表为空)，除基层员工外，每个员工都有一个或多个直接下级。 这个公司现在要办party，你可以决定哪些员工来，哪些员工不来。但是要遵循如下规则。

1.如果某个员工来了，那么这个员工的所有直接下级都不能来

2.派对的整体快乐值是所有到场员工快乐值的累加

3.你的目标是让派对的整体快乐值尽量大

给定一棵多叉树的头节点boss，请返回派对的最大快乐值。



**首先从X开始分析，x整棵树可能性**

![](https://img-blog.csdnimg.cn/20190127171228272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

```java
public static class Employee { 
		public int happy; // 这名员工可以带来的快乐值 
	    public List<Employee> nexts; // 这名员工有哪些直接下级 
	}
	
	
	public static int maxHappy(Employee boss) {
		Info headInfo = process(boss);
		return Math.max(headInfo.laiMaxHappy, headInfo.buMaxHappy);
	}
	
	// 得到一个员工，来or不来，分别对应的最大happy！！！！
	public static class Info{
		public int laiMaxHappy;
		public int buMaxHappy;
		public Info(int lai, int bu) {
			laiMaxHappy = lai;
			buMaxHappy = bu;
		}
	}
	// 得到一个员工，来or不来，分别对应的最大happy！！！！
	public static Info process(Employee x) {
		//1. basecase
		if(x.nexts.isEmpty()) {
			return new Info(x.happy,0);
		}
		int lai = x.happy;
		int bu = 0;
		for(Employee next : x.nexts) {
			Info nextInfo = process(next);// 2. 默得到子树的info信息
			// 3. 整合信息【拆解黑盒，得到info中具体的】
			lai += nextInfo.buMaxHappy;
			bu += Math.max(nextInfo.laiMaxHappy, nextInfo.buMaxHappy);
		}// 4. 返回info
		return new Info(lai,bu);
	}
```



### 题目三 最大搜索二叉子树

![](https://img-blog.csdnimg.cn/20190127171241364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

1. 可能性分析

和x有关，整树是最大搜索二叉树，左边的最大数小于x小于右边的最小数

和x无关

    左树BSTsize大小是最大的

    右树BSTsize大小是最大的

2. 根据可能性分析，**列出所有需要的信息**

![](https://img-blog.csdnimg.cn/20190127171941512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

3. **合并**第二步的信息，写出需要的信息结构【info构造】

![](https://img-blog.csdnimg.cn/20190127171941512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

4. 设计递归函数，递归函数是处理以X为头节点的情况下的答案。 包括设计递归的basecase，默认直接得到左树和右树的所有信息【用黑盒】，以及把可能性做整合【拆黑盒】，并且要返回第三步的信息结构这四个小步骤

```java
public static Node getMaxBST(Node head) {
		return process(head).maxBSTHead;
	}

	public static class ReturnType {
		public Node maxBSTHead;
		public int maxBSTSize;
		public int min;
		public int max;

		public ReturnType(Node maxBSTHead, int maxBSTSize, int min, int max) {
			this.maxBSTHead = maxBSTHead;
			this.maxBSTSize = maxBSTSize;
			this.min = min;
			this.max = max;
		}
	}

	public static ReturnType process(Node X) {
		// base case : 如果子树是空树
		// 最小值为系统最大
		// 最大值为系统最小
		if (X == null) {
			return new ReturnType(null, 0, Integer.MAX_VALUE, Integer.MIN_VALUE);
		}
		// 默认直接得到左树全部信息
		ReturnType lData = process(X.left);
		// 默认直接得到右树全部信息
		ReturnType rData = process(X.right);
		// 以下过程为信息整合
		// 同时以X为头的子树也做同样的要求，也需要返回如ReturnType描述的全部信息
		// 以X为头的子树的最小值是：左树最小、右树最小、X的值，三者中最小的
		int min = Math.min(X.value, Math.min(lData.min, rData.min));
		// 以X为头的子树的最大值是：左树最大、右树最大、X的值，三者中最大的
		int max = Math.max(X.value, Math.max(lData.max, rData.max));
		// 如果只考虑可能性一和可能性二，以X为头的子树的最大搜索二叉树大小
		int maxBSTSize = Math.max(lData.maxBSTSize, rData.maxBSTSize);
		// 如果只考虑可能性一和可能性二，以X为头的子树的最大搜索二叉树头节点
		Node maxBSTHead = lData.maxBSTSize >= rData.maxBSTSize ? lData.maxBSTHead
				: rData.maxBSTHead;
		// 利用收集的信息，可以判断是否存在可能性三
		if (lData.maxBSTHead == X.left && rData.maxBSTHead == X.right
				&& X.value > lData.max && X.value < rData.min) {
			maxBSTSize = lData.maxBSTSize + rData.maxBSTSize + 1;
			maxBSTHead = X;
		}
		// 信息全部搞定，返回
		return new ReturnType(maxBSTHead, maxBSTSize, min, max);
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
```





