# 剑指offer——二叉树镜像、反转单词序列 - 刘炫320的博客 - CSDN博客
2017年06月11日 21:11:15[刘炫320](https://me.csdn.net/qq_35082030)阅读数：276
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 引言
这一次和之前的不太一样，因为这一次是同时写两题，因为这两题比较相近，而且各自没有太多可以挖掘的地方，就把它们放在一起了。
# 2. 二叉树镜像
## 2.1 问题描述
> 
操作给定的二叉树，将其变换为源二叉树的镜像。 
  二叉树的镜像定义：源二叉树  
              8 
             /  \ 
            6   10 
           / \  / \ 
          5  7 9 11 
          镜像二叉树 
              8 
             /  \ 
            10   6 
           / \  / \ 
          11 9 7  5
## 2.2 解决办法
遇见二叉树，基本上的思路都是递归、遍历，然后对三个部分，父节点和两个叶子节点进行操作。因此这个题目的思路就是，遍历一次，然后交换左右结点，然后分别遍历两个孩子结点。代码如下：
```java
//树节点的数据结构
class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;
    public TreeNode(int val) {
        this.val = val;
    }
}
//镜像二叉树
public static void Mirror(TreeNode root) {
        if(root!=null){
            TreeNode temp=root.left;
            root.left=root.right;
            root.right=temp;
            Mirror(root.left);
            Mirror(root.right);
        }
    }
```
# 3. 反转单词序列
## 3.1 问题描述
> 
最近来了一个新员工Fish，每天早晨总是会拿着一本英文杂志，写些句子在本子上。同事Cat对Fish写的内容颇感兴趣，有一天他向Fish借来翻看，但却读不懂它的意思。例如，“student. a am I”。后来才意识到，这家伙原来把句子单词的顺序翻转了，正确的句子应该是“I am a student.”。Cat对一一的翻转这些单词顺序可不在行，你能帮助他么？
## 3.2解决办法
可以看到这道题其实和第一道题有类似的地方，但又不相同。如果直接考虑反转整个字符串的话，其实是比较困难的，但是如果“负负得正”的话，这道题就非常容易了。而之所以能想到这种想法，可能需要的还是知识迁移能力了吧。代码如下：
```java
public static String ReverseSentence(String str) {
        StringBuffer sb = new StringBuffer(str);
            //令人惊奇的是，java里的String居然没有reverse()方法
            String stringnew=sb.reverse().toString();
            String[] strings=stringnew.split(" ");
            //这里要注意的是，如果原字符串是若干个空格组成，那么也要原样输出
            if(strings.length!=0){
                StringBuffer tempsb=new StringBuffer("");
                for(int i=0;i<strings.length;i++){
                    tempsb.append(new StringBuffer(strings[i]).reverse().toString());
                    tempsb.append(" ");
                }
                return tempsb.toString().trim();
            }
            else{
                return sb.toString();
            }   
    }
```
# 4. 小结
这两道题都比较简单，因此贵在见到过，会了，就行。当然它们巧妙的想法，还是值得我们学习的。
