# AVL树的插入与删除 - ggabcda的博客 - CSDN博客





2017年08月26日 18:09:27[ggabcda](https://me.csdn.net/ggabcda)阅读数：176








AVL 树要在插入和删除结点后保持平衡，旋转操作必不可少。关键是理解什么时候应该左旋、右旋和双旋。在Youtube上看到一位老师的视频对这个概念讲解得非常清楚，再结合算法书和网络的博文，记录如下。

## 1.1 AVL 的旋转

一棵AVL树是其每个节点的左子树和右子树的高度差最多为1的二叉查找树(空树高度定义为-1)。AVL树插入和删除时都可能破坏AVL的特性，可以通过对树进行修正来保证特性，修正方法称为旋转。

下面以4个插入操作为例，说明不同旋转对应的场景。

### 1.1.1 LL-R

![](http://www.lillianyl.com/wp-content/uploads/2017/08/LL-e1503330161328.jpg)

插入结点为6，沿着插入路径向上找到第一个不平衡的结点18；从18开始往下看插入方式，16是18的左孩子的左孩子，这种方式称为LL，用来平衡的旋转方式为右旋，即R。

### 1.1.2 RR-L

![](http://www.lillianyl.com/wp-content/uploads/2017/08/RR-e1503330391454.jpg)

插入结点为48，沿着插入路径向上找到第一个不平衡的结点24；从24开始往下看插入方式，48是24的右孩子的右孩子，这种方式称为RR，用来平衡的旋转方式为左旋，即L。

### 1.1.3 LR-LR

![](http://www.lillianyl.com/wp-content/uploads/2017/08/LR-e1503330569520.jpg)

插入的结点是21，沿着插入路径向上找到第一个不平衡的结点28；从28开始往下看插入方式，21是28的左孩子的右孩子，这种方式称为LR, 用来平衡的旋转方式为双旋转，称为LR旋转。LR旋转即先把R路径行的15进行L旋转，再把L路径上的18进行R旋转。注意顺序。

### 1.1.4 RL-RL

![](http://www.lillianyl.com/wp-content/uploads/2017/08/RL.jpg)

插入的结点是36，沿着插入路径向上找到第一个不平衡的结点24；从24开始往下看插入方式，36是24的右孩子的左孩子，这种方式称为RL, 用来平衡的旋转方式为双旋转，称为RL旋转。RL旋转即先把L路径行的91进行R旋转，再把R路径上的24进行L旋转。注意顺序。

### 1.1.5 Problem-Solution

LL问题，使用R旋转；

RR问题，使用L旋转；

LR问题，使用LR旋转(再次强调是先把R分支进行L旋转，再把L分支进行R旋转)

RL问题，使用RL旋转(再次强调是先把L分支进行R旋转，再把R分支进行L旋转)

## 1.2插入

每次插入后判断树是否平衡，有两种方式：一是每个结点存储平衡因子并随时更新；二是每个结点存储当前的高度(叶结点为0，每向上一层高度+1)。据相关资料，“存储平衡因子所得到的些微的速度优势很难抵消清晰度和相对简明性的损失”，实际需要的也只是两棵子树的高度差，所以建议在每个结点上存储树的高度。



>   struct AvlNode
{
	ElementType Element;
	AvlTree Left;
	AvlTree Right;
	int Height;
	struct AvlNode()
	{
		Element = -1;
		Left = NULL;
		Right = NULL;
		Height = 0;
	}
};

插入结点时，如果树的高度不变，那么插入完成；如果高度出现不平衡，则根据插入情况适当做单旋或者双旋，更新高度(并解决好与树其他部分的连接)，从而完成插入。只需要判断高度差为2的情况，因为一出现不平衡，就立刻调整，所以高度差不会大于2。整个插入过程，就是二叉搜索树的插入+平衡调整。

>   
AvlTree Insert(ElementType x, AvlTree T)
{
	if (T == NULL)
	{
		//创建并返回一棵单结点的树
		T = new AvlNode();
		assert(T);
		T->Element = x;
		return T;
	}
	else if (x < T->Element)
	{
		T->Left = Insert(x, T->Left);
		//调整最深的不平衡的结点
		//高度不平衡时，该结点的两棵子树高度差为2
		if (Height(T->Left) - Height(T->Right) == 2)
		{
			if (x < T->Left->Element)//Problem: LL->Solution:R
				T = SingleRotateR(T);
			else  //Problem: LR->Solution: LR
				T = DoubleRotateLR(T);
		}
	}
	else if (x > T->Element)
	{
		T->Right = Insert(x, T->Right);
		if (Height(T->Right) - Height(T->Left) == 2)
		{
			if (x > T->Right->Element)//Problem: RR->Solution:L
				T = SingleRotateL(T);
			else//Problem: RL->Solution:RL
				T = DoubleRotateRL(T);
		}
	}
	T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
	return T;
}

下面来看看旋转。沿插入点向上的第一个不平衡的点(也是最深的不平衡点)，记为α点，则L旋转和R旋转，LR旋转和RL旋转，分别都是关于这一点的镜像对称，因此我们就讨论R旋转和LR旋转。

**R旋转：**

**![](http://www.lillianyl.com/wp-content/uploads/2017/08/%E5%8D%95%E6%97%8B%E8%BD%AC-300x68.png)**

在子树X中插入结点，假设K2为α点，这是LL问题，所以要进行R旋转。根据上图调整指针：

>   
Position SingleRotateR(Position k2)
{
	Position k1;

	k1 = k2->Left;
	k2->Left = k1->Right;
	k1->Right = k2;

	k2->Height = Max(Height(k2->Left), Height(k2->Right)) + 1;
	k1->Height = Max(Height(k1->Left), Height(k1->Right)) + 1;

	//k1为新的根结点
	return k1;
}


**LR旋转：**

**![](http://www.lillianyl.com/wp-content/uploads/2017/08/%E5%8F%8C%E6%97%8B%E8%BD%AC1-300x81.png)**

如上图，假设K2是α点，插入位置是在Y子树中。这里我们把Y子树看做一个由一个根结点和两棵子树构成，如下图：

![](http://www.lillianyl.com/wp-content/uploads/2017/08/%E5%8F%8C%E6%97%8B%E8%BD%AC2-300x85.png)



根据我们在第一节中讲的，LR问题用LR旋转解决，先对R分支进行L旋转，再对L分支进行R旋转：

>   
Position DoubleRotateLR(Position k3)
{
	k3->Left = SingleRotateL(k3->Left);
	return SingleRotateR(k3);
}






## 1.3 删除

AVL的删除和二叉搜索树的删除很像，多了平衡的操作。 这里贴出伪代码来理解，完整代码实现：https://github.com/yulifromchina/exercise/tree/master/algorithm/tree/avl%20tree。

伪代码大致如下：

>  
Delete(Tree, data)
{
	if(data < Tree->data) Delete(Tree->left, data);**balance(Tree);**
        else if(data > Tree->data) Delete(Tree->right,data);**balance(Tree);**
        else
             if(Tree->left && Tree->right)
                    replace Tree->data with successor or precursor;
                    Delete(Tree->right/left, successor/precursor);
                    **balance(Tree);**
             else
                   if(Tree->left==NULL)
                         Tree = Tree->right;
                   else if(Tree->right==NULL)
                         Tree = Tree->left;
                   free(Tree's origin memory)
        **update(Tree's Height)**}



当待删除结点是叶子结点时，直接删除即可，不用考虑平衡，因为这是递归调用过程，返回上一层叶子结点的父结点时会进行平衡；当待删除结点是只有左子树或右子树时，用该结点的父节点指向左子树或者右子树，再释放该结点；当待删除结点儿女双全时，用前驱或后继赋值，并递归删除前驱或后继。是不是很二叉搜索树很像？另外，每次递归回来都需要平衡一下，这其实就是从插入的结点向上平衡到根结点的过程。

## 1.4 参考
- Youtebu视频：Basics of AVL tree. 
- 
《数据结构与算法分析：C语言描述》

-  一步一步写平衡二叉树（AVL树） 

(完)





