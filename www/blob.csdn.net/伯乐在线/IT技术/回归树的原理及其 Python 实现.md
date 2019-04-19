# 回归树的原理及其 Python 实现 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [伯乐在线读者](http://www.jobbole.com/members/jobbole) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
提到回归树，相信大家应该都不会觉得陌生（不陌生你点进来干嘛[捂脸]），大名鼎鼎的 GBDT 算法就是用回归树组合而成的。本文就回归树的基本原理进行讲解，并手把手、肩并肩地带您实现这一算法。
完整实现代码请参考 github： [https://github.com/tushushu/Imylu/blob/master/regression_tree.py](https://link.zhihu.com/?target=https%3A//github.com/tushushu/Imylu/blob/master/regression_tree.py)
## 1. 原理篇
我们用人话而不是大段的数学公式，来讲讲回归树是怎么一回事。
## 1.1 最简单的模型
如果预测某个连续变量的大小，最简单的模型之一就是用平均值。比如同事的平均年龄是 28 岁，那么新来了一批同事，在不知道这些同事的任何信息的情况下，直觉上用平均值 28 来预测是比较准确的，至少比 0 岁或者 100 岁要靠谱一些。我们不妨证明一下我们的直觉：
- 定义损失函数 L，其中 y_hat 是对 y 预测值，使用 MSE 来评估损失：
![ L = -Largefrac{1}{2}normalsizesum_{i=0}^m(y_i-hat{y}) ^ 2](https://www.zhihu.com/equation?tex=+L+%3D+-%5CLarge%5Cfrac%7B1%7D%7B2%7D%5Cnormalsize%5Csum_%7Bi%3D0%7D%5Em%28y_i-%5Chat%7By%7D%29+%5E+2)
- 对 y_hat 求导:
![ large frac{mathrm{d}L}{mathrm{d}hat{y}} normalsize = sum_{i=0}^m(y_i-hat{y}) = sum_{i=0}^my_i - sum_{i=0}^mhat{y} = sum_{i=0}^my_i - m*hat{y} ](https://www.zhihu.com/equation?tex=+%5Clarge+%5Cfrac%7B%5Cmathrm%7Bd%7DL%7D%7B%5Cmathrm%7Bd%7D%5Chat%7By%7D%7D+%5Cnormalsize+%3D+%5Csum_%7Bi%3D0%7D%5Em%28y_i-%5Chat%7By%7D%29+%3D+%5Csum_%7Bi%3D0%7D%5Emy_i+-+%5Csum_%7Bi%3D0%7D%5Em%5Chat%7By%7D+%3D+%5Csum_%7Bi%3D0%7D%5Emy_i+-+m%2A%5Chat%7By%7D+)
- 令导数等于 0，最小化 MSE，则:
![ sum_{i=0}^my_i - m*hat{y} = 0](https://www.zhihu.com/equation?tex=+%5Csum_%7Bi%3D0%7D%5Emy_i+-+m%2A%5Chat%7By%7D+%3D+0)
- 所以，
![ hat{y} = Largefrac{1}{m}normalsizesum_{i=0}^my_i ](https://www.zhihu.com/equation?tex=+%5Chat%7By%7D+%3D+%5CLarge%5Cfrac%7B1%7D%7Bm%7D%5Cnormalsize%5Csum_%7Bi%3D0%7D%5Emy_i+)
- 结论，如果要用一个常量来预测 y，用 y 的均值是一个最佳的选择。
## 1.2 加一点难度
仍然是预测同事年龄，这次我们预先知道了同事的职级，假设职级的范围是整数1-10，如何能让这个信息帮助我们更加准确的预测年龄呢？
一个思路是根据职级把同事分为两组，这两组分别应用我们之前提到的“平均值”模型。比如职级小于 5 的同事分到A组，大于或等于5的分到 B 组，A 组的平均年龄是 25 岁，B 组的平均年龄是 35 岁。如果新来了一个同事，职级是 3，应该被分到 A 组，我们就预测他的年龄是 25 岁。
## 1.3 最佳分割点
还有一个问题待解决，如何取一个最佳的分割点对不同职级的同事进行分组呢？
我们尝试所有 m 个可能的分割点 P_i，沿用之前的损失函数，对 A、B 两组分别计算 Loss 并相加得到 L_i。最小的 L_i 所对应的 P_i 就是我们要找的“最佳分割点”。
## 1.4 运用多个变量
再复杂一些，如果我们不仅仅知道了同事的职级，还知道了同事的工资（貌似不科学），该如何预测同事的年龄呢？
我们可以分别根据职级、工资计算出职级和工资的最佳分割点P_1, P_2，对应的Loss L_1, L_2。然后比较L_1和L2，取较小者。假设L_1 < L_2，那么按照P_1把不同职级的同事分为A、B两组。在A、B组内分别计算工资所对应的分割点，再分为C、D两组。这样我们就得到了AC, AD, BC, BD四组同事以及对应的平均年龄用于预测。
## 1.5 答案揭晓
如何实现这种1 to 2, 2 to 4, 4 to 8的算法呢？
熟悉数据结构的同学自然会想到二叉树，这种树被称为回归树，顾名思义利用树形结构求解回归问题。
## 2. 实现篇
本人用全宇宙最简单的编程语言——Python实现了回归树算法，没有依赖任何第三方库，便于学习和使用。简单说明一下实现过程，更详细的注释请参考本人github上的代码。
## 2.1 创建Node类
初始化，存储预测值、左右结点、特征和分割点


```
class Node(object):
    def __init__(self, score=None):
        self.score = score
        self.left = None
        self.right = None
        self.feature = None
        self.split = None
```
## 2.2 创建回归树类
初始化，存储根节点和树的高度。


```
class RegressionTree(object):
    def __init__(self):
        self.root = Node()
        self.height = 0
```
## 2.3 计算分割点、MSE
根据自变量X、因变量y、X元素中被取出的行号idx，列号feature以及分割点split，计算分割后的MSE。注意这里为了减少计算量，用到了方差公式：
![ D(X) = E{[X-E(X)]^2} = E(X^2)-[E(X)]^2](https://www.zhihu.com/equation?tex=+D%28X%29+%3D+E%7B%5BX-E%28X%29%5D%5E2%7D+%3D+E%28X%5E2%29-%5BE%28X%29%5D%5E2)


```
def _get_split_mse(self, X, y, idx, feature, split):
    split_sum = [0, 0]
    split_cnt = [0, 0]
    split_sqr_sum = [0, 0]
    for i in idx:
        xi, yi = X[i][feature], y[i]
        if xi < split:
            split_cnt[0] += 1
            split_sum[0] += yi
            split_sqr_sum[0] += yi ** 2
        else:
            split_cnt[1] += 1
            split_sum[1] += yi
            split_sqr_sum[1] += yi ** 2
    split_avg = [split_sum[0] / split_cnt[0], split_sum[1] / split_cnt[1]]
    split_mse = [split_sqr_sum[0] - split_sum[0] * split_avg[0],
                    split_sqr_sum[1] - split_sum[1] * split_avg[1]]
    return sum(split_mse), split, split_avg
```
## 2.4 计算最佳分割点
遍历特征某一列的所有的不重复的点，找出MSE最小的点作为最佳分割点。如果特征中没有不重复的元素则返回None。


```
def _choose_split_point(self, X, y, idx, feature):
    unique = set([X[i][feature] for i in idx])
    if len(unique) == 1:
        return None
    unique.remove(min(unique))
    mse, split, split_avg = min(
        (self._get_split_mse(X, y, idx, feature, split)
            for split in unique), key=lambda x: x[0])
    return mse, feature, split, split_avg
```
## 2.5 选择最佳特征
遍历所有特征，计算最佳分割点对应的MSE，找出MSE最小的特征、对应的分割点，左右子节点对应的均值和行号。如果所有的特征都没有不重复元素则返回None


```
def _choose_feature(self, X, y, idx):
    m = len(X[0])
    split_rets = [x for x in map(lambda x: self._choose_split_point(
        X, y, idx, x), range(m)) if x is not None]
    if split_rets == []:
        return None
    _, feature, split, split_avg = min(
        split_rets, key=lambda x: x[0])
    idx_split = [[], []]
    while idx:
        i = idx.pop()
        xi = X[i][feature]
        if xi < split:
            idx_split[0].append(i)
        else:
            idx_split[1].append(i)
    return feature, split, split_avg, idx_split
```
## 2.6 规则转文字
将规则用文字表达出来，方便我们查看规则。


```
def _expr2literal(self, expr):
    feature, op, split = expr
    op = ">=" if op == 1 else "<"
    return "Feature%d %s %.4f" % (feature, op, split)
```
## 2.7 获取规则
将回归树的所有规则都用文字表达出来，方便我们了解树的全貌。这里用到了队列+广度优先搜索。有兴趣也可以试试递归或者深度优先搜索。


```
def _get_rules(self):
    que = [[self.root, []]]
    self.rules = []
    while que:
        nd, exprs = que.pop(0)
        if not(nd.left or nd.right):
            literals = list(map(self._expr2literal, exprs))
            self.rules.append([literals, nd.score])
        if nd.left:
            rule_left = copy(exprs)
            rule_left.append([nd.feature, -1, nd.split])
            que.append([nd.left, rule_left])
        if nd.right:
            rule_right = copy(exprs)
            rule_right.append([nd.feature, 1, nd.split])
            que.append([nd.right, rule_right])
```
## 2.8 训练模型
仍然使用队列+广度优先搜索，训练模型的过程中需要注意：
- 控制树的最大深度max_depth；
- 控制分裂时最少的样本量min_samples_split；
- 叶子结点至少有两个不重复的y值；
- 至少有一个特征是没有重复值的。


```
def fit(self, X, y, max_depth=5, min_samples_split=2):
    self.root = Node()
    que = [[0, self.root, list(range(len(y)))]]
    while que:
        depth, nd, idx = que.pop(0)
        if depth == max_depth:
            break
        if len(idx) < min_samples_split or \
                set(map(lambda i: y[i], idx)) == 1:
            continue
        feature_rets = self._choose_feature(X, y, idx)
        if feature_rets is None:
            continue
        nd.feature, nd.split, split_avg, idx_split = feature_rets
        nd.left = Node(split_avg[0])
        nd.right = Node(split_avg[1])
        que.append([depth+1, nd.left, idx_split[0]])
        que.append([depth+1, nd.right, idx_split[1]])
    self.height = depth
    self._get_rules()
```
## 2.9 打印规则
模型训练完毕，查看一下模型生成的规则


```
def print_rules(self):
    for i, rule in enumerate(self.rules):
        literals, score = rule
        print("Rule %d: " % i, ' | '.join(
            literals) + ' => split_hat %.4f' % score)
```
## 2.10 预测一个样本


```
def _predict(self, row):
    nd = self.root
    while nd.left and nd.right:
        if row[nd.feature] < nd.split:
            nd = nd.left
        else:
            nd = nd.right
    return nd.score
```
## 2.11 预测多个样本


```
def predict(self, X):
    return [self._predict(Xi) for Xi in X]
```
## 3 效果评估
## 3.1 main函数
使用著名的波士顿房价数据集，按照7:3的比例拆分为训练集和测试集，训练模型，并统计准确度。


```
@run_time
def main():
    print("Tesing the accuracy of RegressionTree...")
    # Load data
    X, y = load_boston_house_prices()
    # Split data randomly, train set rate 70%
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, random_state=10)
    # Train model
    reg = RegressionTree()
    reg.fit(X=X_train, y=y_train, max_depth=4)
    # Show rules
    reg.print_rules()
    # Model accuracy
    get_r2(reg, X_test, y_test)
```
## 3.2 效果展示
最终生成了15条规则，拟合优度0.801，运行时间1.74秒，效果还算不错~
![v2-f18bda130e08931020827c562c57f22e_hd](http://jbcdn2.b0.upaiyun.com/2018/08/ec2505e9bb1a66c58473212feef54d1f.jpg)
## 3.3 工具函数
本人自定义了一些工具函数，可以在github上查看 [https://github.com/tushushu/Imylu/blob/master/utils.py](https://github.com/tushushu/Imylu/blob/master/utils.py) 1. run_time – 测试函数运行时间 2. load_boston_house_prices – 加载波士顿房价数据 3. train_test_split – 拆分训练集、测试机 4. get_r2 – 计算拟合优度
## 总结
回归树的原理：
> 
损失最小化，平均值大法。 最佳行与列，效果顶呱呱。
回归树的实现：
> 
一顿操作猛如虎，加减乘除二叉树。
原文：https://zhuanlan.zhihu.com/p/41688007
【关于作者】
李小文：先后从事过数据分析、数据挖掘工作，主要开发语言是Python，现任一家小型互联网公司的算法工程师。Github: [https://github.com/tushushu](https://github.com/tushushu)
