# 自动平衡二叉树的构建-AVL树 - weixin_33985507的博客 - CSDN博客
2012年04月28日 10:52:57[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
AVL树是基于二叉搜索树的。但是它是自动平衡的，意思是，它左子树的深度和右子树的深度差要么是0,±1。没有其他可能。这就是AVL树，这棵树长的比较对称，不会出现极端的一边倒的情况。这也就意味着AVL在创建过程中，根节点也会不断的变换。AVL树的目的就是为了解决搜索二叉搜索树的时候可能出现最坏复杂度的情况。AVL树极端情况下复杂度也就log(n)。
但是AVL树的建立有点复杂。网上查找了部分资料，维基百科的AVL定义是我主要的参考来源。具体定义可以自行查看[http://en.wikipedia.org/wiki/AVL_tree](http://en.wikipedia.org/wiki/AVL_tree)。
在实现过程中，最让我头疼的是平衡因子的计算，资料中也没有说明具体的算法，或者说了也没看懂。网上搜索了一些资料，也是极尽坑爹，要么含糊不说，要么一笔带过。始终不知道如何计算。因此本人在程序中使用的是计算出左右子树的最大深度，做减法。由于计算左右子树是一个递归算法，也是相对有些复杂。因此算法的整体复杂度也随之增加。
为了降低程序的复杂度，对于AVL树节点的定义信息，也有所增加，比如增加了指向父节点的指针。当指明孩子的时候，自动的也确定父亲。代码如下：
- public node leftchild   
-        {   
- set
-            {   
-                _leftchild = value;   
- if (_leftchild != null)   
-                {   
-                    _leftchild.father = this;//确定父结点 
- 
-                }   
-            }   
- get
-            {   
- return _leftchild;   
-            }   
-        }   
- public node rightchild   
-        {   
- set
-            {   
-                _rightchild = value;   
- if (_rightchild != null)   
-                {   
-                    _rightchild.father = this;//确定父结点 
- 
-                }   
-            }   
- get
-            {   
- return _rightchild;   
-            }   
-        }   
- 
具体的类定义可以看下图。
![image](http://cnn237111.blog.51cto.com/attachment/201204/28/2359144_13355809771qeg.png)
其中balance是平衡因子。实现方式如下：
- publicint balance   
-       {   
- get
-           {   
- return maxDepth(this.leftchild) - maxDepth(this.rightchild);   
-           }   
-       } 
AVL树构建中的关键步骤叫做转换。分成4种情况的转换。具体的内容可以参考维基百科。本人参照维基百科的图例写出4个转换方式。这四种分别是LR，LL，RL，RR。如下图：
![375px-AVL_Tree_Rebalancing_svg](http://cnn237111.blog.51cto.com/attachment/201204/28/2359144_1335580989jnzf.png)
此处简述一下LR转换。首先要匹配哪个结点是哪个结点。要找准了，不然就容易出错。LR中，结点4转到结点5和3中间，并且结点B成为结点3右孩子了。其中LR到LL的时候结点5依然是根结点，没变。那么程序就如下写法，其中root是根结点参数，但此处的转换不需要有根结点参与。程序的写法尽量照图，把要转动的点作为leaf结点传进来。即node3就是leaf，然后依次匹配好哪个是node4，哪个是node5,哪个是B，全都匹配好之后，把图上的动作翻译成代码。此处需要对链表操作有一定的基础。懂的自然懂。当LR经过图上的转换后，变成LL型了，那么直接调用LL的转换方式就可以了。
- staticvoid LL_To_Balanced(ref node root, node leaf)  
-        {  
- 
-            node node4 = leaf;  
-            node node3 = node4.leftchild;  
-            node node5 = node4.father;  
-            node C = node4.rightchild;  
- //以上代码根据图把结点给找准了，下面就开始动作了。 
- 
- if (node5.father != null)//此处涉及到node4变成根结点了。那么如果node5的father存在的话，要指向node4的。 
-            {  
- if (node5.isleftchild)  
-                    node5.father.leftchild = node4;  
- elseif (node5.isrightchild)  
-                    node5.father.rightchild = node4;  
-            }  
- else
-            {  
-                root = node4;//如果node5是整棵树的根结点，那么现在就变成node4了 
-                node4.father = null;  
-            }  
-            node4.rightchild = node5;  
-            node5.leftchild = C;  
-        }  
再看LL型的转换，首先，同样的找准结点，匹配好哪个是结点4，哪个是结点5，哪个是结点3，那个结点C，因为LL的转换也就只涉及到结点3,4,5,C这几个结点。代码中，把结点4当做leaf，因此传值的时候要传对了。由于LL转换涉及到树的根结点的变换，根据图中的示例，假设node5有个father，那么这个father现在是把node4作为孩子了。至于是左孩子还是右孩子，就看node5是左孩子还是右孩子。假设node5没有father，也就是说node5是根节点，那么node4就成为根结点了。
- staticvoid LL_To_Balanced(ref node root, node leaf)  
- {  
- 
-     node node4 = leaf;  
-     node node3 = node4.leftchild;  
-     node node5 = node4.father;  
-     node C = node4.rightchild;  
- //以上代码根据图把结点给找准了，下面就开始动作了。 
- 
- if (node5.father != null)//此处涉及到node4变成根结点了。那么如果node5的father存在的话，要指向node4的。 
-     {  
- if (node5.isleftchild)  
-             node5.father.leftchild = node4;  
- elseif (node5.isrightchild)  
-             node5.father.rightchild = node4;  
-     }  
- else
-     {  
-         root = node4;//如果node5是整棵树的根结点，那么现在就变成node4了 
-         node4.father = null;  
-     }  
-     node4.rightchild = node5;  
-     node5.leftchild = C;  
- }  
RL和RR的转换也是如上面所述的算法。关键要匹配好那些结点。
本人之前写过二叉搜索树的插入，可以参考前面的文章。AVL的建立和二叉搜索树是类似的，根据结点的大小插入位子。但是多了一步，就是要做转型，也就是上面说的4种转换方式。
那么代码可以先借用构建二叉搜索树的代码了。插完结点后，调用转型方法不就OK了嘛。如下的方法，就是生成了普通的二叉搜索树。然后在对其转换。
- //此处算法就是插入二叉搜索树 
- staticvoid InsertIntoAVL(node root, node leaf)  
-        {  
- if (root == null)  
-            {  
-                root = leaf;  
- return;  
-            }  
- if (root.nodevalue == leaf.nodevalue)  
-            {  
- return;  
-            }  
- elseif (root.nodevalue > leaf.nodevalue)  
-            {  
- if (root.hasleftchild == false)  
-                {  
-                    root.leftchild = leaf;  
-                }  
- else
-                {  
-                    InsertIntoAVL(root.leftchild, leaf);  
-                }  
-            }  
- elseif (root.nodevalue < leaf.nodevalue)  
-            {  
- if (root.hasrightchild == false)  
-                {  
-                    root.rightchild = leaf;  
-                }  
- else
-                {  
-                    InsertIntoAVL(root.rightchild, leaf);  
-                }  
-            }  
-        } 
以上通过代码插入结点，算法同二叉搜索树的结点插入，接下来确定转换问题。转换的时候，要确定，对谁转换，哪种类型的转换。
先第一个问题，对谁转换。那当然是插入了哪个结点，这个结点有可能打破了平衡，因此转换肯定和该节点有关。在我的代码中，以新插入的结点作为参照，依次找它的父节点。（此处就知道为什么要做node的定义中增加father属性了），一旦找到某个父节点的平衡因子是±2，可以开始处理第二个问题了。此处再强调的是，平衡因子的算法是左-右，因此如果是+2，那么就是说明左边更深，反之亦反。
第二个问题，哪种类型的转换，维基百科上也有现成的说法，拿来照搬就行。简单的说就是如果一个结点的平衡因子是±2了，那么就看它的孩子的平衡因子，是±1，不同的值代表了不同的类型。具体的逻辑，可以看我的代码反推。此处要注意的是，传进去的leaf结点，我都是把平衡因子是±2的子结点作为leaf结点的。当然你也可以直接就用平衡因子是±2结点作为参数传递。
代码实现如下，此方法的参数root就是表示这颗AVL树的根结点，因为很有可能在构建这棵树的时候根结点会变化，所以要时刻记录下，打印树的时候可以从根结点开始。leaf参数就是之前你新插入的结点，根据这个leaf为基准找father，你可以根据代码反推一些逻辑：
- //做旋转 
- staticvoid Revolve(ref node root, node leaf)  
-        {  
-            Console.ForegroundColor = ConsoleColor.Yellow;  
- if (root == null)//如果一棵树是空树，即没有根结点的情况下，插入一个叶子，那么根结点就是这样叶子，也不需要做旋转了。 
-            {  
-                root = leaf;  
-                Console.ForegroundColor = ConsoleColor.Gray;  
- return;  
-            }  
-            node itsfather = leaf.father;  
- while (itsfather != null)  
-            {  
- if (itsfather.balance == 2)//LR或者LL 
-                {  
- if (itsfather.leftchild.balance == 1)  
-                    {  
-                        Console.WriteLine("LL");  
-                        LL_To_Balanced(ref root, itsfather.leftchild);  
-                    }  
- elseif (itsfather.leftchild.balance == -1)  
-                    {  
-                        Console.WriteLine("LR");  
-                        LR_To_Balanced(ref root, itsfather.leftchild);  
- 
-                    }  
-                    Console.ForegroundColor = ConsoleColor.Gray;  
- return;  
-                }  
- elseif (itsfather.balance == -2)  
-                {  
- if (itsfather.rightchild.balance == -1)  
-                    {  
-                        Console.WriteLine("RR");  
-                        RR_To_Balanced(ref root, itsfather.rightchild);  
-                    }  
- elseif (itsfather.rightchild.balance == 1)  
-                    {  
-                        RL_To_Balanced(ref root, itsfather.rightchild);  
-                        Console.WriteLine("RL");  
-                    }  
-                    Console.ForegroundColor = ConsoleColor.Gray;  
- return;  
-                }  
-                itsfather = itsfather.father;  
-            }  
- 
-            Console.WriteLine("no need to revolve");  
-            Console.ForegroundColor = ConsoleColor.Gray;  
-        } 
最后，就是调用插入结点的方法。为了让结果直观，本人写了个插入方法，主要打印树的结构。可自行反推逻辑。 
- //插入结点，打印一些信息  
- privatestatic node InsertNode(ref node root, node thenode)   
-        {   
-            InsertIntoAVL(root, thenode);   
-            Console.WriteLine("插入了" + thenode.nodevalue);   
-            printtree(root);   
-            Console.WriteLine("开始对其旋转");   
-            Revolve(ref root, thenode);   
-            Console.ForegroundColor = ConsoleColor.Green;   
-            Console.WriteLine("旋转后的结构如下");   
-            Console.ForegroundColor = ConsoleColor.Gray;   
-            printtree(root);   
-            Console.WriteLine("******END*******");   
- return root;   
-        } 
Main方法中，调用就如下：
- node node_1 = new node(1);   
-             node node_2 = new node(2);   
-             node node_3 = new node(3);   
-             node node_4 = new node(4);   
-             node node_5 = new node(5);   
-             node node_6 = new node(6);   
-             node node_7 = new node(7);   
-             node node_8 = new node(8);   
-             node node_9 = new node(9);   
-             node node_10 = new node(10);   
-             node node_11 = new node(11);   
-             node node_12 = new node(12);   
-             node node_13 = new node(13);   
-             node node_14 = new node(14);   
-             node node_15 = new node(15);   
-             node node_16 = new node(16);   
-             node node_17 = new node(17);   
-             node node_18 = new node(18);   
-             node node_19 = new node(19);   
-             node node_20 = new node(20);   
-             node node_21 = new node(21);   
-             node node_22 = new node(22);  
- 
- 
-             node root = null;   
-             InsertNode(ref root, node_10);   
-             InsertNode(ref root, node_8);   
-             InsertNode(ref root, node_5);   
-             InsertNode(ref root, node_12);   
-             InsertNode(ref root, node_17);   
-             InsertNode(ref root, node_9);   
-             InsertNode(ref root, node_1);   
-             InsertNode(ref root, node_4);  
- 
结果如下：
![image](http://cnn237111.blog.51cto.com/attachment/201204/28/2359144_1335581561pzjw.png)
![image](http://cnn237111.blog.51cto.com/attachment/201204/28/2359144_1335581568c64Z.png)
![image](http://cnn237111.blog.51cto.com/attachment/201204/28/2359144_1335581572Kdkf.png)
具体代码，看附件。
后记：
AVL树的构建，看起来简单，但是真的写起代码来还是很麻烦。思路清晰是关键。还有很多要改进的地方，比如，平衡因子的计算，本人算法实在不算高明。还有优化的余地，比如回溯法。
**水平有限，如代码中或叙述中有错误，欢迎告知。有更好的算法者，也欢迎告知。**
