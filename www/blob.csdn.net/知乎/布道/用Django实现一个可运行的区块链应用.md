# 用Django实现一个可运行的区块链应用 - 知乎
# 



> **本文主要译自：[learn-blockchains-by-building-one](https://link.zhihu.com/?target=https%3A//hackernoon.com/learn-blockchains-by-building-one-117428612f46) ，原文flask运行有问题，对代码进行调整，使用Django运行。代码文章下网盘链接。**

对数字货币的崛起感到新奇的我们，并且想知道其背后的技术——区块链是怎样实现的。

但是完全搞懂区块链并非易事，我喜欢在实践中学习，通过写代码来学习技术会掌握得更牢固。通过构建一个区块链可以加深对区块链的理解。

## **准备工作**

本文要求读者对Python有基本的理解，能读写基本的Python，并且需要对HTTP请求有基本的了解。

我们知道区块链是由区块的记录构成的不可变、有序的链结构，记录可以是交易、文件或任何你想要的数据，重要的是它们是通过哈希值（hashes）链接起来的。

## **环境准备**

环境准备，确保已经安装Python3.5, pip , django, requests，urllib，json，hashlib
安装方法：


`pip install django requests`




同时还需要一个HTTP客户端，比如Postman，cURL或其它客户端，本文以Postman为例。

## **开始创建Blockchain**

通过django-admin startproject block创建一个block的项目，在项目中创建一个demo项目django-admin startproject demo ，目录结构：
![](https://pic3.zhimg.com/v2-f5f59c53a1473dd79fa051cf9b503ba6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='182' height='353'></svg>)
## **Blockchain类**

在views中创建一个Blockchain类，在构造函数中创建了两个列表，一个用于储存区块链，一个用于储存交易。

以下是Blockchain类的框架：



```
class Blockchain(object):
    def __init__(self):
        self.chain = []
        self.current_transactions = []
        
    def new_block(self):
        # Creates a new Block and adds it to the chain
        pass
    
    def new_transaction(self):
        # Adds a new transaction to the list of transactions
        pass
    
    @staticmethod
    def hash(block):
        # Hashes a Block
        pass

    @property
    def last_block(self):
        # Returns the last Block in the chain
        pass
```





Blockchain类用来管理链条，它能存储交易，加入新块等，下面我们来进一步完善这些方法。

## **块结构**

每个区块包含属性：索引（index），Unix时间戳（timestamp），交易列表（transactions），工作量证明（稍后解释）以及前一个区块的Hash值。

以下是一个区块的结构：



```
block = {
    'index': 1,
    'timestamp': 1506057125.900785,
    'transactions': [
        {
            'sender': "8527147fe1f5426f9dd545de4b27ee00",
            'recipient': "a77f5cdfa2934df3954a5c7c7da5df1f",
            'amount': 5,
        }
    ],
    'proof': 324984774000,
    'previous_hash': "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824"
}
```


到这里，区块链的概念就清楚了，每个新的区块都包含上一个区块的Hash，这是关键的一点，它保障了区块链不可变性。如果攻击者破坏了前面的某个区块，那么后面所有区块的Hash都会变得不正确。不理解的话，慢慢消化。

## **加入交易**

接下来我们需要添加一个交易，来完善下new_transaction方法



```
class Blockchain(object):
    ...
    
    def new_transaction(self, sender, recipient, amount):
        """
        生成新交易信息，信息将加入到下一个待挖的区块中
        :param sender: <str> Address of the Sender
        :param recipient: <str> Address of the Recipient
        :param amount: <int> Amount
        :return: <int> The index of the Block that will hold this transaction
        """

        self.current_transactions.append({
            'sender': sender,
            'recipient': recipient,
            'amount': amount,
        })

        return self.last_block['index'] + 1
```





方法向列表中添加一个交易记录，并返回该记录将被添加到的区块(下一个待挖掘的区块)的索引，等下在用户提交交易时会有用。

## **创建新块**

当Blockchain实例化后，我们需要构造一个创世块（没有前区块的第一个区块），并且给它加上一个工作量证明。
每个区块都需要经过工作量证明，俗称挖矿，稍后会继续讲解。

为了构造创世块，我们还需要完善new_block(), new_transaction() 和hash() 方法：


```
class Blockchain(object):
    def __init__(self):
        self.chain = []
        self.current_transactions = []
        self.new_block(previous_hash=1, proof=100)
        self.nodes = set()
    def new_block(self,proof,previous_hash= None):
        block = {
            'index': len(self.chain) + 1,
            'timestamp': time(),
            'transactions': self.current_transactions,
            'proof':proof ,
            'previous_hash': previous_hash or self.hash(self.chain[-1]),
        }
        self.current_transactions = []
        self.chain.append(block)
        return block

    def new_transaction(self,sender,recipient,amount):
        self.current_transactions.append({
            'sender': sender,
            'recipient': recipient,
            'amount': amount,
        })
        return self.last_block['index']+1

    @staticmethod
    def hash(block):
        block_string = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()
```


通过上面的代码和注释可以对区块链有直观的了解，接下来我们看看区块是怎么挖出来的。

## **理解工作量证明**

新的区块依赖工作量证明算法（PoW）来构造。PoW的目标是找出一个符合特定条件的数字，**这个数字很难计算出来，但容易验证**。这就是工作量证明的核心思想。

为了方便理解，举个例子：

假设一个整数 x 乘以另一个整数 y 的积的 Hash 值必须以 0 结尾，即 hash(x * y) = ac23dc…0。设变量 x = 5，求 y 的值？

用Python实现如下：


```
from hashlib import sha256
x = 5
y = 0
while sha256(str(x*y).encode()).hexdigest()[:4] != "0000":
      y += 1
      print(y,sha256(str(x*y).encode()).hexdigest()[:4])
print(y)
```


在比特币中，使用称为[Hashcash](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Hashcash)的工作量证明算法，它和上面的问题很类似。矿工们为了争夺创建区块的权利而争相计算结果。通常，计算难度与目标字符串需要满足的特定字符的数量成正比，矿工算出结果后，会获得比特币奖励。
当然，在网络上非常容易验证这个结果。

## **实现工作量证明**

让我们来实现一个相似PoW算法，规则是：寻找一个数 p，使得它与前一个区块的 proof 拼接成的字符串的 Hash 值以 4 个零开头。


```
import hashlib
import json

from time import time
from uuid import uuid4


class Blockchain(object):
    ...
    def last_block(self):
        return self.chain[-1]

    def proof_of_work(self, last_proof):
        proof = 0
        while self.valid_proof(last_proof, proof) is False:
            proof += 1

        return proof

    @staticmethod
    def valid_proof(last_proof, proof):
        guess = str(last_proof*proof).encode()
        guess_hash = hashlib.sha256(guess).hexdigest()
        return guess_hash[:5] == "00000"
```


衡量算法复杂度的办法是修改零开头的个数。使用4个来用于演示，你会发现多一个零都会大大增加计算出结果所需的时间。

现在Blockchain类基本已经完成了，接下来使用HTTP requests来进行交互。

## **Blockchain作为API接口**

我们将使用Python django框架，这是一个轻量Web应用框架，它方便将网络请求映射到 Python函数，现在我们来让来试一下：

我们将创建三个接口：
- /transactions/new 创建一个交易并添加到区块
- /mine 告诉服务器去挖掘新的区块
- /chain 返回整个区块链

## **创建节点**

我们的“django web服务器”将扮演区块链网络中的一个节点。我们先添加一些框架代码：


```
node_identifier = str(uuid4()).replace('-', '')

# Instantiate the Blockchain
blockchain = Blockchain()
def mine(request):
    last_block = blockchain.last_block
    last_proof = last_block['proof']
    proof = blockchain.proof_of_work(last_proof)
    print(proof)
    blockchain.new_transaction(
         sender="0",
         recipient=node_identifier,
         amount=1,
     )

     # Forge the new Block by adding it to the chain
    block = blockchain.new_block(proof)

    response = {
         'message': "New Block Forged",
         'index': block['index'],
         'transactions': block['transactions'],
         'proof': block['proof'],
         'previous_hash': block['previous_hash'],
    }
    print(response)
    return HttpResponse(json.dumps(response))
def new_transaction(request):
    values = json.loads(request.body.decode('utf-8'))
    required = ['sender', 'recipient', 'amount']
    if not all(k in values for k in required):
        return 'Missing values'
    index = blockchain.new_transaction(values['sender'], values['recipient'], values['amount'])
    print(index)
    response = {'message': 'Transaction will be added to Block %s'%index}
    return HttpResponse(json.dumps(response))
def full_chain(request):
    response = {
        'chain': blockchain.chain,
        'length': len(blockchain.chain),
    }
    return HttpResponse(json.dumps(response))
```


添加url路由节点：运行服务


```
from demo import views
urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^mine', views.mine),
    url(r'^transactions/new/', views.new_transaction),
    url(r'^chain/', views.full_chain),
    url(r'^register', views.register_nodes),
    url(r'^resolve', views.consensus),
]
```


运行服务

`python manage.py runserver 127.0.0.1:8000`

## **发送交易**

发送到节点的交易数据，结构如下：


```
{
 "sender": "my address",
 "recipient": "someone else's address",
 "amount": 5
}
```


向服务添加一个交易
![](https://pic3.zhimg.com/v2-e9e9142125463b3c8d3cd2f3ad8036ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='868' height='435'></svg>)
## **挖矿**

挖矿正是神奇所在，它很简单，做了一下三件事：
- 计算工作量证明PoW
- 通过新增一个交易授予矿工（自己）一个币
- 构造新区块并将其添加到链中


```
def proof_of_work(self, last_proof):
        proof = 0
        while self.valid_proof(last_proof, proof) is False:
            proof += 1

        return proof

    @staticmethod
    def valid_proof(last_proof, proof):
        guess = str(last_proof*proof).encode()
        guess_hash = hashlib.sha256(guess).hexdigest()
        return guess_hash[:5] == "00000"
```


注意交易的接收者是我们自己的服务器节点，我们做的大部分工作都只是围绕Blockchain类方法进行交互。到此，我们的区块链就算完成了，我们来实际运行下

## **运行区块链**

使用Postman 去和API进行交互

让我们通过请求 [http://127.0.0.1:8000/mine](https://link.zhihu.com/?target=http%3A//localhost%3A5000/mine) 来进行挖矿
![](https://pic3.zhimg.com/v2-fac2391e379194bbc3932e43ba84f5da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='879' height='473'></svg>)



在挖了两次矿之后，就有3个块了，通过请求 [http://localhost:8000/chain](https://link.zhihu.com/?target=http%3A//localhost%3A5000/chain) 可以得到所有的块信息。


```
{
    "chain": [
        {
            "transactions": [],
            "proof": 100,
            "timestamp": 1520314374.7261052,
            "index": 1,
            "previous_hash": 1
        },
        {
            "transactions": [
                {
                    "sender": "0",
                    "recipient": "27d4aae55b2848dcae52bc722d86e0c3",
                    "amount": 1
                }
            ],
            "proof": 1771087,
            "timestamp": 1520314389.5019505,
            "index": 2,
            "previous_hash": "32fa73f48240160257e95fdf8422c6df734b5d7e8ceb69a41a5578643c1d36fb"
        },
        {
            "transactions": [
                {
                    "sender": "d4ee26eee15148ee92c6cd394edd9705",
                    "recipient": "5",
                    "amount": 500
                },
                {
                    "sender": "0",
                    "recipient": "27d4aae55b2848dcae52bc722d86e0c3",
                    "amount": 1
                }
            ],
            "proof": 100,
            "timestamp": 1520314592.4745598,
            "index": 3,
            "previous_hash": "e6b1be488e0ed20fe3ec51135e5fafb4dfffaa28a190967106a5dd3e89e4b3aa"
        }
    ],
    "length": 3
}
```


## **一致性（共识）**

我们已经有了一个基本的区块链可以接受交易和挖矿。但是区块链系统应该是分布式的。既然是分布式的，那么我们究竟拿什么保证所有节点有同样的链呢？这就是一致性问题，我们要想在网络上有多个节点，就必须实现一个一致性的算法。

## **注册节点**

在实现一致性算法之前，我们需要找到一种方式让一个节点知道它相邻的节点。每个节点都需要保存一份包含网络中其它节点的记录。因此让我们新增几个接口：
- /register 接收URL形式的新节点列表
- /resolve 执行一致性算法，解决任何冲突，确保节点拥有正确的链

我们修改下Blockchain的init函数并提供一个注册节点方法：



```
from urllib.parse import urlparse
...


class Blockchain(object):
    def __init__(self):
        ...
        self.nodes = set()
        ...
    def register_node(self, address):
        parsed_url = urlparse(address)
        self.nodes.add(parsed_url.netloc)
```


我们用 set 来储存节点，这是一种避免重复添加节点的简单方法。

## **实现共识算法**

前面提到，冲突是指不同的节点拥有不同的链，为了解决这个问题，规定最长的、有效的链才是最终的链，换句话说，网络中有效最长链才是实际的链。

我们使用一下的算法，来达到网络中的共识



```
class Blockchain(object):
    def __init__(self):
        ...
   
    def valid_chain(self, chain):
        last_block = chain[0]
        current_index = 1
        while current_index < len(chain):
            block = chain[current_index]
            if block['previous_hash'] != self.hash(last_block):
                return False

            # Check that the Proof of Work is correct
            if not self.valid_proof(last_block['proof'], block['proof']):
                return False

            last_block = block
            current_index += 1

        return True

    def resolve_conflicts(self):
        neighbours = self.nodes
        new_chain = None

        max_length = len(self.chain)

        for node in neighbours:
            response = requests.get('http://%s/chain' %node)

            if response.status_code == 200:
                length = json.loads(response)['length']
                chain = json.loads(response)['chain']

                # Check if the length is longer and the chain is valid
                if length > max_length and self.valid_chain(chain):
                    max_length = length
                    new_chain = chain

        # Replace our chain if we discovered a new, valid chain longer than ours
        if new_chain:
            self.chain = new_chain
            return True

        return False
```





第一个方法 valid_chain() 用来检查是否是有效链，遍历每个块验证hash和proof.

第2个方法 resolve_conflicts() 用来解决冲突，遍历所有的邻居节点，并用上一个方法检查链的有效性， **如果发现有效更长链，就替换掉自己的链**

在url中添加两个路由，一个用来注册节点，一个用来解决冲突。



```
from demo import views
urlpatterns = [
    url(r'^register', views.register_nodes),
    url(r'^resolve', views.consensus),
]
```


你可以在不同的机器运行节点，或在一台机机开启不同的网络端口来模拟多节点的网络，这里在同一台机器开启不同的端口演示，在不同的终端运行一下命令，就启动了两个节点：[http://127.0.0.1:8000](https://link.zhihu.com/?target=http%3A//127.0.0.1%3A8000/)和 [http://127.0.0.1:8100](https://link.zhihu.com/?target=http%3A//127.0.0.1%3A8100/)
![](https://pic1.zhimg.com/v2-b3bfa2d76bd35f35a8cff6105c2f9d84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='882' height='501'></svg>)
然后在节点8100节点上挖两个块，确保是更长的链，然后在节点8000节点上访问接口/resolve ,这时节点8100的链会通过共识算法被节点8000节点的链取代。
![](https://pic3.zhimg.com/v2-8bd8597d4ea14f389899dadc3bb2bb4a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='873' height='572'></svg>)
**此文提供的python代码可本地运行，并将整个项目进行打包上传[百度云网盘](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1bZSSyQJYkcvWa0J1Hmm0OA)，关注并私信我，获取网盘密码。**

**地址：**[https://pan.baidu.com/s/1bZSSyQJYkcvWa0J1Hmm0OA](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1bZSSyQJYkcvWa0J1Hmm0OA)

**广告时间：最后给自己代个盐~~欢迎大家有空时翻下我牌子([知乎号：布道](https://www.zhihu.com/people/dockersky/columns) )，看看[“开发运维”](https://zhuanlan.zhihu.com/idevops)专栏的文章，希望多些关注和点赞是给作者最好的鼓励 ！**


