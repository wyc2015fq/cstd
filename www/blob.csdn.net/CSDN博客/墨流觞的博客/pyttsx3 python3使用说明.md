# pyttsx3 python3使用说明 - 墨流觞的博客 - CSDN博客





2018年09月13日 19:50:29[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1443








# python3 pyttsx3 文字转语音库使用

最近再做一个项目，需要文字转语音，决定使用此库了。

#### 1.安装

```python
pip install pyttsx3
        # 若未安装pywin32, 需要安装此库的
        pip install pywin32
```

#### 2.官方文档

官方网址[https://pyttsx3.readthedocs.io/en/latest/engine.html](https://pyttsx3.readthedocs.io/en/latest/engine.html)

#### 3.使用
- 简单使用

```python
import pyttsx3
# 初始化， 必须要有奥
engine = pyttsx3.init()

engine.say('Sally sells seashells by the seashore.')
engine.say('The quick brown fox jumped over the lazy dog.')
# 注意，没有本句话是没有声音的
engine.runAndWait()
```
- 播放中文语音，python3下直接写就好了，不需要设置语言blabla的 

就不要在意合成语音的僵硬问题了。。。

```python
import pyttsx3
# 初始化， 必须要有奥
engine = pyttsx3.init()

engine.say('语音合成开始')
engine.say('我会说中文了，开森，开森')

# 注意，没有本句话是没有声音的
engine.runAndWait()
```
- 我在这项目要用的，如果要多次调用say()函数，并不是一次调用就结束了。

每次调用都有加上engine.runAndWait()就可以了

```python
import pyttsx3
# 初始化， 必须要有奥
engine = pyttsx3.init()

engine.say('语音合成开始')
engine.say('我会说中文了，开森，开森')

# 注意，没有本句话是没有声音的
engine.runAndWait()
engine.say('我能说第二句话了')
engine.runAndWait()
```
- 来看一下复杂一点的, 通过条件语句来确定播报语音

```python
import pyttsx3
engine = pyttsx3.init()



def VOICE_TEXT(names):
    unknown_name = []
    known_name = []
    for name in names:
        if name == 'Unknown':
            unknown_name.append(name)
        else:
            known_name.append(name)
    # 记录两类人数量
    num_known = len(known_name)
    num_unknown = len(unknown_name)
    if len(names) == 0:
        return

    # 5s 播报一次
    elif num_unknown == 0:

        text = '欢迎'
        for index in range(num_known):
            if index + 1 == num_known and index != 0:
                text += '和{}'.format(known_name[index])
            else:
                text += known_name[index]
        engine.say(text)
        engine.runAndWait()
    # 3s 播报一次
    elif num_known == 0:

        text = '警告，警告，有陌生人未识别，有陌生人未识别'
        engine.say(text)
        engine.runAndWait()

    # 7s 播报一次
    else:
        text = '欢迎'
        for index in range(num_known):
            if index + 1 == num_known and index != 0:
                text += '和{}'.format(known_name[index])
            else:
                text += known_name[index]
        engine.say(text)
        engine.say('请注意， 您旁边有未识别人员，您将不允许进入，请询问值班人员，谢谢合作')
        engine.runAndWait()


if __name__ == '__main__':
    names = ['醉陌', '醉陌']

    VOICE_TEXT(names)
    names_ = ['Unknown', '醉陌']

    VOICE_TEXT(names_)
    names__ = ['Unknown']
    VOICE_TEXT(names__)
```
- 最终实现， 程序中定时调用某一个函数，而在此函数中调用语音播报函数，于是添加了几个变量来维护函数播报与否

```python
import pyttsx3

KUNOWN = 0
UNKNOWN = 0
UNKNOWN_KNOWN = 0
engine = pyttsx3.init()
engine.setProperty('voice', 'zh')


def VOICE_TEXT(names):
    unknown_name = []
    known_name = []
    for name in names:
        if name == 'Unknown':
            unknown_name.append(name)
        else:
            known_name.append(name)
    # 记录两类人数量
    num_known = len(known_name)
    num_unknown = len(unknown_name)
    if len(names) == 0:
        return

    # 5s 播报一次
    elif num_unknown == 0:
        global KUNOWN

        KUNOWN += 1
        print('KUNOWN:', KUNOWN)
        if KUNOWN == 15:
            KUNOWN = 1

            text = '欢迎'
            for index in range(num_known):
                if index + 1 == num_known and index != 0:
                    text += '和{}'.format(known_name[index])
                else:
                    text += known_name[index]
            engine.say(text)
            engine.runAndWait()
    # 3s 播报一次
    elif num_known == 0:

        global UNKNOWN
        UNKNOWN += 1
        print('UNKNOWN', UNKNOWN)
        if UNKNOWN == 10 :
            UNKNOWN = 1
            text = '警告，警告，有陌生人未识别，有陌生人未识别'
            engine.say(text)
            engine.runAndWait()

    # 7s 播报一次
    else:
        global UNKNOWN_KNOWN
        UNKNOWN_KNOWN += 1
        print('UNKUOWN_KNOWN', UNKNOWN_KNOWN)
        if UNKNOWN_KNOWN == 25:
            UNKNOWN_KNOWN = 2
            text = '欢迎'
            for index in range(num_known):
                if index + 1 == num_known and index != 0:
                    text += '和{}'.format(known_name[index])
                else:
                    text += known_name[index]
            engine.say(text)
            engine.say('请注意， 您旁边有未识别人员，您将不允许进入，请询问值班人员，谢谢合作')
            engine.runAndWait()


if __name__ == '__main__':
    names = ['醉陌', '醉陌']

    VOICE_TEXT(names)
```



