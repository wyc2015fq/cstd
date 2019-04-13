
# 优先队列（priority queue）的实现 - Zhang's Wikipedia - CSDN博客


2019年02月24日 12:41:28[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：164


底层数据结构，最简单的情况为一维数组；
两大接口（以大顶堆为例）：push首先将要插入的值置于底层数据结构的末尾：heap[size-1] = value
不断地将其与其父节点比较，heap[parent] >= heap[pos]，break
否则交换 heap[parent] 与 heap[pos] 的值，并将 pos = parent

poppos = 0
heap[0] = heap[size-1]
将当前值不断地与其左右孩子节点比较：leftChild <= rightChild如果 heap[pos] >= heap[rightChild]，break
否则交换 heap[pos] 与 heap[rightChild]
pos = rightChild

否则，将 heap[pos] 与 heap[leftChild] 进行比较如果 heap[pos] > =  heap[leftChild]，break
否则交换 heap[pos]，heap[leftChild]
pos = leftChild



## 1. java 实现
如果当前索引为 pos父节点索引：parent = (pos+1)/2 - 1
左孩子：leftChild = pos*2 + 1
右孩子：rightChild = pos*2 +  2 或 rightChild = leftChild + 1
`package queue;
import java.util.Arrays;
public class PriorityQueue {
    private int size;
    private int[] heap;
    public PriorityQueue(int maxSize) {
        heap = new int[maxSize];
        size = 0;
    }
    public void push(int value) {
        if (size == heap.length) throw new IllegalStateException();
        int pos = size;
        heap[pos] = value;
        while (pos > 0) {
            int parent = (pos+1) / 2 - 1;
            if (heap[parent] >= heap[pos]) break;
            swapIndices(pos, parent);
            pos = parent;
        }
        ++size;
    }
    public int pop() {
        if (size == 0)  throw new IllegalStateException();
        int toRet = heap[0];
        heap[0] = heap[size-1];
        --size;
        int pos = 0;
        while (pos < size / 2) {
            int leftChild = pos * 2 + 1;
            int rightChild = pos * 2 + 2;       // leftChild + 1
		
			// 右孩子不一定存在
            if (rightChild < size && heap[leftChild] < heap[rightChild]) {
                if (heap[pos] >= heap[rightChild]) break;
                // heap[pos] < heap[rightChild]
                swapIndices(pos, rightChild);
                pos = rightChild;
            } else {						// 右孩子不存在，或右孩子存在，但右孩子小于左孩子
                if (heap[pos] >= heap[leftChild]) break;
                swapIndices(pos, leftChild);
                pos = leftChild;
            }
        }
        return toRet;
    }
    private void swapIndices(int i, int j) {
        int temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }
    public void printQueue() {
        System.out.println(Arrays.toString(heap));
    }
    public static void main(String[] args) {
        PriorityQueue priorityQueue = new PriorityQueue(10);
        priorityQueue.push(0);
        priorityQueue.push(1);
        priorityQueue.push(2);
        priorityQueue.push(3);
        priorityQueue.push(4);
        priorityQueue.printQueue();
        System.out.println(priorityQueue.pop());
        System.out.println(priorityQueue.pop());
        System.out.println(priorityQueue.pop());
    }
}`

