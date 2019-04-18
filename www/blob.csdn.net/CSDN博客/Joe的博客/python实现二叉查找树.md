# python实现二叉查找树 - Joe的博客 - CSDN博客





2017年05月01日 11:28:47[Joe-Han](https://me.csdn.net/u010089444)阅读数：8111








# **1. 定义**

二叉查找树（Binary Search Tree），又称为二叉搜索树、二叉排序树。其或者是一棵空树；或者是具有以下性质的二叉树：
- 若左子树不空，则左子树上所有结点的值均小于或等于它的根结点的值
- 若右子树不空，则右子树上所有结点的值均大于或等于它的根结点的值
- 左、右子树也分别为二叉排序树 
![图 1](https://img-blog.csdn.net/20170427151212447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图 1 


树中节点的定义如下：
```python
class Node:
    def __init__(self, data):
        self.data = data
        self.lchild = None
        self.rchild = None
```

# **2. 查找与插入**

当二叉查找树不为空时：
- 首先将给定值与根结点的关键字比较，若相等，则查找成功
- 若小于根结点的关键字值，递归查左子树
- 若大于根结点的关键字值，递归查右子树
- 若子树为空，查找不成功

二叉排序树是一种动态树表。其特点是：树的结构通常不是一次生成的，而是在查找过程中，当树中不存在关键字等于给定值的结点时再进行插入。新插入的结点一定是一个新添加的叶子结点，并且是查找不成功时查找路径上访问的最后一个结点的左孩子或右孩子结点。如下图所示： 

![这里写图片描述](https://img-blog.csdn.net/20170427151416933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


插入与搜索的代码：

```python
# 搜索
    def search(self, node, parent, data):
        if node is None:
            return False, node, parent
        if node.data == data:
            return True, node, parent
        if node.data > data:
            return self.search(node.lchild, node, data)
        else:
            return self.search(node.rchild, node, data)

    # 插入
    def insert(self, data):
        flag, n, p = self.search(self.root, self.root, data)
        if not flag:
            new_node = Node(data)
            if data > p.data:
                p.rchild = new_node
            else:
                p.lchild = new_node
```

# **3. 删除**

二叉查找树的删除操作分为三种情况：
- 如果待删除的节点是叶子节点，那么可以立即被删除，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170427162046592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 如果节点只有一个儿子，则将此节点parent的指针指向此节点的儿子，然后删除节点，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170427162233873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 如果节点有两个儿子，则将其右子树的最小数据代替此节点的数据，并将其右子树的最小数据删除，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170427162146639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
删除节点的代码：

```python
# 删除
    def delete(self, root, data):
        flag, n, p = self.search(root, root, data)
        if flag is False:
            print "无该关键字，删除失败"
        else:
            if n.lchild is None:
                if n == p.lchild:
                    p.lchild = n.rchild
                else:
                    p.rchild = n.rchild
                del n
            elif n.rchild is None:
                if n == p.lchild:
                    p.lchild = n.lchild
                else:
                    p.rchild = n.lchild
                del n
            else:  # 左右子树均不为空
                pre = n.rchild
                if pre.lchild is None:
                    n.data = pre.data
                    n.rchild = pre.rchild
                    del pre
                else:
                    next = pre.lchild
                    while next.lchild is not None:
                        pre = next
                        next = next.lchild
                    n.data = next.data
                    pre.lchild = next.rchild
                    del next
```

# **4. 遍历**

## **1. 先序遍历**

访问顺序如下：
- 访问根节点
- 先序遍历左子树
- 先序遍历右子树

以图 1为例，访问顺序为：4，2，1，3，5，6

```python
# 先序遍历
    def preOrderTraverse(self, node):
        if node is not None:
            print node.data,
            self.preOrderTraverse(node.lchild)
            self.preOrderTraverse(node.rchild)
```

## **2. 中序遍历**

访问顺序如下：
- 中序遍历左子树
- 访问根节点
- 中序遍历右子树

以图 1为例，访问顺序为：1，2，3，5，7，9

```python
# 中序遍历
    def inOrderTraverse(self, node):
        if node is not None:
            self.inOrderTraverse(node.lchild)
            print node.data,
            self.inOrderTraverse(node.rchild)
```

## **3. 后序遍历**

访问顺序如下：
- 后序遍历左子树
- 后序遍历右子树
- 访问根节点 

以图 1为例，访问顺序为：1，3，2，9，7，5

```python
# 后序遍历
    def postOrderTraverse(self, node):
        if node is not None:
            self.postOrderTraverse(node.lchild)
            self.postOrderTraverse(node.rchild)
            print node.data,
```

# **5. 完整代码**

```python
# encoding: utf-8
class Node:
    def __init__(self, data):
        self.data = data
        self.lchild = None
        self.rchild = None

class BST:
    def __init__(self, node_list):
        self.root = Node(node_list[0])
        for data in node_list[1:]:
            self.insert(data)

    # 搜索
    def search(self, node, parent, data):
        if node is None:
            return False, node, parent
        if node.data == data:
            return True, node, parent
        if node.data > data:
            return self.search(node.lchild, node, data)
        else:
            return self.search(node.rchild, node, data)

    # 插入
    def insert(self, data):
        flag, n, p = self.search(self.root, self.root, data)
        if not flag:
            new_node = Node(data)
            if data > p.data:
                p.rchild = new_node
            else:
                p.lchild = new_node

    # 删除
    def delete(self, root, data):
        flag, n, p = self.search(root, root, data)
        if flag is False:
            print "无该关键字，删除失败"
        else:
            if n.lchild is None:
                if n == p.lchild:
                    p.lchild = n.rchild
                else:
                    p.rchild = n.rchild
                del p
            elif n.rchild is None:
                if n == p.lchild:
                    p.lchild = n.lchild
                else:
                    p.rchild = n.lchild
                del p
            else:  # 左右子树均不为空
                pre = n.rchild
                if pre.lchild is None:
                    n.data = pre.data
                    n.rchild = pre.rchild
                    del pre
                else:
                    next = pre.lchild
                    while next.lchild is not None:
                        pre = next
                        next = next.lchild
                    n.data = next.data
                    pre.lchild = next.rchild
                    del p


    # 先序遍历
    def preOrderTraverse(self, node):
        if node is not None:
            print node.data,
            self.preOrderTraverse(node.lchild)
            self.preOrderTraverse(node.rchild)

    # 中序遍历
    def inOrderTraverse(self, node):
        if node is not None:
            self.inOrderTraverse(node.lchild)
            print node.data,
            self.inOrderTraverse(node.rchild)

    # 后序遍历
    def postOrderTraverse(self, node):
        if node is not None:
            self.postOrderTraverse(node.lchild)
            self.postOrderTraverse(node.rchild)
            print node.data,

a = [49, 38, 65, 97, 60, 76, 13, 27, 5, 1]
bst = BST(a)  # 创建二叉查找树
bst.inOrderTraverse(bst.root)  # 中序遍历

bst.delete(bst.root, 49)
print
bst.inOrderTraverse(bst.root)
```







