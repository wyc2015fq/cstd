# 红黑树（建议配合MIT的那个算法导论课程一起看） - PeterBishop - CSDN博客





2018年08月06日 23:27:07[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：77








将一个节点插入到红黑树中，需要执行哪些步骤呢？首先，将红黑树当作一颗二叉查找树，将节点插入；然后，将节点着色为红色；最后，通过旋转和重新着色等方法来修正该树，使之重新成为一颗红黑树。详细描述如下：

**第一步: 将红黑树当作一颗二叉查找树，将节点插入。**

       红黑树本身就是一颗二叉查找树，将节点插入后，该树仍然是一颗二叉查找树。也就意味着，树的键值仍然是有序的。此外，无论是左旋还是右旋，若旋转之前这棵树是二叉查找树，旋转之后它一定还是二叉查找树。这也就意味着，任何的旋转和重新着色操作，都不会改变它仍然是一颗二叉查找树的事实。

       好吧？那接下来，我们就来想方设法的旋转以及重新着色，使这颗树重新成为红黑树！

**第二步：将插入的节点着色为"红色"。**

       为什么着色成红色，而不是黑色呢？为什么呢？在回答之前，我们需要重新温习一下红黑树的特性：

(1) 每个节点或者是黑色，或者是红色。

(2) 根节点是黑色。

(3) 每个叶子节点是黑色。 [注意：这里叶子节点，是指为空的叶子节点！]

(4) 如果一个节点是红色的，则它的子节点必须是黑色的。

(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。

       将插入的节点着色为红色，不会违背"特性(5)"！少违背一条特性，就意味着我们需要处理的情况越少。接下来，就要努力的让这棵树满足其它性质即可；满足了的话，它就又是一颗红黑树了。o(∩∩)o...哈哈

**第三步: 通过一系列的旋转或着色等操作，使之重新成为一颗红黑树。**

       第二步中，将插入节点着色为"红色"之后，不会违背"特性(5)"。那它到底会违背哪些特性呢？

       对于"特性(1)"，显然不会违背了。因为我们已经将它涂成红色了。

       对于"特性(2)"，显然也不会违背。在第一步中，我们是将红黑树当作二叉查找树，然后执行的插入操作。而根据二叉查找数的特点，插入操作不会改变根节点。所以，根节点仍然是黑色。

       对于"特性(3)"，显然不会违背了。这里的叶子节点是指的空叶子节点，插入非空节点并不会对它们造成影响。

       对于"特性(4)"，是有可能违背的！

       那接下来，想办法使之"满足特性(4)"，就可以将树重新构造成红黑树了。


||**现象说明**|**处理策略**|
|----|----|----|
|Case 1|当前节点的父节点是红色，且当前节点的祖父节点的另一个子节点（叔叔节点）也是红色。|(01) 将“父节点”设为黑色。			(02) 将“叔叔节点”设为黑色。			(03) 将“祖父节点”设为“红色”。			(04) 将“祖父节点”设为“当前节点”(红色节点)；即，之后继续对“当前节点”进行操作。|
|Case 2|当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的右孩子|(01) 将“父节点”作为“新的当前节点”。			(02) 以“新的当前节点”为支点进行左旋。|
|Case 3|当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的左孩子|(01) 将“父节点”设为“黑色”。			(02) 将“祖父节点”设为“红色”。			(03) 以“祖父节点”为支点进行右旋。|

上面三种情况(Case)处理问题的核心思路都是：将红色的节点移到根节点；然后，将根节点设为黑色。下面对它们详细进行介绍。

**1. (Case 1)叔叔是红色**

**1.1 现象说明**

当前节点(即，被插入节点)的父节点是红色，且当前节点的祖父节点的另一个子节点（叔叔节点）也是红色。

**1.2 处理策略**

(01) 将“父节点”设为黑色。

(02) 将“叔叔节点”设为黑色。

(03) 将“祖父节点”设为“红色”。

(04) 将“祖父节点”设为“当前节点”(红色节点)；即，之后继续对“当前节点”进行操作。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

    “当前节点”和“父节点”都是红色，违背“特性(4)”。所以，将“父节点”设置“黑色”以解决这个问题。

    但是，将“父节点”由“红色”变成“黑色”之后，违背了“特性(5)”：因为，包含“父节点”的分支的黑色节点的总数增加了1。  解决这个问题的办法是：将“祖父节点”由“黑色”变成红色，同时，将“叔叔节点”由“红色”变成“黑色”。关于这里，说明几点：第一，为什么“祖父节点”之前是黑色？这个应该很容易想明白，因为在变换操作之前，该树是红黑树，“父节点”是红色，那么“祖父节点”一定是黑色。 第二，为什么将“祖父节点”由“黑色”变成红色，同时，将“叔叔节点”由“红色”变成“黑色”；能解决“包含‘父节点’的分支的黑色节点的总数增加了1”的问题。这个道理也很简单。“包含‘父节点’的分支的黑色节点的总数增加了1” 同时也意味着 “包含‘祖父节点’的分支的黑色节点的总数增加了1”，既然这样，我们通过将“祖父节点”由“黑色”变成“红色”以解决“包含‘祖父节点’的分支的黑色节点的总数增加了1”的问题； 但是，这样处理之后又会引起另一个问题“包含‘叔叔’节点的分支的黑色节点的总数减少了1”，现在我们已知“叔叔节点”是“红色”，将“叔叔节点”设为“黑色”就能解决这个问题。 所以，将“祖父节点”由“黑色”变成红色，同时，将“叔叔节点”由“红色”变成“黑色”；就解决了该问题。

    按照上面的步骤处理之后：当前节点、父节点、叔叔节点之间都不会违背红黑树特性，但祖父节点却不一定。若此时，祖父节点是根节点，直接将祖父节点设为“黑色”，那就完全解决这个问题了；若祖父节点不是根节点，那我们需要将“祖父节点”设为“新的当前节点”，接着对“新的当前节点”进行分析。

**1.3 示意图**

[**![](https://images0.cnblogs.com/i/497634/201403/251759273578917.jpg)**](https://images0.cnblogs.com/i/497634/201403/251759273578917.jpg)



**2. (Case 2)叔叔是黑色，且当前节点是右孩子**

**2.1 现象说明**

当前节点(即，被插入节点)的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的右孩子

**2.2 处理策略**

(01) 将“父节点”作为“新的当前节点”。

(02) 以“新的当前节点”为支点进行左旋。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

      首先，将“父节点”作为“新的当前节点”；接着，以“新的当前节点”为支点进行左旋。 为了便于理解，我们先说明第(02)步，再说明第(01)步；为了便于说明，我们设置“父节点”的代号为F(Father)，“当前节点”的代号为S(Son)。

为什么要“以F为支点进行左旋”呢？根据已知条件可知：S是F的右孩子。而之前我们说过，我们处理红黑树的核心思想：将红色的节点移到根节点；然后，将根节点设为黑色。既然是“将红色的节点移到根节点”，那就是说要不断的将破坏红黑树特性的红色节点上移(即向根方向移动)。 而S又是一个右孩子，因此，我们可以通过“左旋”来将S上移！ 

      按照上面的步骤(以F为支点进行左旋)处理之后：若S变成了根节点，那么直接将其设为“黑色”，就完全解决问题了；若S不是根节点，那我们需要执行步骤(01)，即“将F设为‘新的当前节点’”。那为什么不继续以S为新的当前节点继续处理，而需要以F为新的当前节点来进行处理呢？这是因为“左旋”之后，F变成了S的“子节点”，即S变成了F的父节点；而我们处理问题的时候，需要从下至上(由叶到根)方向进行处理；也就是说，必须先解决“孩子”的问题，再解决“父亲”的问题；所以，我们执行步骤(01)：将“父节点”作为“新的当前节点”。

**2.2 示意图**

[**![](https://images0.cnblogs.com/i/497634/201403/251801031546918.jpg)**](https://images0.cnblogs.com/i/497634/201403/251801031546918.jpg)



**3. (Case 3)叔叔是黑色，且当前节点是左孩子**

**3.1 现象说明**

当前节点(即，被插入节点)的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的左孩子

**3.2 处理策略**

(01) 将“父节点”设为“黑色”。

(02) 将“祖父节点”设为“红色”。

(03) 以“祖父节点”为支点进行右旋。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

      为了便于说明，我们设置“当前节点”为S(Original Son)，“兄弟节点”为B(Brother)，“叔叔节点”为U(Uncle)，“父节点”为F(Father)，祖父节点为G(Grand-Father)。

      S和F都是红色，违背了红黑树的“特性(4)”，我们可以将F由“红色”变为“黑色”，就解决了“违背‘特性(4)’”的问题；但却引起了其它问题：违背特性(5)，因为将F由红色改为黑色之后，所有经过F的分支的黑色节点的个数增加了1。那我们如何解决“所有经过F的分支的黑色节点的个数增加了1”的问题呢？ 我们可以通过“将G由黑色变成红色”，同时“以G为支点进行右旋”来解决。

**2.3 示意图**

![](https://images0.cnblogs.com/i/497634/201404/170945094945387.jpg)

提示：上面的进行Case 3处理之后，再将节点"120"当作当前节点，就变成了Case 2的情况。







将红黑树内的某一个节点删除。需要执行的操作依次是：首先，将红黑树当作一颗二叉查找树，将该节点从二叉查找树中删除；然后，通过"旋转和重新着色"等一系列来修正该树，使之重新成为一棵红黑树。详细描述如下：

**第一步：将红黑树当作一颗二叉查找树，将节点删除。**

       这和"删除常规二叉查找树中删除节点的方法是一样的"。分3种情况：

       ① 被删除节点没有儿子，即为叶节点。那么，直接将该节点删除就OK了。

       ② 被删除节点只有一个儿子。那么，直接删除该节点，并用该节点的唯一子节点顶替它的位置。

       ③ 被删除节点有两个儿子。那么，先找出它的后继节点；然后把“它的后继节点的内容”复制给“该节点的内容”；之后，删除“它的后继节点”。在这里，后继节点相当于替身，在将后继节点的内容复制给"被删除节点"之后，再将后继节点删除。这样就巧妙的将问题转换为"删除后继节点"的情况了，下面就考虑后继节点。 在"被删除节点"有两个非空子节点的情况下，它的后继节点不可能是双子非空。既然"的后继节点"不可能双子都非空，就意味着"该节点的后继节点"要么没有儿子，要么只有一个儿子。若没有儿子，则按"情况① "进行处理；若只有一个儿子，则按"情况② "进行处理。

**第二步：通过"旋转和重新着色"等一系列来修正该树，使之重新成为一棵红黑树。**

       因为"第一步"中删除节点之后，可能会违背红黑树的特性。所以需要通过"旋转和重新着色"来修正该树，使之重新成为一棵红黑树。



前面我们将"删除红黑树中的节点"大致分为两步，在第一步中"将红黑树当作一颗二叉查找树，将节点删除"后，可能违反"特性(2)、(4)、(5)"三个特性。第二步需要解决上面的三个问题，进而保持红黑树的全部特性。

      为了便于分析，我们假设"x包含一个额外的黑色"(x原本的颜色还存在)，这样就不会违反"特性(5)"。为什么呢？

      通过RB-DELETE算法，我们知道：删除节点y之后，x占据了原来节点y的位置。 既然删除y(y是黑色)，意味着减少一个黑色节点；那么，再在该位置上增加一个黑色即可。这样，当我们假设"x包含一个额外的黑色"，就正好弥补了"删除y所丢失的黑色节点"，也就不会违反"特性(5)"。 因此，假设"x包含一个额外的黑色"(x原本的颜色还存在)，这样就不会违反"特性(5)"。

      现在，x不仅包含它原本的颜色属性，x还包含一个额外的黑色。即x的颜色属性是"红+黑"或"黑+黑"，它违反了"特性(1)"。

      现在，我们面临的问题，由解决"违反了特性(2)、(4)、(5)三个特性"转换成了"解决违反特性(1)、(2)、(4)三个特性"。RB-DELETE-FIXUP需要做的就是通过算法恢复红黑树的特性(1)、(2)、(4)。RB-DELETE-FIXUP的思想是：将x所包含的额外的黑色不断沿树上移(向根方向移动)，直到出现下面的姿态：

a) x指向一个"红+黑"节点。此时，将x设为一个"黑"节点即可。

b) x指向根。此时，将x设为一个"黑"节点即可。

c) 非前面两种姿态。

将上面的姿态，可以概括为3种情况。

① 情况说明：x是“红+黑”节点。

    处理方法：直接把x设为黑色，结束。此时红黑树性质全部恢复。

② 情况说明：x是“黑+黑”节点，且x是根。

    处理方法：什么都不做，结束。此时红黑树性质全部恢复。

③ 情况说明：x是“黑+黑”节点，且x不是根。

    处理方法：这种情况又可以划分为4种子情况。这4种子情况如下表所示：
||**现象说明**|**处理策略**|
|----|----|----|
|**Case 1**|x是"黑+黑"节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。|(01) 将x的兄弟节点设为“黑色”。			(02) 将x的父节点设为“红色”。			(03) 对x的父节点进行左旋。			(04) 左旋后，重新设置x的兄弟节点。|
|**Case 2**|x是“黑+黑”节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。|(01) 将x的兄弟节点设为“红色”。			(02) 设置“x的父节点”为“新的x节点”。|
|**Case 3**|x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。|(01) 将x兄弟节点的左孩子设为“黑色”。			(02) 将x兄弟节点设为“红色”。			(03) 对x的兄弟节点进行右旋。			(04) 右旋后，重新设置x的兄弟节点。|
|**Case 4**|x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色。|(01) 将x父节点颜色 赋值给 x的兄弟节点。			(02) 将x父节点设为“黑色”。			(03) 将x兄弟节点的右子节设为“黑色”。			(04) 对x的父节点进行左旋。			(05) 设置“x”为“根节点”。|



**1. (Case 1)x是"黑+黑"节点，x的兄弟节点是红色**

**1.1 现象说明**

x是"黑+黑"节点，x的兄弟节点是红色。(此时x的父节点和x的兄弟节点的子节点都是黑节点)。

**1.2 处理策略**

(01) 将x的兄弟节点设为“黑色”。

(02) 将x的父节点设为“红色”。

(03) 对x的父节点进行左旋。

(04) 左旋后，重新设置x的兄弟节点。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

      这样做的目的是将“Case 1”转换为“Case 2”、“Case 3”或“Case 4”，从而进行进一步的处理。对x的父节点进行左旋；左旋后，为了保持红黑树特性，就需要在左旋前“将x的兄弟节点设为黑色”，同时“将x的父节点设为红色”；左旋后，由于x的兄弟节点发生了变化，需要更新x的兄弟节点，从而进行后续处理。

**1.3 示意图**

![](https://images0.cnblogs.com/i/497634/201403/251813539515702.jpg)



**2. (Case 2) x是"黑+黑"节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色**

**2.1 现象说明**

x是“黑+黑”节点，x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。

**2.2 处理策略**

(01) 将x的兄弟节点设为“红色”。

(02) 设置“x的父节点”为“新的x节点”。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

      这个情况的处理思想：是将“x中多余的一个黑色属性上移(往根方向移动)”。 x是“黑+黑”节点，我们将x由“黑+黑”节点 变成 “黑”节点，多余的一个“黑”属性移到x的父节点中，即x的父节点多出了一个黑属性(若x的父节点原先是“黑”，则此时变成了“黑+黑”；若x的父节点原先时“红”，则此时变成了“红+黑”)。 此时，需要注意的是：所有经过x的分支中黑节点个数没变化；但是，所有经过x的兄弟节点的分支中黑色节点的个数增加了1(因为x的父节点多了一个黑色属性)！为了解决这个问题，我们需要将“所有经过x的兄弟节点的分支中黑色节点的个数减1”即可，那么就可以通过“将x的兄弟节点由黑色变成红色”来实现。

      经过上面的步骤(将x的兄弟节点设为红色)，多余的一个颜色属性(黑色)已经跑到x的父节点中。我们需要将x的父节点设为“新的x节点”进行处理。若“新的x节点”是“黑+红”，直接将“新的x节点”设为黑色，即可完全解决该问题；若“新的x节点”是“黑+黑”，则需要对“新的x节点”进行进一步处理。

**2.3 示意图**

[**![](https://images0.cnblogs.com/i/497634/201403/251814572322069.jpg)**](https://images0.cnblogs.com/i/497634/201403/251814572322069.jpg)



**3. (Case 3)x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的**

**3.1 现象说明**

x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的左孩子是红色，右孩子是黑色的。

**3.2 处理策略**

(01) 将x兄弟节点的左孩子设为“黑色”。

(02) 将x兄弟节点设为“红色”。

(03) 对x的兄弟节点进行右旋。

(04) 右旋后，重新设置x的兄弟节点。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

       我们处理“Case 3”的目的是为了将“Case 3”进行转换，转换成“Case 4”,从而进行进一步的处理。转换的方式是对x的兄弟节点进行右旋；为了保证右旋后，它仍然是红黑树，就需要在右旋前“将x的兄弟节点的左孩子设为黑色”，同时“将x的兄弟节点设为红色”；右旋后，由于x的兄弟节点发生了变化，需要更新x的兄弟节点，从而进行后续处理。

**3.3 示意图**

[**![](https://images0.cnblogs.com/i/497634/201403/251815496235531.jpg)**](https://images0.cnblogs.com/i/497634/201403/251815496235531.jpg)



**4. (Case 4)x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色**

**4.1 现象说明**

x是“黑+黑”节点，x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色。

**4.2 处理策略**

(01) 将x父节点颜色 赋值给 x的兄弟节点。

(02) 将x父节点设为“黑色”。

(03) 将x兄弟节点的右子节设为“黑色”。

(04) 对x的父节点进行左旋。

(05) 设置“x”为“根节点”。

**下面谈谈为什么要这样处理。**(建议理解的时候，通过下面的图进行对比)

      我们处理“Case 4”的目的是：去掉x中额外的黑色，将x变成单独的黑色。处理的方式是“：进行颜色修改，然后对x的父节点进行左旋。下面，我们来分析是如何实现的。

      为了便于说明，我们设置“当前节点”为S(Original Son)，“兄弟节点”为B(Brother)，“兄弟节点的左孩子”为BLS(Brother's Left Son)，“兄弟节点的右孩子”为BRS(Brother's Right Son)，“父节点”为F(Father)。

      我们要对F进行左旋。但在左旋前，我们需要调换F和B的颜色，并设置BRS为黑色。为什么需要这里处理呢？因为左旋后，F和BLS是父子关系，而我们已知BL是红色，如果F是红色，则违背了“特性(4)”；为了解决这一问题，我们将“F设置为黑色”。 但是，F设置为黑色之后，为了保证满足“特性(5)”，即为了保证左旋之后：

      第一，“同时经过根节点和S的分支的黑色节点个数不变”。

             若满足“第一”，只需要S丢弃它多余的颜色即可。因为S的颜色是“黑+黑”，而左旋后“同时经过根节点和S的分支的黑色节点个数”增加了1；现在，只需将S由“黑+黑”变成单独的“黑”节点，即可满足“第一”。

      第二，“同时经过根节点和BLS的分支的黑色节点数不变”。

             若满足“第二”，只需要将“F的原始颜色”赋值给B即可。之前，我们已经将“F设置为黑色”(即，将B的颜色"黑色"，赋值给了F)。至此，我们算是调换了F和B的颜色。

      第三，“同时经过根节点和BRS的分支的黑色节点数不变”。

             在“第二”已经满足的情况下，若要满足“第三”，只需要将BRS设置为“黑色”即可。

经过，上面的处理之后。红黑树的特性全部得到的满足！接着，我们将x设为根节点，就可以跳出while循环(参考伪代码)；即完成了全部处理。

至此，我们就完成了Case 4的处理。理解Case 4的核心，是了解如何“去掉当前节点额外的黑色”。

**4.3 示意图**

![](https://images0.cnblogs.com/i/497634/201403/251817189983393.jpg)



```cpp
/**
* C语言实现的红黑树(Red Black Tree)
*
* @author skywang
* @date 2013/11/18
*/

#include <stdio.h>
#include <stdlib.h>

#define _RED_BLACK_TREE_H_
#define RED        0    // 红色节点
#define BLACK    1    // 黑色节点

typedef int Type;

// 红黑树的节点
typedef struct RBTreeNode {
	unsigned char color;        // 颜色(RED 或 BLACK)
	Type   key;                    // 关键字(键值)
	struct RBTreeNode *left;    // 左孩子
	struct RBTreeNode *right;    // 右孩子
	struct RBTreeNode *parent;    // 父结点
}Node, *RBTree;

// 红黑树的根
typedef struct rb_root {
	Node *node;
}RBRoot;


#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

/*
* 创建红黑树，返回"红黑树的根"！
*/
RBRoot* create_rbtree()
{
	RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
	root->node = NULL;

	return root;
}

/*
* 前序遍历"红黑树"
*/
static void preorder(RBTree tree)
{
	if (tree != NULL)
	{
		printf("%d ", tree->key);
		preorder(tree->left);
		preorder(tree->right);
	}
}
void preorder_rbtree(RBRoot *root)
{
	if (root)
		preorder(root->node);
}

/*
* 中序遍历"红黑树"
*/
static void inorder(RBTree tree)
{
	if (tree != NULL)
	{
		inorder(tree->left);
		printf("%d ", tree->key);
		inorder(tree->right);
	}
}

void inorder_rbtree(RBRoot *root)
{
	if (root)
		inorder(root->node);
}

/*
* 后序遍历"红黑树"
*/
static void postorder(RBTree tree)
{
	if (tree != NULL)
	{
		postorder(tree->left);
		postorder(tree->right);
		printf("%d ", tree->key);
	}
}

void postorder_rbtree(RBRoot *root)
{
	if (root)
		postorder(root->node);
}

/*
* (递归实现)查找"红黑树x"中键值为key的节点
*/
static Node* search(RBTree x, Type key)
{
	if (x == NULL || x->key == key)
		return x;

	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}
int rbtree_search(RBRoot *root, Type key)
{
	if (root)
		return search(root->node, key) ? 0 : -1;
}

/*
* (非递归实现)查找"红黑树x"中键值为key的节点
*/
static Node* iterative_search(RBTree x, Type key)
{
	while ((x != NULL) && (x->key != key))
	{
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}
int iterative_rbtree_search(RBRoot *root, Type key)
{
	if (root)
		return iterative_search(root->node, key) ? 0 : -1;
}

/*
* 查找最小结点：返回tree为根结点的红黑树的最小结点。
*/
static Node* minimum(RBTree tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->left != NULL)
		tree = tree->left;
	return tree;
}

int rbtree_minimum(RBRoot *root, int *val)
{
	Node *node;

	if (root)
		node = minimum(root->node);

	if (node == NULL)
		return -1;

	*val = node->key;
	return 0;
}

/*
* 查找最大结点：返回tree为根结点的红黑树的最大结点。
*/
static Node* maximum(RBTree tree)
{
	if (tree == NULL)
		return NULL;

	while (tree->right != NULL)
		tree = tree->right;
	return tree;
}

int rbtree_maximum(RBRoot *root, int *val)
{
	Node *node;

	if (root)
		node = maximum(root->node);

	if (node == NULL)
		return -1;

	*val = node->key;
	return 0;
}

/*
* 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
*/
static Node* rbtree_successor(RBTree x)
{
	// 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
	if (x->right != NULL)
		return minimum(x->right);

	// 如果x没有右孩子。则x有以下两种可能：
	// (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
	// (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
	Node* y = x->parent;
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
* 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
*/
static Node* rbtree_predecessor(RBTree x)
{
	// 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
	if (x->left != NULL)
		return maximum(x->left);

	// 如果x没有左孩子。则x有以下两种可能：
	// (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
	// (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
	Node* y = x->parent;
	while ((y != NULL) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
* 对红黑树的节点(x)进行左旋转
*
* 左旋示意图(对节点x进行左旋)：
*      px                              px
*     /                               /
*    x                               y
*   /  \      --(左旋)-->           / \                #
*  lx   y                          x  ry
*     /   \                       /  \
*    ly   ry                     lx  ly
*
*
*/
static void rbtree_left_rotate(RBRoot *root, Node *x)
{
	// 设置x的右孩子为y
	Node *y = x->right;

	// 将 “y的左孩子” 设为 “x的右孩子”；
	// 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;

	// 将 “x的父亲” 设为 “y的父亲”
	y->parent = x->parent;

	if (x->parent == NULL)
	{
		//tree = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
		root->node = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
	}
	else
	{
		if (x->parent->left == x)
			x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
		else
			x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
	}

	// 将 “x” 设为 “y的左孩子”
	y->left = x;
	// 将 “x的父节点” 设为 “y”
	x->parent = y;
}

/*
* 对红黑树的节点(y)进行右旋转
*
* 右旋示意图(对节点y进行左旋)：
*            py                               py
*           /                                /
*          y                                x
*         /  \      --(右旋)-->            /  \                     #
*        x   ry                           lx   y
*       / \                                   / \                   #
*      lx  rx                                rx  ry
*
*/
static void rbtree_right_rotate(RBRoot *root, Node *y)
{
	// 设置x是当前节点的左孩子。
	Node *x = y->left;

	// 将 “x的右孩子” 设为 “y的左孩子”；
	// 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;

	// 将 “y的父亲” 设为 “x的父亲”
	x->parent = y->parent;

	if (y->parent == NULL)
	{
		//tree = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
		root->node = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
	}
	else
	{
		if (y == y->parent->right)
			y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
		else
			y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
	}

	// 将 “y” 设为 “x的右孩子”
	x->right = y;

	// 将 “y的父节点” 设为 “x”
	y->parent = x;
}

/*
* 红黑树插入修正函数
*
* 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
* 目的是将它重新塑造成一颗红黑树。
*
* 参数说明：
*     root 红黑树的根
*     node 插入的结点        // 对应《算法导论》中的z
*/
static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
	Node *parent, *gparent;

	// 若“父节点存在，并且父节点的颜色是红色”
	while ((parent = rb_parent(node)) && rb_is_red(parent))
	{
		gparent = rb_parent(parent);

		//若“父节点”是“祖父节点的左孩子”
		if (parent == gparent->left)
		{
			// Case 1条件：叔叔节点是红色
			{
				Node *uncle = gparent->right;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			// Case 2条件：叔叔是黑色，且当前节点是右孩子
			if (parent->right == node)
			{
				Node *tmp;
				rbtree_left_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			// Case 3条件：叔叔是黑色，且当前节点是左孩子。
			rb_set_black(parent);
			rb_set_red(gparent);
			rbtree_right_rotate(root, gparent);
		}
		else//若“z的父节点”是“z的祖父节点的右孩子”
		{
			// Case 1条件：叔叔节点是红色
			{
				Node *uncle = gparent->left;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			// Case 2条件：叔叔是黑色，且当前节点是左孩子
			if (parent->left == node)
			{
				Node *tmp;
				rbtree_right_rotate(root, parent);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			// Case 3条件：叔叔是黑色，且当前节点是右孩子。
			rb_set_black(parent);
			rb_set_red(gparent);
			rbtree_left_rotate(root, gparent);
		}
	}

	// 将根节点设为黑色
	rb_set_black(root->node);
}

/*
* 添加节点：将节点(node)插入到红黑树中
*
* 参数说明：
*     root 红黑树的根
*     node 插入的结点        // 对应《算法导论》中的z
*/
static void rbtree_insert(RBRoot *root, Node *node)
{
	Node *y = NULL;
	Node *x = root->node;

	// 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
	while (x != NULL)
	{
		y = x;
		if (node->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	rb_parent(node) = y;

	if (y != NULL)
	{
		if (node->key < y->key)
			y->left = node;                // 情况2：若“node所包含的值” < “y所包含的值”，则将node设为“y的左孩子”
		else
			y->right = node;            // 情况3：(“node所包含的值” >= “y所包含的值”)将node设为“y的右孩子” 
	}
	else
	{
		root->node = node;                // 情况1：若y是空节点，则将node设为根
	}

	// 2. 设置节点的颜色为红色
	node->color = RED;

	// 3. 将它重新修正为一颗二叉查找树
	rbtree_insert_fixup(root, node);
}

/*
* 创建结点
*
* 参数说明：
*     key 是键值。
*     parent 是父结点。
*     left 是左孩子。
*     right 是右孩子。
*/
static Node* create_rbtree_node(Type key, Node *parent, Node *left, Node* right)
{
	Node* p;

	if ((p = (Node *)malloc(sizeof(Node))) == NULL)
		return NULL;
	p->key = key;
	p->left = left;
	p->right = right;
	p->parent = parent;
	p->color = BLACK; // 默认为黑色

	return p;
}

/*
* 新建结点(节点键值为key)，并将其插入到红黑树中
*
* 参数说明：
*     root 红黑树的根
*     key 插入结点的键值
* 返回值：
*     0，插入成功
*     -1，插入失败
*/
int insert_rbtree(RBRoot *root, Type key)
{
	Node *node;    // 新建结点

				   // 不允许插入相同键值的节点。
				   // (若想允许插入相同键值的节点，注释掉下面两句话即可！)
	if (search(root->node, key) != NULL)
		return -1;

	// 如果新建结点失败，则返回。
	if ((node = create_rbtree_node(key, NULL, NULL, NULL)) == NULL)
		return -1;

	rbtree_insert(root, node);

	return 0;
}

/*
* 红黑树删除修正函数
*
* 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
* 目的是将它重新塑造成一颗红黑树。
*
* 参数说明：
*     root 红黑树的根
*     node 待修正的节点
*/
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
	Node *other;

	while ((!node || rb_is_black(node)) && node != root->node)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (rb_is_red(other))
			{
				// Case 1: x的兄弟w是红色的  
				rb_set_black(other);
				rb_set_red(parent);
				rbtree_left_rotate(root, parent);
				other = parent->right;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->right || rb_is_black(other->right))
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					rb_set_black(other->left);
					rb_set_red(other);
					rbtree_right_rotate(root, other);
					other = parent->right;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->right);
				rbtree_left_rotate(root, parent);
				node = root->node;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (rb_is_red(other))
			{
				// Case 1: x的兄弟w是红色的  
				rb_set_black(other);
				rb_set_red(parent);
				rbtree_right_rotate(root, parent);
				other = parent->left;
			}
			if ((!other->left || rb_is_black(other->left)) &&
				(!other->right || rb_is_black(other->right)))
			{
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->left || rb_is_black(other->left))
				{
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
					rb_set_black(other->right);
					rb_set_red(other);
					rbtree_left_rotate(root, other);
					other = parent->left;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->left);
				rbtree_right_rotate(root, parent);
				node = root->node;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}

/*
* 删除结点
*
* 参数说明：
*     tree 红黑树的根结点
*     node 删除的结点
*/
void rbtree_delete(RBRoot *root, Node *node)
{
	Node *child, *parent;
	int color;

	// 被删除节点的"左右孩子都不为空"的情况。
	if ((node->left != NULL) && (node->right != NULL))
	{
		// 被删节点的后继节点。(称为"取代节点")
		// 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
		Node *replace = node;

		// 获取后继节点
		replace = replace->right;
		while (replace->left != NULL)
			replace = replace->left;

		// "node节点"不是根节点(只有根节点不存在父节点)
		if (rb_parent(node))
		{
			if (rb_parent(node)->left == node)
				rb_parent(node)->left = replace;
			else
				rb_parent(node)->right = replace;
		}
		else
			// "node节点"是根节点，更新根节点。
			root->node = replace;

		// child是"取代节点"的右孩子，也是需要"调整的节点"。
		// "取代节点"肯定不存在左孩子！因为它是一个后继节点。
		child = replace->right;
		parent = rb_parent(replace);
		// 保存"取代节点"的颜色
		color = rb_color(replace);

		// "被删除节点"是"它的后继节点的父节点"
		if (parent == node)
		{
			parent = replace;
		}
		else
		{
			// child不为空
			if (child)
				rb_set_parent(child, parent);
			parent->left = child;

			replace->right = node->right;
			rb_set_parent(node->right, replace);
		}

		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;

		if (color == BLACK)
			rbtree_delete_fixup(root, child, parent);
		free(node);

		return;
	}

	if (node->left != NULL)
		child = node->left;
	else
		child = node->right;

	parent = node->parent;
	// 保存"取代节点"的颜色
	color = node->color;

	if (child)
		child->parent = parent;

	// "node节点"不是根节点
	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root->node = child;

	if (color == BLACK)
		rbtree_delete_fixup(root, child, parent);
	free(node);
}

/*
* 删除键值为key的结点
*
* 参数说明：
*     tree 红黑树的根结点
*     key 键值
*/
void delete_rbtree(RBRoot *root, Type key)
{
	Node *z, *node;

	if ((z = search(root->node, key)) != NULL)
		rbtree_delete(root, z);
}

/*
* 销毁红黑树
*/
static void rbtree_destroy(RBTree tree)
{
	if (tree == NULL)
		return;

	if (tree->left != NULL)
		rbtree_destroy(tree->left);
	if (tree->right != NULL)
		rbtree_destroy(tree->right);

	free(tree);
}

void destroy_rbtree(RBRoot *root)
{
	if (root != NULL)
		rbtree_destroy(root->node);

	free(root);
}

/*
* 打印"红黑树"
*
* tree       -- 红黑树的节点
* key        -- 节点的键值
* direction  --  0，表示该节点是根节点;
*               -1，表示该节点是它的父结点的左孩子;
*                1，表示该节点是它的父结点的右孩子。
*/
static void rbtree_print(RBTree tree, Type key, int direction)
{
	if (tree != NULL)
	{
		if (direction == 0)    // tree是根节点
			printf("%2d(B) is root\n", tree->key);
		else                // tree是分支节点
			printf("%2d(%s) is %2d's %6s child\n", tree->key, rb_is_red(tree) ? "R" : "B", key, direction == 1 ? "right" : "left");

		rbtree_print(tree->left, tree->key, -1);
		rbtree_print(tree->right, tree->key, 1);
	}
}

void print_rbtree(RBRoot *root)
{
	if (root != NULL && root->node != NULL)
		rbtree_print(root->node, root->node->key, 0);
}
```





