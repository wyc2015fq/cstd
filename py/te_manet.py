import numpy as np
import matplotlib.pyplot as plt

import mxnet as mx

# 定义输入数据
X_data = np.linspace(-1, 1, 100)
noise = np.random.normal(0, 0.5, 100)
y_data = 5 * X_data + noise

# Plot 显示
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ax.scatter(X_data, y_data)

# 定义mxnet变量
X = mx.symbol.Variable('data')
Y = mx.symbol.Variable('softmax_label')

# 定义网络
Y_ = mx.symbol.FullyConnected(data=X, num_hidden=1, name='pre')
loss = mx.symbol.LinearRegressionOutput(data=Y_, label=Y, name='loss')

# 定义模型
model = mx.model.FeedForward(
            ctx=mx.cpu(),
            symbol=loss,
            num_epoch=100,
            learning_rate=0.001,
            numpy_batch_size=1
        )

# 训练模型
model.fit(X=X_data, y=y_data)

# 预测
prediction = model.predict(X_data)
print(prediction)
lines = ax.plot(X_data, prediction, 'r-', lw=5)
plt.show()
