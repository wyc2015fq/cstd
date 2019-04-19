# source insight 汉字问题的解决方案与函数调用关系图配置 - xqhrs232的专栏 - CSDN博客
2013年11月02日 10:58:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1157
原文地址::[http://hi.baidu.com/a263238386/item/33022fdecfe36d14e1f46f29](http://hi.baidu.com/a263238386/item/33022fdecfe36d14e1f46f29)
【问题】
Source Insight里输入中文，字间距相当的大。
【答案】
1.Options->Style Properties
2. 在左边Style Name下找到Comment Multi Line和Comment.在其右边对应的Font属性框下的Font Name中选“Pick…” 设置为宋体、常规、小四。确定，退回Style Properties界面，Size设为10。最后设置Clolors框下Foreground，点“Pick…”选择一种自己喜欢的颜色就OK了。
![](http://hiphotos.baidu.com/a263238386/pic/item/f2e045444a0eee21530ffe5d.jpg)
3.Done
【问题】
删除半个汉字
【答案】
① 复制入SourceInsight安装目录；
② Project→Open Project，打开Base项目；
③ 将复制过去的SuperBackspace.em添加入Base项目；
④ 重启SourceInsight；
⑤ Options→Key Assignments，将Marco: SuperBackspace绑定到BackSpace键；
⑥ Enjoy！！
【问题】
汉字字体大小
【答案】
在Option里的Document Option（ALT+T）
左边有个Screen Font，点进去就可以设置了===================SuperBackspace.em============================================
/* 代替SourceInsight原有的Backspace功能（希望如此）
* 增加了对双字节汉字的支持，在删除汉字的时候也能同时删除汉字的高字节而缓解半个汉字问题
* 能够对光标在汉字中间的情况进行自动修正
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
macro SuperBackspace()
{
hwnd = GetCurrentWnd();
hbuf = GetCurrentBuf(); if (hbuf == 0)
stop; // empty buffer // get current cursor postion
ipos = GetWndSelIchFirst(hwnd); // get current line number
ln = GetBufLnCur(hbuf); if ((GetBufSelText(hbuf) != “”) || (GetWndSelLnFirst(hwnd) != GetWndSelLnLast(hwnd))) {
// sth. was selected, del selection
SetBufSelText(hbuf, ” “); // stupid & buggy sourceinsight :(
// del the ” ”
SuperBackspace(1);
stop;
} // copy current line
text = GetBufLine(hbuf, ln); // get string length
len = strlen(text); // if the cursor is at the start of line, combine with prev line
if (ipos == 0 || len == 0) {
if (ln <= 0)
stop; // top of file
ln = ln – 1; // do not use “ln–” for compatibility with older versions
prevline = GetBufLine(hbuf, ln);
prevlen = strlen(prevline);
// combine two lines
text = cat(prevline, text);
// del two lines
DelBufLine(hbuf, ln);
DelBufLine(hbuf, ln);
// insert the combined one
InsBufLine(hbuf, ln, text);
// set the cursor position
SetBufIns(hbuf, ln, prevlen);
stop;
} num = 1; // del one char
if (ipos >= 1) {
// process Chinese character
i = ipos;
count = 0;
while (AsciiFromChar(text[i - 1]) >= 160) {
i = i – 1;
count = count + 1;
if (i == 0)
break;
}
if (count > 0) {
// I think it might be a two-byte character
num = 2;
// This idiot does not support mod and bitwise operators
if ((count / 2 * 2 != count) && (ipos < len))
ipos = ipos + 1; // adjust cursor position
}
} // keeping safe
if (ipos – num < 0)
num = ipos;
// del char(s)
text = cat(strmid(text, 0, ipos – num), strmid(text, ipos, len));
DelBufLine(hbuf, ln);
InsBufLine(hbuf, ln, text);
SetBufIns(hbuf, ln, ipos – num);
stop;
}=========================================================================
Source Insight 函数调用关系图 / 被调用关系图的排序问题
<1> 右下角的调用关系图 Calls
默认情况下，上一级函数调用下一级函数的的出现次序是按照字母升序排列的，在查看时非常费眼力。如何让下一级函数的调用次序按照上一级的函数的调用先后次序排列呢。如下操作(1)和(2),其中(2)还能按照其他次序排列，这点非常方便。
![](http://hiphotos.baidu.com/a263238386/pic/item/728756ee1a17cf442cf5342f.jpg)
<2> 查看被调用关系
在Relation window窗口右键，选relation window properties，其中的For Function = referenced by function就可以了（本来是Calls）。![image](http://hiphotos.baidu.com/a263238386/pic/item/799729f8b8baa562dbf9fd5d.jpg)
