# 3.1 NLTK中的TF-IDF
from nltk.text import TextCollection
from sklearn.feature_extraction.text import TfidfVectorizer
# 3.2 3.3 中文TF-IDF
import os
import re
import jieba.posseg as pseg
import jieba.analyse

if 1:
    text1 = 'I like the movie so much '
    text2 = 'That is a good movie '
    text3 = 'This is a great one '
    text4 = 'That is a really bad movie '
    text5 = 'This is a terrible movie'

    # 构建TextCollection对象
    tc = TextCollection([text1, text2, text3,
                            text4, text5])
    new_text = 'That one is a good movie. This is so good!'
    word = 'That'
    tf_idf_val = tc.tf_idf(word, new_text)
    print('{}的TF-IDF值为：{}'.format(word, tf_idf_val))

    # 3.2 sklearn中的TF-IDF
    vectorizer = TfidfVectorizer()
    feat = vectorizer.fit_transform([text1, text2, text3, text4, text5])
    print(feat.toarray())
    print(vectorizer.get_feature_names())
    feat_array = feat.toarray()
    print(feat_array)
    print(feat_array.shape)
    print(feat_array[0:2, :])


# 加载停用词表
stopwords = [line.rstrip() for line in open(os.path.join('./', 'stopwords.txt'), 'r',encoding='utf-8')]


def proc_text(raw_line):
    """
        处理文本数据
        返回分词结果
    """
    # 1. 使用正则表达式去除非中文字符
    chinese_only = raw_line
    if 0:
        filter_pattern = re.compile('[^\u4E00-\u9FD5]+')
        chinese_only = filter_pattern.sub('', raw_line)

    # 2. 结巴分词+词性标注
    word_list = pseg.cut(chinese_only)
    # 3. 去除停用词，保留有意义的词性
    # 动词，形容词，副词
    used_flags = ['v', 'a', 'ad']
    used_flags = ['n', 'eng']
    meaninful_words = []
    for word, flag in word_list:
        if (word not in stopwords) and (flag in used_flags):
            meaninful_words.append(word)
    return ' '.join(meaninful_words)


ch_text1 = ' 非常失望，剧本完全敷衍了事，主线剧情没突破大家可以理解，可所有的人物都缺乏动机，正邪之间、妇联内部都没什么火花。团结-分裂-团结的三段式虽然老套但其实也可以利用积攒下来的形象魅力搞出意思，但剧本写得非常肤浅、平面。场面上调度混乱呆板，满屏的铁甲审美疲劳。只有笑点算得上差强人意。'
ch_text2 = ' 2015年度最失望作品。以为面面俱到，实则画蛇添足；以为主题深刻，实则老调重弹；以为推陈出新，实则俗不可耐；以为场面很high，实则high劲不足。气！上一集的趣味全无，这集的笑点明显刻意到心虚。全片没有任何片段给我有紧张激动的时候，太弱了，跟奥创一样。'
ch_text3 = ' 《铁人2》中勾引钢铁侠，《妇联1》中勾引鹰眼，《美队2》中勾引美国队长，在《妇联2》中终于……跟绿巨人表白了，黑寡妇用实际行动告诉了我们什么叫忠贞不二；而且为了治疗不孕不育连作战武器都变成了两支验孕棒(坚决相信快银没有死，后面还得回来)'
ch_text4 = ' 虽然从头打到尾，但是真的很无聊啊。'
ch_text5 = ' 剧情不如第一集好玩了，全靠密集笑点在提神。僧多粥少的直接后果就是每部寡姐都要换着队友谈恋爱，这特么比打斗还辛苦啊，真心求放过～～～（结尾彩蛋还以为是洛基呢，结果我呸！）'

ch_texts = [ch_text1, ch_text2, ch_text3, ch_text4, ch_text5]




#词性标注也叫词类标注。POS tagging是part-of-speech tagging的缩写

if 0:
    string = "我爱北京天安门"
    words = pseg.cut(string)
    print("===" * 20)
    # 返回的是生成器哦！
    print(words)

    # 该如何打印呢？
    for word,flag in words:
        print("%s  %s"%(word, flag))



if 0:
    fn = 'D:\code\git\ywlydd\cstd\doc/0-1背包问题的动态规划算法.md'
    # t就是文本模式
    f=open(fn,'rt',encoding='utf8')
    t=f.read()
    f.close()
    #print(t)
    chinese_only = t
    if 1:
        filter_pattern = re.compile('[^\u4E00-\u9FD5]+')
        chinese_only = filter_pattern.sub('', t)
    kwords = jieba.analyse.extract_tags(chinese_only, 20, False)
    #word_list = proc_text(t)
    #word_list = pseg.cut(t)
    print(kwords)
    ch_vectorizer = TfidfVectorizer()
    ch_feats = ch_vectorizer.fit_transform(kwords)
    print(len(ch_feats))
    print(ch_feats)
    print(list(kwords))
    print(list(kwords))




if 1:
    print(len(stopwords))

    corpus = [proc_text(ch_text) for ch_text in ch_texts]
    #print(corpus)
    ch_vectorizer = TfidfVectorizer()
    ch_feats = ch_vectorizer.fit_transform(corpus)
    print(ch_vectorizer.get_feature_names())
    print(ch_feats.toarray())
    print(ch_feats.toarray()[0, :])
