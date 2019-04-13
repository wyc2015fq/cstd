
# 使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月14日 09:44:27[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：509标签：[RNN																](https://so.csdn.net/so/search/s.do?q=RNN&t=blog)[LSTN																](https://so.csdn.net/so/search/s.do?q=LSTN&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=RNN&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6667024196603740685/](https://www.toutiao.com/a6667024196603740685/)

# 循环神经网络
RNNs是一种特殊类型的神经网络，具有允许信息在网络中的不同步骤中持续存在的**循环**。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/bbc32a5137ff4a43aec722fd1aacaaff)
循环神经网络的循环
循环使神经网络返回并检查所有先前单词中发生的事情，然后再确定当前单词的实际含义。RNN可以被看作是重复地复制粘贴同一网络，每次新的复制粘贴添加的信息比前一个更多。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p3.pstatp.com/large/pgc-image/9c9da809fee74979beb81794fb3a64da)
展开的循环神经网络
RNN的应用与传统的NN有很大的不同，因为它们没有一个具体的输出和输入集，而是以序列作为输入或输出。
**那么我们可以用RNN干什么？**
自然语言处理
股票市场数据（时间序列分析）
图像/视频字幕
翻译
等等等等
RNN也有不同的模型可供遵循。
**1.固定到序列**
RNN接收固定大小的输入并输出序列。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/5fa465ff1ca9435096397d50e89bb4df)
对图像进行分析后生成文本
**2. 序列到固定**
RNN接收输入序列并输出固定大小。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/2e7dee25d34a4867adf734cdf3fc780d)
情绪分析，其中给定的句子被分类为表达积极或消极情绪
**3.序列到序列**
RNN接收输入序列并输出序列。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/faad965c516043e7a2bc6cf353f032ec)
机器翻译：RNN用一种语言读取一个句子，然后在另一种语言中输 出
这可以帮助您高度了解RNNs！
**消失的梯度问题**
在使用**基于梯度的优化技术的**任何网络中都会出现此问题**。**当计算反向传播（计算相对于权重的损失梯度）时，随着反向传播算法在网络中移动，梯度变得非常小。这会导致较早的层比以后的层学习速度慢。这降低了RNNs的有效性，因为它们通常无法充分考虑长序列。随着所需信息之间的差距变大，RNN的效率越来越差。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p3.pstatp.com/large/pgc-image/ed85c4566000413482d7dc0a7f8b0136)
现在这个问题的一个常见解决方案是使用不会导致梯度消失的激活函数，例如RELU，而不是其他激活函数，如sigmoid或hyperbolic tangent。更好的解决方案是使用长短期记忆网络！
**长、短期记忆**
LSTM网络的设计只有一个目的——解决传统RNNs所具有的长期依赖性问题。
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/6712016935be42aba1ca38e890f918dd)
LSTM中的重复模块包含4层，而RNN中只有一层
LSTMs运行良好的原因主要是由于网络中存在的单元状态。这是您在图中的网络顶部看到的行。信息很容易流经这个单元状态而不会被改变。连接到单元状态的门可以在需要时添加或删除信息。
**神经机器翻译**
由于我们对LSTMs和RNNs有了基本的了解，让我们尝试将其中的一部分用于使用Keras开发机器翻译模型。
这将演示如何使用序列到序列LSTM网络将文本从英语翻译成法语。
```python
from __future__ import print_function
from keras.models import Model
from keras.layers import Input, LSTM, Dense
import numpy as np
batch_size = 64 # Batch size for training.
epochs = 100 # Number of epochs to train for.
latent_dim = 256 # Latent dimensionality of the encoding space.
num_samples = 10000 # Number of samples to train on.
# Path to the data txt file on disk.
data_path = 'fra-eng/fra.txt'
```
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/ab137bf5bc2640c8820376d8f8195bc5)
我们从Keras导入LSTM层并在此处设置几个重要变量。
```python
input_texts = []
target_texts = []
input_characters = set()
target_characters = set()
with open(data_path, 'r', encoding='utf-8') as f:
 lines = f.read().split('
')
for line in lines[: min(num_samples, len(lines) - 1)]:
 input_text, target_text = line.split('	')
 # We use "tab" as the "start sequence" character
 # for the targets, and "
" as "end sequence" character.
 target_text = '	' + target_text + '
'
 input_texts.append(input_text)
 target_texts.append(target_text)
 for char in input_text:
 if char not in input_characters:
 input_characters.add(char)
 for char in target_text:
 if char not in target_characters:
 target_characters.add(char)
input_characters = sorted(list(input_characters))
target_characters = sorted(list(target_characters))
num_encoder_tokens = len(input_characters)
num_decoder_tokens = len(target_characters)
max_encoder_seq_length = max([len(txt) for txt in input_texts])
max_decoder_seq_length = max([len(txt) for txt in target_texts])
```
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p3.pstatp.com/large/pgc-image/aadca4f68a1442eea5083daa00c8e08b)
现在我们需要将文本向量化为数字格式，使神经网络更容易处理。
```python
encoder_inputs = Input(shape=(None, num_encoder_tokens))
encoder = LSTM(latent_dim, return_state=True)
encoder_outputs, state_h, state_c = encoder(encoder_inputs)
# We discard `encoder_outputs` and only keep the states.
encoder_states = [state_h, state_c]
# Set up the decoder, using `encoder_states` as initial state.
decoder_inputs = Input(shape=(None, num_decoder_tokens))
# We set up our decoder to return full output sequences,
# and to return internal states as well. We don't use the
# return states in the training model, but we will use them in inference.
decoder_lstm = LSTM(latent_dim, return_sequences=True, return_state=True)
decoder_outputs, _, _ = decoder_lstm(decoder_inputs,
 initial_state=encoder_states)
decoder_dense = Dense(num_decoder_tokens, activation='softmax')
decoder_outputs = decoder_dense(decoder_outputs)
# Define the model that will turn
# `encoder_input_data` & `decoder_input_data` into `decoder_target_data`
model = Model([encoder_inputs, decoder_inputs], decoder_outputs)
```
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p3.pstatp.com/large/pgc-image/0f7e185815d94300b4c749125c9e5aaf)
现在我们需要创建我们的模型。由于它是序列到序列的模型，我们需要有两个部分—编码器和解码器。编码器输出隐藏状态，其中包含作为输入获得的所有信息。然后解码器获取此信息，然后以指定的格式生成输出（在本例中，它是法语）。
在此之后，只需训练模型并使用它。如下所示：
```python
'''
#Sequence to sequence example in Keras (character-level).
This script demonstrates how to implement a basic character-level
sequence-to-sequence model. We apply it to translating
short English sentences into short French sentences,
character-by-character. Note that it is fairly unusual to
do character-level machine translation, as word-level
models are more common in this domain.
**Data download**
[English to French sentence pairs.
](http://www.manythings.org/anki/fra-eng.zip)
[Lots of neat sentence pairs datasets.
](http://www.manythings.org/anki/)
'''
from __future__ import print_function
from keras.models import Model
from keras.layers import Input, LSTM, Dense
import numpy as np
batch_size = 64 # Batch size for training.
epochs = 100 # Number of epochs to train for.
latent_dim = 256 # Latent dimensionality of the encoding space.
num_samples = 10000 # Number of samples to train on.
# Path to the data txt file on disk.
data_path = 'fra-eng/fra.txt'
# Vectorize the data.
input_texts = []
target_texts = []
input_characters = set()
target_characters = set()
with open(data_path, 'r', encoding='utf-8') as f:
 lines = f.read().split('
')
for line in lines[: min(num_samples, len(lines) - 1)]:
 input_text, target_text = line.split('	')
 # We use "tab" as the "start sequence" character
 # for the targets, and "
" as "end sequence" character.
 target_text = '	' + target_text + '
'
 input_texts.append(input_text)
 target_texts.append(target_text)
 for char in input_text:
 if char not in input_characters:
 input_characters.add(char)
 for char in target_text:
 if char not in target_characters:
 target_characters.add(char)
input_characters = sorted(list(input_characters))
target_characters = sorted(list(target_characters))
num_encoder_tokens = len(input_characters)
num_decoder_tokens = len(target_characters)
max_encoder_seq_length = max([len(txt) for txt in input_texts])
max_decoder_seq_length = max([len(txt) for txt in target_texts])
print('Number of samples:', len(input_texts))
print('Number of unique input tokens:', num_encoder_tokens)
print('Number of unique output tokens:', num_decoder_tokens)
print('Max sequence length for inputs:', max_encoder_seq_length)
print('Max sequence length for outputs:', max_decoder_seq_length)
input_token_index = dict(
 [(char, i) for i, char in enumerate(input_characters)])
target_token_index = dict(
 [(char, i) for i, char in enumerate(target_characters)])
encoder_input_data = np.zeros(
 (len(input_texts), max_encoder_seq_length, num_encoder_tokens),
 dtype='float32')
decoder_input_data = np.zeros(
 (len(input_texts), max_decoder_seq_length, num_decoder_tokens),
 dtype='float32')
decoder_target_data = np.zeros(
 (len(input_texts), max_decoder_seq_length, num_decoder_tokens),
 dtype='float32')
for i, (input_text, target_text) in enumerate(zip(input_texts, target_texts)):
 for t, char in enumerate(input_text):
 encoder_input_data[i, t, input_token_index[char]] = 1.
 for t, char in enumerate(target_text):
 # decoder_target_data is ahead of decoder_input_data by one timestep
 decoder_input_data[i, t, target_token_index[char]] = 1.
 if t > 0:
 # decoder_target_data will be ahead by one timestep
 # and will not include the start character.
 decoder_target_data[i, t - 1, target_token_index[char]] = 1.
# Define an input sequence and process it.
encoder_inputs = Input(shape=(None, num_encoder_tokens))
encoder = LSTM(latent_dim, return_state=True)
encoder_outputs, state_h, state_c = encoder(encoder_inputs)
# We discard `encoder_outputs` and only keep the states.
encoder_states = [state_h, state_c]
# Set up the decoder, using `encoder_states` as initial state.
decoder_inputs = Input(shape=(None, num_decoder_tokens))
# We set up our decoder to return full output sequences,
# and to return internal states as well. We don't use the
# return states in the training model, but we will use them in inference.
decoder_lstm = LSTM(latent_dim, return_sequences=True, return_state=True)
decoder_outputs, _, _ = decoder_lstm(decoder_inputs,
 initial_state=encoder_states)
decoder_dense = Dense(num_decoder_tokens, activation='softmax')
decoder_outputs = decoder_dense(decoder_outputs)
# Define the model that will turn
# `encoder_input_data` & `decoder_input_data` into `decoder_target_data`
model = Model([encoder_inputs, decoder_inputs], decoder_outputs)
# Run training
model.compile(optimizer='rmsprop', loss='categorical_crossentropy')
model.fit([encoder_input_data, decoder_input_data], decoder_target_data,
 batch_size=batch_size,
 epochs=epochs,
 validation_split=0.2)
# Save model
model.save('s2s.h5')
# Next: inference mode (sampling).
# Here's the drill:
# 1) encode input and retrieve initial decoder state
# 2) run one step of decoder with this initial state
# and a "start of sequence" token as target.
# Output will be the next target token
# 3) Repeat with the current target token and current states
# Define sampling models
encoder_model = Model(encoder_inputs, encoder_states)
decoder_state_input_h = Input(shape=(latent_dim,))
decoder_state_input_c = Input(shape=(latent_dim,))
decoder_states_inputs = [decoder_state_input_h, decoder_state_input_c]
decoder_outputs, state_h, state_c = decoder_lstm(
 decoder_inputs, initial_state=decoder_states_inputs)
decoder_states = [state_h, state_c]
decoder_outputs = decoder_dense(decoder_outputs)
decoder_model = Model(
 [decoder_inputs] + decoder_states_inputs,
 [decoder_outputs] + decoder_states)
# Reverse-lookup token index to decode sequences back to
# something readable.
reverse_input_char_index = dict(
 (i, char) for char, i in input_token_index.items())
reverse_target_char_index = dict(
 (i, char) for char, i in target_token_index.items())
def decode_sequence(input_seq):
 # Encode the input as state vectors.
 states_value = encoder_model.predict(input_seq)
 # Generate empty target sequence of length 1.
 target_seq = np.zeros((1, 1, num_decoder_tokens))
 # Populate the first character of target sequence with the start character.
 target_seq[0, 0, target_token_index['	']] = 1.
 # Sampling loop for a batch of sequences
 # (to simplify, here we assume a batch of size 1).
 stop_condition = False
 decoded_sentence = ''
 while not stop_condition:
 output_tokens, h, c = decoder_model.predict(
 [target_seq] + states_value)
 # Sample a token
 sampled_token_index = np.argmax(output_tokens[0, -1, :])
 sampled_char = reverse_target_char_index[sampled_token_index]
 decoded_sentence += sampled_char
 # Exit condition: either hit max length
 # or find stop character.
 if (sampled_char == '
' or
 len(decoded_sentence) > max_decoder_seq_length):
 stop_condition = True
 # Update the target sequence (of length 1).
 target_seq = np.zeros((1, 1, num_decoder_tokens))
 target_seq[0, 0, sampled_token_index] = 1.
 # Update states
 states_value = [h, c]
 return decoded_sentence
for seq_index in range(100):
 # Take one sequence (part of the training set)
 # for trying out decoding.
 input_seq = encoder_input_data[seq_index: seq_index + 1]
 decoded_sentence = decode_sequence(input_seq)
 print('-')
 print('Input sentence:', input_texts[seq_index])
 print('Decoded sentence:', decoded_sentence)
```
![使用RNNs进行机器翻译——介绍RNN和LSTM网络及其应用](http://p1.pstatp.com/large/pgc-image/3ad93488e33d4f728a8012bbc9d4051b)
**结论**
RNNs和LSTMs网络今天提供了非常酷的用例，我们看到它在大量技术中使用。
循环神经网络与传统的ANNs不同，因为它们不需要固定的输入/输出大小，并且它们也使用先前的数据进行预测。
LSTM网络专门用于解决RNNs中的长期依赖性问题，并且通过使用单元状态，它们非常善于利用不久前的数据（其本质上是固有的）。

