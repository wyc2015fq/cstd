# 堆 (Heap) - weixin_33985507的博客 - CSDN博客
2017年03月07日 22:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
“堆”这种数据结构常用在“优先级队列”的实现上, 比如Java中的PriorityQueue。今天讲讲什么是堆，如何去实现一个堆。
</br>
</br>
### 什么是堆
- 堆是一个二叉树。
- 堆的任意节点，总大于(小于)子节点。（大于即为大根堆，小于即为小根堆，根节点是最值）
- 而且堆总是完全树。（除了最底层，其它层的节点都被塞满）
下图即为一个大根堆：
![2857128-4dc5d3f9697fe035.jpg](https://upload-images.jianshu.io/upload_images/2857128-4dc5d3f9697fe035.jpg)
堆的数组表现形式.jpg
</br>
</br>
### 数组实现堆
在程序中，堆通常使用数组来实现(因为增删频繁)。并遵循下面的规则:
- 元素的父元素：(index-1)/2
- 元素的左孩子：(index*2)+1
- 元素的右孩子：(index*2)+2
</br>
</br>
### 堆的操作
对于堆，通常有下面几种常见操作:
- 构建堆 (给定一个数组，按照堆的逻辑去重构它)
- 删除根节点 (从堆中取出最值)
- 插入一个节点 (加入一个节点到堆中，当然要重构堆)
三种操作，但在实现的时候主要涉及到两个核心方法，`adjustDown`向下调整，和`adjustUp`向上调整。
</br>
</br>
### adjustDown
```
//向下调整
    public void adjustDown(int current) {
        int right = getRightChildIndex(current);// 右孩子
        int left = getLeftChildIndex(current);// 左孩子
        int max = findMax(current, right, left);// 三个节点，值最大的index
        if (max != current) {
            // 需要调整
            swap(current, max);
            // 递归调整与根节点进行交换的节点，保证下层也是堆
            adjustDown(max);
        }
    }
    // 三者最大的节点的索引
    private int findMax(int node, int right, int left) {
        int max = node;
        if (right < length && data[right] > data[max]) {
            max = right;
        }
        if (left < length && data[left] > data[max]) {
            max = left;
        }
        return max;
    }
    //交换数据
    public void swap(int n1, int n2) {
        int temp = data[n1];
        data[n1] = data[n2];
        data[n2] = temp;
    }
```
</br>
</br>
### 构建堆
构建堆时用到了`adjustDown`，从底部非叶子节点开始，每到某一节点，都会向下去调整，使得这个节点和其子节点形成堆。
```
protected int[] data;
    protected int length = 0;
    public MaxHeap(int[] data) {
        this.data = data;
        this.length = data.length;
        // 从最后一个节点的父节点开始构建堆
        int start = getParentIndex(length - 1);
        for (; start >= 0; start--) {
            adjustDown(start);
        }
    }
```
![2857128-e55f75a19dcfa61e.PNG](https://upload-images.jianshu.io/upload_images/2857128-e55f75a19dcfa61e.PNG)
传入一个数组
![2857128-67ac9deef7a95083.jpg](https://upload-images.jianshu.io/upload_images/2857128-67ac9deef7a95083.jpg)
未调整过的堆
![2857128-04cab470d1e057af.jpg](https://upload-images.jianshu.io/upload_images/2857128-04cab470d1e057af.jpg)
index为3的地方，是最后一个非叶子节点，从这个节点开始构建堆
![2857128-8c77961b9bac120f.jpg](https://upload-images.jianshu.io/upload_images/2857128-8c77961b9bac120f.jpg)
为index为2的地方构建堆
![2857128-d70e096ec5e30614.jpg](https://upload-images.jianshu.io/upload_images/2857128-d70e096ec5e30614.jpg)
为index为1的地方构建堆
![2857128-8a435d6644b9d85d.jpg](https://upload-images.jianshu.io/upload_images/2857128-8a435d6644b9d85d.jpg)
为index为1的地方，继续向下递归构建堆，即对index为3的地方构建堆
可以看到，在对index为1的地方构建堆时，因4和18的位置调换了，子堆可能已不符合要求，所以必须递归调用去重新构建子堆。这就是关键。
</br>
</br>
### 删除根节点
```
public MaxHeap remove() {
        // 将最后一个节点与头结点交换
        swap(0, length - 1);
        // 重新复制一个数组
        int[] newData = new int[length - 1];
        System.arraycopy(data, 0, newData, 0, length - 1);
        this.data = newData;
        this.length = length - 1;
        // 只需要从根节点，向下调整一次即可
        adjustDown(0);
        return this;
    }
```
</br>
</br>
### 增加一个节点
```
public MaxHeap insert(int value) {
        // 插入到数组最后
        int[] newData = new int[length + 1];
        System.arraycopy(data, 0, newData, 0, length);
        newData[length] = value;
        this.data = newData;
        this.length = length + 1;
        // 从最后一个节点开始自下而上调整堆(只要比父节点大，就交互)
        adjustUpHeap(this.length - 1);
        return this;
    }
    public void adjustUpHeap(int node) {
        int parent = getParentIndex(node);// 父节点
        if (parent >= 0 && data[parent] < data[node]) {
            swap(node, parent);
            adjustUpHeap(parent);// 递归调整与根节点进行交换的节点，保证上层也是堆
        }
    }
```
</br>
</br>
### 时间复杂度
![2857128-efb1a201bf08c7c5.PNG](https://upload-images.jianshu.io/upload_images/2857128-efb1a201bf08c7c5.PNG)
堆的各种操作
</br>
</br>
### 代码
[Code](https://link.jianshu.com?t=https://github.com/SylarChen/CodeForBlog/tree/master/data-structure-heap)
</br>
</br>
### 参考
[数据结构系列——堆](https://link.jianshu.com?t=http://vickyqi.com/2015/11/19/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E7%B3%BB%E5%88%97%E2%80%94%E2%80%94%E5%A0%86/)
