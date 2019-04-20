# 平衡二叉树 AVL树结构详解 [Java实现] - 纯洁的微笑 - CSDN博客
2018年11月21日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：136
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqRpodSXxeodVhS8ibrWHQ678UicA5SHDH22LtaMzKAh4s3mhe9f66kWhkNAf47bxeznw0ZQVb1tfwg/640?wx_fmt=jpeg)
作者
NeroJings
来源
https://blog.csdn.net/zhang6622056/article/details/82698859
**本文思维导图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPnIPrURiaHJ4bLSe7Dwa7GMrDFoP9ZEicUPIr7NajomVLiaCx6du02hHCA/640?wx_fmt=png)
**简述**
先不说平衡二叉树，我们单开来说，这样比较方便理解。 
先说二叉树，再说平衡条件，没那么多花里胡哨的理论，我只是想让大家看完能明白，能写出来。
**二叉树**
什么是二叉树？二叉树数据结构，顾名思义，只有两个叉，在数据结构中，操作性能要远高于线性结构，有O(height)的索引性能。与线性结构有相同的空间复杂度,特性如下： 
- 
**每个节点最多只有两个儿子节点**
- 
**左儿子小，右儿子大 (大小按照我们默认的比较规则，本例用int来比较)**
**线行找7与二叉数找7**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPicVcBYY8BicAtpuesibkQdOuic5PFHr32tcw4AXQM9G4xIA4c2icn4HJLcQ/640?wx_fmt=gif)
线性找7
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPOLXPxRk3eAeOAu5M94LHFL83cwe4uIyplLv1fXDPR1nZ76MCcLJ1cw/640?wx_fmt=gif)
二叉树找7 
okay,我想大家聪明人已经看出来了，二叉树搜索用了2次，而线性结构却用了5次。 
说白了，二叉树结构，我每次问一个节点，都会离着我的目标越来越近，但是线性的则不然，我必须一个个问。 
说到这儿，我想会有博友提出质疑了，如果线性查找，7恰好就在第一个呢？那不是一下就找到了吗？ 
哈哈，你怎么不上天呢 - -。还第一个。开个小玩笑。 
这就是二叉树索引的好处。相比看图比码字要清楚的多。
**平衡条件**
那么，什么叫平衡呢？其实很简单，任何一个节点的子节点高度差必须小于2
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGP59ob8FOcFmCDoDT2vveib5t8pMXmRmwUQYkBQSAk3UR8Y9rduiaIXRoQ/640?wx_fmt=png)
第一个二叉平衡树 
- 
从下往上数，第一个高度为1(比较符合日常生活数数),那我们数数吧
- 
5:———1高度 | 4,7,23,71 ———2高度| 6,50 ———3高度 | 15 ———4高度
- 
**比如节点6，那么4和7的高度都是2，那就2-2 < 2 。平衡！！**
**难点一  递归**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPSapDcaklDakAK65jxyvmyccsLxc6x7OLJcO6Cko7FznCxicVlICuD4A/640?wx_fmt=gif)
递归查找 
我又加入了一些节点，方便大家理解递归深度
- 
**每一次****正向****橙色线条的滚动，就是一次递归查找**
- 
**每一次****正向****橙色线条的滚动，方法的入栈！**
- 
**递归的深度，取决于线条走了几次，那就有多大的栈深度**
- 
**本次查找，刨除root，共4次进栈 **
**难点二 回溯**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPCLYADro7Sojs1CibuK2uf2XHSZ3ibZBI4TLNsibEfozTiaBjGAHmSdfSow/640?wx_fmt=gif)
插入回溯
先不要关心这个旋转操作，如图所示，我们在递归的基础上，沿着线条理解一下回溯
- 
**每一次****逆向****橙****色线条的滚动，就是一次回溯**
- 
**操作递归的每一个节点，都会在回溯的轨迹上**
- 
**正因为每一次递归，都有每一次回溯，那么，我们就可以先完成相关操作(增加或删除)之后，判定平衡 **
**4种旋转**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGP498am5PKFOWM3ppHbcOUkjXWeGggy180dC2YjeFfvVd7zs46ACicRKA/640?wx_fmt=gif)
左左类型旋转
博主尽量放慢了速度，让大家看清楚究竟旋转是如何进行的,这是一个插入操作，我们看到在不平衡的时候，进行了左旋转，这里我们看到
- 
**正向插入**，递归3-2-1
- 
**逆向回溯**，1-2 判断平衡条件 ，是平衡的
- 
**再次回溯**，2-3，3的左边高度为2，右边没有节点为0，那么2-0 > 1，不平衡！
**到这里我们基本上理解了平衡的判断，下面正式说一下旋转:**
- 
**判断不平衡边** 在3节点判定，不平衡，那么左边高，我们需要调整左边，获取左边节点2
- 
**判断旋转类型** 这时候我们拿到节点2，判断节点2哪边高。左边高，为左左类型。右边高为左右旋转类型，我们先不管
- 
**旋转操作** 3.left = 2.right; 2.right = 3; 重新计算，2和3节点的高度
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPTIJFVnGWJl5EWYy8rudAtu2icCbcL7vTicEorDotiaQ0aXvT3eufBJFuw/640?wx_fmt=gif)
右右类型旋转[同上，不再叙述] 
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPzcpicqJsqx3j207x0wNU3R96kTHt9JTeic9sbkaTCXick9icP0ocZHibQwA/640?wx_fmt=gif)
左右类型旋转
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/PgqYrEEtEnqv7ibAc03bMMylf9GKTWtGPQM9UUMsiaTbjicFoqNsMopWiblicGHUhb8HvHfEsakk0M2V2dTJ1sGChaA/640?wx_fmt=gif)
右左类型旋转
到此旋转就说完了，希望大家好好的理解第一个左左类型!（理解了一个也就都理解了）
后续部分没有讲是因为说太多反而更乱。 
后续的理解不了没关系，我们代码在看。
**代码基础部分**
**node类**
```
public static class AvlNodeInteger{
        private Integer value;
        private Integer height;
        private AvlNodeInteger left;
        private AvlNodeInteger right;
        public AvlNodeInteger(int t){
            initNode(t,null,null,1);
        }
        public AvlNodeInteger(int t,AvlNodeInteger left,AvlNodeInteger right){
            initNode(t,left,right,null);
        }
        private void initNode(int t,AvlNodeInteger left,AvlNodeInteger right,Integer height){
            this.setValue(t);
            this.left = left;
            this.right = right;
            this.height = height;
        }
        public Integer getValue() {
            return value;
        }
        public void setValue(Integer value) {
            this.value = value;
        }
        public Integer getHeight() {
            return height;
        }
        public void setHeight(Integer height) {
            this.height = height;
        }
        public AvlNodeInteger getLeft() {
            return left;
        }
        public void setLeft(AvlNodeInteger left) {
            this.left = left;
        }
        public AvlNodeInteger getRight() {
            return right;
        }
        public void setRight(AvlNodeInteger right) {
            this.right = right;
        }
    }
```
**高度计算**
/***
     * 求一个节点的高度
     * @param t
     * @return
     */
privateintheight(AvlNodeInteger t){
returnnull == t ? 0 : t.getHeight();
    }
```
/***
     * 求左右子节点最大高度
     * @param left
     * @param right
     * @return
     */
    private int maxHeight(AvlNodeInteger left,AvlNodeInteger right){
        return height(left) > height(right) ? height(left)  : height(right);
    }
```
**插入操作**
**旋转**
```java
/***
     * 左左旋转模型
     * @param node  旋转之前的parent node 节点
     * @return  旋转之后的parent node节点
     */
    private AvlNodeInteger leftLeftRotate(AvlNodeInteger node){
        AvlNodeInteger newRoot = node.getLeft();
        node.setLeft(newRoot.getRight());
        newRoot.setRight(node);
        //由此node的高度降低了，newRoot的高度提高了。
         //newRoot的高度由node的高度而来
        node.setHeight(maxHeight(node.getLeft(),node.getRight())+1);
        newRoot.setHeight(maxHeight(newRoot.getLeft(),newRoot.getRight())+1);
        return newRoot;
    }
    /***
     * 右右旋转模型
     * @param node
     * @return
     */
    private AvlNodeInteger rightRightRotate(AvlNodeInteger node){
        AvlNodeInteger newRoot = node.getRight();
        node.setRight(newRoot.getLeft());
        newRoot.setLeft(node);
        //由此node的高度降低了，newRoot的高度提高了。
        //newRoot的高度由node的高度而来
        node.setHeight(maxHeight(node.getLeft(),node.getRight()));
        newRoot.setHeight(maxHeight(newRoot.getLeft(),newRoot.getRight()));
        return newRoot;
    }
    /**
     * 左右模型，先右右，再左左
     * @param node
     * @return
     */
    private AvlNodeInteger leftRightRotate(AvlNodeInteger node){
        //注意传递的参数
        node.setLeft(rightRightRotate(node.getLeft()));
        return leftLeftRotate(node);
    }
    /***
     * 右左模型，先左左，在右右
     * @param node
     * @return
     */
    private AvlNodeInteger rightLeftRotate(AvlNodeInteger node){
        node.setRight(leftLeftRotate(node.getRight()));
        return rightRightRotate(node);
    }
```
**insert**
```php
/****
     * 对外开放，插入操作
     * @param val
     * @throws Exception
     */
    public void insert(Integer val) throws Exception {
        if(null == root){
            initRoot(val);
            size++;
            return;
        }
        if(contains(val)) throw new Exception("The value is already exist!");
        insertNode(this.root,val);
        size++;
    }
    /**
     * 递归插入
     * parent == null 到最底部插入前节点判断情况
     * @param parent
     * @param val
     * @return
     */
    private AvlNodeInteger insertNode(AvlNodeInteger parent,Integer val){
        if(parent == null){
           return createSingleNode(val);
        }
        if(val < parent.getValue()){    //插入判断，小于父节点，插入到右边
            //注意理解回溯，这里最终返回的是插入完成节点
            //每一层回溯，都会返回相应当时递归的节点！！！
            parent.setLeft(insertNode(parent.getLeft(),val));
            //判断平衡，不要在意这里的parent是谁，
            //这个parent肯定是递归层级上，回溯的一个节点！每一个节点都需要判断平衡
            if(height(parent.getLeft()) - height(parent.getRight()) > 1){
                Integer compareVal = (Integer) parent.getLeft().getValue();
                //左左旋转类型
                if(val < Integer.valueOf(compareVal)){
                    parent = leftLeftRotate(parent);
                }else{                  //左右旋转类型
                    parent = leftRightRotate(parent);
                }
            }
        }
        if(val > parent.getValue()){   //插入判断，小于父节点，插入到右边
            //注意理解回溯，这里最终返回的是插入完成节点
            //每一层回溯，都会返回相应当时递归的节点！！！
            parent.setRight(insertNode(parent.getRight(),val));
            //判断平衡，不要在意这里的parent是谁，
            //这个parent肯定是递归层级上，回溯的一个节点！每一个节点都需要判断平衡
            if(height(parent.getRight()) - height(parent.getLeft()) > 2){
                Integer compareVal = (Integer) parent.getLeft().getValue();
                if(val > compareVal){
                    parent = rightRightRotate(parent);
                }else{
                    parent = rightLeftRotate(parent);
                }
            }
        }
        parent.setHeight((maxHeight(parent.getLeft(),parent.getRight()))+1);
        return parent;
    }
```
**删除操作**
public void remove(Integer val) {
if(null == val || null == root){
return;
        }
if(!contains(val)){
return;
        }
        remove(root,val);
    }
/****
     * AVL删除，平衡树实现
     * @param parent
     * @param val
     * @return
     */
private AvlNodeInteger remove(AvlNodeInteger parent,Integer val){
if(val < parent.getValue()){        //左子树递归查询
//删除以后返回替换的新节点
            AvlNodeInteger newLeft = remove(parent.getLeft(),val);
parent.setLeft(newLeft);
//检查是否平衡，删除的左边，那么用右边-左边
if(height(parent.getRight()) - height(parent.getLeft()) > 1){
                AvlNodeInteger tempNode = parent.getRight();
if(height(tempNode.getLeft()) > height(tempNode.getRight())){       //RL类型
                    rightLeftRotate(parent);
                }else{      //RR类型
                    rightRightRotate(parent);
                }
            }
        }elseif(val > parent.getValue()){  //右子树递归查找
//删除以后返回替换的新节点
            AvlNodeInteger newRight = remove(parent.getRight(),val);
parent.setRight(newRight);
//检查是否平衡
if(height(parent.getLeft()) - height(parent.getRight()) > 1){
                    AvlNodeInteger tempNode = parent.getLeft();
if(height(tempNode.getLeft()) > height(tempNode.getRight())){   //LL类型
                        leftLeftRotate(parent);
                    }else{             //LR类型
                        leftRightRotate(parent);
                    }
            }
        }else{   //相等，匹配成功
if(null != parent.getLeft() && null != parent.getRight()){  //左右子节点都不为空
//判断高度，高的一方，拿到最大(左)，最小(右)的节点，作为替换节点。
//删除原来匹配节点
//左边更高，获取到左边最大的节点
if(parent.getLeft().getHeight() > parent.getRight().getHeight()){
                    AvlNodeInteger leftMax = getMax(parent.getLeft());
parent.setLeft(remove(parent.getLeft(),leftMax.getValue()));
                    leftMax.setLeft(parent.getLeft());
                    leftMax.setRight(parent.getRight());
                    leftMax.setHeight(maxHeight(leftMax.getLeft(),leftMax.getRight()));
parent = leftMax;
                }else{                  //右边更高，获取到右边最小的节点
                    AvlNodeInteger rightMin = getMin(parent.getRight());
parent.setRight(remove(parent.getRight(),rightMin.getValue()));
                    rightMin.setLeft(parent.getLeft());
                    rightMin.setRight(parent.getRight());
                    rightMin.setHeight(maxHeight(parent.getLeft(),parent.getRight())+1);
parent = rightMin;
                }
            }else{
//有任意一方节点为空，则不为空的那一方作为替换节点，删除原来的节点
parent = null;
            }
        }
returnparent;
    }
/***
     * 删除时用到，获取当前节点子节点最大值
     * @param currentRoot
     * @return
     */
private AvlNodeInteger getMax(AvlNodeInteger currentRoot){
if(currentRoot.getRight() != null){
                currentRoot = getMax(currentRoot.getRight());
            }
return currentRoot;
    }
/***
     * 删除时用到，获取当前节点子节点最小值
     * @param currentRoot
     * @return
     */
private AvlNodeInteger getMin(AvlNodeInteger currentRoot){
if(currentRoot.getLeft() != null){
                currentRoot = getMin(currentRoot.getLeft());
            }
return currentRoot;
    }
以上就是难点插入和删除的实现了， 没有过多阐述，是因为大家如果真的理解了上面说明的理论， 那么应该没有问题来理解这些code。 
当然有任何问题大家可以在留言区回复我 ，欢迎大家指正!
**4种遍历**
- 
前序遍历 根左右
- 
中序遍历 左跟右
- 
后序遍历 左右根
- 
层级遍历 从root开始，一层层
```php
/***java
     * 前序遍历
     * 1-根节点
     * 2-左节点
     * 3-右节点
     * 根左右
     * @param parent
     */
    private void xianxu(AvlNodeInteger parent){
        System.out.println(parent.getValue());
        if(null != parent.getLeft()){
            xianxu(parent.getLeft());
        }
        if(null != parent.getRight()){
            xianxu(parent.getRight());
        }
    }
 /***
     * 中序遍历
     * 左节点
     * 根节点
     * 右节点
     * 
     * 左根右
     * @param parent
     */
    private void zhongxu(AvlNodeInteger parent){
        if(null != parent.getLeft()){
            zhongxu(parent.getLeft());
        }
        System.out.println(parent.getValue());
        if(null != parent.getRight()){
            zhongxu(parent.getRight());
        }
    }
    /***
     * 后续遍历
     * 左右根
     * 左节点
     * 右节点
     * 根节点
     */
    private void houxu(AvlNodeInteger parent){
        if(null != parent.getLeft()){
            houxu(parent.getLeft());
        }
        if(null != parent.getRight()){
            houxu(parent.getRight());
        }
        System.out.println(parent);
    }
    /***
     * 层级遍历
     * @param parent
     */
    private void cengji(List<AvlNodeInteger> parent){
        if(null == parent || parent.size() == 0) return;
        //打印当前层
        List<AvlNodeInteger> AvlNodeIntegers = new ArrayList<AvlNodeInteger>();
        int k = 0;
        for(int i = 0 ; i < parent.size() ; i++){
            AvlNodeInteger currentNode = parent.get(i);
            System.out.println(currentNode.getValue()+",");
            if(null != currentNode.getLeft()){
                AvlNodeIntegers.add(currentNode.getLeft());
                k++;
            }
            if(null != currentNode.getRight()){
                AvlNodeIntegers.add(currentNode.getRight());
                k++;
            }
        }
        System.out.println("--------------------------");
        cengji(AvlNodeIntegers);
    }
```
**完整源码****见：平衡二叉树 AVL树结构详解 [Java实现]--源码部分**
END
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnrnzUNX2KRVxAAzQcdevxOMnrGvPJ9mMeUR4XiatKR6F3auSmDICibxseIKoNLfo3fhiam8icfbSl4FTw/640?wx_fmt=jpeg)
