# 数据结构与算法之PHP实现二叉树的遍历 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年03月26日 11:53:28[baofenny](https://me.csdn.net/jiaminbao)阅读数：41
**一、二叉树的遍历**
以某种特定顺序访问树中所有的节点称为**树的遍历**，遍历二叉树可分**深度优先遍历**和**广度优先遍历**。
**深度优先遍历**：对每一个可能的分支路径深入到不能再深入为止，而且每个节点只能访问一次。可以细分为**先序**遍历、**中序**遍历、**后序**遍历。
|深度优先遍历|先序遍历|中序遍历|后序遍历|
|----|----|----|----|
|解释|对任一子树，先访问根，然后遍历其左子树，最后遍历其右子树。即根节点->左子树->右子树。|对任一子树，先遍历其左子树，然后访问根，最后遍历其右子树。即左子树->根节点->右子树。|对任一子树，先遍历其左子树，然后遍历其右子树，最后访问根。即左子树->右子树->根节点。|
|原则|①输出根。②访问左子树。【先访问左子树中的左子树，再访问左子树中的右子树。】直到访问到叶子节点后输出。③访问右子树。【先访问右子树中的左子树，再访问右子树中的右子树。】直到访问到叶子节点后输出。|①访问左子树。【先访问左子树中的左子树，再访问左子树中的右子树。】直到访问到叶子节点后输出。②输出根。③访问右子树。【先访问右子树中的左子树，再访问右子树中的右子树。】直到访问到叶子节点后输出。|①访问左子树。【先访问左子树中的左子树，再访问左子树中的右子树】。直到访问到叶子节点后输出。②访问右子树。【先访问右子树中的左子树，再访问右子树中的右子树】。直到访问到叶子节点后输出。③再返回访问根，并输出。|
|遍历步骤![](https://img-blog.csdnimg.cn/20190326115317477.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW1pbmJhbw==,size_16,color_FFFFFF,t_70)|A作为根，先输出A。从A开始，先访问A的左子树。B为左子树的根节点，输出B。B的左子树为D，输出D。D无左右子树，则看B的右子树，为E，输出E。E无左右子树，则A的左子树全部输出完。再访问A的右子树，C为右子树的根节点，输出C。C的左子树为F，输出F。F无左右子树，且C无右子树，A的右子树全部输出完。|A作为根。从A开始，先访问A的左子树。B的左子树为D，输出D。D无左右子树，则B的左子树已访问完，访问并输出B。再看B的右子树，为E，输出E。E无左右子树，则A的左子树全部输出完，返回并输出A。同理，再看A的右子树。C的左子树为F，输出F。F无左右子树，则C的左子树已访问完，返回并输出C。C无右子树，则A的右子树全部输出完。|先访问A的左子树。再访问左子树中的左子树。即A的左子树为B，再访问B的左子树D。D没有左右子树，输出D。然后访问左子树中的右子树。即访问B的右子树E，E没有左右子树，输出E。再输出B。然后访问A的右子树。再访问右子树中的左子树。即A的右子树为C，再访问C的左子树F。F没有左右子树，输出F。然后访问右子树中的右子树。即访问C的右子树，C没有右子树，输出C。再输出A。|
|遍历结果|A（BDE）（CF）|（DBE）A（FC）|（DEB）（FC）A|



**广度优先遍历**：又叫层次遍历，从上往下对每一层依次访问，在每一层中，从左往右（也可以从右往左）访问节点，访问完一层就进入下一层，直到没有节点访问为止。
**二、PHP用递归、非递归方式实现深度优先遍历二叉树（先序、中序、后序）**
1、先序遍历
<?php
class Node {
    public $value;
    public $left;
    public $right;
}
class BT {
    // 非递归
    // 前序遍历 根节点→左子树→右子树
    // 先访问根节点，再遍历左子树，最后遍历右子树；并且在遍历左右子树时，仍需先遍历根节点，然后访问左子树，最后遍历右子树
    public function preOrder($root) {
        $stack = array();
        array_push($stack, $root);
        while (!empty($stack)) {
            $center_node = array_pop($stack);
            echo $center_node->value . "  "; // 先输出根节点
            if ($center_node->right != null) {
                array_push($stack, $center_node->right); // 压入左子树
            }
            if ($center_node->left != null) {
                array_push($stack, $center_node->left);
            }
        }
    }
    // 递归
    // 前序遍历
    public function pre_order($root) {
        if ($root != null) {
            echo $root->value . " ";        // 根
            if ($root->left != null) {
                $this->pre_order($root->left);     //递归遍历左树
            }
            if ($root->right != null) {
                $this->pre_order($root->right);    //递归遍历右树
            }
        }
    }
}
// 测试
$a = new Node();
$b = new Node();
$c = new Node();
$d = new Node();
$e = new Node();
$f = new Node();
$a->value = "A";
$b->value = "B";
$c->value = "C";
$d->value = "D";
$e->value = "E";
$f->value = "F";
$a->left = $b;
$a->right = $c;
$b->left = $d;
$b->right = $e;
$c->left = $f;
$bst = new BT();
echo "----深度优先----";
echo "</br>";
echo "非递归--前序遍历：";
$bst->preOrder($a);
echo "</br>";
echo "递归--前序遍历：";
$bst->pre_order($a);
2、中序遍历
class Node {
    public $value;
    public $left;
    public $right;
}
class BT {
    // 非递归
    // 中序遍历
    // 左子树→根节点→右子树
    public function inOrder($root) {
        $stack = array();
        $current_node = $root;
        while (!empty($stack) || $current_node != null) {
            while ($current_node != null) {
                array_push($stack, $current_node);
                $current_node = $current_node->left;
            }
            $current_node = array_pop($stack);
            echo $current_node->value . " ";
            $current_node = $current_node->right;
        }
    }
    // 递归
    // 中序遍历
    public function in_order($root) {
        if ($root != null) {
            if ($root->left != null) {
                $this->in_order($root->left);  // 递归遍历左树
            }
            echo $root->value . " ";
            if ($root->right != null) {
                $this->in_order($root->right); // 递归遍历右树
            }
        }
    }
}
// 测试
$a = new Node();
$b = new Node();
$c = new Node();
$d = new Node();
$e = new Node();
$f = new Node();
$a->value = "A";
$b->value = "B";
$c->value = "C";
$d->value = "D";
$e->value = "E";
$f->value = "F";
$a->left = $b;
$a->right = $c;
$b->left = $d;
$b->right = $e;
$c->left = $f;
$bst = new BT();
echo "----深度优先----";
echo "</br>";
echo "非递归--中序遍历：";
$bst->inOrder($a);
echo "</br>";
echo "递归--中序遍历：";
$bst->in_order($a);
echo "</br>";
3、后序遍历
<?php
class Node {
    public $value;
    public $left;
    public $right;
}
class BT {
    // 非递归
    // 后序遍历 左子树→右子树→根节点
    // 先遍历左子树，然后遍历右子树，最后访问根节点；同样，在遍历左右子树的时候同样要先遍历左子树，然后遍历右子树，最后访问根节点
    public function postOrder($root) {
        $stack = array();
        $out_stack = array();
        array_push($stack, $root);
        while (!empty($stack)) {
            $center_node = array_pop($stack);
            array_push($out_stack, $center_node); // 最先压入根节点，最后输出
            if ($center_node->left != null) {
                array_push($stack, $center_node->left);
            }
            if ($center_node->right != null) {
                array_push($stack, $center_node->right);
            }
        }
        while (!empty($out_stack)) {
            $center_node = array_pop($out_stack);
            echo $center_node->value . "  ";
        }
    }
    // 递归
    // 后序遍历
    public function post_order($root) {
        if ($root != null) {
            if ($root->left != null) {
                $this->post_order($root->left);  // 递归遍历左树
            }
            if ($root->right != null) {
                $this->post_order($root->right); // 递归遍历右树
            }
            echo $root->value . " ";    // 根
        }
    }
}
// 测试
$a = new Node();
$b = new Node();
$c = new Node();
$d = new Node();
$e = new Node();
$f = new Node();
$a->value = "A";
$b->value = "B";
$c->value = "C";
$d->value = "D";
$e->value = "E";
$f->value = "F";
$a->left = $b;
$a->right = $c;
$b->left = $d;
$b->right = $e;
$c->left = $f;
$bst = new BT();
echo "----深度优先----";
echo "</br>";
echo "非递归--后序遍历：";
$bst->postOrder($a);
echo "</br>";
echo "递归--后序遍历：";
$bst->post_order($a);
echo "</br>";
**三、PHP用递归、非递归方式实现广度优先遍历二叉树**
<?php
class Node {
    public $value;
    public $left;
    public $right;
}
class BT {
    // 非递归
    public function levelOrder($root) {
        if ($root == null) {
            return;
        }
        $node = $root;
        $queue = array();
        array_push($queue, $node);  // 根节点入队
        while (!empty($queue)) {    // 持续输出节点，直到队列为空
            $node = array_shift($queue);    // 队首元素出队
            echo $node->value . " ";
            // 左节点先入队
            if ($node->left != null) {
                array_push($queue, $node->left);
            }
            // 然后右节点入队
            if ($node->right != null) {
                array_push($queue, $node->right);
            }
        }
    }
    // 递归
    // 获取树的层数（最大深度）
    function getDepth($root) {
        if ($root == null) { // 节点为空
            return 0;
        }
        if ($root->left == null && $root->right == null) { // 只有根节点
            return 1;
        }
        $left_depth = $this->getDepth($root->left);
        $right_depth = $this->getDepth($root->right);
        return ($left_depth > $right_depth ? $left_depth : $right_depth) + 1;
//        return $left_depth > $right_depth ? ($left_depth + 1) : ($right_depth + 1);
    }
    public function level_order($root) {
        // 空树或层级不合理
        $depth = $this->getDepth($root);
        if ($root == null || $depth < 1) {
            return;
        }
        for ($i = 1; $i <= $depth; $i++) {
            $this->printTree($root, $i);
        }
    }
    public function printTree($root, $level) {
        // 空树或层级不合理
        if ($root == null || $level < 1) {
            return;
        }
        if ($level == 1) {
            echo $root->value;
        }
        $this->printTree($root->left, $level - 1);
        $this->printTree($root->right, $level - 1);
    }
}
// 测试
$a = new Node();
$b = new Node();
$c = new Node();
$d = new Node();
$e = new Node();
$f = new Node();
$a->value = "A";
$b->value = "B";
$c->value = "C";
$d->value = "D";
$e->value = "E";
$f->value = "F";
$a->left = $b;
$a->right = $c;
$b->left = $d;
$b->right = $e;
$c->left = $f;
$bst = new BT();
echo "----广度优先----";
echo "</br>";
echo "非递归：";
$bst->levelOrder($a);
echo "</br>";
echo "递归：";
$bst->level_order($a);
