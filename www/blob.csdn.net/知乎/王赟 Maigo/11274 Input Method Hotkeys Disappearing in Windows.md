# 11274 Input Method Hotkeys Disappearing in Windows - 知乎
# 

　　This is an English translation of the following post:
[王赟 Maigo：10562 Windows 中输入法快捷键莫名消失​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-74d0db391334e0ade77446a5df01b116_ipico.jpg)](https://zhuanlan.zhihu.com/p/26033913)
　　This article describes a bug in Windows about the unexpected disappearance of input method hotkeys. I noticed this bug when I was using Windows 7; it still persists in Windows 10 (version 1803). It is hard to reproduce the bug reliably, nor have I found a solution. Despite multiple reports of the bug on the internet [1~6], the bug has not attracted enough attention due to the lack of details in the reports, and the small population of users that use multiple languages and input methods. I have filed a report to Windows Feedback Hub [[7]](https://link.zhihu.com/?target=https%3A//aka.ms/Crlmts) as well, but again I could not include enough details due to the space restriction. Recently the bug has received the attention of [@Belleve](https://www.zhihu.com/people/b6d28ac2b88b7f230552bab4a0aceaca), and I would like to thank him for passing this post on to the input method team of Microsoft.

## 1. Description of the Bug

　　I have installed multiple languages and input methods, as shown in Figure 1. In order to conveniently switch between the input methods, I have set hotkeys for the seven input methods I use most frequently. All the hotkeys are in the form of "Ctrl + Shift + number"; the numbers 4~9 and 0 correspond to French (Canada), Spanish, Sogou Wubi, Sogou Pinyin, Japanese, Korean, and English, respectively (Sogou Wubi and Sogou Pinyin are two input methods for Chinese).
![](https://pic3.zhimg.com/v2-74d0db391334e0ade77446a5df01b116_b.png)Figure 1. Dialog for setting input method hotkeys. To bring this out: right click on the language bar, then choose "settings", "advanced keyboard settings", "language bar options", "advanced key settings".
　　I have noticed that not long after I set the hotkeys (usually a few days), some of the hotkeys would stop working. Most often these are Ctrl + Shift + 4 (French) and Ctrl + Shift + 6 (Sogou Wubi). Some people have suggested that this usually happens after events such as "hibernating then waking up", "logging out then logging in", "powering off then turning on". According to my experience, these events are indeed correlated with hotkeys stopping working, but they are neither sufficient nor necessary conditions.

　　When hotkeys have stopped working, I need to reset them in the dialog in Figure 1. However, the hotkeys in the dialog do not always match those that are still valid. Sometimes Figure 1 still displays all the hotkeys even though some have stopped working; sometimes Figure 1 does not show all the hotkeys while all the hotkeys are still valid. If I reset the hotkeys and then click "OK" to close the dialog, there's a probability that some hotkeys still have no effect. If I open the dialog again, it may or may not display all the hotkeys.

## 2. Registry Entries Related to the Bug

　　I tried to pin down the reason of the bug. Some web searching revealed that the settings for input method hotkeys are stored in the registry at "HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys". After resetting the hotkeys, I found four entries at this location. For conciseness, I have gathered the contents of all the four entries in Figure 2.

![](https://pic1.zhimg.com/v2-c765f0d0b5fd6cfe652e3d2854f04060_b.png)Figure 2. Registry entries related to input method hotkeys
　　Each entry contains three binary values called "Key Modifiers", "Target IME", and "Virtual Key". "Target IME" is an ID for the input method to be switch to. It is in the little-endian format, so it needs to be read backwards. According to [[8]](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html), the meanings of the IDs can be found at "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts":
- "e0230804" stands for Sogou Wubi；
- I did not find "04090409" or "040a0c0a", but found "00000409" and "0000040a", and I suppose they stand for English and Spanish;
- I did not find "f0200c0c" nor anything similar.

According to the same post [[8]](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html), the hotkey itself is encoded in both "Key Modifiers" and "Virtual Key". The first byte of "Key Modifiers" specify the modifier keys; it is a sum of any subset of Shift (04), Ctrl (02), and Alt (01). The second byte specifies whether to use the modifier keys on the left side, the right side or either, and is a sum of any subset of left (80) and right (40). "Virtual Key" stands for the ASCII code of the key to be pressed with the modifier keys. Applying this analysis to Figure 2, the hotkeys represented by the four entries are:
- Ctrl (either side) + key with ASCII code of 0xDE -- I don't know what this means;
- Ctrl (either side) + Shift (either side) + 0, which I assigned to English；
- Ctrl (either side) + Shift (either side) + 4, which I assigned to French；
- Ctrl (either side) + Shift (either side) + 5, which I assigned to Spanish；

It can be seen that not all hotkeys are contained in the registry. An interesting fact is that Ctrl + Shift + 4, while contained in the registry, is one of the two most frequently disappearing hotkeys. On the other hand, Ctrl + Shift + 7/8/9, which almost never disappear, are not in the registry.

　　I tried to modify the hotkey settings in Figure 1 and see if anything changes in Figure 2, or, conversely, modify the registry and see if the hotkeys change in Figure 1. It turned out that most of the time the two did not respond to changes in each other; sometimes there were responses, but they did not agree with the modifications. This announced the failure of my efforts to find the cause of the bug via the registry.

## 3. A Partially Effective Hack

　　A hack has been proposed in [[1]](https://link.zhihu.com/?target=https%3A//answers.microsoft.com/en-us/windows/forum/windows_8-desktop/custom-hotkeys-to-change-input-language-disappear/66d1d89d-e5dc-41e1-a8b3-48d596ab8e11) and [[2]](https://link.zhihu.com/?target=https%3A//superuser.com/questions/1031736/language-hotkeys-removed-by-itself-in-windows-10): copying the language settings to the welcome screen, system accounts and new user accounts, as shown in Figure 3. I found that this could keep the hotkeys valid for one or two months, but ultimately they would disappear.

![](https://pic3.zhimg.com/v2-d8dfa36220bc1b80596001014682b8e2_b.png)Figure 3. Dialog for copying language settings. To bring this out: right click on the language bar, then choose "settings", "administrative language settings", "copy settings".
　　A look at the registry revealed that the hack copied the entry "HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys" to the subdirectories ".DEFAULT", "S-1-5-18", "S-1-5-19" and "S-1-5-20" under "HKEY_USERS". Originally this key only existed in the subdirectory "S-1-5-21-XXX", which is also the location pointed to by "HKEY_CURRENT_USER". It appears that the hack did copy language settings, but it is still a puzzle why it could keep the hotkeys valid longer. The hack is also unintuitive as a solution to the bug.

## References

[1] [Custom hotkeys to change input language disappear frequently](https://link.zhihu.com/?target=https%3A//answers.microsoft.com/en-us/windows/forum/windows_8-desktop/custom-hotkeys-to-change-input-language-disappear/66d1d89d-e5dc-41e1-a8b3-48d596ab8e11)
[2] [Language Hotkeys removed by itself in Windows 10](https://link.zhihu.com/?target=https%3A//superuser.com/questions/1031736/language-hotkeys-removed-by-itself-in-windows-10)
[3] [Input language and other Windows 7 hotkeys reset every time](https://link.zhihu.com/?target=https%3A//superuser.com/questions/529917/input-language-and-other-windows-7-hotkeys-reset-every-time)
[4] [https://aka.ms/W7brr2](https://link.zhihu.com/?target=https%3A//aka.ms/W7brr2)
[5] [https://aka.ms/Yifkcc](https://link.zhihu.com/?target=https%3A//aka.ms/Yifkcc)
[6] [https://aka.ms/Luk7jg](https://link.zhihu.com/?target=https%3A//aka.ms/Luk7jg)
[7] [https://aka.ms/Crlmts](https://link.zhihu.com/?target=https%3A//aka.ms/Crlmts)
[8] [彻底摆脱SHIFT的阴影！--用注册表修改输入法热键 - carprog - 博客园](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html)

