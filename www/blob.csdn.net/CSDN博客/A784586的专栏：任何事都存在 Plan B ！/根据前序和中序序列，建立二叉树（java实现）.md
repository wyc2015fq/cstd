# 根据前序和中序序列，建立二叉树（java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年01月09日 18:13:19[QuJack](https://me.csdn.net/A784586)阅读数：2992








根据前序序列：int[] preSort={1,2,4,7,3,5,6,8};

中序序列：int[] inSort=new int[]{4,7,2,1,5,3,8,6};

建立二叉树，求后序遍历等问题。

前序遍历，中序遍历，后序遍历，层次遍历，四种遍历中，必须包含中序遍历+三选一，两个序列就可以恢复出二叉树的形态。






```java
//方案一：
package com.mytest.mymain;


import java.util.Arrays;


 //定义节点
class BinaryTree{
public int value;
public BinaryTree leftNode;
public BinaryTree rightNode;
BinaryTree(int x) { value = x; }
}


public class ConstrontTree {

public static void main(String[] args) throws Exception {
int[] preSort={1,2,4,7,3,5,6,8};
int[] inSort=new int[]{4,7,2,1,5,3,8,6};
BinaryTree root=startBuildTree(preSort,inSort);
}
//01递归生成树
private static BinaryTree startBuildTree(int[] preSort,int[] inSort) throws Exception {
//异常判断
if(preSort==null || inSort==null){
return null;
}
if(preSort.length!=inSort.length){
throw new Exception("不满足条件的非法输入！");
}

BinaryTree root=null;
for(int i=0;i<inSort.length;i++){
if(inSort[i]==preSort[0]){
root=new BinaryTree(preSort[0]);
System.out.println(preSort[0]);

root.leftNode=startBuildTree(
Arrays.copyOfRange(preSort, 1, i+1),
Arrays.copyOfRange(inSort, 0, i));
root.rightNode=startBuildTree(
Arrays.copyOfRange(preSort, i+1, preSort.length),
Arrays.copyOfRange(inSort, i+1, inSort.length));
}
}


return root;
}


}
//方案二：
//对于查找中序序列中根节点所在位置做进一步优化：红色部分对比
publicclass Solution {
   
    publicTreeNode reConstructBinaryTree(int[] pre,int[] in) {
               if(pre==null||in==null){
            returnnull;
        }
 
        java.util.HashMap<Integer,Integer> map= newjava.util.HashMap<Integer, Integer>();
        for(inti=0;i<in.length;i++){
            map.put(in[i],i);
        }

        returnpreIn(pre,0,pre.length-1,in,0,in.length-1,map);
    }
     
      publicTreeNode preIn(int[] p,intpi,intpj,int[] 


n,intni,intnj,java.util.HashMap<Integer,Integer> map){
 
        if(pi>pj){
            returnnull;
        }
        TreeNode head=newTreeNode(p[pi]);
        intindex=map.get(p[pi]);                  //这样比原始方案一的方式效率要高，值得思考的地方
        head.left=preIn(p,pi+1,pi+index-ni,n,ni,index-1,map);
        head.right=preIn(p,pi+index-ni+1,pj,n,index+1,nj,map);
        returnhead;
    }
```
















