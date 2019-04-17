# 数据结构与算法（二）:线性表、栈、树(二叉树，AVL树)、图 - Machine Learning with Peppa - CSDN博客





2018年01月17日 13:56:28[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：273
所属专栏：[有趣的算法](https://blog.csdn.net/column/details/19022.html)









### 三、树与二叉树

树型结构是一类非常重要的非线性数据结构，其中以树和二叉树最为常用。在介绍二叉树之前，我们先简单了解一下树的相关内容。



树


树 是由n（n>=1）个有限节点组成一个具有层次关系的集合。它具有以下特点：每个节点有零个或多个子节点；没有父节点的节点称为 根 节点；每一个非根节点有且只有一个 父节点 ；除了根节点外，每个子节点可以分为多个不相交的子树。



树的结构

二叉树基本概念



定义

二叉树是每个节点最多有两棵子树的树结构。通常子树被称作“左子树”和“右子树”。二叉树常被用于实现二叉查找树和二叉堆。

相关性质

二叉树的每个结点至多只有2棵子树(不存在度大于2的结点)，二叉树的子树有左右之分，次序不能颠倒。

二叉树的第i层至多有2(i-1)个结点；深度为k的二叉树至多有2k-1个结点。

一棵深度为k，且有2^k-1个节点的二叉树称之为** 满二叉树 **；


深度为k，有n个节点的二叉树，当且仅当其每一个节点都与深度为k的满二叉树中，序号为1至n的节点对应时，称之为完全二叉树 。



三种遍历方法

在二叉树的一些应用中，常常要求在树中查找具有某种特征的节点，或者对树中全部节点进行某种处理，这就涉及到二叉树的遍历。二叉树主要是由3个基本单元组成，根节点、左子树和右子树。如果限定先左后右，那么根据这三个部分遍历的顺序不同，可以分为先序遍历、中序遍历和后续遍历三种。



(1) 先序遍历 若二叉树为空，则空操作，否则先访问根节点，再先序遍历左子树，最后先序遍历右子树。 (2) 中序遍历 若二叉树为空，则空操作，否则先中序遍历左子树，再访问根节点，最后中序遍历右子树。(3) 后序遍历 若二叉树为空，则空操作，否则先后序遍历左子树访问根节点，再后序遍历右子树，最后访问根节点。



给定二叉树写出三种遍历结果

树和二叉树的区别

(1) 二叉树每个节点最多有2个子节点，树则无限制。 (2) 二叉树中节点的子树分为左子树和右子树，即使某节点只有一棵子树，也要指明该子树是左子树还是右子树，即二叉树是有序的。 (3) 树决不能为空，它至少有一个节点，而一棵二叉树可以是空的。



上面我们主要对二叉树的相关概念进行了介绍，下面我们将从二叉查找树开始，介绍二叉树的几种常见类型，同时将之前的理论部分用代码实现出来。



#### 二叉查找树


定义

二叉查找树就是二叉排序树，也叫二叉搜索树。二叉查找树或者是一棵空树，或者是具有下列性质的二叉树： (1) 若左子树不空，则左子树上所有结点的值均小于它的根结点的值；(2) 若右子树不空，则右子树上所有结点的值均大于它的根结点的值；(3) 左、右子树也分别为二叉排序树；(4) 没有键值相等的结点。



典型的二叉查找树的构建过程

性能分析

对于二叉查找树来说，当给定值相同但顺序不同时，所构建的二叉查找树形态是不同的，下面看一个例子。



不同形态平衡二叉树的ASL不同

可以看到，含有n个节点的二叉查找树的平均查找长度和树的形态有关。最坏情况下，当先后插入的关键字有序时，构成的二叉查找树蜕变为单支树，树的深度为n，其平均查找长度(n+1)/2(和顺序查找相同），最好的情况是二叉查找树的形态和折半查找的判定树相同，其平均查找长度和log2(n)成正比。平均情况下，二叉查找树的平均查找长度和logn是等数量级的，所以为了获得更好的性能，通常在二叉查找树的构建过程需要进行“平衡化处理”，之后我们将介绍平衡二叉树和红黑树，这些均可以使查找树的高度为O(log(n))。



代码10 二叉树的节点



class TreeNode<E> {



    E element;

    TreeNode<E> left;

    TreeNode<E> right;



    public TreeNode(E e) {

        element = e;

    }

}

二叉查找树的三种遍历都可以直接用递归的方法来实现：



代码12 先序遍历

protected void preorder(TreeNode<E> root) {



    if (root == null)

        return;



    System.out.println(root.element + " ");



    preorder(root.left);



    preorder(root.right);

}

代码13 中序遍历

protected void inorder(TreeNode<E> root) {



    if (root == null)

        return;



    inorder(root.left);



    System.out.println(root.element + " ");



    inorder(root.right);

}

代码14 后序遍历

protected void postorder(TreeNode<E> root) {



    if (root == null)

        return;



    postorder(root.left);



    postorder(root.right);



    System.out.println(root.element + " ");

}

代码15 二叉查找树的简单实现

/**

 * @author JackalTsc

 */

public class MyBinSearchTree<E extends Comparable<E>> {



    // 根

    private TreeNode<E> root;



    // 默认构造函数

    public MyBinSearchTree() {

    }



    // 二叉查找树的搜索

    public boolean search(E e) {



        TreeNode<E> current = root;



        while (current != null) {



            if (e.compareTo(current.element) < 0) {

                current = current.left;

            } else if (e.compareTo(current.element) > 0) {

                current = current.right;

            } else {

                return true;

            }

        }



        return false;

    }



    // 二叉查找树的插入

    public boolean insert(E e) {



        // 如果之前是空二叉树 插入的元素就作为根节点

        if (root == null) {

            root = createNewNode(e);

        } else {

            // 否则就从根节点开始遍历 直到找到合适的父节点

            TreeNode<E> parent = null;

            TreeNode<E> current = root;

            while (current != null) {

                if (e.compareTo(current.element) < 0) {

                    parent = current;

                    current = current.left;

                } else if (e.compareTo(current.element) > 0) {

                    parent = current;

                    current = current.right;

                } else {

                    return false;

                }

            }

            // 插入

            if (e.compareTo(parent.element) < 0) {

                parent.left = createNewNode(e);

            } else {

                parent.right = createNewNode(e);

            }

        }

        return true;

    }



    // 创建新的节点

    protected TreeNode<E> createNewNode(E e) {

        return new TreeNode(e);

    }



}



// 二叉树的节点

class TreeNode<E extends Comparable<E>> {



    E element;

    TreeNode<E> left;

    TreeNode<E> right;



    public TreeNode(E e) {

        element = e;

    }

}



上面的代码15主要展示了一个自己实现的简单的二叉查找树，其中包括了几个常见的操作，当然更多的操作还是需要大家自己去完成。因为在二叉查找树中删除节点的操作比较复杂，所以下面我详细介绍一下这里。



二叉查找树中删除节点分析

要在二叉查找树中删除一个元素，首先需要定位包含该元素的节点，以及它的父节点。假设current指向二叉查找树中包含该元素的节点，而parent指向current节点的父节点，current节点可能是parent节点的左孩子，也可能是右孩子。这里需要考虑两种情况：



current节点没有左孩子，那么只需要将patent节点和current节点的右孩子相连。

current节点有一个左孩子，假设rightMost指向包含current节点的左子树中最大元素的节点，而parentOfRightMost指向rightMost节点的父节点。那么先使用rightMost节点中的元素值替换current节点中的元素值，将parentOfRightMost节点和rightMost节点的左孩子相连，然后删除rightMost节点。

    // 二叉搜索树删除节点

    public boolean delete(E e) {



        TreeNode<E> parent = null;

        TreeNode<E> current = root;



        // 找到要删除的节点的位置

        while (current != null) {

            if (e.compareTo(current.element) < 0) {

                parent = current;

                current = current.left;

            } else if (e.compareTo(current.element) > 0) {

                parent = current;

                current = current.right;

            } else {

                break;

            }

        }



        // 没找到要删除的节点

        if (current == null) {

            return false;

        }



        // 考虑第一种情况

        if (current.left == null) {

            if (parent == null) {

                root = current.right;

            } else {

                if (e.compareTo(parent.element) < 0) {

                    parent.left = current.right;

                } else {

                    parent.right = current.right;

                }

            }

        } else { // 考虑第二种情况

            TreeNode<E> parentOfRightMost = current;

            TreeNode<E> rightMost = current.left;

            // 找到左子树中最大的元素节点

            while (rightMost.right != null) {

                parentOfRightMost = rightMost;

                rightMost = rightMost.right;

            }



            // 替换

            current.element = rightMost.element;



            // parentOfRightMost和rightMost左孩子相连

            if (parentOfRightMost.right == rightMost) {

                parentOfRightMost.right = rightMost.left;

            } else {

                parentOfRightMost.left = rightMost.left;

            }

        }



        return true;

    }




### 平衡二叉树


平衡二叉树又称AVL树，它或者是一棵空树，或者是具有下列性质的二叉树：它的左子树和右子树都是平衡二叉树，且左子树和右子树的深度之差的绝对值不超过1。



平衡二叉树

AVL树是最先发明的自平衡二叉查找树算法。在AVL中任何节点的两个儿子子树的高度最大差别为1，所以它也被称为高度平衡树，n个结点的AVL树最大深度约1.44log2n。查找、插入和删除在平均和最坏情况下都是O（log n）。增加和删除可能需要通过一次或多次树旋转来重新平衡这个树。



红黑树



红黑树是平衡二叉树的一种，它保证在最坏情况下基本动态集合操作的事件复杂度为O(log n)。红黑树和平衡二叉树区别如下：(1) 红黑树放弃了追求完全平衡，追求大致平衡，在与平衡二叉树的时间复杂度相差不大的情况下，保证每次插入最多只需要三次旋转就能达到平衡，实现起来也更为简单。(2) 平衡二叉树追求绝对平衡，条件比较苛刻，实现起来比较麻烦，每次插入新节点之后需要旋转的次数不能预知。点击查看更多



### 四、图

简介

图是一种较线性表和树更为复杂的数据结构，在线性表中，数据元素之间仅有线性关系，在树形结构中，数据元素之间有着明显的层次关系，而在图形结构中，节点之间的关系可以是任意的，图中任意两个数据元素之间都可能相关。图的应用相当广泛，特别是近年来的迅速发展，已经渗入到诸如语言学、逻辑学、物理、化学、电讯工程、计算机科学以及数学的其他分支中。



相关阅读

因为图这部分的内容还是比较多的，这里就不详细介绍了，有需要的可以自己搜索相关资料。



(1) 《百度百科对图的介绍》

(2) 《数据结构之图（存储结构、遍历）》



五、总结

到这里，关于常见的数据结构的整理就结束了，断断续续大概花了两天时间写完，在总结的过程中，通过查阅相关资料，结合书本内容，收获还是很大的，在下一篇博客中将会介绍常用数据结构与算法整理总结（下）之算法篇，欢迎大家关注。



文章同样收录在我的个人网站中：[点击打开链接](http://118.126.108.52/index.php/2018/01/16/%E4%BA%86%E8%A7%A3%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E7%AE%97%E6%B3%95%E7%BA%BF%E6%80%A7%E8%A1%A8%E6%95%B0%E7%BB%84%EF%BC%8C%E9%93%BE%E8%A1%A8%E3%80%81%E6%A0%88%E3%80%81%E6%A0%91/)



更多干货内容欢迎访问~
            


