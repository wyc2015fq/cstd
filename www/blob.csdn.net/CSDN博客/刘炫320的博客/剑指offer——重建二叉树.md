# 剑指offer——重建二叉树 - 刘炫320的博客 - CSDN博客
2017年07月06日 21:29:30[刘炫320](https://me.csdn.net/qq_35082030)阅读数：252
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
# 2. 解决方法
这个问题其实在学习数据结构的时候这道题是非常经典的题目，当时还是手算的，不仅是给出前序和中序，还有给出中序和后序的，所以原理上我们是懂的。但是对于二叉树来讲，一定是要给出中序的，因为先序和后序给我们提供的信息是一样的–告诉我们谁是根节点，中序则告诉我们左右子树在哪儿。
## 2.1 level1
这一张图说的很清楚了，如下图所示： 
![这里写图片描述](https://uploadfiles.nowcoder.com/images/20170605/8788852_1496674193455_D60F8B66AA5C0250BB4D8DDD6DD73335)
我们知道，前序遍历的第一个是根节点，其后的若干个为左子树，省下的就是右子树。而对于中序遍历来讲，根结点（也就是前序遍历中的第一个数）的左边是左子树，其数目可以确定，右边是右子树，其数目也可以确定。因此我们就照这个思路来，但是由于题目给出的接口不好，因此才写了如下的程序：
```java
public static TreeNode reConstructBinaryTree(int [] pre,int [] in) {
        if(pre==null||in==null){
            return null;
        }
        if(pre.length!=in.length){
            return null;
        }
        if (pre.length==0&&in.length==0){
            return null;
        }
        //前序序列第一个是头
        int parent=pre[0];
        //左孩子
        java.util.ArrayList<Integer> inleftchild=new java.util.ArrayList<Integer>(); 
        //右孩子
        java.util.ArrayList<Integer> inrightchild=new java.util.ArrayList<Integer>(); 
        boolean flag=false;
        int postion=0;
        //找到中序的中间那个
        for(int i=0;i<in.length;i++){
            if(in[i]==parent){
                flag=true;
                postion=i;
            }
            else{
                if(flag==false){
                    inleftchild.add(in[i]);
                }
                else{
                    inrightchild.add(in[i]);
                }
            }
        }
        //左孩子
        java.util.ArrayList<Integer> preleftchild=new java.util.ArrayList<Integer>(); 
        //右孩子
        java.util.ArrayList<Integer> prerightchild=new java.util.ArrayList<Integer>();
        for(int i=1;i<pre.length;i++){
            if(i<=postion){
                preleftchild.add(pre[i]);
            }
            else{
                prerightchild.add(pre[i]);
            }
        }
        TreeNode tnNode=new TreeNode(parent);
        int[] prel=new int[preleftchild.size()];
        for(int i=0;i<preleftchild.size();i++){
            prel[i]=preleftchild.get(i);
        }
        int[] prer=new int[prerightchild.size()];
        for(int i=0;i<prerightchild.size();i++){
            prer[i]=prerightchild.get(i);
        }
        int[] inl=new int[inleftchild.size()];
        for(int i=0;i<inleftchild.size();i++){
            inl[i]=inleftchild.get(i);
        }
        int[] inr=new int[inrightchild.size()];
        for(int i=0;i<inrightchild.size();i++){
            inr[i]=inrightchild.get(i);
        }
        tnNode.left=reConstructBinaryTree(prel,inl);
        tnNode.right=reConstructBinaryTree(prer, inr);
        return tnNode;
    }
```
# 2.2 level2
这么看来实在是太长了，为什么，因为这个接口不好，那么如果写成我们习惯性的接口的样子，是不是就会简单了许多，没错：
```
public static TreeNode reConstructBinaryTree(int [] pre,int [] in) {
        TreeNode root=reConstructBinaryTree(pre,0,pre.length-1,in,0,in.length-1);
        return root;
    }
    //前序遍历{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}
    private static TreeNode reConstructBinaryTree(int [] pre,int startPre,int endPre,int [] in,int startIn,int endIn) {
        if(startPre>endPre||startIn>endIn)
            return null;
        TreeNode root=new TreeNode(pre[startPre]);
        for(int i=startIn;i<=endIn;i++)
            if(in[i]==pre[startPre]){
                root.left=reConstructBinaryTree(pre,startPre+1,startPre+i-startIn,in,startIn,i-1);
                root.right=reConstructBinaryTree(pre,i-startIn+startPre+1,endPre,in,i+1,endIn);
                break;
            }
        return root;
    }
```
这个写法的最大亮点在于递归的时机，不用像第一次那样需要记录，而是立即递归，并且最优的break极大的减少了时间复杂度。
# 2.3 level3
事实上，仍然可以按照我们原来的思路做下去的，只不过样子是这样的：
```
public static TreeNode reConstructBinaryTree(int [] pre,int [] in) {
           if(pre.length == 0||in.length == 0){
                return null;
            }
            TreeNode nodes = new TreeNode(pre[0]);
            for(int i = 0; i < in.length; i++){
                if(pre[0] == in[i]){
                    nodes.left = reConstructBinaryTree(java.util.Arrays.copyOfRange(pre, 1, i+1), java.util.Arrays.copyOfRange(in, 0, i));
                    nodes.right = reConstructBinaryTree(java.util.Arrays.copyOfRange(pre, i+1, pre.length), java.util.Arrays.copyOfRange(in, i+1,in.length));
                    break;
                }
            }
            return nodes;
        }
```
使用java.util.Arrays.copyOfRange（）函数就可以完成我们所需要的数组截取。
# 3. 小结
通过上面的题目，我们首先是可以巩固树的操作（我在这块是非常薄弱的），另外可以熟悉java的数组操作，以免走弯路。另外说一点，二叉树是一定要中序的，但是二叉搜索树是不需要中序的，因为它本身就告诉了我们中序（依靠节点的大小）。（2014年阿里巴巴题目）
