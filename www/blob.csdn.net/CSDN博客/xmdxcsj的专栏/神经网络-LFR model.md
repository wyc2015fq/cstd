# 神经网络-LFR model - xmdxcsj的专栏 - CSDN博客





2016年12月02日 21:14:27[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1987








## CLDNN[1]

不同的网络结构有不同的优势 

- CNN擅长减少频率偏移 

- LSTM擅长对时序信号进行建模 

- DNN可以对特征做更高阶的抽象，更容易进行分类 
![这里写图片描述](https://img-blog.csdn.net/20161202211349846)

CLDNN依次将CNN/LSTM/DNN进行串联组合成一个新的网络，相当于依次进行频域变化/时域关联/特征抽象，相比于单一的LSTM网络，可以获得性能的提升。
## delay constraint[2]

对ctc模型来讲，在数据帧和输出label之间存在延时，即label的尖峰可能在实际语音延迟一段时间后出现，类似于下图： 
![这里写图片描述](https://img-blog.csdn.net/20161202211406753)

为了改善这种延时的问题，可以人为的限定延时的时间范围。具体做法是在ctc训练过程中，以对齐的label作为标准，在前后向变量计算的过程中只选择延时在一定范围内的路径。 

人为缩小这种delay会引起识别率的下降，但是经过smbr训练以后，不同delay时间对应识别率保持一致。
## LFR[3]

由于CTC模型是序列到序列的训练关系，所以可以采用lower frame rate的方式进行训练，比如每30ms计算一次声学得分（即使用三帧数据中的一帧进行解码）。 

除了ctc这种训练方式，传统的lstm模型也进行了LFR(lower frame rate)的测试。使用时需要将cd-state修改为cd-phone，即放大输出单元的粒度，为跳帧提供空间。 

相比于CTC-30ms，CLDNN LFR-40ms（即每四帧数据使用一帧）的优势： 

- 准确率提升 

- shorter output delay 

- 没有ctc对训练数据量敏感（训练语料减少带来的性能降低没有ctc明显）
## 参考文献

[1]. Convolutional, long short-term memory, fully connected deep neural networks 

[2]. Acoustic modelling with CD-CTC-sMBR LSTM RNNs 

[3]. Lower Frame Rate Neural Network Acoustic Models






