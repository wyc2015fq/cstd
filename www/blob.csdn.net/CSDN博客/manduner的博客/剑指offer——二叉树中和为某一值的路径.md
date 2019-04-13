
# 剑指offer——二叉树中和为某一值的路径 - manduner的博客 - CSDN博客


2019年04月01日 22:57:12[Manduner_TJU](https://me.csdn.net/manduner)阅读数：12


# 1,问题描述
输入一颗二叉树的跟节点和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的根结点开始往下一直到叶结点所经过的结点形成一条路径。(注意: 在返回值的list中，数组长度大的数组靠前)
# 2，解题思路
## （1）注意事项
满足该题条件的路径一定是从根节点到叶节点，也就是说，即使根节点到某个非叶子节点的节点值之和等于目标值，那该条路径也不能算作本题定义的路径。
如果你采用的是深度优先遍历，那么该题括号中的注意部分是可以忽略的。因为输入的是树的层次遍历序列，所以，左子树的深度一定大于等于右子树的深度，所以先深度优先遍历左子树再遍历右子树，所得到满足条件的数组长度一定是从大到小。
深度优先遍历要时刻注意节点回退的问题，节点回退的操作在源码中有标注。
本题源码中有几处注意事项，源码中已经标注。
## （2）思路分析
从根节点开始遍历，每遍历一个节点，先将该节点添加进list中，再判断一下是否已经满足条件，若已经满足，则将该数组添加进大的list中，并进行回退；否则，继续递归遍历该节点的左子树以及右子树，遍历完一个子树后要进行节点回退。
一种思路两种实现方案，第一种方案进行了各种判断，目的是减少递归的次数。第二种方案实现起来简单粗暴，几乎没有任何判断，但是递归的次数可能比第一种方案要多。
# 3，源码
```python
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
    private ArrayList<ArrayList<Integer>> arrayList = new ArrayList<ArrayList<Integer>>();
    private ArrayList<Integer> list = new ArrayList<Integer>();
    public ArrayList<ArrayList<Integer>> FindPath(TreeNode root,int target) {
        if(root==null) return arrayList;
        //findPath(root,target);
        findPath1(root,target);
        return arrayList;
    }
    //方案一
    public void findPath(TreeNode root, int target){
        //如果root.val>target，说明没有满足的路径
        if(root==null || root.val>target) return;
        target -= root.val;
        list.add(root.val);
        if(target>0) {
            if(root.left!=null) findPath(root.left, target);
            if(root.right!=null) findPath(root.right, target);
            //遍历到叶节点后要进行节点回退。
            list.remove(list.size()-1);
        }else if(target==0 && root.left==null && root.right==null) {
            //new ArrayList<Integer>(list)是分配一个新的引用指向现有的list，
            //目的是不改变现有的list，方便以后可以继续操作该list
            arrayList.add(new ArrayList<Integer>(list));
            //找到满足条件的路径后要进行节点回退。
            list.remove(list.size()-1);
        }else {
            //没有找到满足条件的路径，也要进行节点回退。
            list.remove(list.size()-1);
        }
    }
    //方案二：
    private void findPath1(TreeNode root, int target) {
        if(root==null) return;
        target -= root.val;
        list.add(root.val);
        if(target==0 && root.left==null && root.right==null) {
            arrayList.add(new ArrayList<Integer>(list));
        }
        findPath1(root.left, target);
        findPath1(root.right, target);
        list.remove(list.size()-1);
    }
}
```


