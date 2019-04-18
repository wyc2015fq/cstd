# vi查找替换命令 - nosmatch的专栏 - CSDN博客
2011年07月20日 17:11:32[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：603

添加行号：
在VI的命令模式下输入“:set nu” 
或者修改vi配置文件“vi ~/.vimrc”，在其中添加“set nu” 
在VI的命令模式下输入“:set nu”，就有行号了。 
但是想将这个设置写进VI的配置文件，就 
# vi ~/.vimrc 
在这个文件中，添加 
set nu 
就行了 
set nu       -- to display line no
set nonu    -- not to display line no
ctrl + g 可以知道cursor的那一行的行数
shift+g可以到最后一行。在:下输入行号，可到达对应行
要自当前光标位置向上搜索，请使用以下命令：
/pattern Enter 
其中，pattern 表示要搜索的特定字符序列。
要自当前光标位置向下搜索，请使用以下命令：
?pattern Enter 
按下 Enter 键后，vi 将搜索指定的 pattern，并将光标定位在 pattern 的第一个字符处。
要搜索 place 的其他匹配，请按 n 或 N：
n，继续朝同一方向搜索 place。
N，反方向进行搜索。
如果 vi 未找到指定的 pattern，光标位置将不变，屏幕底部显示以下消息：
Pattern: 未找到
搜索特殊匹配 
在上面的示例中，vi 查找到包含 place 的任何序列，其中包括 displace、placement 和 replaced。
要查找单个的 place，请键入该单词，并在其前后各加一个空格：
/ place Enter 
要查找仅出现在行首的 place，请在该单词前加一个插字符号 (^)：
/^place Enter 
要查找仅出现在行尾的 place，请在该单词后加一个货币符号 ($)：
/place$ Enter 
使用 ^
要逐字搜索这种带有插字符号 (^) 或货币符号 ($) 的字符，请在字符前加一个反斜线 (/)。反斜线命令 vi 搜索特殊字符。
使用 $
特殊字符是指在 vi 中具有特殊功能的字符（例如 ^、$、*、/ 和 .）。例如，$ 通常表示“转至行尾”，但是，如果 $ 前紧跟一个 /，则 $ 只是一个普通的字符。
使用 /
例如，/(No /$ money) 向上搜索字符序列 (No $ money)。紧跟在 $ 之前的转义字符 (/) 命令 vi 逐字搜索货币符号。
撤销命令 ：
：u
恢复命令：
：red
