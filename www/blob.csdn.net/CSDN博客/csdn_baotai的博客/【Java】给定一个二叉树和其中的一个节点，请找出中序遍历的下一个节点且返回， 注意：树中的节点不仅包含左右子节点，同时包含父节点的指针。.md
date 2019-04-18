# 【Java】给定一个二叉树和其中的一个节点，请找出中序遍历的下一个节点且返回，  注意：树中的节点不仅包含左右子节点，同时包含父节点的指针。 - csdn_baotai的博客 - CSDN博客

2018年08月20日 00:14:29[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：95


# 题目描述：

##        给定一个二叉树和其中的一个节点，请找出中序遍历的下一个节点且返回，注意：树中的节点不仅包含左右子节点，同时包含父节点的指针。

# 解题思路：

对于二叉树中所有的节点，可以大致分为两类：

1、有右子树的，那么下个结点就是右子树最左边的点；

2、没有右子树的，也可以分成两类

            a)是父节点左孩子 ，那么父节点就是下一个节点 ；

            b)是父节点的右孩子，找他的父节点的父节点的父节点直到当前结点是其父节点的左孩子位置。如果没有，那么他就是尾节点。

```java
package Test2018_8_19_BinaryTree_InorderTraversal;

/**
 * 给定一个二叉树和其中的一个节点，请找出中序遍历的下一个节点且返回，
 * 注意：树中的节点不仅包含左右子节点，同时包含父节点的指针。
 * @author 李宝泰
 *
 */
class TreeLinkNode{
	int val;
	TreeLinkNode left;
	TreeLinkNode right;
	TreeLinkNode next;//指向父节点  这个对于解题很重要
	public TreeLinkNode(int val) {
		super();
		this.val = val;
	}
}
//
//1、有右子树的，那么下个结点就是右子树最左边的点；
//2、没有右子树的，也可以分成两类，a)是父节点左孩子 ，那么父节点就是下一个节点 ；
//			        b)是父节点的右孩子，找他的父节点的父节点的父节点...直到当前结点是其父节点的左孩子位置。如果没有，那么他就是尾节点。
//                        

class InorderTraversalOfNext {
	public TreeLinkNode GetNext(TreeLinkNode pNode){
        if(pNode==null)
            return null;
        //有右子树的，那么下个结点就是右子树最左边的点；
        if(pNode.right!=null)
        {
            pNode=pNode.right;
            while(pNode.left != null)
                pNode=pNode.left;
            return pNode;
        } 
        //是父节点的右孩子，找他的父节点的父节点的父节点...直到当前结点是其父节点的左孩子位置
        while(pNode.next!=null)
        {
            TreeLinkNode proot=pNode.next;
            if(proot.left==pNode)
                return proot;
            pNode=pNode.next;
        }
        //如果没有，那么他就是尾节点。
        return null;
    }
}
```

