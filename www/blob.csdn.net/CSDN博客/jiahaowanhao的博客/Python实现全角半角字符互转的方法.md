
# Python实现全角半角字符互转的方法 - jiahaowanhao的博客 - CSDN博客


2018年05月14日 21:56:00[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：91


[Python实现全角半角字符互转的方法](http://cda.pinggu.org/view/25537.html)
相信对于每一个编程人员来说，在文本处理的时候，经常会遇到全角半角不一致的问题。于是需要程序能够快速的在两者之间互转。由于全角半角本身存在着映射关系，所以处理起来并不复杂。
具体规则为：
全角字符unicode编码从65281~65374 （十六进制 0xFF01 ~ 0xFF5E）
半角字符unicode编码从33~126 （十六进制 0x21~ 0x7E）
空格比较特殊，全角为 12288（0x3000），半角为 32（0x20）
而且除空格外，全角/半角按unicode编码排序在顺序上是对应的（半角 + 65248 = 全角）
所以可以直接通过用+-法来处理非空格数据，对空格单独处理。
用到的一些函数
chr()函数用一个范围在range（256）内的（就是0～255）整数作参数，返回一个对应的字符。
unichr()跟它一样，只不过返回的是Unicode字符。
ord()函数是chr()函数或unichr()函数的配对函数，它以一个字符（长度为1的字符串）作为参数，返回对应的ASCII数值，或者Unicode数值。
先来打印下映射关系：
for i in xrange(33,127):
print i,chr(i),i+65248,unichr(i+65248)
返回结果
33 ! 65281 ！
34 " 65282 ＂
35 \# 65283 ＃
36 $ 65284 ＄
37 % 65285 ％
38 & 65286 ＆
39 ' 65287 ＇
40 ( 65288 （
41 ) 65289 ）
42 * 65290 ＊
43 + 65291 ＋
44 , 65292 ，
45 - 65293 －
46 . 65294 ．
47 / 65295 ／
48 0 65296 ０
49 1 65297 １
50 2 65298 ２
51 3 65299 ３
52 4 65300 ４
53 5 65301 ５
54 6 65302 ６
55 7 65303 ７
56 8 65304 ８
57 9 65305 ９
58 : 65306 ：
59 ; 65307 ；
60 < 65308 ＜
61 = 65309 ＝
62 > 65310 ＞
63 ? 65311 ？
64 @ 65312 ＠
65 A 65313 Ａ
66 B 65314 Ｂ
67 C 65315 Ｃ
68 D 65316 Ｄ
69 E 65317 Ｅ
70 F 65318 Ｆ
71 G 65319 Ｇ
72 H 65320 Ｈ
73 I 65321 Ｉ
74 J 65322 Ｊ
75 K 65323 Ｋ
76 L 65324 Ｌ
77 M 65325 Ｍ
78 N 65326 Ｎ
79 O 65327 Ｏ
80 P 65328 Ｐ
81 Q 65329 Ｑ
82 R 65330 Ｒ
83 S 65331 Ｓ
84 T 65332 Ｔ
85 U 65333 Ｕ
86 V 65334 Ｖ
87 W 65335 Ｗ
88 X 65336 Ｘ
89 Y 65337 Ｙ
90 Z 65338 Ｚ
91 [ 65339 ［
92 \ 65340 ＼
93 ] 65341 ］
94 ^ 65342 ＾
95 _ 65343 ＿
96 ` 65344 ｀
97 a 65345 ａ
98 b 65346 ｂ
99 c 65347 ｃ
100 d 65348 ｄ
101 e 65349 ｅ
102 f 65350 ｆ
103 g 65351 ｇ
104 h 65352 ｈ
105 i 65353 ｉ
106 j 65354 ｊ
107 k 65355 ｋ
108 l 65356 ｌ
109 m 65357 ｍ
110 n 65358 ｎ
111 o 65359 ｏ
112 p 65360 ｐ
113 q 65361 ｑ
114 r 65362 ｒ
115 s 65363 ｓ
116 t 65364 ｔ
117 u 65365 ｕ
118 v 65366 ｖ
119 w 65367 ｗ
120 x 65368 ｘ
121 y 65369 ｙ
122 z 65370 ｚ
123 { 65371 ｛
124 | 65372 ｜
125 } 65373 ｝
126 ~ 65374 ～
把全角转成半角：
def full2half(s):
n = []
s = s.decode('utf-8')
for char in s:
num = ord(char)
if num == 0x3000:
num = 32
elif 0xFF01 <= num <= 0xFF5E:
num -= 0xfee0
num = unichr(num)
n.append(num)
return ''.join(n)
把半角转成全角：
def half2full(s):
n = []
s = s.decode('utf-8')
for char in s:
num = char(char)
if num == 320:
num = 0x3000
elif 0x21 <= num <= 0x7E:
num += 0xfee0
num = unichr(num)
n.append(num)
return ''.join(n)
上面的实现方式非常的简单，但是现实情况下可能并不会把所以的字符统一进行转换，比如中文文章中我们期望将所有出现的字母和数字全部转化成半角，而常见标点符号统一使用全角，上面的转化就不适合了。
解决方案，是自定义词典。
\#!/usr/bin/env python
\# -*- coding: utf-8 -*-
FH_SPACE = FHS = ((u"　", u" "),)
FH_NUM = FHN = (
(u"０", u"0"), (u"１", u"1"), (u"２", u"2"), (u"３", u"3"), (u"４", u"4"),
(u"５", u"5"), (u"６", u"6"), (u"７", u"7"), (u"８", u"8"), (u"９", u"9"),
)
FH_ALPHA = FHA = (
(u"ａ", u"a"), (u"ｂ", u"b"), (u"ｃ", u"c"), (u"ｄ", u"d"), (u"ｅ", u"e"),
(u"ｆ", u"f"), (u"ｇ", u"g"), (u"ｈ", u"h"), (u"ｉ", u"i"), (u"ｊ", u"j"),
(u"ｋ", u"k"), (u"ｌ", u"l"), (u"ｍ", u"m"), (u"ｎ", u"n"), (u"ｏ", u"o"),
(u"ｐ", u"p"), (u"ｑ", u"q"), (u"ｒ", u"r"), (u"ｓ", u"s"), (u"ｔ", u"t"),
(u"ｕ", u"u"), (u"ｖ", u"v"), (u"ｗ", u"w"), (u"ｘ", u"x"), (u"ｙ", u"y"), (u"ｚ", u"z"),
(u"Ａ", u"A"), (u"Ｂ", u"B"), (u"Ｃ", u"C"), (u"Ｄ", u"D"), (u"Ｅ", u"E"),
(u"Ｆ", u"F"), (u"Ｇ", u"G"), (u"Ｈ", u"H"), (u"Ｉ", u"I"), (u"Ｊ", u"J"),
(u"Ｋ", u"K"), (u"Ｌ", u"L"), (u"Ｍ", u"M"), (u"Ｎ", u"N"), (u"Ｏ", u"O"),
(u"Ｐ", u"P"), (u"Ｑ", u"Q"), (u"Ｒ", u"R"), (u"Ｓ", u"S"), (u"Ｔ", u"T"),
(u"Ｕ", u"U"), (u"Ｖ", u"V"), (u"Ｗ", u"W"), (u"Ｘ", u"X"), (u"Ｙ", u"Y"), (u"Ｚ", u"Z"),
)
FH_PUNCTUATION = FHP = (
(u"．", u"."), (u"，", u","), (u"！", u"!"), (u"？", u"?"), (u"”", u'"'),
(u"'", u"'"), (u"‘", u"`"), (u"＠", u"@"), (u"＿", u"_"), (u"：", u":"),
(u"；", u";"), (u"＃", u"\#"), (u"＄", u"$"), (u"％", u"%"), (u"＆", u"&"),
(u"（", u"("), (u"）", u")"), (u"‐", u"-"), (u"＝", u"="), (u"＊", u"*"),
(u"＋", u"+"), (u"－", u"-"), (u"／", u"/"), (u"＜", u"<"), (u"＞", u">"),
(u"［", u"["), (u"￥", u"\\"), (u"］", u"]"), (u"＾", u"^"), (u"｛", u"{"),
(u"｜", u"|"), (u"｝", u"}"), (u"～", u"~"),
)
FH_ASCII = HAC = lambda: ((fr, to) for m in (FH_ALPHA, FH_NUM, FH_PUNCTUATION) for fr, to in m)
HF_SPACE = HFS = ((u" ", u"　"),)
HF_NUM = HFN = lambda: ((h, z) for z, h in FH_NUM)
HF_ALPHA = HFA = lambda: ((h, z) for z, h in FH_ALPHA)
HF_PUNCTUATION = HFP = lambda: ((h, z) for z, h in FH_PUNCTUATION)
HF_ASCII = ZAC = lambda: ((h, z) for z, h in FH_ASCII())

def convert(text, *maps, **ops):
""" 全角/半角转换
args:
text: unicode string need to convert
maps: conversion maps
skip: skip out of character. In a tuple or string
return: converted unicode string
"""
if "skip" in ops:
skip = ops["skip"]
if isinstance(skip, basestring):
skip = tuple(skip)
def replace(text, fr, to):
return text if fr in skip else text.replace(fr, to)
else:
def replace(text, fr, to):
return text.replace(fr, to)
for m in maps:
if callable(m):
m = m()
elif isinstance(m, dict):
m = m.items()
for fr, to in m:
text = replace(text, fr, to)
return text

if __name__ == '__main__':
text = u"成田空港—【ＪＲ特急成田エクスプレス号・横浜行，2站】—東京—【ＪＲ新幹線はやぶさ号・新青森行,6站 】—新青森—【ＪＲ特急スーパー白鳥号・函館行，4站 】—函館"
print convert(text, FH_ASCII, {u"【": u"[", u"】": u"]", u",": u"，", u".": u"。", u"?": u"？", u"!": u"！"}, spit="，。？！“”")
特别注意：引号在英语体系中引号是不区分前引号和后引号。
总结
以上就是关于Python实现全角半角字符互转的方法，希望本文的内容对大家的学习或者工作能带来一定的帮助

