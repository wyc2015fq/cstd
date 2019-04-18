# 关于hibernate 一对多关联的 理解(inverse和cascade ) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月14日 22:10:45[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：40


1：一对多 关联 分为 单向维护和双向维护，通过 inverse 来控制，inverse=“true” 就是单向，就让另外一个表来维护，inverse=“false”是双向维护、两个都会去维护。

2：我们通过 在两个pojo 中，设置对方的 全局变量，来告诉hibernate这两个表的关联关系。

3：双向维护会存在sql冗余，假如存在一个部门表、一个员工表、部门表的depid是主键、员工表的depid是外键（关联部门表），当我们 new 两个员工，new 一个部门，然后执行 session.save(dep)的时候会出现如下语句

```
Hibernate: 
    insert 
    into
        dep
        (depname, parentid) 
    values
        (?, ?)
Hibernate: 
    insert 
    into
        employee
        (empname, cardno, sex, age, nation, depid) 
    values
        (?, ?, ?, ?, ?, ?)
Hibernate: 
    insert 
    into
        employee
        (empname, cardno, sex, age, nation, depid) 
    values
        (?, ?, ?, ?, ?, ?)
Hibernate: 
    update
        employee 
    set
        depid=? 
    where
        empid=?
Hibernate: 
    update
        employee 
    set
        depid=? 
    where
        empid=?
```

请注意 红色 的两条 sql 语句，这个部门pojo维护的时候产生的，而实际上，前3条sql就已经能够达到我们预期的结果。

那么我们该如何理解产生的这两条 update语句呢？

上面的第二条中 我说了，hibernate是通过pojo中的对方的全局变量来获取 两个表之间的关系的，当部门pojo来维护表关系的时候，它发现 它的这个类 里面有个 set<EmpVo> 这个集合、里面存放着2个员工信息、这两个员工信息上面写着它的部门是哪个，那么部门维护的时候 就会去数据库中 更改 员工表的 depid(部门id、外键)这个字段的值，但是其实在部门表维护之前、员工表已经执行了3条insert语句，将一个部门、两个员工的信息插入到了各自的表中，所以就存在sql冗余。

解决这个问题的方式就是 在 一对多 这个关系中 ，“一” 的一方 设置 inverse=“true” ，从而使得 “一” 的一方 放弃维护权限、让 “多”的一方 来 维护sql。  

4：一对多的关系 中 ，如果是单向维护，只能是 "多" 的一方来维护，不能是“一”的一方来维护。

     可以这么理解：   部门 存了 许多 员工、它 自己 没法 为 每一个 员工 去 insert，只能 每 一个 员工 他 自己 去 insert、也就    是 来 这个 部门 做登记，部门 能 做的 就是 这员工 登记了 以后、根据 关系、然后给他们 安排 指定的部门。反之他忙不过来。

5：我在这里说一下 cascade 这个属性，网上有人说当 设置了 inverse=“true”的时候  cascade 这个属性会失效，但是在我的测试中， 设置了 inverse=“true”，然后把 cascade 属性 删除， 执行 save(dep) ，结果只有 一条insert语句，就是仅仅向部门表中插入了一条部门的数据，而员工表并没有新增数据，所以说 设置了 inverse=“true”的时候  cascade 这个属性并不会失效。 cascade属性有哪些值以及分别什么作用，这里不再赘述，网上有很多。


