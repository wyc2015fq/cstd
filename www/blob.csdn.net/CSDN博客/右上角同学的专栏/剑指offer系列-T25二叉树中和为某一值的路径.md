# 剑指offer系列-T25二叉树中和为某一值的路径 - 右上角同学的专栏 - CSDN博客
2016年07月15日 00:25:58[右上角徐](https://me.csdn.net/u011032983)阅读数：215
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
      本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
import java.util.ArrayList;
/**
 * @author xhl 
 * 二叉树中和为某一值的路径 
 * 题目描述 
 * 输入一颗二叉树和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的
 * 根结点开始往下一直到叶结点所经过的结点形成一条路径。
 * 思路：二叉树的大多数问题都可递归解决。本题，递归结束的条件是到达叶子节点，和非叶子结点值 root.val > target
 */
public class offerT25 {
/**
* @param args
*/
ArrayList<ArrayList<Integer>> list = new ArrayList<ArrayList<Integer>>();
ArrayList<Integer> Route = new ArrayList<Integer>();
public void FindPath(BinaryTreeNode root, int target) {
if (root == null || target < 0)
System.out.print("error");
else {
/*叶子节点处*/
if (root.left == null && root.right == null) {
if (root.val == target) {
Route.add(root.val);
System.out.print(Route);
Route.remove(Route.size() - 1);
}
}
/*非叶子节点处*/
else if (root.val < target) {
Route.add(root.val);
if (root.left != null) {
FindPath(root.left, target - root.val);
//Route.remove(Route.size() - 1);
}
if (root.right != null) {
//Route.add(root.val);
FindPath(root.right, target - root.val);
}
Route.remove(Route.size() - 1);
}
}
}
public static void main(String[] args) {
// TODO Auto-generated method stub
offerT25 o = new offerT25();
BinaryTreeNode target1 = new BinaryTreeNode(1);
BinaryTreeNode target2 = new BinaryTreeNode(2);
BinaryTreeNode target3 = new BinaryTreeNode(3);
BinaryTreeNode target4 = new BinaryTreeNode(4);
BinaryTreeNode target5 = new BinaryTreeNode(5);
BinaryTreeNode target6 = new BinaryTreeNode(7);
BinaryTreeNode target7 = new BinaryTreeNode(7);
target1.left = target2;
target1.right = target3;
target2.left = target4;
target2.right = target5;
target3.left = target6;
target3.right = target7;
o.FindPath(target1, 11);
}
}
