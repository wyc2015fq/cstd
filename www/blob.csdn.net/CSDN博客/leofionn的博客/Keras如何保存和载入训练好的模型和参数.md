
# Keras如何保存和载入训练好的模型和参数 - leofionn的博客 - CSDN博客


2019年02月10日 20:37:03[leofionn](https://me.csdn.net/qq_36142114)阅读数：180


1，保存模型：
my_model = create_model_function( ...... )
my_model.compile( ...... )
my_model.fit( ...... )
model_name . save( filepath, overwrite: bool=True, include_optimizer: bool=True )
filepath：保存的路径
overwrite：如果存在源文件，是否覆盖
include_optimizer：是否保存优化器状态
ex : mymodel.save(filepath="p402/my_model.h5", includeoptimizer=False)
2, 载入模型:
my_model = keras . models . load_model( filepath )
载入后可以继续训练：
my_model . fit( X_train_2，Y_train_2 )
也可以直接评估：
preds = my_model . evaluate( X_test, Y_test )
print ( "Loss = " + str( preds[0] ) )
print ( "Test Accuracy = " + str( preds[1] ) )
3, 如果仅保存模型的结构，而不包含其权重或配置信息，可以使用：
\# save as JSON
json_string = model.to_json()
\# save as YAML
yaml_string = model.to_yaml()
从保存好的json文件或yaml文件中载入模型：
\# model reconstruction from JSON:
from keras.models import model_from_json
model = model_from_json(json_string)
\# model reconstruction from YAML
model = model_from_yaml(yaml_string)
4，如果需要保存模型的权重，可通过下面的代码利用HDF5进行保存：
model.save_weights('my_model_weights.h5')
若在代码中初始化一个完全相同的模型，请使用：
model.load_weights('my_model_weights.h5')
5，若要加载权重到不同的网络结构（有些层一样）中，例如fine-tune或transfer-learning，可通过层名字来加载模型：
model.load_weights('my_model_weights.h5', by_name=True)
如：
"""
假如原模型为：
model = Sequential()
model.add(Dense(2, input_dim=3, name="dense_1"))
model.add(Dense(3, name="dense_2"))
...
model.save_weights(fname)
"""
\# new model
model = Sequential()
model.add(Dense(2, input_dim=3, name="dense_1"))  \# will be loaded
model.add(Dense(10, name="new_dense"))  \# will not be loaded
\# load weights from first model; will only affect the first layer, dense_1.
model.load_weights(fname, by_name=True)

