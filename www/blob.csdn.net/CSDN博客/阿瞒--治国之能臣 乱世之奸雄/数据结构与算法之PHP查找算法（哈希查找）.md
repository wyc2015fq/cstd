# 数据结构与算法之PHP查找算法（哈希查找） - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年04月02日 14:07:23[baofenny](https://me.csdn.net/jiaminbao)阅读数：31
个人分类：[数据结构与算法](https://blog.csdn.net/jiaminbao/article/category/8795800)
一、哈希查找的定义
提起哈希，我第一印象就是PHP里的关联数组，它是由一组key/value的键值对组成的集合，应用了散列技术。
哈希表的定义如下：
哈希表（Hash table，也叫散列表），是根据关键码值(Key/value)而直接进行访问的数据结构。也就是说，它通过把关键码值映射到表中一个位置来访问记录，以加快查找的速度。这个映射函数叫做散列函数，存放记录的数组叫做散列表。
给定表M，存在函数f(key)，对任意给定的关键字值key，代入函数后若能得到包含该关键字的记录在表中的地址，则称表M为哈希(Hash）表，函数f(key)为哈希(Hash) 函数。
而哈希查找，是在记录的存储位置和记录的关键字之间建立一个确定的对应关系f，使得每个关键字key对应一个存储位置f(key)。查找时，根据这个确定的对应关系找到给定值的映射f(key)，若查找集合中存在这个记录，则必定在f(key)的位置上。
哈希查找并不查找数据本身，而是先将数据映射为一个整数(它的哈希值)，并将哈希值相同的数据存放在同一个位置，即以哈希值为索引构造一个数组。
二、设计哈希表
哈希查找的操作步骤如下：
1、取数据元素的关键字key，计算其哈希函数值。若该地址对应的存储空间还没有被占用，则将该元素存入；否则执行第2步解决冲突。
2、根据选择的冲突处理方法，计算关键字key的下一个存储地址。若下一个存储地址仍被占用，则继续执行，直到找到能用的存储地址为止。
最常用的哈希函数构造方法是除留余数法。取关键字被某个不大于哈希表表长m的数p除后所得余数为哈希地址，即Hash(key)=key mod p (p≤m)，其中，除数p称作模，mod称作取模（求余数）。
比如：有12个关键字：12, 29, 56, 25, 15, 78, 16, 67, 21, 38, 22, 47，如果采用除留余数法，哈希函数可以设计为f(key) = key mod 12，比如12 mod 12 = 0，它存储在下标为0的位置，29 mod 12 = 5，它存储在下标为5的位置。
|下标|0|1|2|3|4|5|6|7|8|9|10|11|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|关键字|12|25|38|15|16|29|78|67|56|21|22|47|
但如果将16改为30，它和78的余数都为6，就会和78所对应的下标位置冲突了，生成的hash表只有11个元素，下标为6的位置对应的值是30，先前的78就被覆盖掉了。这就是哈希冲突。
因此合理选取p值是很重要的，实践证明：若散列表表长为m，通常p为小于或等于表长（最好接近m）的最大质数，此时产生的哈希函数较好。
构造哈希表的代码如下：
<?php
class HashTable{
    public $arr = array();
    public $size = 10;
    public function __construct(){
        // SplFixedArray创建的数组比一般的Array()效率更高，因为更接近C的数组。
        // 创建时需要指定尺寸
        $this->arr = new SplFixedArray($this->size);
    }
    // 简单hash算法。输入key，输出hash后的整数
    private function key2Hash($key){
        $len = strlen($key);
        // key中每个字符所对应的ASCII的值
        $asciiTotal = 0;
        for($i = 0; $i < $len; $i++){
            $asciiTotal += ord($key[$i]);
        }
        return $asciiTotal % $this->size;
    }
    // 赋值
    public function set($key, $value){
        $hash = $this->key2Hash($key);
        $this->arr[$hash] = $value;
        return true;
    }
    // 取值
    public function get($key){
        $hash = $this->key2Hash($key);
        return $this->arr[$hash];
    }
　　// 哈希查找
　　public function hashSearch($key) {
        $hash = $this->key2Hash($key);
        return $this->arr[$hash];
    }
    // 改变哈希表长度
    public function editSize($size){
        $this->size = $size;
        $this->arr->setSize($size);
    }
}
// 测试1
$ht = new HashTable();
for($i=0; $i < 15; $i++){
    $ht->set('key' . $i, 'value' . $i);
}
print_r($ht->arr);
/*
SplFixedArray Object
(
    [0] => value14
    [1] => value4
    [2] => value5
    [3] => value6
    [4] => value7
    [5] => value8
    [6] => value10
    [7] => value11
    [8] => value12
    [9] => value13
)
*/
// 测试2
$ht->editSize(15);
for($i = 0; $i < 15; $i++){
    $ht->set('key' . $i, 'value' . $i);
}
print_r($ht->arr);
/*
SplFixedArray Object
(
    [0] => value14
    [1] => value4
    [2] => value0
    [3] => value1
    [4] => value2
    [5] => value3
    [6] => value10
    [7] => value11
    [8] => value12
    [9] => value13
    [10] => value14
    [11] => value9
    [12] =>
    [13] =>
    [14] =>
)
*/
可以看到，哈希表大小不论是10还是15，都会出现赋值时后操作覆盖前操作的问题。因此，在建造哈希表时不仅要设定一个好的哈希函数，还要设定一种处理冲突的方法。
三、哈希冲突（碰撞）
哈希表不可避免冲突（collision）现象：对不同的关键字可能得到同一哈希地址，即key1≠key2，而hash(key1)=hash(key2）。具有相同函数值的关键字对该哈希函数来说称为**同义词（synonym）**。解决哈希冲突常用的两种方法是：开放定址法和链地址法。
1、开放定址法
当冲突发生时，就去寻找下一个空的散列地址，只要散列表足够大，空的散列地址总能找到，并将记录存入。公式为：fi(key) = (f(key)+di) MOD m (di=1,2,3,......,m-1)。
比如，12个关键字：12, 67, 56, 16, 25, 37, 22, 29, 15, 47, 48, 34，用散列函数f(key) = key mod 12。当计算前5个数12, 67, 56, 16, 25时，都是没有冲突的散列地址，直接存入：
|下标|0|1|2|3|4|5|6|7|8|9|10|11|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|关键字|12|25|||16|||67|56||||
计算key = 37时，发现f(37) = 1，与25所在的位置冲突。
于是我们应用上面的公式f(37) = (f(37)+1) mod 12 = 2。于是将37存入下标为2的位置。
|下标|0|1|2|3|4|5|6|7|8|9|10|11|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|关键字|12|25|37||16|||67|56||||
接下来22,29,15,47都没有冲突，正常的存入：
|下标|0|1|2|3|4|5|6|7|8|9|10|11|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|关键字|12|25|37|15|16|29||67|56||22|47|
到了 key=48，计算得到f(48) = 0，与12所在的0位置冲突了，应用公式f(48) = (f(48)+1) mod 12 = 1，又与25所在的位置冲突。继续应用公式f(48) = (f(48)+2) mod 12=2，还是冲突……一直到 f(48) = (f(48)+6) mod 12 = 6时，才有空位，存入：
|下标|0|1|2|3|4|5|6|7|8|9|10|11|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|关键字|12|25|37|15|16|29|48|67|56||22|47|
同理，最后一个key = 34，存入到下标为9的位置上。
开放定址法解决冲突代码如下：
<?php
/**
* 开放定址法解决冲突
*/
class HashTable{
    public $arr = array();
    public $size = 12;
    public function __construct(){
        // 创建时需要指定尺寸
        $this->arr = new SplFixedArray($this->size);
    }
    // Hash函数:采用取余法,用关键字的值取余表的长度,作为哈希存储的地址
    public function key2Hash($key){
        return $key % $this->size;
    }
    // 赋值
    public function set($key, $value){
        $cur = 0;
        $hash = $this->key2Hash($key);
        if (isset($this->arr[$hash])) {
            while (isset($this->arr[$hash]) && $cur < $this->size) {
                $hash = $this->key2Hash($hash + 1); // 开放定址法处理冲突
                $cur++;
            }
        }
        $this->arr[$hash] = $value;
    }
    // 查找
    public function hashSearch($key){
        $hash = $this->key2Hash($key);
        $k = 0;
        while (isset($this->arr[$hash]) && $this->arr[$hash] != $key && $k < $this->size) {
            $hash = $this->key2Hash($hash + 1);
            $k++;
        }
        if ($this->arr[$hash] == $key) { // 找到
            return $hash;
        } else {    // 没找到
            return -1;
        }
    }
}
//测试
$ht = new HashTable();
$keys = [12, 67, 56, 16, 25, 37, 22, 29, 15, 47, 48, 34];
for ($i = 0; $i < count($keys); $i++) {
    $key = $keys[$i];
    $ht->set($key, $key);
}
print_r($ht->arr);
$pos = $ht->hashSearch(37);
echo $pos;　　// 2
2、链地址法（拉链法）
将所有的相同Hash值的key放在一个链表中，比如key3和key14在hash之后都是0，那么在数组的键为0的地方以链表的形式存储这两个值。这样，无论有多少个冲突，都只是在当前位置给单链表增加节点。
![](https://images2017.cnblogs.com/blog/953680/201712/953680-20171227143030191-1641667299.png)
<?php
/**
* 链地址法解决冲突
*/
// 创建HashNode类，用来存储key和value的值，并且存储相同hash的另一个元素。
class HashNode{
    public $key;
    public $value;
    public $nextNode;
    public function __construct($key, $value, $nextNode = Null){
        $this->key = $key;              // 节点的关键字
        $this->value = $value;          // 节点的值
        $this->nextNode = $nextNode;    // 指向具有相同Hash值节点的指针
    }
}
class HashTable{
    public $arr;
    public $size = 10;
    public function __construct(){
        $this->arr = new SplFixedArray($this->size);
    }
    // 简单hash算法。输入key，输出hash后的整数
    public function key2Hash($key){
        $asciiTotal = 0;
        $len = strlen($key);
        for($i=0; $i<$len; $i++){
            $asciiTotal += ord($key[$i]);
        }
        return $asciiTotal % $this->size;
    }
    // 赋值
    public function set($key, $value){
        $hash = $this->key2Hash($key);
        if (isset($this->arr[$hash])){
            $newNode = new HashNode($key, $value, $this->arr[$hash]);
        } else {
            $newNode = new HashNode($key, $value, null);
        }
        $this->arr[$hash] = $newNode;
        return true;
    }
    // 取值
    public function get($key){
        $hash = $this->key2Hash($key);
        $current = $this->arr[$hash];
        while (!empty($current)){
            if($current->key == $key){
                return $current->value;
            }
            $current = $current->nextNode;
        }
        return NULL;
    }
    // 哈希查找
    public function hashSearch($key){
        $hash = $this->key2Hash($key);
        $current = $this->arr[$hash];
        while (isset($current)) { //遍历当前链表
            if ($current->key == $key){  //比较当前节点的关键字
                return $current->value;  //查找成功
            }
            $current = $current->nextNode;  //比较下一个节点
        }
        return null;  //查找失败
    }
}
//测试1
$newArr = new HashTable();
for($i = 0; $i < 30; $i++){
    $newArr->set('key'.$i, 'value'.$i);
}
print_r($newArr->arr);
print_r($newArr->get('key3'));
修改后的插入的算法流程如下：
1)    使用Hash函数计算关键字的Hash值，通过Hash值定位到Hash表的指定位置。
2)    如果此位置已经被其他节点占用，把新节点的nextNode指向此节点，否则把新节点nextNode指向此节点，否则把新节点nextNode设置为null。
3)    把新节点保存到Hash表的当前位置。
经过这三个步骤，相同的Hash值得节点会被连接到同一个链表。
修改后的查找算法流程如下：
1)    使用Hash函数计算关键字的Hash值，通过Hash值定位到Hash表的指定位置。
2)    遍历当前链表，比较链表中的每个节点的关键字与查找关键字是否相等。如果相等，查找成功。
3)    如果整个链表都没有要查找的关键字，查找失败。
