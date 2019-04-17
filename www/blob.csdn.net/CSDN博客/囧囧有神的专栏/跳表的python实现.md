# 跳表的python实现 - 囧囧有神的专栏 - CSDN博客





2014年01月10日 11:56:40[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：2692








跳表的性能和红黑树 AVL差不多，但是结构相当简单。

只需要简单操作链表，就能很容易实现跳表。




参考如下文章，自己用python实现了一个跳表的例程


http://blog.sina.com.cn/s/blog_72995dcc01017w1t.html






```python
##
#  Example of Skip List source code for c
##
import random
import sys

minint=0
maxint=65535
class Node(object):
    def __init__(self, key=minint, value=None, level=1):
        self.key = key
        self.value = value
        self.level = level
        self.right = None
        self.down = None

class SkipList(object):
    def __init__(self):
        self.top = Node(minint)
        self.top.right = Node(maxint)
        self.nodenum = 0
        pass

    def getNode(self, key):
        node = self.top
        while 1:
            while key >= node.right.key :
                node = node.right
            if( key == node.key):
                return node
            if node.down == None:
                return None
            node = node.down
    def findNode(self, key):
        return self.searchNode(self.top,key)

    def searchNode(self, node, key):
        while key >= node.right.key:
            node = node.right
        if key == node.key:
            return node
        if node.down == None:
            return None
        return self.searchNode(node.down, key)


    def insertNode(self, top, key, value):
        while key >= top.right.key:
            top = top.right
        if key == top.key:
            return None
        if top.down == None:
            node = Node(key, value)
            node.right = top.right
            top.right = node
            node.level = top.level
            self.nodenum = self.nodenum + 1
            return node
        downnode = self.insertNode(top.down, key, value)
        if downnode:
            node = Node(key, value)
            node.right = top.right
            top.right = node
            node.down = downnode
            node.level = top.level
            return node
        return None

    def insert(self, key, value):
        k = self.getK()
        for l in xrange(self.top.level+1,k+1):
            topleft = Node(minint, level=l)
            topright = Node(maxint, level=l)
            topleft.right = topright
            topleft.down = self.top
            self.top = topleft
            print "l="+str(self.top.level)
        top = self.top
        while top.level != k:
            top = top.down
        
        self.insertNode(top, key, value)
        print "num="+str(self.nodenum)+" "+str(k)+" "+str(key)+" "+str(value)+" insert OK"
        
    def getK(self):
        k = 1
        while random.randint(0,1):
            k = k + 1
        return k

def printNode(node):
    print "level="+str(node.level)+" key="+str(node.key)+" value="+str(node.value)
    pass
if __name__ == '__main__':
    skiplist = SkipList()
    for x in xrange(1,10):
        skiplist.insert(x, random.randint(3,1000))
    printNode(skiplist.getNode(4))
    printNode(skiplist.findNode(4))
```








