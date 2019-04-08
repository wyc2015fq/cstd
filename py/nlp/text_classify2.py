# 训练集
texts = ['酸奶蛋糕', '酸奶芝士', '芝士蛋糕', '酸奶芝士蛋糕',
         '文本挖掘', '挖掘数据', '文本数据', '挖掘文本数据']
labels = ['food', 'food', 'food', 'food',
          'IT', 'IT', 'IT', 'IT']

# 分词
from jieba import lcut
ls_of_words = [lcut(text) for text in texts]

# 构造词典
from gensim import corpora
dictionary = corpora.Dictionary(ls_of_words)
dt = dictionary.token2id
length = len(dt)

print(dt)
# one hot representation
for word, wid in dt.items():
    vector = [0] * length
    vector[wid] = 1

# 句向量
ls_of_wid = []
for words in ls_of_words:
    vector = [0] * length
    for word in words:
        vector[dt[word]] += 1
    ls_of_wid.append(vector)

print(ls_of_wid)
# 数据切分
from sklearn.model_selection import train_test_split
train_labels, test_labels, train_wids, test_wids = train_test_split(labels, ls_of_wid)

# 贝叶斯模型训练
from sklearn.naive_bayes import MultinomialNB
classifier = MultinomialNB()  # 朴素贝叶斯分类器
classifier.fit(train_wids, train_labels)

t = classifier.predict_proba(ls_of_wid);
print(t)

# 模型测评
score = classifier.score(test_wids, test_labels)
print(score)
