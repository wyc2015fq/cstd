# numpy的random模块 - lyx的专栏 - CSDN博客





2016年02月24日 20:07:26[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：5084








## 

## 

转自http://www.mamicode.com/info-detail-507676.html

## 排列
|[shuffle](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.shuffle.html#numpy.random.shuffle)(x)|现场修改序列，改变自身内容。（类似洗牌，打乱顺序）```>>> arr = np.arange(10)>>> np.random.shuffle(arr)>>> arr[1 7 5 2 9 4 3 6 0 8]```This function only shuffles the array along the first index of a multi-dimensional array:```>>> arr = np.arange(9).reshape((3, 3))>>> np.random.shuffle(arr)>>> arrarray([[3, 4, 5],       [6, 7, 8],       [0, 1, 2]])```|
|----|----|
|[permutation](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.permutation.html#numpy.random.permutation)(x)|返回一个随机排列```>>> np.random.permutation(10)array([1, 7, 4, 3, 0, 9, 2, 5, 8, 6])``````>>> np.random.permutation([1, 4, 9, 12, 15])array([15,  1,  9,  4, 12])``````>>> arr = np.arange(9).reshape((3, 3))>>> np.random.permutation(arr)array([[6, 7, 8],       [0, 1, 2],       [3, 4, 5]])```|


## 

## 




## 

## 简单的随机数据
|[rand](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.rand.html#numpy.random.rand)(d0, d1, ..., dn)|随机值```>>> np.random.rand(3,2)array([[ 0.14022471,  0.96360618],  #random       [ 0.37601032,  0.25528411],  #random       [ 0.49313049,  0.94909878]]) #random```|
|----|----|
|[randn](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.randn.html#numpy.random.randn)(d0, d1, ..., dn)|返回一个样本，具有标准正态分布。NotesFor random samples from ![技术分享](http://docs.scipy.org/doc/numpy/_images/math/93af1f49bf6bbf05f549f49609becdb5f7039538.png), use:```sigma * np.random.randn(...) + mu```Examples```>>> np.random.randn()2.1923875335537315 #random```Two-by-four array of samples from N(3, 6.25):```>>> 2.5 * np.random.randn(2, 4) + 3array([[-4.49401501,  4.00950034, -1.81814867,  7.29718677],  #random       [ 0.39924804,  4.68456316,  4.99394529,  4.84057254]]) #random```|
|[randint](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.randint.html#numpy.random.randint)(low[, high, size])|返回随机的整数，位于半开区间 [low, high)。```>>> np.random.randint(2, size=10)array([1, 0, 0, 0, 1, 1, 0, 0, 1, 0])>>> np.random.randint(1, size=10)array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])```Generate a 2 x 4 array of ints between 0 and 4, inclusive:```>>> np.random.randint(5, size=(2, 4))array([[4, 0, 2, 1],       [3, 2, 2, 0]])```|
|[random_integers](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.random_integers.html#numpy.random.random_integers)(low[, high, size])|返回随机的整数，位于闭区间 [low, high]。NotesTo sample from N evenly spaced floating-point numbers between a and b, use:```a + (b - a) * (np.random.random_integers(N) - 1) / (N - 1.)```Examples```>>> np.random.random_integers(5)4>>> type(np.random.random_integers(5))<type ‘int‘>>>> np.random.random_integers(5, size=(3.,2.))array([[5, 4],       [3, 3],       [4, 5]])```Choose five random numbers from the set of five evenly-spaced numbers between 0 and 2.5, inclusive (*i.e.*, from the set![技术分享](http://docs.scipy.org/doc/numpy/_images/math/260812782a8a4f35a929d637a38520175045eaa2.png)):```>>> 2.5 * (np.random.random_integers(5, size=(5,)) - 1) / 4.array([ 0.625,  1.25 ,  0.625,  0.625,  2.5  ])```Roll two six sided dice 1000 times and sum the results:```>>> d1 = np.random.random_integers(1, 6, 1000)>>> d2 = np.random.random_integers(1, 6, 1000)>>> dsums = d1 + d2```Display results as a histogram:```>>> import matplotlib.pyplot as plt>>> count, bins, ignored = plt.hist(dsums, 11, normed=True)>>> plt.show()```|
|[random_sample](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.random_sample.html#numpy.random.random_sample)([size])|返回随机的浮点数，在半开区间 [0.0, 1.0)。To sample ![技术分享](http://docs.scipy.org/doc/numpy/_images/math/e9a05a99f961e8f094b3869fcddd366857d7b0d9.png) multiply the output of[random_sample](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.random_sample.html#numpy.random.random_sample) by*(b-a)* and add *a*:```(b - a) * random_sample() + a```Examples```>>> np.random.random_sample()0.47108547995356098>>> type(np.random.random_sample())<type ‘float‘>>>> np.random.random_sample((5,))array([ 0.30220482,  0.86820401,  0.1654503 ,  0.11659149,  0.54323428])```Three-by-two array of random numbers from [-5, 0):```>>> 5 * np.random.random_sample((3, 2)) - 5array([[-3.99149989, -0.52338984],       [-2.99091858, -0.79479508],       [-1.23204345, -1.75224494]])```|
|[random](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.random.html#numpy.random.random)([size])|返回随机的浮点数，在半开区间 [0.0, 1.0)。（官网例子与random_sample完全一样）|
|[ranf](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.ranf.html#numpy.random.ranf)([size])|返回随机的浮点数，在半开区间 [0.0, 1.0)。（官网例子与random_sample完全一样）|
|[sample](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.sample.html#numpy.random.sample)([size])|返回随机的浮点数，在半开区间 [0.0, 1.0)。（官网例子与random_sample完全一样）|
|[choice](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.choice.html#numpy.random.choice)(a[, size, replace, p])|生成一个随机样本，从一个给定的一维数组ExamplesGenerate a uniform random sample from np.arange(5) of size 3:```>>> np.random.choice(5, 3)array([0, 3, 4])>>> #This is equivalent to np.random.randint(0,5,3)```Generate a non-uniform random sample from np.arange(5) of size 3:```>>> np.random.choice(5, 3, p=[0.1, 0, 0.3, 0.6, 0])array([3, 3, 0])```Generate a uniform random sample from np.arange(5) of size 3 without replacement:```>>> np.random.choice(5, 3, replace=False)array([3,1,0])>>> #This is equivalent to np.random.permutation(np.arange(5))[:3]```Generate a non-uniform random sample from np.arange(5) of size 3 without replacement:```>>> np.random.choice(5, 3, replace=False, p=[0.1, 0, 0.3, 0.6, 0])array([2, 3, 0])```Any of the above can be repeated with an arbitrary array-like instead of just integers. For instance:```>>> aa_milne_arr = [‘pooh‘, ‘rabbit‘, ‘piglet‘, ‘Christopher‘]>>> np.random.choice(aa_milne_arr, 5, p=[0.5, 0.1, 0.1, 0.3])array([‘pooh‘, ‘pooh‘, ‘pooh‘, ‘Christopher‘, ‘piglet‘],      dtype=‘|S11‘)```|
|[bytes](http://docs.scipy.org/doc/numpy/reference/generated/numpy.random.bytes.html#numpy.random.bytes)(length)|返回随机字节。```>>> np.random.bytes(10) ‘ eh\x85\x022SZ\xbf\xa4‘ #random```|



