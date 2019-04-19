# 剑指offer——从上往下打印二叉树 - 刘炫320的博客 - CSDN博客
2017年07月15日 21:07:48[刘炫320](https://me.csdn.net/qq_35082030)阅读数：204
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
从上往下打印出二叉树的每个节点，同层节点从左至右打印。
# 2. 解决方法
其实这道题就是二叉树的层次遍历。这个在学数据结构的时候就已经考察了。其主要思想就是建立一个队列，然后递归的入队出队即可。
## 2.1 level1
按照上述思想，如下展示：
```
ArrayList<Integer> alArrayList=new ArrayList<Integer>();
    Queue<TreeNode> queue=new LinkedList<TreeNode>();
    public ArrayList<Integer> PrintFromTopToBottom(TreeNode root) {
        Recursive(root);
        return alArrayList;
    }
    public void Recursive(TreeNode root){
        if(root==null){
            if(!queue.isEmpty())
                Recursive(queue.poll());
            return ;
        }
        else{
            alArrayList.add(root.val);
            queue.add(root.left);
            queue.add(root.right);
            Recursive(queue.poll());
        }
    }
```
## 2.2 level2
但是这里有一个问题，它需要递归实现，有可能出现栈溢出的问题。因此改用循环实现：
```
public ArrayList<Integer> PrintFromTopToBottom(TreeNode root) {
        ArrayList<Integer> list = new ArrayList<Integer>();
        if(root==null){
            return list;
        }
        Queue<TreeNode> queue = new LinkedList<TreeNode>();
        queue.offer(root);
        while (!queue.isEmpty()) {
            TreeNode treeNode = queue.poll();
            if (treeNode.left != null) {
                queue.offer(treeNode.left);
            }
            if (treeNode.right != null) {
                queue.offer(treeNode.right);
            }
            list.add(treeNode.val);
        }
        return list;
    }
```
## 2.3 level3
事实上第一个代码质量不高，第二个仍然依靠了队列，通过递归的手段，完全可以不用队列实现：
```php
ArrayList<Integer> list=new ArrayList<Integer>();
    public ArrayList<Integer> PrintFromTopToBottom(TreeNode root) {
        if(root!=null)
            list.add(root.val);
        print(root);
        return list;
    }
    public  void print(TreeNode root){
        if(root!=null){
            if(root.left!=null)
                list.add(root.left.val);
            if(root.right!=null)
                list.add(root.right.val);
            print(root.left);
            print(root.right);
        }
```
# 3. 小结
本章中我们主要复习了二叉树的层次遍历，也成为广度遍历。其实基础很重要。不知道有没有同学注意到我在两个代码中的队列的添加一个使用了add，一个使用了offer。事实上他们是有区别的：《[队列的几个相似的用法](http://blog.csdn.net/ustcjackylau/article/details/42454779)》。
