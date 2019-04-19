# keras 保存模型和加载模型 - 数据之美的博客 - CSDN博客
2018年12月03日 12:12:49[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：93
```python
import numpy as np
np.random.seed(1337)  # for reproducibility
 
from keras.models import Sequential
from keras.layers import Dense
from keras.models import load_model
 
# create some data
X = np.linspace(-1, 1, 200)
np.random.shuffle(X)    # randomize the data
Y = 0.5 * X + 2 + np.random.normal(0, 0.05, (200, ))
X_train, Y_train = X[:160], Y[:160]     # first 160 data points
X_test, Y_test = X[160:], Y[160:]       # last 40 data points
model = Sequential()
model.add(Dense(output_dim=1, input_dim=1))
model.compile(loss='mse', optimizer='sgd')
for step in range(301):
    cost = model.train_on_batch(X_train, Y_train)
 
# save
print('test before save: ', model.predict(X_test[0:2]))
model.save('my_model.h5')   # HDF5 file, you have to pip3 install h5py if don't have it
del model  # deletes the existing model
 
# load
model = load_model('my_model.h5')
print('test after load: ', model.predict(X_test[0:2]))
```
说明：
1、保存模型的api:
`model.save('my_model.h5')`
2、加载模型
`model = load_model('my_model.h5')`
