# 纸上谈兵: 哈希表 (hash table) - 文章 - 伯乐在线
原文出处： [Vamei](http://www.cnblogs.com/vamei/archive/2013/03/24/2970339.html)
#### HASH
哈希表(hash table)是从一个集合A到另一个集合B的映射(mapping)。映射是一种对应关系，而且集合A的某个元素只能对应集合B中的一个元素。但反过来，集合B中的一个元素可能对应多个集合A中的元素。如果B中的元素只能对应A中的一个元素，这样的映射被称为一一映射。这样的对应关系在现实生活中很常见，比如：
A  -> B
人 -> 身份证号
日期 -> 星座
上面两个映射中，人 -> 身份证号是一一映射的关系。在哈希表中，上述对应过程称为hashing。A中元素a对应B中元素b，a被称为键值(key)，b被称为a的hash值(hash value)。
![](http://jbcdn2.b0.upaiyun.com/2016/11/6b9cd25ac7aa6e7d914e2833ad288cbe.jpg)
韦小宝的hash值
映射在数学上相当于一个函数f(x):A->B。比如 f(x) = 3x + 2。哈希表的核心是一个哈希函数(hash function)，这个函数规定了集合A中的元素如何对应到集合B中的元素。比如：
![%e5%93%88%e5%b8%8c%e8%a1%a8](http://jbcdn2.b0.upaiyun.com/2016/11/78393c5762bb814aaa27282a2cab9f86.jpg)
上述对应中，哈希函数表示为hash(x) = x % 10。也就是说，给一个三位数，我们取它的最后一位作为该三位数的hash值。
哈希表在计算机科学中应用广泛。比如：
Ethernet中的FCS：参看[小喇叭开始广播 (以太网与WiFi协议)](http://www.cnblogs.com/vamei/archive/2012/11/27/2790065.html)
IP协议中的checksum：参看[我尽力 (IP协议详解)](http://www.cnblogs.com/vamei/archive/2012/12/02/2796988.html)
git中的hash值：参看[版本管理三国志](http://www.cnblogs.com/vamei/archive/2013/02/21/2918069.html)
上述应用中，我们用一个hash值来代表键值。比如在git中，文件内容为键值，并用SHA算法作为hash function，将文件内容对应为固定长度的字符串(hash值)。如果文件内容发生变化，那么所对应的字符串就会发生变化。git通过比较较短的hash值，就可以知道文件内容是否发生变动。
再比如计算机的登陆密码，一般是一串字符。然而，为了安全起见，计算机不会直接保存该字符串，而是保存该字符串的hash值(使用MD5、SHA或者其他算法作为hash函数)。当用户下次登陆的时候，输入密码字符串。如果该密码字符串的hash值与保存的hash值一致，那么就认为用户输入了正确的密码。这样，就算黑客闯入了数据库中的密码记录，他能看到的也只是密码的hash值。上面所使用的hash函数有很好的单向性：很难从hash值去推测键值。因此，黑客无法获知用户的密码。
(之前有报道多家网站用户密码泄露的时间，就是因为这些网站存储明文密码，而不是hash值，见[多家网站卷入CSDN泄密事件 明文密码成争议焦点](http://www.chinanews.com/it/2011/12-23/3553242.shtml))
注意，hash只要求从A到B的对应为一个映射，它并没有限定该对应关系为一一映射。因此会有这样的可能：两个不同的键值对应同一个hash值。这种情况叫做hash碰撞(hash collision)。比如网络协议中的checksum就可能出现这种状况，即所要校验的内容与原文并不同，但与原文生成的checksum(hash值)相同。再比如，MD5算法常用来计算密码的hash值。已经有实验表明，MD5算法有可能发生碰撞，也就是不同的明文密码生成相同的hash值，这将给系统带来很大的安全漏洞。(参考[hash collision](http://en.wikipedia.org/wiki/MD5#Collision_vulnerabilities)）
#### HASH与搜索
hash表被广泛的用于搜索。设定集合A为搜索对象，集合B为存储位置，利用hash函数将搜索对象与存储位置对应起来。这样，我们就可以通过一次hash，将对象所在位置找到。一种常见的情形是，将集合B设定在数组下标。由于数组可以根据数组下标进行随机存取(random access，算法复杂度为1)，所以搜索操作将取决于hash函数的复杂程度。
比如我们以人名(字符串)为键值，以数组下标为hash值。每个数组元素中存储有一个指针，指向记录 (有人名和电话号码)。
下面是一个简单的hash函数:

```
#define HASHSIZE 1007
/* By Vamei
 * hash function
 */
int hash(char *p)
{
    int value=0;
    while((*p) != '\0') {
       value = value + (int) (*p); // convert char to int, and sum
       p++;
    }
    return (value % HASHSIZE); // won's exceed HASHSIZE
}
```
hash value of “Vamei”: 498
hash value of “Obama”: 480
我们可以建立一个HASHSIZE大小的数组records，用于储存记录。HASHSIZE被选择为质数，以便hash值能更加均匀的分布。在搜索”Vamei”的记录时，可以经过hash，得到hash值498，再直接读取records[498]，就可以读取记录了。
(666666是Obama的电话号码，111111是Vamei的电话号码。纯属杜撰，请勿当真)
![](http://jbcdn2.b0.upaiyun.com/2016/11/9442976160ae72be260a24dc23231f58.png)
hash搜索
如果不采用hash，而只是在一个数组中搜索的话，我们需要依次访问每个记录，直到找到目标记录，算法复杂度为n。我们可以考虑一下为什么会有这样的差别。数组虽然可以随机读取，但数组下标是随机的，它与元素值没有任何关系，所以我们要逐次访问各个元素。通过hash函数，我们限定了每个下标位置可能存储的元素。这样，我们利用键值和hash函数，就可以具备相当的先验知识，来选择适当的下标进行搜索。在没有hash碰撞的前提下，我们只需要选择一次，就可以保证该下标指向的元素是我们想要的元素。
#### 冲突
hash函数需要解决hash冲突的问题。比如，上面的hash函数中，”Obama”和”Oaamb”有相同的hash值，发生冲突。我们如何解决呢？
一个方案是将发生冲突的记录用链表储存起来，让hash值指向该链表，这叫做open hashing:
![](http://jbcdn2.b0.upaiyun.com/2016/11/f415579ffd14f4d3cfe68796cca0af4b.png)
open hashing
我们在搜索的时候，先根据hash值找到链表，再根据key值遍历搜索链表，直到找到记录。我们可以用其他数据结构代替链表。
open hashing需要使用指针。我们有时候想要避免使用指针，以保持随机存储的优势，所以采用closed hashing的方式来解决冲突。
![](http://jbcdn2.b0.upaiyun.com/2016/11/989728c63fe07dfd40382536de085a9f.png)
closed hashing
这种情况下，我们将记录放入数组。当有冲突出现的时候，我们将冲突记录放在数组中依然闲置的位置，比如图中Obama被插入后，随后的Oaamb也被hash到480位置。但由于480被占据，Oaamb探测到下一个闲置位置(通过将hash值加1)，并记录。
closed hashing的关键在如何探测下一个位置。上面是将hash值加1。但也可以有其它的方式。概括的说，在第i次的时候，我们应该探测POSITION(i)=(h(x) + f(i)) % HASHSIZE的位置。上面将hash值加1的方式，就相当于设定f(i) = 1。当我们在搜索的时候，就可以利用POSITION(i)，依次探测记录可能出现的位置，直到找到记录。
(f(i)的选择会带来不同的结果，这里不再深入)
如果数组比较满，那么closed hashing需要进行许多次探测才能找到空位。这样将大大减小插入和搜索的效率。这种情况下，需要增大HASHSIZE，并将原来的记录放入到新的比较大的数组中。这样的操作称为rehashing。
#### 总结
hash表，搜索
hash冲突, open hashing, closed hashing
