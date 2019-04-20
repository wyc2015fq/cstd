# linux下request_mem_region的粗略理解 - h13 - 博客园
linux下request_mem_region的粗略理解
文章来源:http://gliethttp.cublog.cn
  Linux把基于I/O映射方式的I/O端口和基于内存映射方式的I/O端口资源统称为“I/O区域”（I/O Region）。I/O Region仍然是一种I/O资源，因此它仍然可以用resource结构类型来描述。
  Linux是以一种倒置的树形结构来管理每一类I/O资源（如：I/O端口、外设内存、DMA和IRQ）的。每一类I/O资源都对应有一颗倒置的资源树，树中的每一个节点都是一个resource结构，而树的根结点root则描述了该类资源的整个资源空间。
1.结构体
  1.1>struct resource iomem_resource = { "PCI mem", 0x00000000, 0xffffffff, IORESOURCE_MEM };
  1.2>struct resource {
                const char *name;
                unsigned long start, end;
                unsigned long flags;
                struct resource *parent, *sibling, *child;
             };
2.调用函数
  request_mem_region(S1D_PHYSICAL_REG_ADDR,S1D_PHYSICAL_REG_SIZE, "EpsonFB_RG")
#define request_mem_region(start,n,name) __request_region(&iomem_resource, (start), (n), (name))
__request_region检查是否可以安全占用起始物理地址S1D_PHYSICAL_REG_ADDR之后的连续S1D_PHYSICAL_REG_SIZE字节大小空间
```
struct resource * __request_region(struct resource *parent, unsigned long start, unsigned long n, const char *name)
{
    struct resource *res = kmalloc(sizeof(*res), GFP_KERNEL);
    if (res) {
        memset(res, 0, sizeof(*res));
        res->name = name;
        res->start = start;
        res->end = start + n - 1;
        res->flags = IORESOURCE_BUSY;
        write_lock(&resource_lock);
        for (;;) {
            struct resource *conflict;
            conflict = __request_resource(parent, res);    //sibling parent下的所有单元,检测申请部分是否存在交叠冲突
            if (!conflict)                                 //conflict=0;申请成功,正常安置了[start,end]到相应位置
                break;
            if (conflict != parent) {
                parent = conflict;
                if (!(conflict->flags & IORESOURCE_BUSY))
                    continue;
            }
             kfree(res);                                   //检测到了资源交叠冲突,kfree归还kmalloc申请的内存
            res = NULL;
            break;
        }
        write_unlock(&resource_lock);
    }
    return res;
}
static struct resource * __request_resource(struct resource *root, struct resource *new)
{
    unsigned long start = new->start;
    unsigned long end = new->end;
    struct resource *tmp, **p;
    if (end < start)
        return root;
    if (start < root->start)
        return root;
    if (end > root->end)
        return root;
    p = &root->child;                                      //root下的第一个链表元素*p.[child链表是以I/O资源物理地址从低到高的顺序排列的]
    for (;;) {
        tmp = *p;
        if (!tmp || tmp->start > end) {
            new->sibling = tmp;
            *p = new;
//可以从root->child=null开始我们的分析考虑,此时tmp=null,那么第一个申请将以!tmp条件满足而进入
//这时root->child的值为new指针,new->sibling = tmp = null;当第二次申请发生时:如果tmp->start > end成立,
//那么,root->child的值为new指针,new->sibling = tmp;这样就链接上了，空间分布图如:
//child=[start,end]-->[tmp->start,tmp->end](1);如果条件tmp->start > end不成立，那么只能是!tmp条件进入
//那么,root->child的值不变,tmp->sibling = new;new->sibling = tmp = null这样就链接上了，空间分布图如:
//child=[child->start,child->end]-->[start,end](2);
//当第三次申请发生时:如果start在(2)中的[child->end,end]之间,那么tmp->end < start将成立,继而continue,
//此时tmp = (2)中的[start,end],因为tmp->start < end,所以继续执行p = &tmp->slibing = null,
//因为tmp->end > start,所以资源冲突,返回(2)中的[start,end]域
//综上的两个边界值情况和一个中间值情况的分析,可以知道代码实现了一个从地地址到高地址的顺序链表
//模型图:childe=[a,b]-->[c,d]-->[e,f],此时有一个[x,y]需要插入进去,tmp作为sibling指针游动
//tmp指向child=[a,b],
//tmp指向[a,b],当tmp->start>y时,插入后的链接图为:child=[x,y]-->[a,b]-->[c,d]-->[e,f]-->null;当tmp->end>=x时,冲突返回tmp
//tmp指向[c,d],当tmp->start>y时,插入后的链接图为:child=[a,b]-->[x,y]-->[c,d]-->[e,f]-->null;当tmp->end>=x时,冲突返回tmp
//tmp指向[e,f],当tmp->start>y时,插入后的链接图为:child=[a,b]-->[c,d]-->[x,y]-->[e,f]-->null;当tmp->end>=x时,冲突返回tmp
//tmp指向null                  ,插入后的链接图为:child=[a,b]-->[c,d]-->[e,f]-->[x,y]-->null;
//顺利的达到了检测冲突,顺序链接的目的
            new->parent = root;    
            return NULL;
        }
        p = &tmp->sibling;
        if (tmp->end < start)
            continue;
        return tmp;
    }
}
本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/yang_dk/archive/2008/02/25/2118952.aspx
```
