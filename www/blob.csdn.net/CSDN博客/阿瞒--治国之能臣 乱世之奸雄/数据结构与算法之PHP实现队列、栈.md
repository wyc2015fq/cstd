# 数据结构与算法之PHP实现队列、栈 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年03月26日 11:51:16[baofenny](https://me.csdn.net/jiaminbao)阅读数：34
一、队列
1）队列（Queue）是一种先进先出（FIFO）的线性表，它只允许在表的前端进行删除操作，在表的后端进行插入操作，进行插入操作的端称为队尾，进行删除操作的端称为队头。即入队只能从队尾入，出队只能从队头出。
2）队列一般拥有队首（front指针）和队尾（rear指针），当一个队列并未存入数据的时候，front和rear指针均指向队首。
3）入队操作：rear后移，存入数据在rear指向的单元，队满不可入队，这同时也表明front总是指向队首元素的前驱。
4）出队操作：front后移，元素出队，队空不可出队。
![](https://images2017.cnblogs.com/blog/953680/201710/953680-20171027154615180-450838767.png)
5）在PHP函数中，array_push函数是向数组尾部添加元素，即入队操作；array_shift函数是删除数组头部元素，即出队操作。
$array =  array('a', 'b');
array_push($array, 'c'); //入队
array_shift($array);     //出队
队列的数组实现
<?php
/**
 * php用数组实现队列:先进先出FIFO
    1. getLength(): 获得队列的长度
    2. isEmpty(): 判断队列是否为空
    3. enqueue(): 入队，在队尾加入数据。
    4. dequeue(): 出队，返回并移除队首数据。队空不可出队。
    5. show(): 遍历队列，并输出
    6. clear(): 清空队列
 */
class Queue {
    // 队列数组
    public $dataStore = array();
    // 获得队列的长度
    public function getLength() {
        return count($this->dataStore);
    }
    // 判断队列是否为空
    public function isEmpty() {
        return $this->getLength() === 0;
    }
    // 入队，在队尾加入数据。
    public function enqueue($element) {
        $this->dataStore[] = $element;
        // array_push($this->dataStore, $element);
    }
    // 出队，返回并移除队首数据。队空不可出队。
    public function dequeue() {
        if (!$this->isEmpty()) {
            return array_shift($this->dataStore);
        }
        return false;
    }
    // 遍历队列，并输出
    public function show() {
        if (!$this->isEmpty()) {
            for ($i = 0; $i < $this->getLength(); $i++) {
                echo $this->dataStore[$i] . PHP_EOL;
            }
        } else {
            return "空";
        }
    }
    // 清空队列
    public function clearQueue() {
        unset($this->dataStore);
        // $this->dataStore = array();
    }
}
// 测试
$q = new Queue();
$q->enqueue('a');
$q->enqueue('b');
$q->enqueue('c');
echo '队列的长度为：' . $q->getLength();
echo "</br>";
echo '队列为：';
$q->show();
echo "</br>";
$q->dequeue();
echo "</br>";
echo "a出队，队列为：";
$q->show();
$q->clearQueue();
echo "清空队列后，队列为" . $q->show();
队列的链表实现
创建链式队列时，需定义两个结构，一个用于描述节点，一个用于描述队列。
<?php
/**
 * php用链表实现队列:先进先出FIFO
    1. isEmpty(): 判断队列是否为空
    2. enqueue(): 入队，在队尾加入数据。
    3. dequeue(): 出队，返回并移除队首数据。队空不可出队。
    4. clear(): 清空队列
    5. show(): 显示队列中的元素
 */
// 节点类
class Node {
    public $data;   // 节点数据
    public $next;   // 下一节点
    public function __construct($data) {
        $this->data = $data;
        $this->next = NULL;
    }
}
// 队列类
class Queue {
    private $header;        // 头节点
    function __construct($data) {
        $this->header = new Node($data);
    }
    // 判断队列是否为空
    public function isEmpty() {
        if ($this->header->next !== null) { // 不为空
            return false;
        }
        return true;
    }
    // 入队，在队尾加入数据。
    public function enqueue($element) {
        $newNode = new Node($element);
        $current = $this->header;
        if ($current->next == null) { // 只有头节点
            $this->header->next = $newNode;
        } else { // 遍历到队尾最后一个元素
            while ($current->next != null) {
                $current = $current->next;
            }
            $current->next = $newNode;
        }
        $newNode->next = null;
    }
    // 出队，返回并移除队首数据。队空不可出队。
    public function dequeue() {
        if ($this->isEmpty()) { // 队列为空
            return false;
        }
        // header头节点没有实际意义，队首节点是header指向的结点。
        $current = $this->header;
        $current->next = $current->next->next;
    }
    // 清空队列
    public function clear() {
        $this->header = null;
    }
    // 显示队列中的元素
    public function show() {
        $current = $this->header;
        if ($this->isEmpty()) {
            echo "空！";
        }
        while ($current->next != null) {
            echo $current->next->data . PHP_EOL;
            $current = $current->next;
        }
    }
}
// 测试
$q = new Queue('header');
$q->enqueue('a');
$q->enqueue('b');
$q->enqueue('c');
echo "队列为：";
$q->show();
echo "</br>";
echo "a出队，队列为：";
$q->dequeue();
$q->show();
echo "</br>";
$q->clear();
echo "清空队列后，队列为";
$q->show();
二、栈
1）栈（Stack）是一种后进先出（LIFO）表，插入删除操作都只能在一个位置上进表，这个位置位于表的末端，叫做栈顶（Top），另一端则称为栈底（Bottom），又称为表头。
2）对栈的基本操作有push和pop，表示进栈和出栈，相当于插入和删除操作。存储数据时，先进入的数据被压入栈底，后进入的数据则在栈顶；读取数据时，从栈顶开始弹出数据。
![](https://images2017.cnblogs.com/blog/953680/201710/953680-20171027154705742-1533401204.png)
3）在PHP函数中，array_push函数是向数组尾部添加元素，即入栈操作；array_pop函数是删除数组尾部元素，即出栈操作。
$array =  array('a', 'b');
array_push($array, 'c'); //入栈
array_pop($array);       //出栈
栈的数组实现
选择用数组表示栈内容必须预先估计栈的最大容量。因为数组一旦创建，其大小是无法改变的，而数组设置过大可能会浪费大量内存，设置过小又可能会溢出。
<?php
/**
 * php用数组实现栈:后入先出LIFO
    1. getLength(): 获得栈的长度
    2. push(): 入栈，在最顶层加入数据。
    3. pop(): 出栈，返回并移除最顶层的数据。
    4. getTop(): 返回最顶层数据的值，但不移除它
    5. clearStack(): 清空栈
    6. show(): 遍历栈元素
 */
class Stack {
    // 使用数组实现栈结构
    public $stack = array();
    // 获得栈的长度
    public function getLength() {
        return count($this->stack);
    }
    // 入栈，在最顶层加入数据。
    public function push($element) {
        $this->stack[] = $element;
    }
    // 出栈，返回并移除最顶层的数据。
    public function pop() {
        if ($this->getLength() > 0) {
            return array_pop($this->stack);
        }
    }
    // 返回最顶层数据的值，但不移除它
    public function getTop() {
        $top = $this->getLength() - 1;
        return $this->stack[$top];
    }
    // 清空栈
    public function clearStack() {
        unset($this->stack);
        // $this->stack = array();
    }
    // 遍历栈元素
    public function show() {
        if ($this->getLength() > 0) {
            for ($i = 0; $i < $this->getLength(); $i++) {
                echo $this->stack[$i] . PHP_EOL;
            }
        }
        echo "空！";
    }
}
// 测试
$s = new Stack();
$s->push('a');
$s->push('b');
$s->push('c');
echo "栈为：";
$s->show();
echo "</br>";
echo '栈顶元素为' . $s->getTop();
echo "</br>";
echo '栈的长度为：' . $s->getLength();
echo "</br>";
$s->pop();
echo "出栈，弹出c，栈为：";
$s->show();
echo "</br>";
echo "清空栈，栈为：";
$s->clearStack();
$s->show();
栈的链表实现
<?php
/**
 * php用数组实现栈:后入先出LIFO
    1. isEmpty(): 判断队列是否为空。
    2. push(): 入栈，插入新的栈顶节点
    3. pop(): 出栈，删除栈顶元素
    4. clear(): 清空栈
    5. show(): 遍历栈元素
 */
// 节点类
class Node {
    public $data;   // 节点数据
    public $next;   // 下一节点
    public function __construct($data) {
        $this->data = $data;
        $this->next = NULL;
    }
}
class Stack {
    private $header;        // 头节点
    function __construct($data) {
        $this->header = new Node($data);
    }
    // 判断栈是否为空
    public function isEmpty() {
        if ($this->header->next !== null) { // 不为空
            return false;
        }
        return true;
    }
    // 入栈，插入新的栈顶节点
    public function push($element) {
        $newNode = new Node($element);
        $current = $this->header;
        if ($current->next == null) { // 只有头节点
            $this->header->next = $newNode;
        } else { // 遍历到栈尾最后一个元素
            while ($current->next != null) {
                $current = $current->next;
            }
            $current->next = $newNode;
        }
        $newNode->next = null;
    }
    // 出栈，删除栈顶元素
    public function pop() {
        if ($this->isEmpty()) { // 栈为空
            return false;
        }
        $current = $this->header;
        while ($current->next->next != null) {
            $current = $current->next;
        }
        $current->next = null;
    }
    // 清空栈
    public function clear() {
        $this->header = null;
    }
    // 显示栈中的元素
    public function show() {
        $current = $this->header;
        if ($this->isEmpty()) {
            echo "空！";
        }
        while ($current->next != null) {
            echo $current->next->data . PHP_EOL;
            $current = $current->next;
        }
    }
}
// 测试
$s = new Stack('header');
$s->push('a');
$s->push('b');
$s->push('c');
echo "栈为：";
$s->show();
echo "</br>";
$s->pop();
echo "出栈，弹出c，栈为：";
$s->show();
echo "</br>";
echo "清空栈，栈为：";
$s->clear();
$s->show();
