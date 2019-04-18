# KD树 - aiaiai010101的博客 - CSDN博客

2017年09月01日 00:11:10[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：316



# KD树的文章里,这篇写的很棒,转载过来.

**原文地址:http://blog.csdn.net/google19890102/article/details/54291615**

# 一、K-近邻算法

K-近邻[算法](http://lib.csdn.net/base/31)是一种典型的无参监督学习算法，对于一个监督学习任务来说，其个训练样本为：


在K-近邻算法中，无需利用训练样本学习出统一的模型，对于一个新的样本，如，通过比较样本与个训练样本的相似度，选择出个最相似的样本，并以这个样本的标签作为样本的标签。

在如上的描述中，样本需要分别与个训练样本计算相似度，通常，使用的相似度的计算方法为欧式距离，即对于样本和样本，其两者之间的相似度为：


对于K-近邻算法的具体过程，可以参见博文[简单易学的机器学习算法——K-近邻算法](http://blog.csdn.net/google19890102/article/details/23924971)。

在K-近邻算法的计算过程中，通过暴力的对每一对样本计算其相似度是非常好费时间的，那么是否存在一种方法，能够加快计算的速度？kd树便是其中的一种方法。

# 二、kd树

kd树是一种对维空间中的实例点进行存储以便对其进行快速检索的树形[数据结构](http://lib.csdn.net/base/31)，且kd树是一种二叉树，表示对维空间的一个划分。

## 1、二叉排序树

在数据结构中，二叉排序树又称二叉查找树或者二叉搜索树。其定义为：二叉排序树，或者是一棵空树，或者是具有下列性质的二叉树：
- 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
- 若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
- 它的左、右子树也分别为二叉排序树。

一个典型的二叉排序树的例子如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170110094142461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在二叉排序树中，若以中序遍历，则得到的是按照值大小排序的结果，即1->3->4->6->7->8->10->13->14。

如果需要检索7，则从根结点开始：
- ->左子树
- ->右子树
- ->右子树
- ->查找结束

但是，对于二叉排序树的建立，若构建二叉排序树的顺序为基本有序时，如按照1->3->4->6->7->8->10->13->14构建二叉排序树，会得到如下的结果：

![这里写图片描述](https://img-blog.csdn.net/20170110094234337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样的话，检索效率会下降，为了避免这样的情况的出现，会对二叉树设置一些条件，如平衡二叉树。对于二叉排序树的更多内容，可以参见[数据结构和算法——二叉排序树](http://blog.csdn.net/google19890102/article/details/54378628)。

## 2、kd树的概念

kd树与二叉排序树的基本思想类似，与二叉排序树不同的是，在kd树中，每一个节点表示的是一个样本，通过选择样本中的某一维特征，将样本划分到不同的节点中，如对于样本，
 考虑数据的第一维，首先，根节点为，由于样本的第一维小于，因此，样本在根节点的左子树上，同理，样本在根节点的右子树上。通过第一维可以构建如下的二叉树模型：

![这里写图片描述](https://img-blog.csdn.net/20170110094327998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在kd树的基本操作中，主要包括kd树的建立和kd树的检索两个部分。

## 3、kd树的建立

构造kd树相当于不断地用垂直于坐标轴的超平面将维空间切分成一系列的维超矩阵区域。选择划分节点的方法主要有两种：
- 顺序选择，即按照数据的顺序依次在kd树中插入节点；
- 选择待划分维数的中位数为划分的节点。在kd树的构建过程中，为了防止出现只有左子树或者只有右子树的情况出现，通常对于每一个节点，选择样本中的中位数作为切分点。这样构建出来的kd树时平衡的。

> 
在李航的《统计[机器学习](http://lib.csdn.net/base/2)》P41中有提到：平衡的kd树搜索时的效率未必是最优的。

在构建kd树的过程中，也可以根据插入数据的顺序构建kd树，以二维数据集为例，其数据的顺序依次为：


对于如上的二维数据集，构建kd树：
- 选择一维最为切分的维度，如选择第维，第一个数为，其第维的值为，以作为kd树的根结点，若第维的值大于为右子树，否则插入到左子树中；
- 对后续的节点依次判断，如，选择第维，其值为，大于，插入到根结点的右子树中，设置其维数为除了第维以外的任一维。。。

按照如上的过程，我们划分出来的kd树如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170123174252687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此时，将样本按照特征空间划分如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170123174757876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由以上的计算过程可以看出对于树中节点，需要有数据项，当前节点的比较维度，指向左子树的指针和指向右子树的指针，可以设置其结构如下：

```
#define MAX_LEN 1024

typedef struct KDtree{
        double data[MAX_LEN]; // 数据
        int dim; // 选择的维度
        struct KDtree *left; // 左子树
        struct KDtree *right; // 右子树
}kdtree_node;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

构造kd树的函数声明为：

```
int kdtree_insert(kdtree_node *&tree_node, double *data, int layer, int dim);
```
- 1
- 2

![](http://static.blog.csdn.net/images/save_snippets_01.png)
- 1
- 2

函数的具体实现如下：

```
// 递归构建kd树，通过节点所在的层数控制选择的维度
int kdtree_insert(kdtree_node * &tree_node, double *data, int layer, int dim){
        // 空树
        if (NULL == tree_node){
                // 申请空间
                tree_node = (kdtree_node *)malloc(sizeof(kdtree_node));
                if (NULL == tree_node) return 1;

                //插入元素
                for (int i = 0; i < dim; i ++){
                        (tree_node->data)[i] = data[i];
                }
                tree_node->dim = layer % (dim);
                tree_node->left = NULL;
                tree_node->right = NULL;

                return 0;
        }

        // 插入左子树
        if (data[tree_node->dim] <= (tree_node->data)[tree_node->dim]){
                return kdtree_insert(tree_node->left, data, ++layer, dim);
        }

        // 插入右子树
        return kdtree_insert(tree_node->right, data, ++layer, dim);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27

当构建好了kd树后，需要对kd树进行遍历，在这里，实现了两种kd树的遍历方法：
- 先序遍历
- 中序遍历

对于先序遍历，其函数的声明为：
`void kdtree_print(kdtree_node *tree, int dim);`- 1

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1

函数的具体实现为：

```
void kdtree_print(kdtree_node *tree, int dim){
        if (tree != NULL){
                fprintf(stderr, "dim:%d\n", tree->dim);
                for (int i = 0; i < dim; i++){
                        fprintf(stderr, "%lf\t", (tree->data)[i]);
                }
                fprintf(stderr, "\n");
                kdtree_print(tree->left, dim);
                kdtree_print(tree->right, dim);
        }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11

对于中序遍历，其函数的声明为：
`void kdtree_print_in(kdtree_node *tree, int dim);`- 1

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1

函数的具体实现为：

```
void kdtree_print_in(kdtree_node *tree, int dim){
        if (tree != NULL){
                kdtree_print_in(tree->left, dim);
                fprintf(stderr, "dim:%d\n", tree->dim);
                for (int i = 0; i < dim; i++){
                        fprintf(stderr, "%lf\t", (tree->data)[i]);
                }
                fprintf(stderr, "\n");
                kdtree_print_in(tree->right, dim);
        }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11

## 4、kd树的检索

与二叉排序树一样，在kd树中，将样本划分到不同的空间中，在查找的过程中，由于查找在某些情况下仅需查找部分的空间，这为查找的过程节省了对大部分数据点的搜索的时间，对于kd树的检索，其具体过程为：
- 从根节点开始，将待检索的样本划分到对应的区域中（在kd树形结构中，从根节点开始查找，直到叶子节点，将这样的查找序列存储到栈中）
- 以栈顶元素与待检索的样本之间的距离作为最短距离min_distance
- 执行出栈操作： 
- 向上回溯，查找到父节点，若父节点与待检索样本之间的距离小于当前的最短距离min_distance，则替换当前的最短距离min_distance
- 以待检索的样本为**圆心**（二维，高维情况下是球心），以min_distance为**半径**画圆，若圆与父节点所在的**平面**相割，则需要将父节点的另一棵子树进栈，重新执行以上的出栈操作

- 直到栈为空

以查找为例，首先，我们需要找到待查找的样本所在的搜索空间，搜索空间如下图中的黑色区域所示：

![这里写图片描述](https://img-blog.csdn.net/20170202110833163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其对应的进栈序列为：。

此时，以到之间的距离为最短距离，最短距离min_distance为1，对栈顶元素出栈，此时栈中的序列为：。以待检索样本为圆心，1为半径画圆，圆与所在平面相割，如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170202111519774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此时，需要检索以为根节点的另外一棵子树，即需要将进栈，此时，栈中的序列为：。

> 
注意：若需要进栈的子树中有很多节点，则根据需要比较的元素的大小，将直到叶节点的所有节点都进栈，这一点在很多地方都写得不清楚。

**本人注释：上边这句非常重要,也很好,网上很多文章这里说的都不清楚,如果入栈的是一个非叶结点,那么将不止入栈这一个非叶结点,而是要按照之前在根树中不断利用特征分量比较进行搜索直到叶结点的方法在以该非叶结点的树中一路搜索到叶结点,并将搜索路径上的结点全部入栈.**

**这种寻找最近邻点的方法可以这样理解:寻找"最有可能"是最近的点,然后考虑其兄弟点所包含的区域中有无可能出现更近的点,所以一些基于KD树的改进方法其实都是改进这个"最有可能"的点的顺序,希望能将最有可能的点放在最先检索的位置.**

**其实可以考虑一种非常极端的情况,我们不按照特征分量的大小比较来搜索叶结点,而是一路选择左边的分支直到叶结点.然后按照距离比较来考察兄弟结点的区域是否可能存在更近的点,有可能存在的话,便将兄弟结点一路按照左边的分支结点一路入栈.**

**KD树其实就是上述极端情况操作的改进,通过比较特征分量大小顺序得到的叶结点很显然比一路向左得到的叶结点更有可能是最近邻结点.**

**而改进KD树就可以考虑如何能使这种可能变得更大.**

**http://blog.csdn.net/app_12062011/article/details/51986805,这篇文章里的BBF机制其实就是优先搜索那些离待查找点较劲的实例点,因为这些实例点代表的区域内含有最近邻点的可能性更大.**

按照上述的步骤，再执行出栈的操作，直到栈为空。

检索过程的函数声明为：
`void search_nearest(kdtree_node *tree, double *data_search, int dim, double *result);`- 1

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1

函数的具体实现为：

```
void search_nearest(kdtree_node *tree, double *data_search, int dim, double *result){
    // 一直找到叶子节点
    fprintf(stderr, "\nstart searching....\n");
    stack<kdtree_node *> st;

    kdtree_node *p = tree;

    while (p->left != NULL || p->right != NULL){
        st.push(p);// 将p压栈
        if (data_search[p->dim] <= (p->data)[p->dim]){// 选择左子树
            // 判断左子树是否为空
            if (p->left == NULL) break;
            p = p->left;
        }else{ // 选择右子树
            if (p->right == NULL) break;
            p = p->right;
        }
    }

    // 现在与栈中的数据进行对比
    double min_distance = distance(data_search, p->data, dim);// 与根结点之间的距离
    fprintf(stderr, "init: %lf\n", min_distance);
    copy2result(p->data, result, dim);
    // 打印最优值
    for (int i = 0; i < dim; i++){
                fprintf(stderr, "%lf\t", result[i]);
        }
        fprintf(stderr, "\n");

    double d = 0;
    while (st.size() > 0){
        kdtree_node *q = st.top();// 找到栈顶元素
        st.pop(); // 出栈

        // 判断与父节点之间的距离
        d = distance(data_search, q->data, dim);

        if (d <= min_distance){
            min_distance = d;
            copy2result(q->data, result, dim);
        }

        // 判断与分隔面是否相交
        double d_line = distance_except_dim(data_search, q->data, q->dim); // 到平面之间的距离
        if (d_line < min_distance){ // 相交
            // 如果本来在右子树，现在查找左子树
            // 如果本来在左子树，现在查找右子树
            if (data_search[q->dim] > (q->data)[q->dim]){
                // 选择左子树
                if (q->left != NULL) q = q->left;
                else q = NULL;
            }else{
                // 选择右子树
                if (q->right != NULL) q = q->right;
                else q = NULL;
            }
            if (q != NULL){
                while (q->left != NULL || q->right != NULL){
                    st.push(q);
                    if (data_search[q->dim] <= (q->data)[q->dim]){
                        if (q->left == NULL) break;
                        q = q->left;
                    }else{
                        if (q->right == NULL) break;
                        q = q->right;
                    }
                }
                if (q->left == NULL && q->right == NULL) st.push(q);
            }
        }

    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74

在函数的实现中，需要用到的函数为：
- 两个样本之间的距离

```
double distance(double *a, double *b, int dim){
    double d = 0.0;
    for (int i = 0; i < dim; i ++){
        d += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return d;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 待检索的样本到平面之间的距离

```
double distance_except_dim(double *a, double *b, int except_dim){
    double d = (a[except_dim] - b[except_dim]) * (a[except_dim] - b[except_dim]);
    return d;
}
```
- 1
- 2
- 3
- 4

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 复制最优的结果

```
void copy2result(double *a, double *result, int dim){
    for (int i = 0; i < dim; i ++){
        result[i] = a[i];
    }
}
```
- 1
- 2
- 3
- 4
- 5

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5

# 三、测试

利用如上的测试集，我们构建kd树，并在kd树中查找，测试代码如下：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdtree.h"

// 解析特征
int parse_feature(char *p, double *fea, int *dim){
        // 解析特征
        char *q = p;
        int i = 0;
        while ((q = strchr(p, '\t')) != NULL){
                *q = 0;
                fea[i] = atof(p);
                //fprintf(stderr, "atof(p):%lf\n", atof(p));
                p = q + 1;
                //r = r + 1;
                i += 1;
        }

        // 解析最后一个
        fea[i] = atof(p);

        *dim = i + 1;
        //fprintf(stderr, "atof(p):%lf\n", atof(p));

        //fprintf(stderr, "fea:%lf\t%lf\n", fea[0], fea[1]);
}

int main(){
        kdtree_node *tree_node = NULL;
        // 从文件中读入数据
        FILE *fp = fopen("data.txt", "r");
        char feature[MAX_LEN];
        double data[MAX_LEN];
        int data_dim = 0; // 数据的维数

        double data_search[2] = {6.0, 3.0};

        while (fgets(feature, MAX_LEN, fp)){
                fprintf(stderr, "%s", feature);
                parse_feature(feature, data, &data_dim);
                fprintf(stderr, "distance: %lf\n", distance(data, data_search, data_dim));
                // 插入到kd树中
                kdtree_insert(tree_node, data, 0, data_dim);
        }
        fclose(fp);
        fprintf(stderr, "dim:%d\n", data_dim);
        fprintf(stderr, "insert_ok\n");
        // test

        kdtree_print(tree_node, data_dim);
        printf("\n");
        kdtree_print_in(tree_node, data_dim);

        double result[2];

        search_nearest(tree_node, data_search, data_dim, result);

        fprintf(stderr, "\n the final result: ");
        for (int i = 0; i < data_dim; i++){
                fprintf(stderr, "%lf\t", result[i]);
        }
        fprintf(stderr, "\n");
        return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66

![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66

以上的代码以上处至Github，其地址为：[kd-tree](https://github.com/zhaozhiyong19890102/kd-tree)。若有不对的地方，欢迎指正。

# 参考文献
- [K近邻算法基础：KD树的操作](http://blog.csdn.net/u011067360/article/details/23934361)
- [k近邻法的C++实现：kd树](http://www.cnblogs.com/90zeng/p/kdtree.html)
- [An intoductory tutorial on kd-trees](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.28.6468&rep=rep1&type=pdf)
- [Range Searching using Kd Tree](http://didamatica2010.di.uniroma1.it/pub/Estrinfo/Materiale/kdtree.pdf)
- [最近邻算法的实现:k-d tree](http://blog.csdn.net/zhl30041839/article/details/9277807)
- [从K近邻算法、距离度量谈到KD树、SIFT+BBF算法](http://blog.csdn.net/v_july_v/article/details/8203674)
- 《统计机器学习》

