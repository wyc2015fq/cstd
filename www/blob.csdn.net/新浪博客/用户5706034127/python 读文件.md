# python 读文件_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
知识点：二进制文件读取，特殊控制字符
从网络上下载了一个网页，里面url包含了控制字符，所以必须用二进制的读取方式。
//a.txt
abc[sub]xyz\26 1234
fp = open('a.txt','r')
print fp.readlines()  #
只能读取abc三个字符
fp = open('a.txt','rb')
print fp.readlines()  #
就能读取全部字符
因为\26或者说sub控制字符等价于EOF
参见wikipedia
The control characters in ASCII still in common use include:
- ); WIDOWS: 1; LETTER-SPACING: normal;
PADDING-RIGHT: 0px; BACKGROUND-COLOR: rgb(255,255,255);
TEXT-INDENT: 0px; -webkit-text-stroke-width: 0px">
- 0 ([null](https://en.wikipedia.org/wiki/Null_character),`NUL`,`[\0](https://en.wikipedia.org/wiki//0)`,`[^@](https://en.wikipedia.org/wiki/^@)`), originally
intended to be an ignored character, but now used by
many[programming
languages](https://en.wikipedia.org/wiki/Programming_language)to mark the end of a
string.
- 7 ([bell](https://en.wikipedia.org/wiki/Bell_character),`BEL`,`[\a](https://en.wikipedia.org/wiki//a)`,`[^G](https://en.wikipedia.org/wiki/^G)`), which may
cause the device receiving it to emit a warning of some kind
(usually audible).
- 8 ([backspace](https://en.wikipedia.org/wiki/Backspace),`BS`,`\b`,`[^H](https://en.wikipedia.org/wiki/^H)`), used either
to erase the last character printed or to overprint it.
- 9 ([horizontal
tab](https://en.wikipedia.org/wiki/Tab_key),`HT`,`[\t](https://en.wikipedia.org/wiki//t)`,`[^I](https://en.wikipedia.org/wiki/^I)`), moves the
printing position some spaces to the right.
- 10 ([line
feed](https://en.wikipedia.org/wiki/Newline),`LF`,`[\n](https://en.wikipedia.org/wiki//n)`,`[^J](https://en.wikipedia.org/wiki/^J)`), used as the
end of line marker in most[UNIX
systems](https://en.wikipedia.org/wiki/Unix)and variants.
- 11 ([vertical
tab](https://en.wikipedia.org/wiki/Tab_key),`VT`,`[\v](https://en.wikipedia.org/wiki//v)`,`[^K](https://en.wikipedia.org/wiki/^K)`), vertical
tabulation.
- 12 ([form
feed](https://en.wikipedia.org/wiki/Page_break),`FF`,`[\f](https://en.wikipedia.org/wiki//f)`,`[^L](https://en.wikipedia.org/wiki/^L)`), to cause a
printer to eject paper to the top of the next page, or a video
terminal to clear the screen.
- 13 ([carriage
return](https://en.wikipedia.org/wiki/Carriage_return),`CR`,`[\r](https://en.wikipedia.org/wiki//r)`,`[^M](https://en.wikipedia.org/wiki/^M)`), used as the
end of line marker in[Classic Mac
OS](https://en.wikipedia.org/wiki/Classic_Mac_OS),[OS-9](https://en.wikipedia.org/wiki/OS-9),[FLEX](https://en.wikipedia.org/wiki/FLEX_(operating_system))(and
variants). A carriage return/line feed pair is used by[CP/M](https://en.wikipedia.org/wiki/CP/M)-80 and its
derivatives including[DOS](https://en.wikipedia.org/wiki/DOS)and[Windows](https://en.wikipedia.org/wiki/Microsoft_Windows),
and by[Application
Layer](https://en.wikipedia.org/wiki/Application_Layer)[protocols](https://en.wikipedia.org/wiki/Communications_protocol)such
as[HTTP](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol).
- 26 ([Control-Z](https://en.wikipedia.org/wiki/Control-Z),`SUB`,`EOF`,`[^Z](https://en.wikipedia.org/wiki/^Z)`).
- 27 ([escape](https://en.wikipedia.org/wiki/Escape_character),`ESC`,`[\e](https://en.wikipedia.org/wiki//e)`([GCC](https://en.wikipedia.org/wiki/GCC_(software))only),`^[`).
- 127 ([delete](https://en.wikipedia.org/wiki/Delete_character),`DEL`,`[^?](https://en.wikipedia.org/wiki/^?)`), originally
intended to be an ignored character, but now used in some systems
to erase a character. Also used by some[Plan9](https://en.wikipedia.org/wiki/Plan_9_from_Bell_Labs)console
programs to send an interrupt note to the current process.
fp = open('new 3.txt')
# fp = sys.stdin # 读文件，读标准输入都是一样的
for line in fp:
print line
lines = fp.readlines()
for line in lines:
print line
line = fp.readline()
while line:
print line
line = fp.readline()
但是下面这种是不行的：
while True:
try:
line = fp.readline()
 print line
except EOFError:
 break
会一直打印空字符串，，读到文件末尾后，是空字符串。
