# 层次遍历二叉树（python） - miner_zhu的博客 - CSDN博客





2018年07月22日 22:44:52[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：613










```python
#按层打印
def printByLevel(root):
    if not root:
        return
    print("Print binary tree by level")
    queue = []
    queue.append(root)
    last = root
    level = 1
    print("Level " + str(level) + ':', end=' ')
    while queue:
        root = queue.pop(0)
        print(root.val, end=' ')
        if root.left:
            nlast = root.left
            queue.append(root.left)
        if root.right:
            nlast = root.right
            queue.append(root.right)
        if root == last and queue:
            last = nlast
            print()
            level += 1
            print("Level " + str(level) + ":", end=' ')
```





