# 二叉树的顺序存储和链式存储结构（java实现） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月13日 13:35:30[QuJack](https://me.csdn.net/A784586)阅读数：1500








**二叉树的顺序存储和链式存储结构（java实现）**

**1.顺序存储结构**

**按完全二叉树编号，存放在数组里。根节点对应放在数组下标为1的位置，不存在的结点对应位置存放'#',缺点：退化的二叉树非常浪费空间，插入删除十分不方便；**


```java
char[] a={'#','a','b',' c','d ',' #',' f',' g','# ',' I'};
```



**2.链式存储结构**

**结点定义：**


- class BinaryTree{  
- publicint value;  
- public BinaryTree leftNode;  
- public BinaryTree rightNode;  
- BinaryTree(int x) { value = x; }  
- }  





