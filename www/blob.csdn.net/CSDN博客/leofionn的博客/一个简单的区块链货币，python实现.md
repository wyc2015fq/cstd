
# 一个简单的区块链货币，python实现 - leofionn的博客 - CSDN博客


2018年05月10日 22:56:28[leofionn](https://me.csdn.net/qq_36142114)阅读数：215


原文
https://blog.csdn.net/vincent__law/article/details/79606357
版权归原作者所有，如需转载，请联系原作者。

区块链的特点和定义，有很多资料可以查询。但对于程序员，代码对于加深理解有着非常大的作用，以下代码为python实现的一个区块链货币，用来帮助理解。代码中配有注释，copy下来可以直接run，测试代码在最后。
理解前提：理解区块链的“账本”的比喻，基于区块链的电子货币的概念，“挖矿”。
源码同时放到了我的github上。
Github（https://github.com/Warmriver/blockChainHW）
```python
import hashlib
```

```python
import random
```

```python
import time
```

```python
# 区块链中交易
```

```python
class
```
```python
Transaction
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(
```
```python
self
```
```python
, from_addr, to_addr, amount)
```
```python
:
```

```python
""
```
```python
"
```

```python
fromAddr: 交易的发起人的地址
```

```python
toAdddr: 交易的收款人的地址
```

```python
amount: 交易金额
```

```python
"
```
```python
""
```

```python
self
```
```python
.from_addr = from_addr
```

```python
self
```
```python
.to_addr = to_addr
```

```python
self
```
```python
.amount = amount
```

```python
def
```
```python
__str__
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
return
```
```python
str(
```
```python
self
```
```python
.from_addr) +
```
```python
" send "
```
```python
+ str(
```
```python
self
```
```python
.amount) +
```
```python
" to "
```
```python
+ str(
```
```python
self
```
```python
.to_addr)
```

```python
class
```
```python
Block
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(
```
```python
self
```
```python
, transactions, timestamp, data =
```
```python
''
```
```python
, previous_hash =
```
```python
'0'
```
```python
, nonce =
```
```python
0
```
```python
)
```
```python
:
```

```python
""
```
```python
"
```

```python
transactions:交易列表，实际应用时并不能这么传递，因为交易的量很很大
```

```python
timestamp:时间戳
```

```python
data:数据
```

```python
nonce:随机数，使用这个数字的改变来更改block的hash值
```

```python
previous_hash:上一个模块的hash值
```

```python
hash:模块的hash值
```

```python
"
```
```python
""
```

```python
self
```
```python
.transactions = transactions
```

```python
self
```
```python
.timestamp = timestamp
```

```python
self
```
```python
.data = data
```

```python
self
```
```python
.previous_hash = previous_hash
```

```python
self
```
```python
.nonce = nonce
```

```python
self
```
```python
.hash =
```
```python
self
```
```python
.calculate_block_hash()
```

```python
# 计算hash值，上一个block的hash值也在材料中
```

```python
def
```
```python
calculate_block_hash
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
combination = str(
```
```python
self
```
```python
.timestamp) + str(
```
```python
self
```
```python
.data) + str(
```
```python
self
```
```python
.previous_hash) + str(
```
```python
self
```
```python
.nonce)
```

```python
for
```
```python
trans
```
```python
in
```
```python
self
```
```python
.
```
```python
transactions:
```

```python
combination += str(trans)
```

```python
return
```
```python
hashlib.sha256(bytes(combination,
```
```python
'utf-8'
```
```python
)).hexdigest()
```

```python
# 挖掘，为了限制用户节点的挖掘能力，增加proof of work机制，增加挖掘的难度，difficulty越大，hash碰撞越难
```

```python
def
```
```python
mineblock
```
```python
(
```
```python
self
```
```python
,difficult)
```
```python
:
```

```python
start =
```
```python
0
```

```python
while
```
```python
[v
```
```python
for
```
```python
v
```
```python
in
```
```python
self
```
```python
.hash[
```
```python
start:
```
```python
difficult]] != [
```
```python
'0'
```
```python
for
```
```python
v
```
```python
in
```
```python
range(start, difficult)]:
```

```python
# while [v for v in self.hash[start:difficult]] != ['0' for v in range(start, difficult)]:
```

```python
self
```
```python
.nonce +=
```
```python
1
```

```python
self
```
```python
.hash =
```
```python
self
```
```python
.calculate_block_hash()
```

```python
print(
```
```python
"挖到了一个 block，"
```
```python
+
```
```python
self
```
```python
.hash+
```
```python
",difficulty为"
```
```python
, difficult)
```

```python
# 区块链
```

```python
class
```
```python
BlockChain
```
```python
:
```

```python
def
```
```python
__init__
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
""
```
```python
"
```

```python
diffculty:难度
```

```python
chain:区块链，这里用数组表示
```

```python
pending_transactions:等待被挖掘的交易列表
```

```python
reward_coin:奖励金，对记账用户的奖励，即区块链货币，设置为一个block奖励1.2个，
```

```python
单一用户修改此项并不会生效，因为每个人都有账本，必须说服多数派，拥有大于百分之50的同意。
```

```python
"
```
```python
""
```

```python
self
```
```python
.difficult =
```
```python
3
```

```python
self
```
```python
.chain = [
```
```python
self
```
```python
.genesis_block()]
```

```python
self
```
```python
.pending_transactions = []
```

```python
self
```
```python
.reward_coin =
```
```python
1.2
```

```python
# 创世块
```

```python
def
```
```python
genesis_block
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
first_transaction = Transaction(
```
```python
"24monkey"
```
```python
,
```
```python
"24monkey"
```
```python
,
```
```python
50
```
```python
)
```

```python
return
```
```python
Block([first_transaction], int(time.time()),
```
```python
"创世模块"
```
```python
)
```

```python
# 获得区块链最新的block
```

```python
def
```
```python
get_latest_block
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
return
```
```python
self
```
```python
.chain[len(
```
```python
self
```
```python
.chain) -
```
```python
1
```
```python
]
```

```python
# 记录交易
```

```python
def
```
```python
transaction_record
```
```python
(
```
```python
self
```
```python
, transaction)
```
```python
:
```

```python
self
```
```python
.pending_transactions.append(transaction)
```

```python
# 对区块链中行为进行交易，mining
```

```python
def
```
```python
mine_pending_transactions
```
```python
(
```
```python
self
```
```python
, reward_addr)
```
```python
:
```

```python
new_block = Block(
```
```python
self
```
```python
.pending_transactions, int(time.time()), )
```

```python
new_block.mineblock(
```
```python
self
```
```python
.difficult)
```

```python
print(
```
```python
"成功记录一笔交易，挖到了一个block"
```
```python
)
```

```python
self
```
```python
.chain.append(new_block)
```

```python
self
```
```python
.pending_transactions = [Transaction(
```
```python
''
```
```python
, reward_addr,
```
```python
self
```
```python
.reward_coin)]
```

```python
# 增加block
```

```python
def
```
```python
add_block
```
```python
(
```
```python
self
```
```python
, block)
```
```python
:
```

```python
block.previous_hash =
```
```python
self
```
```python
.get_latest_block().hash
```

```python
block.mineblock(
```
```python
self
```
```python
.difficult)
```

```python
self
```
```python
.chain.append(block)
```

```python
# 获得一个账户的balance
```

```python
def
```
```python
get_balance
```
```python
(
```
```python
self
```
```python
, addr)
```
```python
:
```

```python
balance =
```
```python
0
```

```python
for
```
```python
block
```
```python
in
```
```python
self
```
```python
.
```
```python
chain:
```

```python
for
```
```python
transaction
```
```python
in
```
```python
block.
```
```python
transactions:
```

```python
if
```
```python
transaction.from_addr ==
```
```python
addr:
```

```python
balance -= transaction.amount
```

```python
elif transaction.to_addr ==
```
```python
addr:
```

```python
balance += transaction.amount
```

```python
return
```
```python
balance
```

```python
# 账本是否本篡改，篡改者需要篡改所有的block且说服他人，篡改很困难
```

```python
def
```
```python
check_chain_validity
```
```python
(
```
```python
self
```
```python
)
```
```python
:
```

```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
1
```
```python
,len(
```
```python
self
```
```python
.chain)):
```

```python
current_block =
```
```python
self
```
```python
.chain[i]
```

```python
previous_block =
```
```python
self
```
```python
.chain[i-
```
```python
1
```
```python
]
```

```python
if
```
```python
(current_block.hash != current_block.calculate_block_hash()):
```

```python
return
```
```python
False
```

```python
if
```
```python
(current_block.previous_hash != previous_block.hash):
```

```python
return
```
```python
False
```

```python
return
```
```python
True
```

```python
def
```
```python
time_s
```
```python
()
```
```python
:
```

```python
return
```
```python
int(time.time())
```


```python
if
```
```python
__name_
```
```python
_
```
```python
==
```
```python
'__main__'
```
```python
:
```

```python
# 初始化一个区块链
```

```python
vin_coin = BlockChain()
```

```python
# 两笔交易
```

```python
trans_a = Transaction(
```
```python
"shiki_addr"
```
```python
,
```
```python
"vincent_addr"
```
```python
,
```
```python
44
```
```python
)
```

```python
trans_b = Transaction(
```
```python
"vincent_addr"
```
```python
,
```
```python
"shiki_addr"
```
```python
,
```
```python
44
```
```python
)
```

```python
""
```
```python
"
```

```python
测试挖矿，proof of work的难度测试
```

```python
"
```
```python
""
```

```python
vin_coin.add_block(Block([trans_a], time_s(), {
```
```python
" reason "
```
```python
:
```
```python
" I owed you"
```
```python
}))
```

```python
vin_coin.add_block(Block([trans_b], time_s(), {
```
```python
" reason "
```
```python
:
```
```python
" I gave it back to you cause I love you"
```
```python
}))
```

```python
""
```
```python
"
```

```python
测试篡改数据的测试
```

```python
"
```
```python
""
```

```python
print(
```
```python
"vin币chain合法吗？"
```
```python
, vin_coin.check_chain_validity())
```

```python
vin_coin.chain[
```
```python
1
```
```python
].transactions[
```
```python
0
```
```python
].amount =
```
```python
40
```

```python
# 修改一个block后，重新计算这个block的hash，并不能成功
```

```python
vin_coin.chain[
```
```python
1
```
```python
].hash = vin_coin.chain[
```
```python
1
```
```python
].calculate_block_hash()
```

```python
print(
```
```python
"vin币chain合法吗？"
```
```python
, vin_coin.check_chain_validity())
```

```python
""
```
```python
"
```

```python
测试vin币奖励机制
```

```python
"
```
```python
""
```

```python
vin_coin.transaction_record(trans_a)
```

```python
vin_coin.transaction_record(trans_b)
```

```python
vin_coin.mine_pending_transactions(
```
```python
"记录者1"
```
```python
)
```

```python
# 由于记录者1获得奖励的事件还没有入账，所以查不到
```

```python
print(
```
```python
"记录者1的账户: "
```
```python
, vin_coin.get_balance(
```
```python
"记录者1"
```
```python
))
```

```python
# 记录者2确认了记录者1的行为，记录者1的账户余额不足
```

```python
vin_coin.mine_pending_transactions(
```
```python
"记录者2"
```
```python
)
```

```python
print(
```
```python
"记录者1的账户: "
```
```python
, vin_coin.get_balance(
```
```python
"记录者1"
```
```python
))
```

