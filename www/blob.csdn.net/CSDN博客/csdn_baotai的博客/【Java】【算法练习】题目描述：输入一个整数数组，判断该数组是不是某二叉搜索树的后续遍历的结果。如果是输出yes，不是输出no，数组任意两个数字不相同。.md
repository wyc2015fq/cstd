# 【Java】【算法练习】题目描述：输入一个整数数组，判断该数组是不是某二叉搜索树的后续遍历的结果。如果是输出yes，不是输出no，数组任意两个数字不相同。 - csdn_baotai的博客 - CSDN博客

2018年08月18日 23:04:06[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：111


# 题目描述：

###         题目描述：输入一个整数数组，判断该数组是不是某二叉搜索树的后续遍历的结果。如果是输出yes，不是输出no，数组任意两个数字不相同。

           解题思路：

对于二叉搜索树的而言，什么是二叉搜索树？

       二叉查找树（Binary Search Tree），（又：[二叉搜索树](https://baike.baidu.com/item/%E4%BA%8C%E5%8F%89%E6%90%9C%E7%B4%A2%E6%A0%91/7077855)，二叉排序树）它或者是一棵空树，或者是具有下列性质的[二叉树](https://baike.baidu.com/item/%E4%BA%8C%E5%8F%89%E6%A0%91/1602879)： 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值； 若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值； 它的左、右子树也分别为[二叉排序树](https://baike.baidu.com/item/%E4%BA%8C%E5%8F%89%E6%8E%92%E5%BA%8F%E6%A0%91/10905079)。

        二叉搜索树按照中序遍历的话就是有序的，由题可知，要对其进行后序遍历，遍历序列最后一个肯定就是根节点，对于根节点来说，其左子树所有的节点都小于根节点的值。其所有右子树的值都大于根节点的值。同样的道理，如果根节点换做是根节点的左节点或者右节点，其大小关系是相同的，所以此题应该采用递归解法。

```java
package Test2018_8_18_BinaryTree;

/**
 * 输入一个整数数组，判断该数组是不是某二叉树的后续遍历的结果、
 * 如果是输出yes，不是输出no，数组任意两个数字不相同
 * @author 李宝泰
 *
 */

import java.util.Arrays;
public class BalancedBinaryTree {
    
    public boolean VerifySquenceOfBST(int [] sequence) {
	    //判断形参合法性
		if(sequence == null||sequence.length<=0){
				return false;
			}
			int len=sequence.length;
			int root=sequence[len-1];//根节点的值
			int i=0;
			for(;i<len-1;i++){//找到root的位置
				if(root<=sequence[i])
					break;
			}
			int j=i;//分开左子树和右子树
			for(;j<len-1;j++){
				if(root>sequence[j]){
					return false;
				}
			}
			
			boolean leftFlag=true;

			if (i>0) {//限定了i的范围
                        //递归
			leftFlag=VerifySquenceOfBST(Arrays.copyOfRange(sequence,0,i));

			}

			boolean rightFlag=true;

			if (i<len-1) {

			rightFlag=VerifySquenceOfBST(Arrays.copyOfRange(sequence,i,sequence.length-1));

			}
       
			return leftFlag&&rightFlag;
	    }

}
```

