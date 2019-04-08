# 自己动手写word2vec (三):构建Huffman树





------

**系列所有帖子** 
[自己动手写word2vec (一):主要概念和流程](http://blog.csdn.net/u014595019/article/details/51884529) 
[自己动手写word2vec (二):统计词频](http://blog.csdn.net/u014595019/article/details/51907294) 
[自己动手写word2vec (三):构建Huffman树 ](http://blog.csdn.net/u014595019/article/details/51925495)
[自己动手写word2vec (四):CBOW和skip-gram模型](http://blog.csdn.net/u014595019/article/details/51943428)

------

这一部分将解释Huffman树的构造方法，并说明了如何根据Huffman树来产生对应的二进制编码。这部分的代码放在[HuffmanTree.py](https://github.com/multiangle/pyword2vec/blob/master/HuffmanTree.py)中

------

## Huffman树的构造

Huffman树的构造方法与Huffman编码密切相关。 
具体的做法可以用下列伪码来描述

```
while (单词列表长度>1) {
    从单词列表中挑选出出现频率最小的两个单词 ;
    创建一个新的中间节点，其左右节点分别是之前的两个单词节点 ;
    从单词列表中删除那两个单词节点并插入新的中间节点 ;
}
```

当循环执行完毕，节点列表中的唯一一个节点即为树的根节点。

具体的执行稍微有点复杂，首先要构造一个Huffman树的节点类，在该类中存储一个节点的相关信息，包括这个节点的概率，左右子树，节点的value ( value存储的内容根据节点类型有所不同，对于叶节点，存储的是这个单词本身，而对于非叶节点，存储的是这个节点中存储的中间向量 ) 和对应的huffman码，如下所示

```python
class HuffmanTreeNode():
    def __init__(self,value,possibility):
        # common part of leaf node and tree node
        self.possibility = possibility
        self.left = None  # 左子树
        self.right = None # 右子树
        # value of leaf node  will be the word, and be
        # mid vector in tree node
        self.value = value # the value of word 
        self.Huffman = "" # store the huffman code
```

Huffman树类中主要的方法有：build_tree，merge，generate_huffman_code。 
build_tree 顾名思义，用来产生树形结构 
merge用来合并两个节点并产生它们的父节点 
generate_huffman_code用来根据已经产生的树结构来为词典中的每个词产生对应的huffman码

所以在构造Huffman树的部分，主要看build_tree,merge和构造方法这三个方法

------

首先来看**构造方法**

```python
class HuffmanTree():
    def __init__(self, word_dict, vec_len=15000):
        self.vec_len = vec_len      # the length of word vector
        self.root = None

        word_dict_list = list(word_dict.values())
        node_list = [HuffmanTreeNode(x['word'],x['possibility']) for x in word_dict_list]
        self.build_tree(node_list)
        # self.build_CBT(node_list)
        self.generate_huffman_code(self.root, word_dict)
```

可以看到，构建HuffmanTree需要由WordCount产生的词典，并且需要指定词向量的长度。

```python
node_list = [HuffmanTreeNode(x['word'],x['possibility']) for x in word_dict_list]
```

这一行代码，对词典中的每个词都产生对应的HuffmanTreeNode对象，以便进一步处理

------

接下来来看**merge**方法

```python
def merge(self,node1,node2):
    top_pos = node1.possibility + node2.possibility # 将概率相加
    top_node = HuffmanTreeNode(np.zeros([1,self.vec_len]), top_pos)
    if node1.possibility >= node2.possibility :
        top_node.left = node1
        top_node.right = node2
    else:
        top_node.left = node2
        top_node.right = node1
    return top_node
```

merge方法非常简单，新生成的节点的概率是两个输入节点的概率之和，其左右子节点即为输入的两个节点。值得注意的是，新生成的节点**肯定不是**叶节点，而非叶结点的value值是中间向量，**初始化为零向量**。

------

最后是**buid_tree**方法

```python
def build_tree(self,node_list):
    while node_list.__len__()>1:
        i1 = 0  # i1表示概率最小的节点
        i2 = 1  # i2 概率第二小的节点
        if node_list[i2].possibility < node_list[i1].possibility :
            [i1,i2] = [i2,i1]
        for i in range(2,node_list.__len__()): # 找到最小的两个节点
            if node_list[i].possibility<node_list[i2].possibility :
                i2 = i
                if node_list[i2].possibility < node_list[i1].possibility :
                    [i1,i2] = [i2,i1]
        top_node = self.merge(node_list[i1],node_list[i2]) # 合并两个节点，生成新的中间节点
        if i1<i2: # 删除两个旧节点
            node_list.pop(i2)
            node_list.pop(i1)
        elif i1>i2:
            node_list.pop(i1)
            node_list.pop(i2)
        else:
            raise RuntimeError('i1 should not be equal to i2')
        node_list.insert(0,top_node) # 插入新节点
    self.root = node_list[0]
```

这个之前已经用伪码描述过了，就不多说了

------

## Huffman码的生成

生成了Huffman树之后，就需要生成对应的Huffman码，以便进行下一步的训练过程。 
生成Huffman码的过程比较简单，假设一个节点的huffman码为“xx”，那么其左节点的huffman码为”xx1”，右节点的huffman码为”xx0”，从上到下直到叶节点，由此生成所有节点(包括叶节点和非叶结点)的Huffman码

```python
def generate_huffman_code(self, node, word_dict):
    stack = [self.root]
    while (stack.__len__()>0):
        node = stack.pop()
        # go along left tree
        while node.left or node.right :
            code = node.Huffman
            node.left.Huffman = code + "1"
            node.right.Huffman = code + "0"
            stack.append(node.right)
            node = node.left
        word = node.value
        code = node.Huffman
        # print(word,'\t',code.__len__(),'\t',node.possibility)
        word_dict[word]['Huffman'] = code
```

这一过程理所当然的需要遍历所有节点。遍历可以用递归来完成，但是这里使用栈来完成遍历过程。