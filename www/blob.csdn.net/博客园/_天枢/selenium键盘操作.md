# selenium键盘操作 - _天枢 - 博客园
## [selenium键盘操作](https://www.cnblogs.com/yhleng/p/9177910.html)
2018-06-13 15:08 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9177910)
键盘操作需引用:
```
from selenium.webdriver.common.keys import Keys
```
操作码表:
```
NULL = '\ue000'
    CANCEL = '\ue001'  # ^break
    HELP = '\ue002'
    BACKSPACE = '\ue003'
    BACK_SPACE = BACKSPACE   #删除键
    TAB = '\ue004'   #TAB键
    CLEAR = '\ue005'  
    RETURN = '\ue006'
    ENTER = '\ue007'   #回车键
    SHIFT = '\ue008'   #Shift键
    LEFT_SHIFT = SHIFT
    CONTROL = '\ue009'
    LEFT_CONTROL = CONTROL   #Ctrl 键
    ALT = '\ue00a'           #Alt 键
    LEFT_ALT = ALT
    PAUSE = '\ue00b'
    ESCAPE = '\ue00c'   #ECS键
    SPACE = '\ue00d'    #空格键 
    PAGE_UP = '\ue00e'   #PgUp 键
    PAGE_DOWN = '\ue00f' #PgDwon 键
    END = '\ue010'    #END 键
    HOME = '\ue011'   #HOME 键
    LEFT = '\ue012'  #左键
    ARROW_LEFT = LEFT  
    UP = '\ue013'    #上键
    ARROW_UP = UP   
    RIGHT = '\ue014'
    ARROW_RIGHT = RIGHT  #右键
    DOWN = '\ue015'      #下键
    ARROW_DOWN = DOWN  
    INSERT = '\ue016'    #insert键 
    DELETE = '\ue017'    #del键
    SEMICOLON = '\ue018'  #';'键
    EQUALS = '\ue019'     #'='键
　　#数字键盘
    NUMPAD0 = '\ue01a'  # number pad keys
    NUMPAD1 = '\ue01b'
    NUMPAD2 = '\ue01c'
    NUMPAD3 = '\ue01d'
    NUMPAD4 = '\ue01e'
    NUMPAD5 = '\ue01f'
    NUMPAD6 = '\ue020'
    NUMPAD7 = '\ue021'
    NUMPAD8 = '\ue022'
    NUMPAD9 = '\ue023'
    MULTIPLY = '\ue024' # '*' 键
    ADD = '\ue025'   # '+' 键
    SEPARATOR = '\ue026'  #','键
    SUBTRACT = '\ue027'  # '-' 键
    DECIMAL = '\ue028'   # '.'键
    DIVIDE = '\ue029'    #'/'键
    F1 = '\ue031'  # function  keys
    F2 = '\ue032'
    F3 = '\ue033'
    F4 = '\ue034'
    F5 = '\ue035'
    F6 = '\ue036'
    F7 = '\ue037'
    F8 = '\ue038'
    F9 = '\ue039'
    F10 = '\ue03a'
    F11 = '\ue03b'
    F12 = '\ue03c'
    META = '\ue03d'
    COMMAND = '\ue03d'
```
组合按键:
```
在使用按键操作的时候我们需要借助一下send_keys()来模拟操作，Keys.CONTROL 也就是我们键盘上的Ctrl键，下面是几个常用的组合键。
send_keys(Keys.CONTROL,'a') 　　#全选（Ctrl+A）
send_keys(Keys.CONTROL,'c') 　　#复制（Ctrl+C）
send_keys(Keys.CONTROL,'x') 　　#剪切（Ctrl+X）
send_keys(Keys.CONTROL,'v') 　　#粘贴（Ctrl+V）
```
非组合常用键:
```
回车键 Keys.ENTER
    删除键 Keys.BACK_SPACE
    空格键 Keys.SPACE
    制表键 Keys.TAB
    回退键 Keys.ESCAPE
    刷新键 Keys.F5
```
