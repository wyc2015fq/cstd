# Python列表中append和extend区别 - Popeye_the_Sailor - CSDN博客
2018年06月23日 14:25:41[_Sailor_](https://me.csdn.net/lz0499)阅读数：587
### Python列表中append和extend区别
在列表中我们需要添加新的元素时，常常会用到append和extend两种方法。
append方法：
append() 方法用于在列表末尾添加新的对象。
## 语法
append()方法语法：
```python
list.append(obj)
```
## 参数
- obj -- 添加到列表末尾的对象。
## 返回值
该方法无返回值，但是会修改原来的列表。
## 实例
以下实例展示了 append()函数的使用方法：
```python
#!/usr/bin/python
aList = [123, 'xyz', 'zara', 'abc'];
aList.append( 2009 );
print "Updated List : ", aList;
```
以上实例输出结果如下：
```python
Updated List :  [123, 'xyz', 'zara', 'abc', 2009]
```
extend方法：
## 描述
extend() 函数用于在列表末尾一次性追加另一个序列中的多个值（用新列表扩展原来的列表）。
## 语法
extend()方法语法：
```python
list.extend(seq)
```
## 参数
- seq -- 元素列表。
## 返回值
该方法没有返回值，但会在已存在的列表中添加新的列表内容。
## 实例
以下实例展示了 extend()函数的使用方法：
```python
#!/usr/bin/python
aList = [123, 'xyz', 'zara', 'abc', 123];
bList = [2009, 'manni'];
aList.extend(bList)
print "Extended List : ", aList ;
```
以上实例输出结果如下：
```python
Extended List :  [123, 'xyz', 'zara', 'abc', 123, 2009, 'manni']
```
假若原始的列表是一个列表的列表,我们可以使用如下方法，最后把列表中的列表元素生成一个同类型的列表。
如下所示：
```python
if __name__ == '__main__':
    original = []
    aList = [['A'], ['B'], ['C'], ['D']]
    bList = [['E'], ['F']]
    for i in aList:
        original.extend(i)
    for i in bList:
            original.extend(i)
    print("Extended List : ", original)
```
实际应用：
    "Where does a wise man hide a leaf? In the forest.    But what does he do if there is no forest? ... He grows a forest to hide it in."
    -- Gilbert Keith Chesterton
    Ever tried to send a secret message to someone without using the postal service? You could use newspapers to tell    your secret. Even if someone finds your message, it's easy to brush them off and that its paranoia and a bogus    conspiracy theory. One of the simplest ways to hide a secret message is to use capital letters. Let's find some of    these secret messages.
    You are given a chunk of text. Gather all capital letters in one word in the order that they appear in the text.
    For example: text =    "**H**ow are you? **E**h, ok. **L**ow or **L**ower?    **O**hhh.",    if we collect all of the capital letters, we get the message "HELLO".
**Input: ** A text as a string (unicode).
**Output: ** The secret message as a string or an empty string.
**Example**
```python
find_message("How are you? Eh, ok. Low or Lower? Ohhh.") == "HELLO"
find_message("hello world!") == ""
```
**How it is used: **    This is a simple exercise in working with strings: iterate, recognize and concatenate.
**Precondition:**    0 < len(text) ≤ 1000
    all(ch in string.printable for ch in text)
Solution：
```python
import re
def find_message(message:str):
    message_str = []
    pattern_words = re.compile('[A-Z]+')
    for words in message.split(" "):
        if(pattern_words.findall(words) == []):
            continue
        else:
            message_str.extend(pattern_words.findall(words))
    return  "".join(message_str)
if __name__ == '__main__':
    assert find_message("How are you? Eh, ok. Low or Lower? Ohhh.") == "HELLO", "hello"
    assert find_message("dnwkldhiqw3ry37xhqdxaifiuoa7eya8w6r87a7y87y&Y&*DS&*DYH*&d8w9y8whd7*&Whdukjldwj*HDJKj") == "YDSDYHWHDJK"
    assert find_message("HELLO WORLD!!!") == "HELLOWORLD", "Capitals"
    print("Coding complete? Click 'Check' to review your tests and earn cool rewards!")
```
