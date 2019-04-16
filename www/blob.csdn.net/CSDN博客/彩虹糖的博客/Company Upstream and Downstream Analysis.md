# Company Upstream and Downstream Analysis - 彩虹糖的博客 - CSDN博客





2018年11月07日 22:30:44[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：94








# Company Upstream and Downstream Analysis

## Background
- Listed companies regularly disclose annual reports in each quarter to disclose the company’s operating conditions. But these reports are too long for humans to read and extract effective information. So using some technology of data mining is very popular nowadays for people who want to have better knowledge of listed companies.
- Most listed companies have their upstream industry and downstream industry. What’s more, their industries can have a huge effect on these companies, so it’s important to know upstream and downstream industries of each company. When we have that information, we can have a better analysis of the factors that influence the company’s profits and help us predict business revenue of certain companies.

## Requirement Analysis
- First, we need to build a dictionary that contains vocabularies that represents certain kinds of industries. With the dictionary, we can get words of industries from annual reports, this is the prerequisite of our analysis.
- Then, we need to identify whether these words are actually words that represent the company’s upstream and downstream industries.
- Finally, show results in a file or on a website.

## Solution
- First, we use word2vec as our basic technology to map words to vectors in order to do classification and other calculation, more will be illustrated later.
- We will use some corpus to collect words that represent industry. I’ll do manual annotation and then use SVM to classify words.
- Then, I will use LSTM to judge whether the word represents upstream or downstream industries.

## Running Environment And Depenencies
- Python 3.6: Python is a popular computer language in machine learning and NLP for its easy grammar and rich packages. All the code are written via python.
- Django: Django is a free and open-source web framework, written in Python, which follows the model-view-template (MVT) architectural pattern. It is maintained by the Django Software Foundation (DSF), an independent organization established as a non-profit. Actually, this project does not necessarily use Django. But considering Django provides us very convenient api to operate database and build a web site, we use it to improve our efficiency.
- Numpy: Numpy is the fundamental package for scientific computing with Python.
- jieba:
- Tensorflow: TensorFlow is an open-source software library for dataflow programming across a range of tasks. It is a symbolic math library, and is also used for machine learning applications such as neural networks.

## Technology Used
- Word2vec:Word2vec is a group of related models that are used to produce word embeddings. These models are shallow, two-layer neural networks that are trained to reconstruct linguistic contexts of words. Word2vec takes as its input a large corpus of text and produces a vector space, typically of several hundred dimensions, with each unique word in the corpus being assigned a corresponding vector in the space. Word vectors are positioned in the vector space such that words that share common contexts in the corpus are located in close proximity to one another in the space.
- SVM:In machine learning, support vector machines (SVMs, also support vector networks) are supervised learning models with associated learning algorithms that analyze data used for classification and regression analysis. Given a set of training examples, each marked as belonging to one or the other of two categories, an SVM training algorithm builds a model that assigns new examples to one category or the other, making it a non-probabilistic binary linear classifier (although methods such as Platt scaling exist to use SVM in a probabilistic classification setting). An SVM model is a representation of the examples as points in space, mapped so that the examples of the separate categories are divided by a clear gap that is as wide as possible. New examples are then mapped into that same space and predicted to belong to a category based on which side of the gap they fall.
- LSTM:Long short-term memory (LSTM) units are units of a recurrent neural network (RNN). An RNN composed of LSTM units is often called an LSTM network. A common LSTM unit is composed of a cell, an input gate, an output gate and a forget gate. The cell remembers values over arbitrary time intervals and the three gates regulate the flow of information into and out of the cell.LSTM networks are well-suited to classifying, processing and making predictions based on time series data, since there can be lags of unknown duration between important events in a time series.

## Data Set
- Sohu news data: The data can be downloaded in [this website](http://www.sogou.com/labs/resource/cs.php)
- Listed companies reports: All the reports are collected in [EastMoney](http://www.eastmoney.com/) This work is done by my classmates.

## Experiment
- 
First, we need to train word to vector, the core code can be seen below

```
def train_data_build():
 	    file = r'F:\train_data.txt'
 	    names = file_name('F:\\data')
 	    for name in names:
 	        f = open(name, errors='ignore')
 	        st = f.read()
 	        with open(file, 'a+') as f:
 	            seg_list = jieba.cut(st, cut_all=False)
 	            f.write(" ".join(seg_list))
 	            f.write('\n')
 	        f.close()
 
 
 def train_data():
     from gensim.models import word2vec
     sentences = word2vec.Text8Corpus('F:\\train_data.txt')
     model = word2vec.Word2Vec(sentences, size=50)
     model.save('word2vec_model')
```

- 
We use keyword “行业” to find words that represent industry. We find all the words that appear before the keyword.

```
def buildData():
     rule = r'(.*)行业'
     compile_name = re.compile(rule, re.M)
     names = file_name('E:\\temp_data\\tmp')
     for name in names:
         f = open(name, errors='ignore')
         st = f.read()
         res_name = compile_name.findall(st)
         for sentence in res_name:
             seg_list = jieba.lcut(sentence,cut_all=False)
             word = seg_list[len(seg_list)-2]
             if len(word)<=1:
                 continue
             values = pseg.cut(word)
             flag_word = True
             for value, flag in values:
                 if flag == 'n':
                     continue
                 else:
                     flag_word = False
             if flag_word:
                 Dictionary.objects.get_or_create(name=word)
```

- 
But the data has many problems we need to do data preprocessing before we use SVM and LSTM to do classification. We delete words that are stop words and words that do not be transfered to a vector.

- 
Then we annotate data artificially. Finally, we use SVM to classify the words.Then we annotate data artificially. Finally, we use SVM to classify the words.

```
def SVM():
     sess = tf.Session()
     words = Divided.objects.all()
     model = gensim.models.Word2Vec.load('D:\\word2vec\\word2vec_from_weixin\\word2vec\\word2vec_wx')
     x_vals = np.array([model[word.name].tolist() for word in words])
     y_vals = np.array([1 if word.is_industry else -1 for word in words])
     train_indices = np.random.choice(len(x_vals), round(len(x_vals) * 0.8), replace=False)
     test_indices = np.array(list(set(range(len(x_vals))) - set(train_indices)))
     x_vals_train = x_vals[train_indices]
     x_vals_test = x_vals[test_indices]
     y_vals_train = y_vals[train_indices]
     y_vals_test = y_vals[test_indices]
     # 批训练中批的大小
     batch_size = 100
     x_data = tf.placeholder(shape=[None, 256], dtype=tf.float32)
     y_target = tf.placeholder(shape=[None, 1], dtype=tf.float32)
     W = tf.Variable(tf.random_normal(shape=[256, 1]))
     b = tf.Variable(tf.random_normal(shape=[1, 1]))
     # 定义损失函数
     model_output = tf.matmul(x_data, W) + b
     l2_norm = tf.reduce_sum(tf.square(W))
     # 软正则化参数
     alpha = tf.constant([0.1])
     # 定义损失函数
     classification_term = tf.reduce_mean(tf.maximum(0., 1. - model_output * y_target))
     loss = classification_term + alpha * l2_norm
     # 输出
     prediction = tf.sign(model_output)
     accuracy = tf.reduce_mean(tf.cast(tf.equal(prediction, y_target), tf.float32))
     train_step = tf.train.GradientDescentOptimizer(0.01).minimize(loss)
     #saver = tf.train.Saver()
     # 开始训练
     sess.run(tf.global_variables_initializer())
     loss_vec = []
     train_accuracy = []
     test_accuracy = []
     for i in range(8000):
         rand_index = np.random.choice(len(x_vals_train), size=batch_size)
         rand_x = x_vals_train[rand_index]
         rand_y = np.transpose([y_vals_train[rand_index]])
         sess.run(train_step, feed_dict={x_data: rand_x, y_target: rand_y})
         temp_loss = sess.run(loss, feed_dict={x_data: rand_x, y_target: rand_y})
         loss_vec.append(temp_loss)
         train_acc_temp = sess.run(accuracy, feed_dict={x_data: x_vals_train, y_target: np.transpose([y_vals_train])})
         train_accuracy.append(train_acc_temp)
         test_acc_temp = sess.run(accuracy, feed_dict={x_data: x_vals_test, y_target: np.transpose([y_vals_test])})
         test_accuracy.append(test_acc_temp)
         if (i + 1) % 100 == 0:
             print('Step #' + str(i + 1) + ' W = ' + str(sess.run(W)) + 'b = ' + str(sess.run(b)))
             print('Loss = ' + str(test_acc_temp))
     #saver.save(sess, "./model/model.ckpt")
     print(train_accuracy)
     print(test_accuracy)
     plt.plot(loss_vec)
     plt.plot(train_accuracy)
     plt.plot(test_accuracy)
     plt.legend(['loss', 'train accuracy', 'test accuracy'])
     plt.ylim(0., 1.)
     plt.show()
```


From the graph, we can find in the iterative process, the accuracy of the model is getting higher and higher, and the loss is less and less.

![SVM](https://img-blog.csdnimg.cn/20181122171621373.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

Save the model and we predict all the words if it is a industry word.

5. After we build the dictionary, we come to the next topic that how to find upstream industry and down stream industry of a company. Here, we use LSTM to do the claasifications. What I want to say is that this is just a experiment and perhaps the result is not so good. We choose this method just because LSTM is usually used in NLP and is good at processing series. The code of LSTM using tensorflow is below:

```
class Model(object):
	    def __init__(self, name):
	        self.type_size = 2
	        self.word_size = 256
	        self.lstm_size = 100
	        # self.transe_size = 100
	        self.dev = 0.01
	        self.hidden_layer = 50
	        self.window = 5
	        self.scope = "root_train_second" if name == "KA+D" else "root"
	
	        self.predict()
	        self.saver = tf.train.Saver(max_to_keep=100)
	        self.initializer = tf.global_variables_initializer()
	
	    def entity(self):
	        self.entity_in = tf.placeholder(tf.float32, [None, self.word_size])
	        self.batch_size = tf.shape(self.entity_in)[0]
	        self.kprob = tf.placeholder(tf.float32)
	        entity_drop = tf.nn.dropout(self.entity_in, self.kprob)
	        return entity_drop
	
	    def attention(self):
	        # this method will be overrided by derived classes
	        pass
	
	    def context(self):
	        # from middle to side
	        self.left_in = [tf.placeholder(tf.float32, [None, self.word_size]) \
	                        for _ in range(self.window)]
	        self.right_in = [tf.placeholder(tf.float32, [None, self.word_size]) \
	                         for _ in range(self.window)]
	
	        # from side to middle
	        self.left_in_rev = [self.left_in[self.window - 1 - i] for i in range(self.window)]
	        self.right_in_rev = [self.right_in[self.window - 1 - i] for i in range(self.window)]
	
	        left_middle_lstm = tf.nn.rnn_cell.LSTMCell(self.lstm_size)
	        right_middle_lstm = tf.nn.rnn_cell.LSTMCell(self.lstm_size)
	        left_side_lstm = tf.nn.rnn_cell.LSTMCell(self.lstm_size)
	        right_side_lstm = tf.nn.rnn_cell.LSTMCell(self.lstm_size)
	
	        with tf.variable_scope(self.scope):
	            with tf.variable_scope('lstm'):
	                # from side to middle
	                left_out_rev, _ = tf.nn.static_rnn(left_middle_lstm, self.left_in_rev, dtype=tf.float32)
	            with tf.variable_scope('lstm', reuse=True):
	                # from side to middle
	                right_out_rev, _ = tf.nn.static_rnn(right_middle_lstm, self.right_in_rev, dtype=tf.float32)
	
	                # from middle to side
	                left_out, _ = tf.nn.static_rnn(left_side_lstm, self.left_in, dtype=tf.float32)
	                right_out, _ = tf.nn.static_rnn(right_side_lstm, self.right_in, dtype=tf.float32)
	
	        self.left_att_in = [tf.concat([left_out[i], left_out_rev[self.window - 1 - i]], 1) \
	                            for i in range(self.window)]
	        self.right_att_in = [tf.concat([right_out[i], right_out_rev[self.window - 1 - i]], 1) \
	                             for i in range(self.window)]
	
	        left_att, right_att = self.attention()
	
	        left_weighted = reduce(tf.add,
	                               [self.left_att_in[i] * left_att[i] for i in range(self.window)])
	        right_weighted = reduce(tf.add,
	                                [self.right_att_in[i] * right_att[i] for i in range(self.window)])
	
	        left_all = reduce(tf.add, [left_att[i] for i in range(self.window)])
	        right_all = reduce(tf.add, [right_att[i] for i in range(self.window)])
	
	        return tf.concat([left_weighted / left_all, right_weighted / right_all], 1)
	
	    def predict(self):
	        # this method will be overrided by derived classes
	        pass
	
	    def fdict(self, now, size, interval, _entity, _context, _label):
	        # this method will be overrided by derived classes
	        pass
	
	    def mag(self, matrix):
	        return tf.reduce_sum(tf.pow(matrix, 2))
	
	    def cross_entropy(self, predicted, truth):
	        return -tf.reduce_sum(truth * tf.log(predicted + 1e-10)) \
	               - tf.reduce_sum((1 - truth) * tf.log(1 - predicted + 1e-10))
	
	
	class SA(Model):
	    def attention(self):
	        W1 = tf.Variable(tf.random_normal([self.lstm_size * 2, self.hidden_layer], stddev=self.dev))
	        W2 = tf.Variable(tf.random_normal([self.hidden_layer, 1], stddev=self.dev))
	
	        left_att = [tf.exp(tf.matmul(tf.tanh(tf.matmul(self.left_att_in[i], W1)), W2)) \
	                    for i in range(self.window)]
	        right_att = [tf.exp(tf.matmul(tf.tanh(tf.matmul(self.right_att_in[i], W1)), W2)) \
	                     for i in range(self.window)]
	
	        return (left_att, right_att)
	
	    def predict(self):
	        x = tf.concat([self.entity(), self.context()], 1)
	
	        W = tf.Variable(tf.random_normal([self.word_size + self.lstm_size * 4, self.type_size],
	                                         stddev=self.dev))
	        self.t = tf.nn.sigmoid(tf.matmul(x, W))
	        self.t_ = tf.placeholder(tf.float32, [None, self.type_size])
	
	        self.loss = self.cross_entropy(self.t, self.t_)
	        self.train = tf.train.AdamOptimizer(0.005).minimize(self.loss)
	
	    def fdict(self, now, size, interval, _entity, _context, _label):
	        fd = {}
	        new_size = int(size / interval)
	
	        ent = np.zeros([new_size, self.word_size])
	        lab = np.zeros([new_size, self.type_size])
	        for i in range(new_size):
	            vec = _entity[now + i * interval]
	            ent[i] = vec
	            lab[i] = _label[now + i * interval]
	        fd[self.entity_in] = ent
	        fd[self.t_] = lab
	
	        for j in range(self.window):
	            left_con = np.zeros([new_size, self.word_size])
	            right_con = np.zeros([new_size, self.word_size])
	            for i in range(new_size):
	                left_con[i, :] = _context[now + i * interval][2 * j]
	                right_con[i, :] = _context[now + i * interval][2 * j + 1]
	            fd[self.left_in[j]] = left_con
	            fd[self.right_in[j]] = right_con
	
	        return fd
```

And the LSTM is referenced other contributors work.

## To Be Finished

## Code in [BusinessInteligenceFinalWork](https://github.com/caozixuan/BusinessInteligenceFinalWork)



