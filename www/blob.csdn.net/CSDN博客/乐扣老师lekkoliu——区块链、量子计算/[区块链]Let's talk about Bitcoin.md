
# [区块链]Let's talk about Bitcoin - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2018年04月17日 17:38:18[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：449



## 1.Who is Satoshi Nakomoto
In October 2008, Satoshi Nakamoto published the paper named”Bitcoin: Peer-to-peer Electronic Cash System”.As of May 2017, Nakomoto has approximately 1 million bitcoins. As of December 2017, the estimated value is $15 billion.
## 2.Paper of Bitcoin
A purely peer-to-peer version of electronic cash would allow online payments to be sent directly from one party to another without going through a financial institution. The network timestamps transactions by hashing them into an ongoing chain of hash-based proof-of-work, forming a record that cannot be changed without redoing the proof-of-work.Digital signatures provide part of the solution, but the main benefits are lost if a trusted third party is still required to prevent double-spending.So we  provide a solution to the double-spending problem using a peer-to-peer network.
## 3.Bitcoin Encryption Mechanism
Some existing encryption mechanisms:
Public and private key multiple encryption.
And dual SHA-256 encryption.
![这里写图片描述](https://img-blog.csdn.net/20180417152211565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180417152211565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Traditional signatures and verification
![这里写图片描述](https://img-blog.csdn.net/20180417152355147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4. Detail about Transaction in Blockchain
We define an electronic coin as a chain of digital signatures. Each owner transfers the coin to the next by digitally signing a hash of the previous transaction and the public key of the next owner and adding these to the end of the coin. A payee can verify the signatures to verify the chain of ownership.Using its characteristics, we can form a chain of transfer records that can be traced back to the earliest transactions.
![这里写图片描述](https://img-blog.csdn.net/2018041715252024?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5.The double flower problem that had to be solved
The problem is that the payee cannot verify that one of the owners did not spend the second coin in another place.A common solution is to introduce a trustworthy central authority or use miners to check the double expenses of each transaction.
## 6.Time recorder: Timestamp Server
A timestamp server works by taking a hash of a block of items to be timestamped and widely publishing the hash. The timestamp proves that the data must have existed at the time, obviously, in order to get into the hash. Each timestamp includes the previous timestamp in its hash, forming a chain, with each additional timestamp reinforcing the ones before it.In simple terms, blockchain is not just a value chain but also a timeline.
![这里写图片描述](https://img-blog.csdn.net/20180417152727493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 7.Bitcoin Consensus  Mechanism：PoW
Bitcoin uses the consensus mechanism PoW.
For a detailed explanation of the consensus mechanism, please refer to this article《[[区块链]共识算法（POW,POS,DPOS,PBFT）介绍和心得](https://blog.csdn.net/lsttoy/article/details/61624287)》
In fact,Trust by majority decision making.
The majority decision is represented by the longest chain, which has the greatest proof-of-work effort invested in it. If a majority of CPU power is controlled by honest nodes, the honest chain will grow the fastest and outpace any competing chains.
Protection：To modify a past block, an attacker would have to redo the proof-of-work of the block and all blocks after it and then catch up with and surpass the work of the honest nodes.
## 8.Blockchain Network and Logical Nodes Workflow
Nodes always consider the longest chain to be the correct one and will keep working on extending it. If two nodes broadcast different versions of the next block simultaneously, some nodes may receive one or the other first.
In that case, they work on the first one they received, but save the other branch in case it becomes longer. The tie will be broken when the next proof of-work is found and one branch becomes longer; the nodes that were working on the other branch will then switch to the longer one.
The following is the network propagation execution logic:
1) New transactions are broadcast to all nodes.
2) Each node collects new transactions into a block.
3) Each node works on finding a difficult proof-of-work for its block.
4) When a node finds a proof-of-work, it broadcasts the block to all nodes.
5) Nodes accept the block only if all transactions in it are valid and not already spent.
6) Nodes express their acceptance of the block by working on creating the next block in the chain, using the hash of the accepted block as the previous hash.
## 9.How to motivate miners to mine to maintain the overall blockchain network?
By convention, the first transaction in a block is a special transaction that starts a new coin owned by the creator of the block.
(50BTC @2008-2012,   25BTC@2012-2016,   12.5BTC@2016-2020)
This adds an incentive for nodes to support the network, and provides a way to initially distribute coins into circulation, since there is no central authority to issue them.The steady addition of a constant of amount of new coins is analogous to gold miners expending resources to add gold to circulation. In our case, it is CPU time and electricity that is expended.
## 10.What are the SPV often spoken by geeks? There is a winding Merkle tree
SPV is short for Simplified Payment Verification.
A user only needs to keep a copy of the block headers of the longest proof-of-work chain, which he can get by querying network nodes until he’s convinced he has the longest chain. He can’t check the transaction for himself, but by linking it to a place in the chain, he can see that a network node has accepted it, and blocks added after it further confirm the network has accepted it.
The main structure is as follows:
![这里写图片描述](https://img-blog.csdn.net/20180417153349762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
In simple words, the mechanism and structure of SPV or Merkel tree ensure that the node determines the longest chain when the network spreads, and the consensus of the entire blockchain can be maintained. (Seemingly still winding?)
## 11.How about Privacy?
for blockchain, the necessity to announce all transactions publicly precludes this method, but privacy can still be maintained by breaking the flow of information in another place: by keeping public keys anonymous.
## 12.Bitcoin Dilemma
As complexity of mining operation increases – the mining profits will drop or become negative even for large Bitcoin mining farms. Please refer 13.1.
New faster mining hardware comes out before you can barely pay off your existing mining hardware.Once Bitcoin price stops growing or drops combined with complexity increases will drive miners to switch to a different coin mining or stop operations.Gradually as miners leave the network – the security of the network will decrease making 51% attack more likely.   Alternatively ever increasing transaction costs are making Bitcoin impractical for micro-payments.
That’s why BCC want to change this situation.
## 13.Some charts
Latest Bitcoin Mining Difficulty (Difficulties Increasing)
![这里写图片描述](https://img-blog.csdn.net/20180417154222229?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Bitcoin Market Value Chart (Investment Risk, Volatility Over Stock Market)
![这里写图片描述](https://img-blog.csdn.net/20180417154159507?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

