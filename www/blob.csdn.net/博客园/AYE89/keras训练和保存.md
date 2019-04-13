
# keras训练和保存 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[keras训练和保存](https://www.cnblogs.com/eniac1946/p/8473391.html)
|Posted on|2018-02-26 15:15|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8473391)|[收藏](#)
|[https://cloud.tencent.com/developer/article/1010815](https://cloud.tencent.com/developer/article/1010815)
|8.更科学地模型训练与模型保存
|filepath =|'|model-ep{epoch:03d}-loss{loss:.3f}-val_loss{val_loss:.3f}.h5|'|checkpoint|= ModelCheckpoint(filepath, monitor=|'|val_loss|'|, verbose=1, save_best_only=True, mode=|'|min|'|)|\#|fit model|model.fit(x, y, epochs=20, verbose=2, callbacks=[checkpoint], validation_data=(x, y))
|save_best_only打开之后，会如下：
|ETA:|3s - loss: 0.5820Epoch 00017: val_loss did not improve|如果val_loss 提高了就会保存，没有提高就不会保存。
|ModelCheckpoint
|keras.callbacks.ModelCheckpoint(filepath, monitor=|'val_loss', verbose=|0, save_best_only=|False, save_weights_only=|False, mode=|'auto', period=|1)|该回调函数将在每个epoch后保存模型到|filepath
|filepath|可以是格式化的字符串，里面的占位符将会被|epoch|值和传入|on_epoch_end|的|logs|关键字所填入
|例如，|filepath|若为|weights.{epoch:02d-{val_loss:.2f}}.hdf5|，则会生成对应epoch和验证集loss的多个文件。
|参数
|filename：字符串，保存模型的路径
|monitor：需要监视的值
|verbose：信息展示模式，0或1
|save_best_only：当设置为|True|时，将只保存在验证集上性能最好的模型
|mode：‘auto’，‘min’，‘max’之一，在|save_best_only=True|时决定性能最佳模型的评判准则，例如，当监测值为|val_acc|时，模式应为|max|，当检测值为|val_loss|时，模式应为|min|。在|auto|模式下，评价准则由被监测值的名字自动推断。
|save_weights_only：若设置为True，则只保存模型权重，否则将保存整个模型（包括模型结构，配置信息等）
|period：CheckPoint之间的间隔的epoch数。

|9.如何在keras中使用tensorboard
|1|RUN = RUN + 1|if|'|RUN|'|in|locals()|else|1|\#|locals() 函数会以字典类型返回当前位置的全部局部变量。|2|3|LOG_DIR = model_save_path +|'|/training_logs/run{}|'|.format(RUN)|4|LOG_FILE_PATH = LOG_DIR +|'|/checkpoint-{epoch:02d}-{val_loss:.4f}.hdf5|'|\#|模型Log文件以及.h5模型文件存放地址|5|6|tensorboard = TensorBoard(log_dir=LOG_DIR, write_images=|True)|7|checkpoint = ModelCheckpoint(filepath=LOG_FILE_PATH, monitor=|'|val_loss|'|, verbose=1, save_best_only=|True)|8|early_stopping = EarlyStopping(monitor=|'|val_loss|'|, patience=5, verbose=1|)|9|10|history = model.fit_generator(generator=gen.generate(True), steps_per_epoch=int(gen.train_batches / 4|),|11|validation_data=gen.generate(False), validation_steps=int(gen.val_batches / 4|),|12|epochs=EPOCHS, verbose=1, callbacks=[tensorboard, checkpoint, early_stopping])
|都是在回调函数中起作用：
|EarlyStopping patience：当early
|（1）stop被激活（如发现loss相比上一个epoch训练没有下降），则经过patience个epoch后停止训练。
|（2）mode：‘auto’，‘min’，‘max’之一，在min模式下，如果检测值停止下降则中止训练。在max模式下，当检测值不再上升则停止训练。
|模型检查点ModelCheckpoint
|（1）save_best_only：当设置为True时，将只保存在验证集上性能最好的模型
|（2） mode：‘auto’，‘min’，‘max’之一，在save_best_only=True时决定性能最佳模型的评判准则，例如，当监测值为val_acc时，模式应为max，当检测值为val_loss时，模式应为min。在auto模式下，评价准则由被监测值的名字自动推断。
|（3）save_weights_only：若设置为True，则只保存模型权重，否则将保存整个模型（包括模型结构，配置信息等）
|（4）period：CheckPoint之间的间隔的epoch数
|可视化tensorboard write_images: 是否将模型权重以图片的形式可视化
|其他内容可参考|[keras中文文档](https://keras-cn.readthedocs.io/en/latest/other/callbacks/#tensorboard)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
