# 两个字符串最长的公共字串问题（LCS） - Arthur的随笔 - CSDN博客
2012年04月05日 16:41:43[largetalk](https://me.csdn.net/largetalk)阅读数：759标签：[tree																[matrix](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)](https://so.csdn.net/so/search/s.do?q=tree&t=blog)
个人分类：[python																[algorithm](https://blog.csdn.net/largetalk/article/category/941205)](https://blog.csdn.net/largetalk/article/category/715661)
两种方法动态规划和后缀树
```python
#!/usr/bin/env python
# --*-- coding: utf8 --*--
#
#Longest common substring
def display_matrix(matrix):
    print '###############################'
    for x in matrix:
        print x
    print '###############################'
def dynamic(str1, str2):
    #dynamic programming
    m = [ [ 0 for x in str1 ] for y in str2 ]
    maxlen = 0
    mj = 0
    display_matrix(m)
    for i in range(len(str2)):
        for j in range(len(str1)):
            if str1[j] == str2[i]:
                if i == 0 or j == 0:
                    m[i][j] = 1
                else:
                    m[i][j] = m[i-1][j-1] + 1
                if m[i][j] > maxlen:
                    maxlen = m[i][j]
                    mj = j
    display_matrix(m)
    print maxlen, str1[j-maxlen:j]
dynamic('abcdefgh', 'abyyixyqcdefgoo')
def display_tree(tr, lvl):
    for k, v in tr.items():
        print '--'*lvl, k, ' : ', v['sid']
        display_tree(tr[k]['sub'], lvl + 1)
def add_str(tr, s, num):
    for begin in range(len(s)):
        tmplvl = tr
        for w in s[begin:]:
            if w in tmplvl:
                tmplvl[w]['sid'].add(num)
            else:
                tmplvl[w] = {'sid': set([num]), 'sub':{}}
            tmplvl = tmplvl[w]['sub']
def findcommon(tr):
    maxstr = ''
    maxlen = 0
    for k, v in tr.items():
        if len(v['sid']) < 2:
            continue
        else:
            tmpstr = k + findcommon(v['sub'])
            if len(tmpstr) > maxlen:
                maxlen = len(tmpstr)
                maxstr = tmpstr
    return maxstr
def suffixTree(str1, str2):
    #suffix tree
    #use dict simulate suffix tree
    tree = {}
    add_str(tree, str1, 1)
    add_str(tree, str2, 2)
    display_tree(tree, 0)
    print findcommon(tree)
suffixTree('abcdeafg', 'abopiuocdeafuo')
```
