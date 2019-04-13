
# 二叉树反转java实现 - bitcarmanlee的博客 - CSDN博客


2017年08月05日 18:07:06[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：907


反转二叉树是数据结构中一种经典的操作。如下图所以，反转二叉树就是交换所有节点的左右子树。
![这里写图片描述](https://img-blog.csdn.net/20170805180242962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
具体代码实现如下：
节点类
```python
/**
 * Created by wanglei on 17/8/5.
 */
```
```python
public
```
```python
class
```
```python
TreeNode
```
```python
{
```
```python
int
```
```python
val =
```
```python
0
```
```python
;
    TreeNode left =
```
```python
null
```
```python
;
    TreeNode right =
```
```python
null
```
```python
;
```
```python
public
```
```python
TreeNode
```
```python
(
```
```python
int
```
```python
val) {
```
```python
this
```
```python
.val = val;
    }
}
```
```python
import java.util.LinkedList;
import java.util.List;
```
```python
/**
 * Created by wanglei on 17/8/5.
 */
```
```python
public
```
```python
class
```
```python
Solution {
```
```python
private
```
```python
int
```
```python
[] array = {
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
7
```
```python
,
```
```python
8
```
```python
,
```
```python
9
```
```python
};
```
```python
private
```
```python
static
```
```python
List<TreeNode> nodeList =
```
```python
null
```
```python
;
```
```python
public
```
```python
void
```
```python
createTree
```
```python
() {
        nodeList =
```
```python
new
```
```python
LinkedList();
```
```python
for
```
```python
(
```
```python
int
```
```python
index =
```
```python
0
```
```python
; index < array.length; index++) {
            nodeList.add(
```
```python
new
```
```python
TreeNode(array[index]));
        }
```
```python
//最后一个父节点在数组中的索引
```
```python
int
```
```python
lastParentIndex = array.length /
```
```python
2
```
```python
-
```
```python
1
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
parentInex =
```
```python
0
```
```python
; parentInex < lastParentIndex; parentInex++) {
            nodeList.
```
```python
get
```
```python
(parentInex).left = nodeList.
```
```python
get
```
```python
(parentInex *
```
```python
2
```
```python
+
```
```python
1
```
```python
);
            nodeList.
```
```python
get
```
```python
(parentInex).right = nodeList.
```
```python
get
```
```python
(parentInex *
```
```python
2
```
```python
+
```
```python
2
```
```python
);
        }
```
```python
// 最后一个父节点:因为最后一个父节点可能没有右孩子，所以单独拿出来处理
```
```python
// 左孩子
```
```python
nodeList.
```
```python
get
```
```python
(lastParentIndex).left = nodeList.
```
```python
get
```
```python
(lastParentIndex *
```
```python
2
```
```python
+
```
```python
1
```
```python
);
```
```python
// 右孩子
```
```python
if
```
```python
(array.length %
```
```python
2
```
```python
==
```
```python
1
```
```python
) {
            nodeList.
```
```python
get
```
```python
(lastParentIndex).right = nodeList.
```
```python
get
```
```python
(lastParentIndex *
```
```python
2
```
```python
+
```
```python
2
```
```python
);
        }
    }
```
```python
// 层次遍历
```
```python
public
```
```python
void
```
```python
levelTraverse
```
```python
(TreeNode root) {
```
```python
if
```
```python
(root ==
```
```python
null
```
```python
)
```
```python
return
```
```python
;
        LinkedList<TreeNode> list =
```
```python
new
```
```python
LinkedList<>();
        list.add(root);
```
```python
while
```
```python
(list.size() !=
```
```python
0
```
```python
) {
            TreeNode node = list.removeFirst();
            System.
```
```python
out
```
```python
.print(node.val +
```
```python
" "
```
```python
);
```
```python
if
```
```python
(node.left !=
```
```python
null
```
```python
) {
                list.add(node.left);
            }
```
```python
if
```
```python
(node.right !=
```
```python
null
```
```python
) {
                list.add(node.right);
            }
        }
        System.
```
```python
out
```
```python
.println();
    }
```
```python
public
```
```python
TreeNode
```
```python
invertTree
```
```python
(TreeNode root) {
```
```python
if
```
```python
(root ==
```
```python
null
```
```python
)
```
```python
return
```
```python
null
```
```python
;
        TreeNode tmp = root.left;
        root.left = root.right;
        root.right = tmp;
        invertTree(root.left);
        invertTree(root.right);
```
```python
return
```
```python
root;
    }
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args) {
        Solution solution =
```
```python
new
```
```python
Solution();
        solution.createTree();
        solution.levelTraverse(nodeList.
```
```python
get
```
```python
(
```
```python
0
```
```python
));
        TreeNode newRoot = solution.invertTree(nodeList.
```
```python
get
```
```python
(
```
```python
0
```
```python
));
        solution.levelTraverse(newRoot);
    }
}
```
最后输出的结果为：
```python
1 2 3 4 5 6 7 8 9 
1 3 2 7 6 5 4 9 8
```

