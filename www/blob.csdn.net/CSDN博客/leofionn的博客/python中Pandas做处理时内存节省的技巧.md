
# python中Pandas做处理时内存节省的技巧 - leofionn的博客 - CSDN博客


2018年12月25日 08:59:38[leofionn](https://me.csdn.net/qq_36142114)阅读数：129


1.查看dataframe占用空间
例如，我们读取之前的所有行情和因子数据：
data = pd.read_csv('total_data.csv', index_col=0)
data.info(memory_usage='deep')
首先，我们读取total_data.csv这个数据，并制定第一列是index，然后，我们获取一下这个dataframe这个对象在内存中的情况。
<class 'pandas.core.frame.DataFrame'>
Int64Index: 91427 entries, 6095 to 91426
Data columns (total 13 columns):
date            91427 non-null object
open            91427 non-null float64
close           91427 non-null float64
high            91427 non-null float64
low             91427 non-null float64
volume          91427 non-null float64
openinterest    91427 non-null int64
raw_factor      91427 non-null float64
sec_id          91427 non-null object
win_secore      91427 non-null float64
z-score         91427 non-null float64
group           91427 non-null int64
neuted_score    78408 non-null float64
dtypes: float64(9), int64(2), object(2)
memory usage: 14.4 MB
我们可以看到，这个dataframe每一列的数据类型，以及，一共占用的内存空间：14.4M。看起来不大的样子。别忘了，我们这里是令标的池为sz50，同时只有一个因子。某种意义上，完全没有意义，笔者只是为了展示多因子模型的整个流程和框架罢了，对于50个股票的标的池，做多因子策略，几乎是没有任何意义的。我们想一想，如果范围是全市场，3000个股票，那么大概就是864M，而且这仅仅是一个因子。如果我们需要把100个因子的内容load到内存中，虽然有时候并不需要这样，那么就是8G，好吧，内存就不够了。
2.时间的处理
大家把数据本地化为csv，然后读取的时候，尽可能养成一个习惯，就是把时间那一列变成timestamp格式。这样有两个好处，一个是存储空间会减小，同时检索的速度会快，而且会减少很多不必要的错误。
csv读取进来的时候，默认时间是str格式，这一格式在pandas中被存储为object格式，还是很占内存的。
data['date'] = pd.to_datetime(data['date'])
然后我们在info一下，就是下面这样了：
<class 'pandas.core.frame.DataFrame'>
Int64Index: 91427 entries, 6095 to 91426
Data columns (total 13 columns):
date            91427 non-null datetime64[ns]
open            91427 non-null float64
close           91427 non-null float64
high            91427 non-null float64
low             91427 non-null float64
volume          91427 non-null float64
openinterest    91427 non-null int64
raw_factor      91427 non-null float64
sec_id          91427 non-null object
win_secore      91427 non-null float64
z-score         91427 non-null float64
group           91427 non-null int64
neuted_score    78408 non-null float64
dtypes: datetime64[ns](1), float64(9), int64(2), object(1)
memory usage: 12.0 MB
我们看到，内存占用少了2M+，这是个不错的现象。
3.修改数字
其实，pandas在读取csv的时候，可以定义读取每一列的类型的，我们看到上面默认是float64，对于整数，默认是int64，知道一点计算机知识的都明白，很多时候我们是不需要这么float64这么大的存储范围的。
假设，我们一开始就定义好浮点数列的数据类型为float16
data = pd.read_csv('total_data.csv', index_col=0, dtype={'open': 'float16', 'close': 'float16', 'high': 'float16', 'low': 'float16','volume': 'float32',
'openinterest': 'int','raw_factor':  'float16','win_secore':   'float16', 'neuted_score':   'float16','z-score': 'float16'
})
然后info一下，
<class 'pandas.core.frame.DataFrame'>
Int64Index: 91427 entries, 6095 to 91426
Data columns (total 13 columns):
date            91427 non-null datetime64[ns]
open            91427 non-null float16
close           91427 non-null float16
high            91427 non-null float16
low             91427 non-null float16
volume          91427 non-null float32
openinterest    91427 non-null int32
raw_factor      91427 non-null float16
sec_id          91427 non-null object
win_secore      91427 non-null float16
z-score         91427 non-null float16
group           91427 non-null int64
neuted_score    78408 non-null float16
dtypes: datetime64[ns](1), float16(8), float32(1), int32(1), int64(1), object(1)
memory usage: 7.1 MB
现在，内存占用只有7.1M了，连原来的一半都不到。
4.catrgory类
然后是最后一个大杀器，就是当某一列中，有很多重复元素的时候，其实必然是存在冗余的，比如，我们的dataframe中股票代码，sec_id和行业类别，group这两列，肯定有很多重复的，那么，我们就可以把这两列设置为category类，这一类本质上就是一个字典的映射。
data['group'] = data['group'].astype('category')
data['sec_id'] = data['sec_id'].astype('category')
然后，我们看一下效果：
<class'pandas.core.frame.DataFrame'>
Int64Index: 91427entries, 6095 to 91426
Data columns(total 13 columns):
date            91427 non-null datetime64[ns]
open            91427 non-null float16
close           91427 non-null float16
high            91427 non-null float16
low             91427 non-null float16
volume          91427 non-null float32
openinterest    91427 non-null int32
raw_factor      91427 non-null float16
sec_id          91427 non-null category
win_secore      91427 non-null float16
z-score         91427 non-null float16
group           91427 non-null category
neuted_score    78408 non-null float16
dtypes:category(2), datetime64[ns](1), float16(8), float32(1), int32(1)
memory usage: 3.7MB
居然只占用了3.7M！连原来的个位数都不到。
5.注意：
当然，笔者这里要说明的是，如果你把上面这样做过处理的dataframe直接放进alphalens或者用别的一些数学性很强的包进行运算，比如用statismodules进行回归，就会有错误。所以，通常的使用场景就是仅仅需要对数据做简单的处理，但是数据很分散，需要多次读取，但是精度又不高，这个时候，如果不做上面这样的处理，多进程由于受到内存限制，可能没有任何速度的提升，但是经过上面的处理，单个处理的内存和cpu都会降低，这个时候就可以多进程并发，加快速度了。
---------------------
原文：https://blog.csdn.net/qtlyx/article/details/78003109


