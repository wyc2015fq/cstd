# outline of discrete mathematics and its applications(9.1-9.6) - 彩虹糖的博客 - CSDN博客





2017年06月14日 09:27:47[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：229
个人分类：[离散数学](https://blog.csdn.net/caozixuan98724/article/category/6944178)









## relations

### 9.1 relations and their properties
- definition: let A and B be set. A binary relation from A to B is a subset of A×B
- aRb:a is said to be related to b by R.
- properties of relations 
- reflexive(自反关系)
- symmetric(对称性)
- transitive(传递性)
- 以上性质空集都具备

- combining relations(组合关系) 
- composing the parent relation with itself(defined recursively递归定义法)
- the relation R on a set A is transitive if and only if R^n 属于 R for n=1,2,3,…


### n-ary relations and their applicaions
- n is called the degree 

### 9.3 representing relations
- representing relations using matrices
- 利用关系矩阵可以判断矩阵自反和对称的关系
- 矩阵的乘法可以用来进行复合关系的判断（矩阵的乘法 大于等于2时取1）
- representing relations using digraphs

### 　9.4 closures of relations(关系的闭包)
- reflexive closure(自反闭包)
- diagonal relation（对角关系）
- symmetric closure(对称闭包)
- 重要吧 Let R be a relation on a set A. There is a path of lengh n. where n is a positive integer from a to b if and only if (a,b) ∈ R^n
- transitive closures（难点？）
- let R be a relation on a set A. the connectivity relation R* consists of the pairs (a,b) such that there is a path of length at least one from a to b in R.
- the transitive closure of a relation R equals the connectivity relation R
- Warshall算法 **没看懂？**

### 9.5 equivalence relations(等价关系)
- a~b a and b are equivalent
- equivalence classes 
- a partition of a set S is a collection of disjoint nonempty subsets of S that have S as their union. 

- equivalence classes and partitions(这个地方有很多地方不明白，需要例子，看一下中文课本)

### 9.6 partial orderings
- definition: relexive antisymmetric and transitive
- partially ordered set or poset
- 代表偏序关系的符号
- comparable and incomparable
- if(S, ≤) is a poset and every two elements of S are comparable, S is called a totally ordered or linearly ordered set, and ≤ is called a total order or a linear order. A totally ordered set is also called a chain
- (S, ≤) is a well-ordered set if it is a poset such that ≤ is a total ordering and every nonempty subset of S has a least element（最小元）.
- lexicographic order(字典序)
- Hasse Diagrams(看一下中文课本的说明吧)
- maximal and minimal elements 
- greatest element and least element(不一定存在)
- upper bound and lower bound(可能有多个上界和下界)
- greatest lower bound(glb(A)) least lower bound(lub(A))
- lattices
- topological sorting(拓扑排序)




