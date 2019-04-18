# 【刘文彬】EOS技术研究：合约与数据库交互 - weixin_33985507的博客 - CSDN博客
2018年12月11日 08:41:30[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
原文链接：醒者呆的博客园，[www.cnblogs.com/Evsward/p/m…](https://link.juejin.im?target=https%3A%2F%2Fwww.cnblogs.com%2FEvsward%2Fp%2Fmulti_index.html)
> 
智能合约操作链数据库是很常见的应用场景。EOS提供了专门的工具来做这件事（相当于Ethereum的leveldb），专业术语叫做持久化API，本文将完整严密地介绍这个工具以及对它的使用测试。
> 
关键字：EOS，智能合约，链数据库，eosio::multi_index，constructor，emplace，erase，find。
## 需求
首先来看EOS中智能合约涉及到持久化的场景需求。一个action在执行时会有上下文变量出现，包括事务机制的处理，这些内容会应用链上分配的内存资源，而如果没有持久化技术，执行超过作用域时就会丢失掉这些上下文数据。因此要使用持久化技术将关键内容记录在链数据库中，任何时候使用都不受影响。持久化技术应该包括：
- 记录一些状态持久化到数据库中
- 具备查询的能力从数据库中获取内容
- 提供C++ 的API来调用这些服务，也服务于合约开发者
### eosio::multi_index
这是模仿boost::multi_index开发的一套库。它使用C++编写，提供了合约与数据库的交互持久化接口。
```
Multi-Index Iterators：不同于其他key-value数据库，multi_index提供了不同类型的key对应的值也是可迭代的复杂集合类型。
复制代码
```
**创建表**
- class/struct 定义一个持久化对象。
- 该对象需要有一个const的成员作为主键，类型为uint64_t
- 二级主键可选，提供不同的键类型
- 为每个二级索引定义一个键导出器，键导出器是一个函数，可以用来从Multi_index表中获取键
**使用Multi-Index表**
一般来讲，对数据库的操作无外乎增删改查，
- 增加对应的方法是emplace
- 修改就是modify
- 删除是erase
- 查找包括find和get，以及迭代器操作
## 实战
下面我们通过一个智能合约操作底层数据库的实例，来演示对持久化api，multi_index的使用。过程中，也会展示相应api的定义。
### 车辆维修跟踪
首先，创建一个service表，用来创建服务记录报告，它包含的字段有：
- 主键，客户ID不可当做主键，因为一个客户可以有多条记录。实际上，我们并不直接需要主键，所以我们可以让系统为我们生成一个。
- 客户ID，与账户名字对应
- 服务日期
- 里程表，汽车里程表
```bash
#include <eosiolib/eosio.hpp>
using namespace eosio;
class vehicle : public eosio::contract {
public:
    /// @abi table
    struct service_rec {
        uint64_t        pkey;
        account_name    customer;
        uint32_t        service_date;
        uint32_t        odometer;
        auto primary_key() const { return pkey; }
        account_name get_customer() const { return customer; }
        EOSLIB_SERIALIZE(service_rec, (pkey)(customer)(service_date)(odometer))
    };
    typedef multi_index<N(service), service_rec> service_table_type;
    using contract::contract;
    /// @abi action
    void exec(account_name owner, account_name customer) {
        print("Hello, ", name{customer});
        service_table_type service_table(current_receiver(), owner);// 构造器，第一个参数是code代表表的拥有者，目前是current_receiver()，也可以使用contract基类的构造器初始化时的账户名_self，第二个参数是scope，在代码层次范围的标识符，这里就使用传入的owner账户。
        uint64_t pkeyf;// 主键
        service_table.emplace(owner, [&](auto &s_rec) {
            s_rec.pkey = service_table.available_primary_key();// 主键自增
            pkeyf = s_rec.pkey;
            print(pkeyf);// 打印主键内容
            s_rec.customer = customer;
            s_rec.service_date = 2000;
            s_rec.odometer = 1000;
        });
        service_rec result = service_table.get(pkeyf);
        print("_", result.pkey);
        print("_", result.customer);
        print("_", result.service_date);
        print("_", result.odometer);
    }
};
EOSIO_ABI(vehicle, (exec))
复制代码
```
使用eosiocpp工具生成abi文件和wast文件。
`eosiocpp -g vehicle.abi vehicle.cpp | eosiocpp -o vehicle.wasm vehicle.cpp`
然后在终端中部署该合约，
`cleos set contract one work/CLionProjects/github.com/eos/contracts/vehicle`
调用合约的exec方法，并输出结果：
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action one exec '["one","two"]' -p one
executed transaction: 3a45eaeb06732ad0c53ba7b157003e1c503f74ed447029d82cecbe12926cc9a9  112 bytes  365 us
#           one <= one::exec                    {"owner":"one","customer":"two"}
>> Hello, two13_13_14927180964919508992_2000_1000
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
通过输出结果，可以知道主键为13，customer账户名被转为无符号32位整型数据14927180964919508992，服务时间为2000，里程表为1000。我们已经成功将数据存入了multi_index并取了出来。
```
删除的话可以通过service_table.erase(result);来删除掉对应记录。
find涉及二级索引，迭代器等操作，end判断等multi_index的api操作没有给出具体实例，未来在其他合约使用时会直接说明。
复制代码
```
## 再演练一个例子
为了更好的熟悉multi_index的机制，我们再演练一个简单的例子：维护一个todolist的数据库表。直接上代码如下：
```bash
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
class todolist : public eosio::contract
{
  public:
    using eosio::contract::contract;
    // @abi table todos i64
    struct todo
    {
        uint64_t id;             // 待办事项主键id
        std::string description; // 待办事项的描述参数
        uint64_t completed;      // 标记一个待办事项是否已完成
        uint64_t primary_key() const { return id; }
        EOSLIB_SERIALIZE(todo, (id)(description)(completed))
    };
    typedef eosio::multi_index<N(todos), todo> todo_table;
    // @abi action
    void create(account_name author, const uint32_t id, const std::string &description)
    {
        todo_table todos(_self, author);
        todos.emplace(author, [&](auto &new_todo) {
            new_todo.id = id;
            new_todo.description = description;
            new_todo.completed = 0;
        });
        eosio::print("todo#", id, " created");
    }
    // @abi action
    void complete(account_name author, const uint32_t id)
    {
        todo_table todos(_self, author);
        auto todo_lookup = todos.find(id);
        eosio_assert(todo_lookup != todos.end(), "Todo does not exist");
        todos.modify(todo_lookup, author, [&](auto &modifiable_todo) {
            modifiable_todo.completed = 1;
        });
        eosio::print("todo#", id, " marked as complete");
    }
    // @abi action
    void destroy(account_name author, const uint32_t id)
    {
        todo_table todos(_self, author);
        auto todo_lookup = todos.find(id);
        todos.erase(todo_lookup);
        eosio::print("todo#", id, " destroyed");
    }
};
EOSIO_ABI(todolist, (create)(complete)(destroy))
复制代码
```
这里加入了对数据的增删改查功能。然后我们使用
`eosiocpp -o todolist.wast todolist.cpp && eosiocpp -g todolist.abi todolist.cpp`
创建对应的abi和wast文件。接下来创建用户todo.user，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos create account eosio todo.user EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
executed transaction: 7d7191e2935c6fd0024f571e228ee11d51f4f44a64ed2ce977326fb27679f0c6  200 bytes  174 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"todo.user","owner":{"threshold":1,"keys":[{"key":"EOS6MRyAjQq8ud7hVNYcfnV...
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
部署合约，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos set contract todo.user work/VSCode-Projects/eos/contracts/todolist -p todo.user
Reading WAST/WASM from work/VSCode-Projects/eos/contracts/todolist/todolist.wasm...
Using already assembled WASM...
Publishing contract...
executed transaction: 03befa58d6a54970db708beaa0520179277b01addf1ec647a76a9b3f6459ff57  5128 bytes  2203 us
#         eosio <= eosio::setcode               {"account":"todo.user","vmtype":0,"vmversion":0,"code":"0061736d01000000016e1260047f7e7f7f0060037f7e...
#         eosio <= eosio::setabi                {"account":"todo.user","abi":{"types":[],"structs":[{"name":"todo","base":"","fields":[{"name":"id",...
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
创建新条目，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action todo.user create '["todo.user",1,"hello, world"]' -p todo.user
executed transaction: 54d9825971370a242fa51fa7cc587a6478dd7852039c263d91058c6b1163a4bc  120 bytes  336 us
#     todo.user <= todo.user::create            {"author":"todo.user","id":1,"description":"hello, world"}
>> todo#1 created
复制代码
```
查询，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos get table todo.user todo.user todos
{
  "rows": [{
      "id": 1,
      "description": "hello, world",
      "completed": 0
    }
  ],
  "more": false
}
复制代码
```
完成事项，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action todo.user complete '["todo.user",1]' -p todo.user
executed transaction: 11423637cb321969961b3ce0305c93ba7f95a83b6d82c1a4e31a08c569f2dcaa  104 bytes  312 us
#     todo.user <= todo.user::complete          {"author":"todo.user","id":1}
>> todo#1 marked as complete
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
再次查询，completed字段已置为1，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos get table todo.user todo.user todos
{
  "rows": [{
      "id": 1,
      "description": "hello, world",
      "completed": 1
    }
  ],
  "more": false
}
复制代码
```
创建多条记录，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action todo.user create '["todo.user",2,"eos funk"]' -p todo.user
executed transaction: 35f417a4d7438e6ea9ffd837e3c261fca0cb3926b534dc6603fdb38f94d5cd77  120 bytes  329 us
#     todo.user <= todo.user::create            {"author":"todo.user","id":2,"description":"eos funk"}
>> todo#2 created
warning: transaction executed locally, but may not be confirmed by the network yet
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action todo.user create '["todo.user",10,"go to bank"]' -p todo.user
executed transaction: cd4cd2c85500e93a79eb5c3b3852a0a96a9fb220696c63f2683b473d58a6ca34  120 bytes  311 us
#     todo.user <= todo.user::create            {"author":"todo.user","id":10,"description":"go to bank"}
>> todo#10 created
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
查询，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos get table todo.user todo.user todos
{
  "rows": [{
      "id": 1,
      "description": "hello, world",
      "completed": 1
    },{
      "id": 2,
      "description": "eos funk",
      "completed": 0
    },{
      "id": 10,
      "description": "go to bank",
      "completed": 0
    }
  ],
  "more": false
}
复制代码
```
删除一条数据，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos push action todo.user destroy '["todo.user",2]' -p todo.user
executed transaction: c87e37f4521e0ce2a865acbeb63c0f3a0681b9b5a0a6c5db8cd53584d6d13dca  104 bytes  2198 us
#     todo.user <= todo.user::destroy           {"author":"todo.user","id":2}
>> todo#2 destroyed
warning: transaction executed locally, but may not be confirmed by the network yet
复制代码
```
再次查询，
```bash
liuwenbin@liuwenbin-H81M-DS2:~$ cleos get table todo.user todo.user todos
{
  "rows": [{
      "id": 1,
      "description": "hello, world",
      "completed": 1
    },{
      "id": 10,
      "description": "go to bank",
      "completed": 0
    }
  ],
  "more": false
}
复制代码
```
这是一个完整的，通过multi_index进行curd的一个例子（第一个例子是官方文档给出的，其中内容有bug）。
## 总结
通过本篇文章的学习，我们掌握了如何在EOS中使用智能合约调用multi_index实现数据的持久化。
## 参考文档
- EOS官方文档
- EOS asia
### **相关文章和视频推荐**
***圆方圆学院汇集大批区块链名师，打造精品的区块链技术课程。 在各大平台都长期有优质免费公开课，欢迎报名收看。***
公开课地址：[ke.qq.com/course/3451…](https://link.juejin.im?target=https%3A%2F%2Fke.qq.com%2Fcourse%2F345101)
