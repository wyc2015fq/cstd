
# Replace or Retrieve Keywords In Documents At Scale - lpty的博客 - CSDN博客

2018年05月04日 10:56:31[lpty](https://me.csdn.net/sinat_33741547)阅读数：1321所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、概念
Flashtext主要用于关键字搜索与替换，来自于论文：[Replace or Retrieve Keywords In Documents At Scale](https://arxiv.org/pdf/1711.00046.pdf)
论文中提到，Flashtext基于 Trie 字典数据结构和 Aho Corasick，但从搜索方式上，只匹配完整的词而不会匹配子字符串，如：
模式串‘apple’不会匹配到字符串’I like Pineapple’中的‘apple’子字符串
从而，使得算法的复杂度降低到O（N），N为字符串长度。与正则O（M*N）的复杂度相比，作者提到：
> Thus FlashText is 82x faster than Regex for 15K terms.
![这里写图片描述](https://img-blog.csdn.net/20180504095434443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\# 二、思想
上面提到，Flashtext基于字典树和AC算法，只匹配完整的词。
\#\#\# 1、基本结构
首先，算法会将模式串构建称为字典树，如下：
![这里写图片描述](https://img-blog.csdn.net/20180504100517354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图中的start与eot作为字典树中的特殊字符代表一个词的边界，在其开源的代码中，表现如下：`def __setitem__(self, keyword, clean_name=None):
   status = False
   if not clean_name and keyword:
       clean_name = keyword
   if keyword and clean_name:
       if not self.case_sensitive:
           keyword = keyword.lower()
       current_dict = self.keyword_trie_dict
       for letter in keyword:
           current_dict = current_dict.setdefault(letter, {})
       if self._keyword not in current_dict:
           status = True
           self._terms_in_trie += 1
       current_dict[self._keyword] = clean_name
   return status`可以看到，这里利用了python的dict结构，将添加进来的词构造成为了字典树。
### 2、搜索
搜索利用字典树结构，进行正向线性寻找，如下：
![这里写图片描述](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在开源的代码中，搜索可以分为两种情况。
[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 情况1
[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)作者将数字及基本的字母默认为非完整词，作为区分标志：
[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`self.non_word_boundaries = set(string.digits + string.letters + '_')`[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)所以默认的情况下，对于由拉丁文组成的语言，如英文，代码处理逻辑如下：
[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`while idx < sentence_len:
   char = sentence[idx]
   # when we reach a character that might denote word end
   if char not in self.non_word_boundaries:
               ...
   elif char in current_dict:
       # we can continue from this char
       current_dict = current_dict[char]
   else:
	   ...
   # if we are end of sentence and have a sequence discovered
   if idx + 1 >= sentence_len:
       if self._keyword in current_dict:
           sequence_found = current_dict[self._keyword]
           keywords_extracted.append((sequence_found, sequence_start_pos, sentence_len))
   idx += 1`[
](https://img-blog.csdn.net/20180504101238802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)对于处在默认非词列表中的单字，存在字典树中则按路径寻找，不存在则跳过。在字典树中进行词搜索，只有当前单字为空格等其他字符或者字符串结束，才会进行是否是完整词判断，达到了只匹配完成词的目的。
同时，在字典树路径中搜索过的字序列，在下一轮搜索中会直接跳过，降低时间复杂度，达到O（N）。
这个位置的搜索是去除子字符串匹配的字典树搜索。
#### 情况2
默认情况下，对于非拉丁文语系的词搜索，
`if char not in self.non_word_boundaries:
       # if end is present in current_dict
       if self._keyword in current_dict or char in current_dict:
           # update longest sequence found
           sequence_found = None
           longest_sequence_found = None
           is_longer_seq_found = False
           if self._keyword in current_dict:
               sequence_found = current_dict[self._keyword]
               longest_sequence_found = current_dict[self._keyword]
               sequence_end_pos = idx
           # re look for longest_sequence from this position
           if char in current_dict:
               current_dict_continued = current_dict[char]
               idy = idx + 1
               while idy < sentence_len:
                   inner_char = sentence[idy]
                   if inner_char not in self.non_word_boundaries and self._keyword in current_dict_continued:
                       # update longest sequence found
                       longest_sequence_found = current_dict_continued[self._keyword]
                       sequence_end_pos = idy
                       is_longer_seq_found = True
                   if inner_char in current_dict_continued:
                       current_dict_continued = current_dict_continued[inner_char]
                   else:
                       break
                   idy += 1
               else:
                   # end of sentence reached.
                   if self._keyword in current_dict_continued:
                       # update longest sequence found
                       longest_sequence_found = current_dict_continued[self._keyword]
                       sequence_end_pos = idy
                       is_longer_seq_found = True
               if is_longer_seq_found:
                   idx = sequence_end_pos
           current_dict = self.keyword_trie_dict
           if longest_sequence_found:
               keywords_extracted.append((longest_sequence_found, sequence_start_pos, idx))
           reset_current_dict = True
       else:
           # we reset current_dict
           current_dict = self.keyword_trie_dict
           reset_current_dict = True
   elif char in current_dict:
       ...
   else:
	   ...`从代码中看到，其实并不能满足情况1中提到的只匹配完整词的目的，如：
模式串为“葡萄”，字符串为“”我喜欢喝葡萄酒“”，这种情况下，葡萄还是能够被匹配出来；
同时，因为搜索逻辑的问题，每一次都是从索引序列+1的位置开始，所以搜索复杂度会远远高于O（N），具体和匹配失败的次数相关。
这个位置的搜索其实本质上是完整的字典树搜索。
### 3、替换
本质和搜索一致，只是把搜索出来的关键词进行替换，这里就不展开说明了。
## 三、应用
在上文中提到，对于非拉丁文体系的语言，如中文，Flashtext无法体现效率，解决的方法如下：
（1）将需要处理的语料分词，如：我  喜欢  喝  葡萄酒
（2）将语料中出现的单字作为非词列表，增加到Flashtext中，如：
`for c in u'我喜欢喝葡萄酒':
    keyword_processor.add_non_word_boundary(c)`（3）进行关键词搜索
具体的应用可以参考：[API文档](http://flashtext.readthedocs.io/en/latest/api.html)

