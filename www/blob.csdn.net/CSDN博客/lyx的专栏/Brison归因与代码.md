# Brison归因与代码 - lyx的专栏 - CSDN博客





2019年03月05日 22:27:39[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：438








      不管是做FOF也好，仅仅想单纯归因也好，Brison是一种比较常见，也算是通用的归因方法，其一般用于权益类或者大类资产配置类基金。

      其实Brison归因的逻辑很简单，假设有一个基准，基准在各类资产的配置上的权重分别是wbi（weight of benchmark of asset i），这些资产的收益率分别是rbi（return of benchmark of asset i），同样的，我们有一个需要归因的基金产品，也就是我们自己的组合，在各类资产上的权重是wpi（weight of portfolio of asset i），收益率分别是rpi（return of portfolio of asset i）。

      首先这里的资产可以是股票、现金、债券、商品。。。。这样的一系列asset类别，也可以是行业划分，比如银行、机械、轻工。。。。。这样的一些列asset。那么，一个好的优秀的基金，肯定是把更多的权重配到更高收益率的资产上，这一个过程体现的是资产配置的能力；如果是针对行业，那么就是行业选择的能力。

      此外，除了配比到收益率高的资产上外，我们还希望，同样的资产，比如，股票，我们的组合中股票资产的单位收益率高于基准中股票的单位收益率，也就是说，我们比基准挑选了更加好的个股。对于行业也是如此，我们挑选了行业中更加优秀的个股。这一部分就是择股能力。

      所谓的Brison归因就是把一个组合的超额收益分解为资产配置的能力、个券选择的能力、以及交互收益。所谓的交互收益就是一个差值，超额和配置能力、选择能力直接的差值，其含义其实就是两个能力共同作用下的一种效应。

基于上述的思想，所以资产配置能力，就可以被写作rbi*(wpi-wbi)，理解一下就是我们先不考虑资产的收益率的多少，先全部用基准的，然后看你的组合超配了多少，如果rbi越大，也就是资产自身收益率高，然后又超配了很多，也就是wpi-wbi大，那么资产配置能力就大。最后对所有的资产，也就是i，做一个加总，就得到了整个基金的配置能力了。

同样的，择券能力可以写作(rpi - rbi)*wbi，理解起来和上面一下，就是先让资产配置权重和基准一样，然后看资产的超额收益。

      最后，放上一段代码吧，刚写的，还热乎呢。代码很简单，就是为了记录一下，方便后面要使用的时候有地方找。

```python
import pandas as pd

def brison_attribution(port_df):
    '''
    asset,return_bench_i,return_portf_i,weight_bench_i,weight_portf_i
    cash,0,0,0,0.05
    equity,0.2,0.3,0.6,0.7
    bond,0.01,0.01,0.3,0.1
    commodity,0.12,0.1,0.1,0.15
    :param port_df:
    :return:
    '''
    port_df['bench_return'] = port_df['return_bench_i'] * port_df['weight_bench_i']
    port_df['port_return'] = port_df['return_portf_i'] * port_df['weight_portf_i']

    port_df['allocation'] = port_df['weight_bench_i'] * (port_df['return_portf_i'] - port_df['return_bench_i'])# q df['rb_wp'] - df['bench_return']
    port_df['selection'] = port_df['return_bench_i'] * (port_df['weight_portf_i'] - port_df['weight_bench_i'])
    port_df['interactive'] = port_df['port_return'] - port_df['bench_return'] - port_df['allocation'] - port_df['selection']
    port_df['excess_return'] = port_df['port_return'] - port_df['bench_return']
    result_df = port_df[['asset', 'allocation', 'selection', 'interactive', 'excess_return']].set_index('asset')
    result_df.loc['summary',] = result_df.sum()
    return result_df
```





