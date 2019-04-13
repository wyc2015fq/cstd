
# 剑指offer——树的子结构 - manduner的博客 - CSDN博客


2019年03月18日 20:46:41[Manduner_TJU](https://me.csdn.net/manduner)阅读数：17


# 1，问题描述
输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）
# 2，解题思路（转自：
# [完美风暴4](https://blog.csdn.net/jsqfengbao/article/details/47191543)
# ）
![](https://img-blog.csdnimg.cn/20190318204156488.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
（1）要查找树A中是否存在和树B结构一样的子树，我们可以分两步：第一步在树A中找到和B的根节点的值一样的结点R，第二步再判断树A中以R为根结点的子树是不是包含和树B一样的结构。
（2）以上面的两棵树为例来详细分析这个过程。首先我们试着在树A中找到值为8的结点。从树A的根节点开始遍历，我们发现它的根节点的值就是8。接着我们就去判断树A的根节点下面的子树是不是含有和树B一样的结构。在树A中，根节点的左子结点的值是8，而树B的根节点的左子结点是9，对应的两个结点不同。
（3）因此我们仍然要遍历A，接着查找值为8的结点。我们在树的第二层中找到一个值为8 的结点，然后进行第二步的判断，即判断这个结点下面的子树是否含有和树B一样结构的子树。于是我们遍历这个结点下面的子树，先后得到两个子节点9和2，这和树B的结构完全相同。此时我们在树A中找到了一个和树B的结构一样的子树，因此树B和树A的子结构。
在面试的时候，我们一定要检查边界条件，即检查空指针。当树A或树B为空的时候，定义相同的输出。如果没有检查并做响应的处理，程序非常容易崩溃，这是面试的时候非常忌讳的事情。
# 3，源码
```python
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
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        if(root1 == null || root2 == null) return false;
        boolean flag = false;
        if(root1.val == root2.val) {
            flag = equalTree(root1.left, root2.left) ? 
                equalTree(root1.right, root2.right) : false;
        }
        if(!flag) {
            flag = HasSubtree(root1.left, root2) ? true : HasSubtree(root1.right, root2);
        }
        return flag;
    }
    public boolean equalTree(TreeNode root1, TreeNode root2) {
        if(root1 == null && root2 != null) return false;
        if(root2 == null) return true;
        if(root1.val == root2.val) {
            return equalTree(root1.left, root2.left) && equalTree(root1.right, root2.right);
        }else {
            return false;
        }
    }
}
```
# 4，番外篇
另一种思想：先将两个二叉树进行中序遍历，再进行字符串匹配，以下是源码（源码有问题，待解决。。。。em。。。。觉得思路是没问题的，但是找不到源码中哪出了问题）
```python
import java.util.ArrayList;	
//方法一：先中序遍历，再字符串匹配
public class Solution {
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        if(root1 == null || root2 == null) return false;
        ArrayList<Integer> array1 = inorderTraversal(root1);
        ArrayList<Integer> array2 = inorderTraversal(root2);
        int n = array1.size();
        int m = array2.size();
        boolean flag = false;
        if(m<=n) {
        	flag = HasSubSequence(array2,m,array1,n);
        }else {
        	flag = HasSubSequence(array1,n,array2,m);
        }
        
        return flag;
    }
    
//	    //中序遍历方法一（需要返回遍历序列的情况下）:空间复杂度太高
//	    public ArrayList<Integer> centerSearch(TreeNode root){
//	        ArrayList<Integer> array = new ArrayList<Integer>();
//	        if(root.left != null){
//	            array.addAll(centerSearch(root.left));
//	        }else{
//	            array.add(root.val);
//	            if(root.right != null) {
//	                array.addAll(centerSearch(root.right));
//	            }
//	        }
//	        return array;
//	    }
    
    //中序遍历方法二（需要返回遍历序列的情况下）：推荐使用
    public ArrayList<Integer> inorderTraversal(TreeNode root) {
    	ArrayList<Integer> resultList = new ArrayList<Integer>();
    	if(root==null) return resultList;
    	creatSequence(resultList, root);
    	return resultList;
    };
    
    public void creatSequence(ArrayList<Integer> resultList, TreeNode root) {
    	if(root.left != null) creatSequence(resultList, root.left);
    	resultList.add(root.val);
    	if(root.right != null) creatSequence(resultList, root.right);
    	return;
    }
    
    public void creatSequence2(ArrayList<Integer> resultList, TreeNode root) {
    	if(root==null) return;
    	creatSequence2(resultList, root.left);
    	resultList.add(root.val);
    	creatSequence2(resultList, root.right);
    }
    
    
    
    public boolean HasSubSequence(ArrayList<Integer> array1, int m, 
    		ArrayList<Integer> array2, int n) {
    	boolean flag = false;
    	int count = 0;
    	int j = 0;
    	for(int i = 0; i<n; i++) {
    		if(array2.get(i) != array1.get(j)) {
    			j = 0;
    			if(count>0) {
    				i--;
    				count=0;
    			}
    		}else {
    			count++;
    			j++;
    			if(count == m) {
    				flag = true;
                    break;
    			}
    		}
    	}
    	
    	return flag;
    }
}
```


