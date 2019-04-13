
# Word2vec基础介绍（三）：构建Huffman树 - lyc_yongcai的博客 - CSDN博客


2017年06月15日 09:38:39[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：639


这一部分将解释Huffman树的构造方法，并说明了如何根据Huffman树来产生对应的二进制编码。
## Huffman树的构造
Huffman树的构造方法与Huffman编码密切相关。
具体的做法可以用下列伪码来描述
```python
while
```
```python
(单词列表长度>1)
```
```python
{
```
```python
从单词列表中挑选出出现频率最小的两个单词 ;
```
```python
创建一个新的中间节点，其左右节点分别是之前的两个单词节点 ;
```
```python
从单词列表中删除那两个单词节点并插入新的中间节点 ;
```
```python
}
```
1
2
3
4
5
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
当循环执行完毕，节点列表中的唯一一个节点即为树的根节点。
具体的执行稍微有点复杂，首先要构造一个Huffman树的节点类，在该类中存储一个节点的相关信息，包括这个节点的概率，左右子树，节点的value ( value存储的内容根据节点类型有所不同，对于叶节点，存储的是这个单词本身，而对于非叶节点，存储的是这个节点中存储的中间向量 ) 和对应的huffman码，如下所示
```python
class
```
```python
HuffmanTreeNode
```
```python
()
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self,value,possibility)
```
```python
:
```
```python
# common part of leaf node and tree node
```
```python
self.possibility = possibility
        self.left =
```
```python
None
```
```python
# 左子树
```
```python
self.right =
```
```python
None
```
```python
# 右子树
```
```python
# value of leaf node  will be the word, and be
```
```python
# mid vector in tree node
```
```python
self.value = value
```
```python
# the value of word
```
```python
self.Huffman =
```
```python
""
```
```python
# store the huffman code
```
1
2
3
4
5
6
7
8
9
10
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
Huffman树类中主要的方法有：build_tree，merge，generate_huffman_code。
build_tree 顾名思义，用来产生树形结构
merge用来合并两个节点并产生它们的父节点
generate_huffman_code用来根据已经产生的树结构来为词典中的每个词产生对应的huffman码
所以在构造Huffman树的部分，主要看build_tree,merge和构造方法这三个方法
首先来看**构造方法**
```python
class
```
```python
HuffmanTree
```
```python
()
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self, word_dict, vec_len=
```
```python
15000
```
```python
)
```
```python
:
```
```python
self.vec_len = vec_len
```
```python
# the length of word vector
```
```python
self.root =
```
```python
None
```
```python
word_dict_list = list(word_dict.values())
        node_list = [HuffmanTreeNode(x[
```
```python
'word'
```
```python
],x[
```
```python
'possibility'
```
```python
])
```
```python
for
```
```python
x
```
```python
in
```
```python
word_dict_list]
        self.build_tree(node_list)
```
```python
# self.build_CBT(node_list)
```
```python
self.generate_huffman_code(self.root, word_dict)
```
1
2
3
4
5
6
7
8
9
10
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
可以看到，构建HuffmanTree需要由WordCount产生的词典，并且需要指定词向量的长度。
```python
node_list = [HuffmanTreeNode(x[
```
```python
'word'
```
```python
],x[
```
```python
'possibility'
```
```python
])
```
```python
for
```
```python
x
```
```python
in
```
```python
word_dict_list]
```
1
![](http://static.blog.csdn.net/images/save_snippets.png)
1
这一行代码，对词典中的每个词都产生对应的HuffmanTreeNode对象，以便进一步处理
接下来来看**merge**方法
```python
def
```
```python
merge
```
```python
(self,node1,node2)
```
```python
:
```
```python
top_pos = node1.possibility + node2.possibility
```
```python
# 将概率相加
```
```python
top_node = HuffmanTreeNode(np.zeros([
```
```python
1
```
```python
,self.vec_len]), top_pos)
```
```python
if
```
```python
node1.possibility >= node2.possibility :
            top_node.left = node1
            top_node.right = node2
```
```python
else
```
```python
:
            top_node.left = node2
            top_node.right = node1
```
```python
return
```
```python
top_node
```
1
2
3
4
5
6
7
8
9
10
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
merge方法非常简单，新生成的节点的概率是两个输入节点的概率之和，其左右子节点即为输入的两个节点。值得注意的是，新生成的节点**肯定不是**叶节点，而非叶结点的value值是中间向量，**初始化为零向量**。
最后是**buid_tree**方法
```python
def
```
```python
build_tree
```
```python
(self,node_list)
```
```python
:
```
```python
while
```
```python
node_list.__len__()>
```
```python
1
```
```python
:
            i1 =
```
```python
0
```
```python
# i1表示概率最小的节点
```
```python
i2 =
```
```python
1
```
```python
# i2 概率第二小的节点
```
```python
if
```
```python
node_list[i2].possibility < node_list[i1].possibility :
                [i1,i2] = [i2,i1]
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
2
```
```python
,node_list.__len__()):
```
```python
# 找到最小的两个节点
```
```python
if
```
```python
node_list[i].possibility<node_list[i2].possibility :
                    i2 = i
```
```python
if
```
```python
node_list[i2].possibility < node_list[i1].possibility :
                        [i1,i2] = [i2,i1]
            top_node = self.merge(node_list[i1],node_list[i2])
```
```python
# 合并两个节点，生成新的中间节点
```
```python
if
```
```python
i1<i2:
```
```python
# 删除两个旧节点
```
```python
node_list.pop(i2)
                node_list.pop(i1)
```
```python
elif
```
```python
i1>i2:
                node_list.pop(i1)
                node_list.pop(i2)
```
```python
else
```
```python
:
```
```python
raise
```
```python
RuntimeError(
```
```python
'i1 should not be equal to i2'
```
```python
)
            node_list.insert(
```
```python
0
```
```python
,top_node)
```
```python
# 插入新节点
```
```python
self.root = node_list[
```
```python
0
```
```python
]
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
这个之前已经用伪码描述过了，就不多说了
## Huffman码的生成
生成了Huffman树之后，就需要生成对应的Huffman码，以便进行下一步的训练过程。
生成Huffman码的过程比较简单，假设一个节点的huffman码为“xx”，那么其左节点的huffman码为”xx1”，右节点的huffman码为”xx0”，从上到下直到叶节点，由此生成所有节点(包括叶节点和非叶结点)的Huffman码
```python
def
```
```python
generate_huffman_code
```
```python
(self, node, word_dict)
```
```python
:
```
```python
stack = [self.root]
```
```python
while
```
```python
(stack.__len__()>
```
```python
0
```
```python
):
            node = stack.pop()
```
```python
# go along left tree
```
```python
while
```
```python
node.left
```
```python
or
```
```python
node.right :
                code = node.Huffman
                node.left.Huffman = code +
```
```python
"1"
```
```python
node.right.Huffman = code +
```
```python
"0"
```
```python
stack.append(node.right)
                node = node.left
            word = node.value
            code = node.Huffman
```
```python
# print(word,'\t',code.__len__(),'\t',node.possibility)
```
```python
word_dict[word][
```
```python
'Huffman'
```
```python
] = code
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
这一过程理所当然的需要遍历所有节点。遍历可以用递归来完成，但是这里使用栈来完成遍历过程。


