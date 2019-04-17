# 输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年02月22日 15:31:13[QuJack](https://me.csdn.net/A784586)阅读数：2303








输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）


思路：递归，注意对空指针的判断很重要。






```java
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
    private boolean Doroot1hasroot2(TreeNode root1,TreeNode root2){
        if(root2==null){
            return true;
        }else if(root1==null){
            return false;
        }
        
       if(root1.val!=root2.val)
           return false;
        
        return Doroot1hasroot2(root1.left,root2.left) && Doroot1hasroot2(root1.right,root2.right);
       
    }
    
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        boolean result=false;
        if(root1!=null && root2!=null){
            if(root1.val==root2.val){
                result=Doroot1hasroot2(root1,root2);
            }
            
            if(!result){
                result=HasSubtree(root1.left,root2);
            }
            if(!result){
                result=HasSubtree(root1.right,root2);
            }
        }
        return result;
    }
}
```





