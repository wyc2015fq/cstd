# [CareerCup] 16.4 A Lock Without Deadlocks 无死锁的锁 - Grandyang - 博客园







# [[CareerCup] 16.4 A Lock Without Deadlocks 无死锁的锁](https://www.cnblogs.com/grandyang/p/5399848.html)







16.4 Design a class which provides a lock only if there are no possible deadlocks.



有很多方法可以避免死锁的发生，一个常用的方法是列出所需要的锁，然后判断锁上这些锁后会不会发生死锁，比如有如下的锁的顺序：

A = {1, 2, 3, 4}

B = {1, 3, 5}

C = {7, 5, 9, 2}

这是有可能产生死锁的，比如当A锁上2等待3，当B锁上3等待5，当C锁上5等待2，我们可以将其看做图，2连上3,3连上5,5连上2，那么就会有环。一条边(w,v)表示锁上v后马上锁w，那么上述里子的图中的边为(1, 2), (2, 3), (3, 4), (1, 3), (3, 5), (7, 5), (5, 9), (9, 2)。那么我们在检测是否有死锁的情况就是要找图中是否存在环，我们可以用DFS来搜索所有的相连的部分，我们用DFS需要标记点的状态，我们首先定义个LockFactory类，用来保存锁的序列，比如上面的A,B,C，然后对于每一个锁序列，我们先将所有的锁都链起来，然后标记为false，然后用DFS判断是否有环，如果有环，则断开当前锁序列之间的所有连接，参见代码如下；



```
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.concurrent.locks.Lock;

public class LockFactory {
    private static LockFactory instance;
    private int numberOfLocks = 5;
    private LockNode[] locks;
    private Hashtable<Integer, LinkedList<LockNode>> lockOrder;
    
    private LockFactory(int count) {
        numberOfLocks = count;
        locks = new LockNode[numberOfLocks];
        lockOrder = new Hashtable<Integer, LinkedList<LockNode>>();
        for (int i = 0; i < numberOfLocks; ++i) {
            locks[i] = new LockNode(i, count);
        }
    }
    
    public static LockFactory getInstance() {
        return instance;
    }
    
    public static LockFactory initialize(int count) {
        if (instance == null) {
            instance = new LockFactory(count);
        }
        return instance;
    }
    
    public boolean hasCycle(Hashtable<Integer, Boolean> touchedNodes, int[] resourcesInOrder) {
        for (int resource : resourcesInOrder) {
            if (touchedNodes.get(resource) == false) {
                LockNode n = locks[resource];
                if (n.hasCycle(touchedNodes)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    public boolean declare(int ownerId, int[] resourcesInOrder) {
        Hashtable<Integer, Boolean> touchedNodes = new Hashtable<Integer, Boolean>();
        int index = 1;
        touchedNodes.put(resourcesInOrder[0], false);
        for (index = 1; index < resourcesInOrder.length; ++index) {
            LockNode pre = locks[resourcesInOrder[index - 1]];
            LockNode cur = locks[resourcesInOrder[index]];
            pre.joinTo(cur);
            touchedNodes.put(resourcesInOrder[index], false);
        }
        if (hasCycle(touchedNodes, resourcesInOrder)) {
            for (int j = 1; j < resourcesInOrder.length; ++j) {
                LockNode p = locks[resourcesInOrder[j - 1]];
                LockNode c = locks[resourcesInOrder[j]];
                p.remove(c);
            }
            return false;
        }
        LinkedList<LockNode> list = new LinkedList<LockNode>();
        for (int i = 0; i < resourcesInOrder.length; ++i) {
            LockNode resource = locks[resourcesInOrder[i]];
            list.add(resource);
        }
        lockOrder.put(ownerId, list);
        return true;
    }
    
    public Lock getLock(int ownerId, int resourceId) {
        LinkedList<LockNode> list = lockOrder.get(ownerId);
        if (list == null) return null;
        LockNode head = list.getFirst();
        if (head.getId() == resourceId) {
            list.removeFirst();
            return head.getLock();
        }
        return null;
    }
}

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class LockNode {
    public enum VisitState {
        FRESH, VISITING, VISITED
    };
    private ArrayList<LockNode> children;
    private int lockId;
    private Lock lock;
    private int maxLocks;
    
    public LockNode(int id, int max) {
        lockId = id;
        children = new ArrayList<LockNode>();
        maxLocks = max;
    }
    
    public void joinTo(LockNode node) {
        children.add(node);
    }
    
    public void remove(LockNode node) {
        children.remove(node);
    }
    
    public boolean hasCycle(Hashtable<Integer, Boolean> touchedNodes) {
        VisitState[] visited = new VisitState[maxLocks];
        for (int i = 0; i < maxLocks; ++i) {
            visited[i] = VisitState.FRESH;
        }
        return hasCycle(visited, touchedNodes);
    }
    
    public boolean hasCycle(VisitState[] visited, Hashtable<Integer, Boolean> touchedNodes) {
        if (touchedNodes.containsKey(lockId)) {
            touchedNodes.put(lockId, true);
        }
        if (visited[lockId] == VisitState.VISITING) {
            return true;
        } else if (visited[lockId] == VisitState.FRESH) {
            visited[lockId] = VisitState.VISITING;
            for (LockNode n : children) {
                if (n.hasCycle(visited, touchedNodes)) {
                    return true;
                }
            }
            visited[lockId] = VisitState.VISITED;
        }
        return false;
    }
    
    public Lock getLock() {
        if (lock == null) {
            lock = new ReentrantLock();
        }
        return lock;
    }
    
    public int getId() {
        return lockId;
    }
}

public class j {
    public static void main(String[] args) {
        int[] res1 = {1, 2, 3, 4};
        int[] res2 = {1, 5, 4, 1};
        int[] res3 = {1, 4, 5};
        LockFactory.initialize(10);
        
        LockFactory lf = LockFactory.getInstance();
        System.out.println(lf.declare(1, res1));
        System.out.println(lf.declare(2, res2));
        System.out.println(lf.declare(3, res3));
        
        System.out.println(lf.getLock(1, 1));
        System.out.println(lf.getLock(1, 2));
        System.out.println(lf.getLock(1, 3));
        System.out.println(lf.getLock(1, 4));
        System.out.println(lf.getLock(2, 1));
        System.out.println(lf.getLock(2, 5));
        System.out.println(lf.getLock(2, 4));
        System.out.println(lf.getLock(3, 1));
        System.out.println(lf.getLock(3, 4));
        System.out.println(lf.getLock(3, 5));
    }
}
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












