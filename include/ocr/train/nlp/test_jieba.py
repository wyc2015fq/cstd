import jieba
text = "　　目前已经有不少部哲学史了，我的目的并不是要仅仅在它们之中再加上一部【1】。"
 
item = text.strip('\n\r').split('\t')
import jieba.analyse
tags = jieba.analyse.extract_tags(item[0])
 
word_lst = []
for t in tags:
   word_lst.append(t)
 
word_dict = {}
for item in word_lst:
   if item not in word_dict:
     word_dict[item] = 1
   else:
     word_dict[item] += 1
 
print(word_dict)
