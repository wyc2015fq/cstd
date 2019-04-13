
# 区块链鼻祖比特币之10：merkle tree与spv节点 - 尹成的技术博客 - CSDN博客

置顶2018年04月27日 15:25:41[尹成](https://me.csdn.net/yincheng01)阅读数：330标签：[比特币](https://so.csdn.net/so/search/s.do?q=比特币&t=blog)[区块链](https://so.csdn.net/so/search/s.do?q=区块链&t=blog)[以太坊](https://so.csdn.net/so/search/s.do?q=以太坊&t=blog)[](https://so.csdn.net/so/search/s.do?q=区块链&t=blog)[](https://so.csdn.net/so/search/s.do?q=比特币&t=blog)个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)
[](https://so.csdn.net/so/search/s.do?q=比特币&t=blog)


区块链的每一个区块都包含了该区块产生期间的所有交易，并用Merkle树的形式表示。Merkle树是一种Hash二叉树，用作快速归纳和校验大规模数据完整性的数据结构。
在比特币网络中，Merkle树被用来归纳一个区块的所有交易，同时生成整个交易集合的数字指纹，且提供了一种校验区块是否存在某交易的高效途径。生成一棵完整的Merkle树需要递归地对哈希节点对进行哈希，并将新生成的哈希节点插入到Merkle树中，直到只剩一个哈希节点，该节点就是Merkle树的根。在比特币的Merkle树中两次使用到了SHA256算法，因此其加密哈希算法也被称为Double-SHA256。
当N个数据元素经过加密后插入Merkle树，至多计算2*log2(N)次就能检查出任意某数据元素是否在该树中，这使得该数据结构非常高效；但同时无法从Merkle树内找到对应的交易，这是区块链单向验证性的特点。Merkle树是自底向上构建的，举例说明：同一时间发生A、B、C、D四笔交易，起始所有交易都存于基础节点，分别进行Hash，以交易A的Hash过程为例，得到HA= SHA256(SHA256(交易A))，同样得到HB、HC、HD；然后创建二层节点HAB=SHA256(SHA256(HA + HB))，同样得到HCD；继续操作直到生成顶端唯一的节点——HABCD。如下图所示：
![](https://img-blog.csdn.net/20180427150132338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为Merkle树是二叉树，所以每一层需要偶数个点，如果节点数为奇数，系统将复制一份数值使得节点数变成偶数。这种偶数个分层节点的树也被称为平衡二叉树。如下图所示，C点就被复制了一份。
![](https://img-blog.csdn.net/20180427150655863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
为四个交易构造Merkle树的方法同样适用于为任意交易数量构造Merkle树。在比特币中，在单个区块中有成百上千的交易是非常普遍的，这些交易都会采用同样的方法归纳起来，产生一个仅仅32字节的数据作为Merkle根。下图出现了多个节点，其运算过程与四个节点是一致的，其实无论多少个节点都可以通过Merkle树归纳为32个字节。
![](https://img-blog.csdn.net/20180427150727268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了证明区块链内存在某一个特定的交易，一个节点只需要计算log2(N)个32字节的哈希值，形成一条从特定交易到树根的认证路径或者Merkle路径即可。随着交易数量的急剧增加，这样的计算量就显得异常重要，因为相对于交易数量的增长，以基底为2的交易数量的对数的增长会缓慢许多。这使得比特币节点能够高效地产生一条10或者12个哈希值（320-384字节）的路径，来证明了在一个巨量字节大小的区块中某笔交易的存在。
在图5中，一个节点能够通过生成一条仅有4个32字节哈希值长度（总128字节）的Merkle路径，来证明区块中存在一笔交易K。该路径有4个哈希值（在图5中由蓝色标注）HL、HIJ、HMNOP和HABCDEFGH。由这4个哈希值产生的认证路径，再通过计算另外四对哈希值HKL、HIJKL、HIJKLMNOP和Merkle树根（在图中由虚线标注），任何节点都能证明HK（在图中由绿色标注）包含在Merkle根中。
![](https://img-blog.csdn.net/20180427150800858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用列表表示交易数量对路径大小的影响，如表1所示，当区块大小由16笔交易（4KB）急剧增加至65535笔交易（16MB）时，为证明交易存在的Merkle路径长度增长极其缓慢，仅仅从128字节到512字节。有了Merkle树，一个节点能够仅下载区块头（80字节/区块），然后从一个节点回溯就能认证一笔交易的存在，而不需要存储或者传输区块链中大多数内容。这被称作简单支付验证（SPV）节点，它不需要下载整个区块，而通过Merkle路径去验证交易的存在。



![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052  备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


