# 二叉树的三种遍历方式（递归、非递归和Morris遍历） - miner_zhu的博客 - CSDN博客





2018年07月13日 19:42:37[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：46标签：[python																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[常用算法python实现](https://blog.csdn.net/miner_zhu/article/category/7800703)








### 二叉树遍历是二叉树的最基本的操作，其实现方式主要有三种：
递归遍历
非递归遍历

Morris遍历

二叉树递归遍历的实现非常容易，非递归实现需要用到栈。而Morris算法可能很多人都不太熟悉，其强大之处在于只需要使用O(1)的空间就能实现对二叉树O(n)时间的遍历。




### 二叉树结点的定义

每个二叉树结点包括一个值以及左孩子和右孩子结点，其定义如下：

```python
#定义二叉树节点
class TreeNode:
	"""docstring for ClassName"""
	def __init__(self, x):
		root.val = x
		root.left = None
		root.right = None
```

### 创建二叉树

```python
#创建二叉树
def creat_tree(root):
	element = input("输入值:")
	if element = '#':
		root = None
	else:
		root = TreeNode(element)
		root.left = creat_tree(root.left)
		root.right = creat_tree(root.right)
```



**二叉树的遍历**

二叉树的遍历，就是按照某条搜索路径访问树中的每一个结点，使得每个结点均被访问一次，而且仅被访问一次。常见的遍历次序有：
- 先序遍历：先访问根结点，再访问左子树，最后访问右子树
- 中序遍历：先访问左子树，再访问根结点，最后访问右子树
- 后序遍历：先访问左子树，再访问右子树，最后访问根结点

下面介绍，二叉树3种遍历方式的实现。


### 递归遍历

递归实现非常简单，按照遍历的次序，对当前结点分别调用左子树和右子树即可。

```python
#前序遍历
def pre_order(root):
	print(root.val,end=' ')
	pre_order(root.left)
	pre_order(root.right)

#中序遍历
def mid_order(root):
	mid_order(root.left)
	print(root.val,end=' ')
	mid_order(root.right)

#后序遍历
def  post_order(root):
	post_order(root.left)
	post_order(root.right)
	print(root.val,end=' ')
```

#### 复杂度分析

二叉树遍历的递归实现，每个结点只需遍历一次，故时间复杂度为O(n)。而使用了递归，最差情况下递归调用的深度为O(n)，所以空间复杂度为O(n)。




### 非递归遍历

二叉树遍历的非递归实现，可以借助栈。

#### 前序遍历
- 将根结点入栈；
- 每次从栈顶弹出一个结点，访问该结点；
- 把当前结点的右孩子入栈；
- 把当前结点的左孩子入栈。

按照以上顺序入栈，这样出栈顺序就与先序遍历一样：先根结点，再左子树，最后右子树。

```python
#非递归前序遍历
def preorder(root):
	if not root:
		return
	stack = []
	while root or len(stack):
		if root:
			stack.append(root)
			print(root.val,end=' ')
			root = root.left
		else:
			root = stack.pop()
			root = root.right
```

#### 中序遍历
- 初始化一个二叉树结点pNode指向根结点；
- 若pNode非空，那么就把pNode入栈，并把pNode变为其左孩子；（直到最左边的结点）
- 若pNode为空，弹出栈顶的结点，并访问该结点，将pNode指向其右孩子（访问最左边的结点，并遍历其右子树）

```python
#非递归中序遍历
def midorder(root):
	if not root:
		return
	stack = []
	while root or len(stack):
		if root:
			stack.append(root)
			root = root.left
		else:
			root = stack.pop()
			print(root.val,end=' ')
			root = root.right
```

#### 后序遍历
- 设置两个栈stk, stk2；
- 将根结点压入第一个栈stk；
- 弹出stk栈顶的结点，并把该结点压入第二个栈stk2；
- 将当前结点的左孩子和右孩子先后分别入栈stk；
- 当所有元素都压入stk2后，依次弹出stk2的栈顶结点，并访问之。

第一个栈的入栈顺序是：根结点，左孩子和右孩子；于是，压入第二个栈的顺序是：根结点，右孩子和左孩子。因此，弹出的顺序就是：左孩子，右孩子和根结点。

```python
#非递归后序遍历
def postorder(root):
	if not root:
		return
	stack1 = []
	stack2 = []
	while root or len(stack1):
		if root:
			stack1.append(root)
			stack2.append(root.val)
			root = root.right
		else:
			root = stack1.pop()
			root = root.left
	while stack2:
		print(stack2.pop(),end=' ')
```

另外，二叉树的后序遍历的非递归实现，也可以只使用一个栈来实现。

使用单栈，增加一个临时变量标记它的左子树与右字数是否被访问。由于是自底向上，如果临时节点不为当前节点的左孩子或右孩子，那么当前节点左子树与右子树已经被访问。

```python
#非递归后序遍历（只用一个栈）
def postOrder(root):
	if not root:
		return
	stack = []
	stack.append(root)
	c = None
	while stack:
		c = stack[-1]
		if c.left and c.left !=root and c.right != root:
			stack.append(c.left)
		elif c.right and c.right != root:
			stack.append(c.right)
		else:
			print(stack().val,end=' ')
			root = c
```

#### 复杂度分析

二叉树遍历的非递归实现，每个结点只需遍历一次，故时间复杂度为O(n)。而使用了栈，空间复杂度为二叉树的高度，故空间复杂度为O(n)。




### Morris遍历

Morris遍历算法最神奇的地方就是，只需要常数的空间即可在O(n)时间内完成二叉树的遍历。O(1)空间进行遍历困难之处在于在遍历的子结点的时候如何重新返回其父节点？在Morris遍历算法中，通过修改叶子结点的左右空指针来指向其前驱或者后继结点来实现的。

#### 中序遍历
- 如果当前结点pNode的左孩子为空，那么输出该结点，并把该结点的右孩子作为当前结点；
- 如果当前结点pNode的左孩子非空，那么就找出该结点在中序遍历中的前驱结点pPre- 当第一次访问该前驱结点pPre时，其右孩子必定为空，那么就将其右孩子设置为当前结点，以便根据这个指针返回到当前结点pNode中，并将当前结点pNode设置为其左孩子；
- 当该前驱结点pPre的右孩子为当前结点，那么就输出当前结点，并把前驱结点的右孩子设置为空（恢复树的结构），将当前结点更新为当前结点的右孩子

- 重复以上两步，直到当前结点为空。

因为只使用了两个辅助指针，所以空间复杂度为O(1)。对于时间复杂度，每次遍历都需要找到其前驱的结点，而寻找前驱结点与树的高度相关，那么直觉上总的时间复杂度为O(nlogn)。其实，并不是每个结点都需要寻找其前驱结点，只有左子树非空的结点才需要寻找其前驱，所有结点寻找前驱走过的路的总和至多为一棵树的结点个数。因此，整个过程每条边最多走两次，一次使定位到该结点，另一次是寻找某个结点的前驱，所以时间复杂度为O(n)。

如以下一棵二叉树。首先，访问的是根结点F，其左孩子非空，所以需要先找到它的前驱结点（寻找路径为B->D->E），将E的右指针指向F，然后当前结点为B。依然需要找到B的前驱结点A，将A的右指针指向B，并将当前结点设置为A。下一步，输出A，并把当前结点设置为A的右孩子B。之后，会访问到B的前驱结点A指向B，那么令A的右指针为空，继续遍历B的右孩子。依次类推。

![一棵二叉树](https://www.linuxidc.com/upload/2015_08/150830091597061.jpg)

```python
#morris中序遍历
def morrisIn(root):
    if not root:
        return
    while root:
        cur = root.left
        if cur:
            while cur.right and cur.right != root:
                cur = cur.right
            if not cur.right:
                cur.right = root
                root = root.left
                continue
            else:
                cur.right = None
        print(root.val, end=' ')
        root = root.right
```

#### 前序遍历

与中序遍历类似，区别仅仅是输出的顺序不同。

```python
#morris前序遍历
def morrisPre(root):
    if not root:
        return
    while root:
        cur = root.left
        if cur:
            while cur.right and cur.right != root:
                cur = cur.right
            if not cur.right:
                cur.right = root
                print(root.val, end=' ')
                root = root.left
                continue
            else:
                cur.right = None
        else:
            print(root.val, end=' ')
        root = root.right
```

#### 后序遍历
- 先建立一个临时结点dummy，并令其左孩子为根结点root，将当前结点设置为dummy；
- 如果当前结点的左孩子为空，则将其右孩子作为当前结点；
- 如果当前结点的左孩子不为空，则找到其在中序遍历中的前驱结点- 如果前驱结点的右孩子为空，将它的右孩子设置为当前结点，将当前结点更新为当前结点的左孩子；
- 如果前驱结点的右孩子为当前结点，倒序输出从当前结点的左孩子到该前驱结点这条路径上所有的结点。将前驱结点的右孩子设置为空，将当前结点更新为当前结点的右孩子。

- 重复以上过程，直到当前结点为空。

```python
#morris后序遍历
def morrisPost(root):
    def printRightEdge(root):
        tail = reverseEdge(root)
        cur = tail
        while cur:
            print(cur.val, end=' ')
            cur = cur.right
        reverseEdge(tail)

    def reverseEdge(root):
        pre = None
        while root:
            next = root.right
            root.right = pre
            pre = root
            root = next
        return pre

    if not root:
        return
    head = root
    while root:
        cur = root.left
        if cur:
            while cur.right and cur.right != root:
                cur = cur.right
            if not cur.right:
                cur.right = root
                root = root.left
                continue
            else:
                cur.right = None
                printRightEdge(root.left)
        root = root.right
    printRightEdge(head)
```







