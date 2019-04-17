# 红黑树的C实现完整源码 - 结构之法 算法之道 - CSDN博客





2011年01月03日 21:02:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：49986









**红****黑树C源码****实现与剖析**

作者：July 、那谁   时间：二零一一年一月三日


-------------------------

**前言：**    红黑树作为一种经典而高级的数据结构，相信，已经被不少人实现过，但不是因为程序不够完善而无法运行，就是因为程序完全没有注释，初学者根本就看不懂。
    此份红黑树的c源码最初从linux-lib-rbtree.c而来，后经一网友那谁（[http://www.cppblog.com/converse/](http://www.cppblog.com/converse/)）用c写了出来。在此，向原作者表示敬意。

    考虑到原来的程序没有注释，所以我特把这份源码放到编译器里，一行一行的完善，一行一行的给它添加注释，至此，红黑树c带注释的源码，就摆在了您眼前，有不妥、不正之处，还望不吝指正。
------------

红黑树的六篇文章：
[**1、教你透彻了解红黑树**](http://blog.csdn.net/v_JULY_v/archive/2010/12/29/6105630.aspx)[**2、红黑树算法的实现与剖析**](http://blog.csdn.net/v_JULY_v/archive/2010/12/31/6109153.aspx)[**3、红黑树的c源码实现与剖析**](http://blog.csdn.net/v_JULY_v/archive/2011/01/03/6114226.aspx)[**4、一步一图一代码，R-B Tree**](http://blog.csdn.net/v_JULY_v/archive/2011/01/09/6124989.aspx)[**5、红黑树插入和删除结点的全程演示**](http://blog.csdn.net/v_JULY_v/archive/2011/03/28/6284050.aspx)[**6、红黑树的c++完整实现源码**](http://blog.csdn.net/v_JULY_v/archive/2011/03/29/6285620.aspx)
-------------------------

ok，咱们开始吧。
    相信，经过我前俩篇博文对红黑树的介绍，你应该对红黑树有了透彻的理解了（没看过的朋友，可事先查上面的倆篇文章，或与此文的源码剖析对应着看）。

    本套源码剖析把重点放在红黑树的3种插入情况，与红黑树的4种删除情况。其余的能从略则尽量简略。

目录：
一、左旋代码分析
二、右旋
三、红黑树查找结点
四、红黑树的插入
五、红黑树的3种插入情况
六、红黑树的删除
七、红黑树的4种删除情况
八、测试用例

好的，咱们还是先从树的左旋、右旋代码，开始（大部分分析，直接给注释）：



```cpp
//一、左旋代码分析
/*-----------------------------------------------------------
|   node           right
|   / /    ==>     / /
|   a  right     node  y
|       / /       / /    
|       b  y     a   b    //左旋
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_left(rb_node_t* node, rb_node_t* root)
{
    rb_node_t* right = node->right;    //指定指针指向 right<--node->right
 
    if ((node->right = right->left))  
    {
        right->left->parent = node;  //好比上面的注释图，node成为b的父母
    }
    right->left = node;   //node成为right的左孩子
 
    if ((right->parent = node->parent))
    {
        if (node == node->parent->right)
        {
            node->parent->right = right;
        }
        else
        {
            node->parent->left = right;
        }
    }
    else
    {
        root = right;
    }
    node->parent = right;  //right成为node的父母
 
    return root;
}


//二、右旋
/*-----------------------------------------------------------
|       node            left
|       / /             / /
|    left  y   ==>    a    node
|   / /                    / /
|  a   b                  b   y  //右旋与左旋差不多，分析略过
-----------------------------------------------------------*/
static rb_node_t* rb_rotate_right(rb_node_t* node, rb_node_t* root)
{
    rb_node_t* left = node->left;
 
    if ((node->left = left->right))
    {
        left->right->parent = node;
    }
    left->right = node;
 
    if ((left->parent = node->parent))
    {
        if (node == node->parent->right)
        {
            node->parent->right = left;
        }
        else
        {
            node->parent->left = left;
        }
    }
    else
    {
        root = left;
    }
    node->parent = left;
 
    return root;
}


//三、红黑树查找结点
//----------------------------------------------------
//rb_search_auxiliary：查找
//rb_node_t* rb_search：返回找到的结点
//----------------------------------------------------
static rb_node_t* rb_search_auxiliary(key_t key, rb_node_t* root, rb_node_t** save)
{
    rb_node_t *node = root, *parent = NULL;
    int ret;
 
    while (node)
    {
        parent = node;
        ret = node->key - key;
        if (0 < ret)
        {
            node = node->left;
        }
        else if (0 > ret)
        {
            node = node->right;
        }
        else
        {
            return node;
        }
    }
 
    if (save)
    {
        *save = parent;
    }
 
    return NULL;
}

//返回上述rb_search_auxiliary查找结果
rb_node_t* rb_search(key_t key, rb_node_t* root)
{
    return rb_search_auxiliary(key, root, NULL);
}


//四、红黑树的插入
//---------------------------------------------------------
//红黑树的插入结点
rb_node_t* rb_insert(key_t key, data_t data, rb_node_t* root)
{
    rb_node_t *parent = NULL, *node;
 
    parent = NULL;
    if ((node = rb_search_auxiliary(key, root, &parent)))  //调用rb_search_auxiliary找到插入结

点的地方
    {
        return root;
    }
 
    node = rb_new_node(key, data);  //分配结点
    node->parent = parent;   
    node->left = node->right = NULL;
    node->color = RED;
 
    if (parent)
    {
        if (parent->key > key)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }
    }
    else
    {
        root = node;
    }
 
    return rb_insert_rebalance(node, root);   //插入结点后，调用rb_insert_rebalance修复红黑树

的性质
}


//五、红黑树的3种插入情况
//接下来，咱们重点分析针对红黑树插入的3种情况，而进行的修复工作。
//--------------------------------------------------------------
//红黑树修复插入的3种情况
//为了在下面的注释中表示方便，也为了让下述代码与我的倆篇文章相对应，
//用z表示当前结点，p[z]表示父母、p[p[z]]表示祖父、y表示叔叔。
//--------------------------------------------------------------
static rb_node_t* rb_insert_rebalance(rb_node_t *node, rb_node_t *root)
{
    rb_node_t *parent, *gparent, *uncle, *tmp;  //父母p[z]、祖父p[p[z]]、叔叔y、临时结点*tmp
 
    while ((parent = node->parent) && parent->color == RED)
    {     //parent 为node的父母，且当父母的颜色为红时
        gparent = parent->parent;   //gparent为祖父
  
        if (parent == gparent->left)  //当祖父的左孩子即为父母时。
                                 //其实上述几行语句，无非就是理顺孩子、父母、祖父的关系。:D。
        {
            uncle = gparent->right;  //定义叔叔的概念，叔叔y就是父母的右孩子。

            if (uncle && uncle->color == RED) //情况1：z的叔叔y是红色的
            {
                uncle->color = BLACK;   //将叔叔结点y着为黑色
                parent->color = BLACK;  //z的父母p[z]也着为黑色。解决z，p[z]都是红色的问题。
                gparent->color = RED;  
                node = gparent;     //将祖父当做新增结点z，指针z上移俩层，且着为红色。
            //上述情况1中，只考虑了z作为父母的右孩子的情况。
            }
            else                     //情况2：z的叔叔y是黑色的，
            {   
                if (parent->right == node)  //且z为右孩子
                {
                    root = rb_rotate_left(parent, root); //左旋[结点z，与父母结点]
                    tmp = parent;
                    parent = node;
                    node = tmp;     //parent与node 互换角色
                }
                             //情况3：z的叔叔y是黑色的，此时z成为了左孩子。
                                    //注意，1：情况3是由上述情况2变化而来的。
                                    //......2：z的叔叔总是黑色的，否则就是情况1了。
                parent->color = BLACK;   //z的父母p[z]着为黑色
                gparent->color = RED;    //原祖父结点着为红色
                root = rb_rotate_right(gparent, root); //右旋[结点z，与祖父结点]
            }
        } 
 
        else 
        {     
        // if (parent == gparent->right) 当祖父的右孩子即为父母时。（解释请看本文评论下第23楼，同时，感谢SupremeHover指正！）
            uncle = gparent->left;  //祖父的左孩子作为叔叔结点。[原理还是与上部分一样的]
            if (uncle && uncle->color == RED)  //情况1：z的叔叔y是红色的
            {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;           //同上。
            }
            else                               //情况2：z的叔叔y是黑色的，
            {
                if (parent->left == node)  //且z为左孩子
                {
                    root = rb_rotate_right(parent, root);  //以结点parent、root右旋
                    tmp = parent;
                    parent = node;
                    node = tmp;       //parent与node 互换角色
                } 
                  //经过情况2的变化，成为了情况3.
                parent->color = BLACK;
                gparent->color = RED;
                root = rb_rotate_left(gparent, root);   //以结点gparent和root左旋
            }
        }
    }
 
    root->color = BLACK; //根结点，不论怎样，都得置为黑色。
    return root;      //返回根结点。
}


//六、红黑树的删除
//------------------------------------------------------------
//红黑树的删除结点
rb_node_t* rb_erase(key_t key, rb_node_t *root)
{
    rb_node_t *child, *parent, *old, *left, *node;
    color_t color;
 
    if (!(node = rb_search_auxiliary(key, root, NULL)))  //调用rb_search_auxiliary查找要删除的

结点
    {
        printf("key %d is not exist!/n");
        return root;
    }
 
    old = node;
 
    if (node->left && node->right)
    {
        node = node->right;
        while ((left = node->left) != NULL)
        {
            node = left;
        }
        child = node->right;
        parent = node->parent;
        color = node->color;
  
        if (child)
        {
            child->parent = parent;
        }
        if (parent)
        {
            if (parent->left == node)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }
        else
        {
            root = child;
        }
  
        if (node->parent == old)
        {
            parent = node;
        }
  
        node->parent = old->parent;
        node->color = old->color;
        node->right = old->right;
        node->left = old->left;
  
        if (old->parent)
        {
            if (old->parent->left == old)
            {
                old->parent->left = node;
            }
            else
            {
                old->parent->right = node;
            }
        } 
        else
        {
            root = node;
        }
  
        old->left->parent = node;
        if (old->right)
        {
            old->right->parent = node;
        }
    }
    else
    {
        if (!node->left)
        {
            child = node->right;
        }
        else if (!node->right)
        {
            child = node->left;
        }
        parent = node->parent;
        color = node->color;
  
        if (child)
        {
            child->parent = parent;
        }
        if (parent)
        {
            if (parent->left == node)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }
        else
        {
            root = child;
        }
    }
 
    free(old);
 
    if (color == BLACK)
    {
        root = rb_erase_rebalance(child, parent, root); //调用rb_erase_rebalance来恢复红黑树性

质
    }
 
    return root;
}


//七、红黑树的4种删除情况
//----------------------------------------------------------------
//红黑树修复删除的4种情况
//为了表示下述注释的方便，也为了让下述代码与我的倆篇文章相对应，
//x表示要删除的结点，*other、w表示兄弟结点，
//----------------------------------------------------------------
static rb_node_t* rb_erase_rebalance(rb_node_t *node, rb_node_t *parent, rb_node_t *root)
{
    rb_node_t *other, *o_left, *o_right;   //x的兄弟*other，兄弟左孩子*o_left,*o_right
 
    while ((!node || node->color == BLACK) && node != root) 
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (other->color == RED)   //情况1：x的兄弟w是红色的
            {
                other->color = BLACK;  
                parent->color = RED;   //上俩行，改变颜色，w->黑、p[x]->红。
                root = rb_rotate_left(parent, root);  //再对p[x]做一次左旋
                other = parent->right;  //x的新兄弟new w 是旋转之前w的某个孩子。其实就是左旋后

的效果。
            }
            if ((!other->left || other->left->color == BLACK) &&
                (!other->right || other->right->color == BLACK))  
                          //情况2：x的兄弟w是黑色，且w的俩个孩子也

都是黑色的

            {                         //由于w和w的俩个孩子都是黑色的，则在x和w上得去掉一黑色，
                other->color = RED;   //于是，兄弟w变为红色。
                node = parent;    //p[x]为新结点x
                parent = node->parent;  //x<-p[x]
            }
            else                       //情况3：x的兄弟w是黑色的，
            {                          //且，w的左孩子是红色，右孩子为黑色。
                if (!other->right || other->right->color == BLACK)
                {
                    if ((o_left = other->left))   //w和其左孩子left[w]，颜色交换。
                    {
                        o_left->color = BLACK;    //w的左孩子变为由黑->红色
                    } 
                    other->color = RED;           //w由黑->红
                    root = rb_rotate_right(other, root);  //再对w进行右旋，从而红黑性质恢复。
                    other = parent->right;        //变化后的，父结点的右孩子，作为新的兄弟结点

w。
                }
                            //情况4：x的兄弟w是黑色的
    
                other->color = parent->color;  //把兄弟节点染成当前节点父节点的颜色。
                parent->color = BLACK;  //把当前节点父节点染成黑色
                if (other->right)      //且w的右孩子是红
                {
                    other->right->color = BLACK;  //兄弟节点w右孩子染成黑色
                }
                root = rb_rotate_left(parent, root);  //并再做一次左旋
                node = root;   //并把x置为根。
                break;
            }
        }
        //下述情况与上述情况，原理一致。分析略。
        else
        {
            other = parent->left;
            if (other->color == RED)
            {
                other->color = BLACK;
                parent->color = RED;
                root = rb_rotate_right(parent, root);
                other = parent->left;
            }
            if ((!other->left || other->left->color == BLACK) &&
                (!other->right || other->right->color == BLACK))
            {
                other->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!other->left || other->left->color == BLACK)
                {
                    if ((o_right = other->right))
                    {
                        o_right->color = BLACK;
                    }
                    other->color = RED;
                    root = rb_rotate_left(other, root);
                    other = parent->left;
                }
                other->color = parent->color;
                parent->color = BLACK;
                if (other->left)
                {
                    other->left->color = BLACK;
                }
                root = rb_rotate_right(parent, root);
                node = root;
                break;
            }
        }
    }
 
    if (node)
    {
        node->color = BLACK;  //最后将node[上述步骤置为了根结点]，改为黑色。
    }  
    return root;  //返回root
}


//八、测试用例
//主函数
int main()
{
    int i, count = 100;
    key_t key;
    rb_node_t* root = NULL, *node = NULL;
    
    srand(time(NULL));
    for (i = 1; i < count; ++i)
    {
        key = rand() % count;
        if ((root = rb_insert(key, i, root)))
        {
            printf("[i = %d] insert key %d success!/n", i, key);
        }
        else
        {
            printf("[i = %d] insert key %d error!/n", i, key);
            exit(-1);
        }
  
        if ((node = rb_search(key, root)))
        {
            printf("[i = %d] search key %d success!/n", i, key);
        }
        else
        {
            printf("[i = %d] search key %d error!/n", i, key);
            exit(-1);
        }
        if (!(i % 10))
        {
            if ((root = rb_erase(key, root)))
            {
                printf("[i = %d] erase key %d success/n", i, key);
            }
            else
            {
                printf("[i = %d] erase key %d error/n", i, key);
            }
        }
    }
 
    return 0;
}
```



ok，完。

后记：

一、欢迎任何人就此份源码，以及我的前述倆篇文章，进行讨论、提议。
但任何人，引用此份源码剖析，必须得注明作者本人July以及出处。
红黑树系列，已经写了三篇文章，相信，教你透彻了解红黑树的目的，应该达到了。
二、本文完整源码，请到此处下载：

[http://download.csdn.net/source/2958890](http://download.csdn.net/source/2958890)
[**1、教你透彻了解红黑树**](http://blog.csdn.net/v_JULY_v/archive/2010/12/29/6105630.aspx)[**2、红黑树算法的实现与剖析**](http://blog.csdn.net/v_JULY_v/archive/2010/12/31/6109153.aspx)[**3、红黑树的c源码实现与剖析**](http://blog.csdn.net/v_JULY_v/archive/2011/01/03/6114226.aspx)[**4、一步一图一代码，R-B Tree**](http://blog.csdn.net/v_JULY_v/archive/2011/01/09/6124989.aspx)[**5、红黑树插入和删除结点的全程演示**](http://blog.csdn.net/v_JULY_v/archive/2011/03/28/6284050.aspx)**[6、红黑树的c++完整实现源码](http://blog.csdn.net/v_JULY_v/archive/2011/03/29/6285620.aspx)**
 转载本BLOG内任何文章，请以超链接形式注明出处。非常感谢。



