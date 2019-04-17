# treap(树堆) - 在思索中前行！ - CSDN博客





2014年08月15日 21:14:54[_Tham](https://me.csdn.net/txl16211)阅读数：503








一棵treap是一棵修改了结点顺序的二叉查找树，如图，显示一个例子，通常树内的每个结点x都有一个关键字值key[x]，另外，还要为结点分配priority[x]，它是一个独立选取的随机数。

假设所有的优先级是不同的，所有的关键字也是不同的。treap的结点排列成让关键字遵循二叉查找树性质，并且优先级遵循最小堆顺序性质：

1.如果v是u的左孩子，则key[v] < key[u].

2.如果v是u的右孩子，则key[v] > key[u].

3.如果v是u的孩子，则priority[u] > priority[u].

这两个性质的结合就是为什么这种树被称为“treap”的原因，因为它同时具有二叉查找树和堆的特征。

![](http://pic002.cnblogs.com/images/2012/305173/2012050612133926.png)



用以下方式考虑treap会有帮助。假设插入关联关键字的结点x1，x2，...，xn到一棵treap内。结果的treap是将这些结点以它们的优先级(随机选取)的顺序插入一棵正常的二叉查找树形成的，亦即priority[xi] < priority[xj]表示xi在xj之前被插入。

在算法导论的12.4节中，其证明了随机构造的二叉查找树的期望高度为O(lgn)，因而treap的期望高度亦是O(lgn)。

treap插入操作：

1.按照二叉树的插入方法，将结点插入到树中

2.根据堆的性质(我们这里为最小堆)和优先级的大小调整结点位置。

treap删除操作：

1.找到相应的结点

2.若该结点为叶子结点，则直接删除；

若该结点为只包含一个叶子结点的结点，则将其叶子结点赋值给它；

若该结点为其他情况下的节点，则进行相应的旋转，直到该结点为上述情况之一，然后进行删除。

旋转主要涉及到右旋转的左旋转，下面把右旋转的图画在下面：

![](http://pic002.cnblogs.com/images/2012/305173/2012050612140287.png)

代码如下:(已通过GCC和VC编译)

PS:请教一下大家，在C语言中是没有引用的，因而在treap_insert(Node* root, int key, int priority)函数中(第40行)，由于root要跟着改变，因而必须传root地址，即&root(第131行)，因而导致在写代码时，显得很不好看，如传root的left的地址为参数，必须写成&((*root)->left)(第72行)。如果用C++写，直接用引用，则代码看起来简洁很多，不知在C语言中如何操作？






```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node_t* Node;
typedef struct treap_t* Treap;

struct node_t
{
  Node left;//左节点
  Node right;//右节点
  int priority;//优先级
  int key;//存储的关键字
};

struct treap_t
{
  Node root;
};

//左旋转
void rotate_left(Node* node)
{
  Node x = (*node)->right;
  (*node)->right = x->left;
  x->left = *node;
  *node = x;
}

//右旋转
void rotate_right(Node* node)
{
  Node x = (*node)->left;
  (*node)->left = x->right;
  x->right = *node;
  *node = x;
}

//插入操作
void treap_insert(Node* root, int key, int priority)
{
  //根为NULL，则直接创建此结点为根结点
  if (*root == NULL)
  {
    *root = (Node)malloc(sizeof(struct node_t));
    (*root)->left = NULL;
    (*root)->right = NULL;
    (*root)->priority = priority;
    (*root)->key = key;
  }
  //向右插入结点
  else if (key < (*root)->key)
  {
    treap_insert(&((*root)->left), key, priority);
    if ((*root)->left->priority < (*root)->priority)
      rotate_right(root);
  }
  //向左插入结点
  else
  {
    treap_insert(&((*root)->right), key, priority);
    if ((*root)->right->priority < (*root)->priority)
      rotate_left(root);
  }
}

void treap_delete(Node* root, int key)
{
  if (*root != NULL)
  {
    if (key < (*root)->key)
      treap_delete(&((*root)->left), key);
    else if (key > (*root)->key)
      treap_delete(&((*root)->right), key);
    else
    {
      //左右孩子都为空不用单独写出来
      if ((*root)->left == NULL)
        *root = (*root)->right;
      else if ((*root)->right == NULL)
        *root = (*root)->left;
      else
      {
        //先旋转，然后再删除
        if ((*root)->left->priority < (*root)->right->priority)
        {
          rotate_right(root);
          treap_delete(&((*root)->right), key);
        }
        else
        {
          rotate_left(root);
          treap_delete(&((*root)->left), key);
        }
      }
    }
  }
}

//中序遍历
void in_order_traverse(Node root)
{
  if (root != NULL)
  {
    in_order_traverse(root->left);
    printf("%d\t", root->key);
    in_order_traverse(root->right);
  }
}

//计算树的高度
int depth(Node node)
{
    if(node == NULL)
        return -1;
    int l = depth(node->left);
    int r = depth(node->right);

    return (l < r)?(r+1):(l+1);
}

int main()
{
  Treap treap = (Treap)malloc(sizeof(struct treap_t));
  treap->root = NULL;
  int i = 0;
  
  srand(time(0));
  
  for (i = 0; i < 100; i++)
    treap_insert(&(treap->root), i, rand());
  in_order_traverse(treap->root);
  printf("\n高度：%d\n", depth(treap->root));
  
  printf("---分割线---\n");

  for (i = 23; i < 59; i++)
    treap_delete(&(treap->root), i);
  in_order_traverse(treap->root);
  printf("\n高度：%d\n", depth(treap->root));
  return 0;
}
```





