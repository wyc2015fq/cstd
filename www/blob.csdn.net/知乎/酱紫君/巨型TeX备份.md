# 巨型TeX备份 - 知乎
# 

本文用于备份 [酱紫君：知乎上的公式是怎么打出来的？](https://www.zhihu.com/question/31298277/answer/275151599)中的超巨型TeX代码...

防止不可抗力导致某天我要重新输一遍...

当然我写到一半我就意识到我是个白痴我为什么不用宏...
- 字体表

```
\begin{array}{ll|l}
 \texttt{"normal"}      &\texttt{}         & ABCDEFGHIJKLMNOPQRSTUVWXYZ\\
 \texttt{"blackboard"}  &\texttt{\mathbb}  &\mathbb{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"boldface"}    &\texttt{\mathbf}  &\mathbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"typewriter"}  &\texttt{\mathtt}  &\mathtt{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"roman"}       &\texttt{\mathrm}  &\mathrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"sans-serif"}  &\texttt{\mathsf}  &\mathsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"calligraphic"}&\texttt{\mathcal} &\mathcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"script"}      &\texttt{\mathscr} &\mathscr{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
 \texttt{"fraktur"}     &\texttt{\mathfrak}&\mathfrak{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\
\end{array}
```

![\begin{array}{ll|l} \texttt{"normal"} &\texttt{} & ABCDEFGHIJKLMNOPQRSTUVWXYZ\\ \texttt{"blackboard"} &\texttt{\mathbb} &\mathbb{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"boldface"} &\texttt{\mathbf} &\mathbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"typewriter"} &\texttt{\mathtt} &\mathtt{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"roman"} &\texttt{\mathrm} &\mathrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"sans-serif"} &\texttt{\mathsf} &\mathsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"calligraphic"}&\texttt{\mathcal} &\mathcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"script"} &\texttt{\mathscr} &\mathscr{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \texttt{"fraktur"} &\texttt{\mathfrak}&\mathfrak{ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\ \end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bll%7Cl%7D+%5Ctexttt%7B%22normal%22%7D+%26%5Ctexttt%7B%7D+%26+ABCDEFGHIJKLMNOPQRSTUVWXYZ%5C%5C+%5Ctexttt%7B%22blackboard%22%7D+%26%5Ctexttt%7B%5Cmathbb%7D+%26%5Cmathbb%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22boldface%22%7D+%26%5Ctexttt%7B%5Cmathbf%7D+%26%5Cmathbf%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22typewriter%22%7D+%26%5Ctexttt%7B%5Cmathtt%7D+%26%5Cmathtt%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22roman%22%7D+%26%5Ctexttt%7B%5Cmathrm%7D+%26%5Cmathrm%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22sans-serif%22%7D+%26%5Ctexttt%7B%5Cmathsf%7D+%26%5Cmathsf%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22calligraphic%22%7D%26%5Ctexttt%7B%5Cmathcal%7D+%26%5Cmathcal%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22script%22%7D+%26%5Ctexttt%7B%5Cmathscr%7D+%26%5Cmathscr%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Ctexttt%7B%22fraktur%22%7D+%26%5Ctexttt%7B%5Cmathfrak%7D%26%5Cmathfrak%7BABCDEFGHIJKLMNOPQRSTUVWXYZ%7D%5C%5C+%5Cend%7Barray%7D)
- 色彩表

```
\begin{array}{|lc|}
\hline
 \verb+\color{black}{黑色}+    & \color{black}{黑色}   \\
 \verb+\color{darkgray}{深灰}+ & \color{darkgray}{深灰}\\
 \verb+\color{gray}{灰色}+     & \color{gray}{灰色}    \\
 \verb+\color{silver}{银色}+   & \color{silver}{银色}  \\
 \verb+\color{lightgray}{浅灰}+&\color{lightgray}{浅灰}\\
 \verb+\color{white}{白色}+    & \color{white}{白色}   \\
\hline
 \verb+\color{brown}{棕色}+    & \color{brown}{棕色}   \\
 \verb+\color{maroon}{栗色}+   & \color{maroon}{栗色}  \\
 \verb+\color{red}{红色}+      & \color{red}{红色}     \\
 \verb+\color{fuchsia}{桃红}+  & \color{fuchsia}{桃红} \\
 \verb+\color{magenta}{品红}+  & \color{magenta}{品红} \\
 \verb+\color{pink}{粉红}+     & \color{pink}{粉红}    \\
 \verb+\color{orange}{橙色}+   & \color{orange}{橙色}  \\
 \verb+\color{yellow}{黄色}+   & \color{yellow}{黄色}  \\
 \verb+\color{lime}{青柠}+     & \color{lime}{青柠}    \\
 \verb+\color{olive}{橄榄}+    & \color{olive}{橄榄}   \\
 \verb+\color{green}{绿色}+    & \color{green}{绿色}   \\
 \verb+\color{aqua}{水绿}+     & \color{aqua}{水绿}    \\
 \verb+\color{cyan}{青色}+     & \color{cyan}{青色}    \\
 \verb+\color{teal}{靛青}+     & \color{teal}{靛青}    \\
 \verb+\color{blue}{蓝色}+     & \color{blue}{蓝色}    \\
 \verb+\color{navy}{海蓝}+     & \color{navy}{海蓝}    \\
 \verb+\color{violet}{罗兰}+   & \color{purple}{罗兰}  \\ 
 \verb+\color{purple}{紫色}+   & \color{purple}{紫色}  \\ 
\hline
\end{array}
```

![\begin{array}{|lc|} \hline \verb+\color{black}{黑色}+ & \color{black}{黑色} \\ \verb+\color{darkgray}{深灰}+ & \color{darkgray}{深灰}\\ \verb+\color{gray}{灰色}+ & \color{gray}{灰色} \\ \verb+\color{silver}{银色}+ & \color{silver}{银色} \\ \verb+\color{lightgray}{浅灰}+&\color{lightgray}{浅灰}\\ \verb+\color{white}{白色}+ & \color{white}{白色} \\ \hline \verb+\color{brown}{棕色}+ & \color{brown}{棕色} \\ \verb+\color{maroon}{栗色}+ & \color{maroon}{栗色} \\ \verb+\color{red}{红色}+ & \color{red}{红色} \\ \verb+\color{fuchsia}{桃红}+ & \color{fuchsia}{桃红} \\ \verb+\color{magenta}{品红}+ & \color{magenta}{品红} \\ \verb+\color{pink}{粉红}+ & \color{pink}{粉红} \\ \verb+\color{orange}{橙色}+ & \color{orange}{橙色} \\ \verb+\color{yellow}{黄色}+ & \color{yellow}{黄色} \\ \verb+\color{lime}{青柠}+ & \color{lime}{青柠} \\ \verb+\color{olive}{橄榄}+ & \color{olive}{橄榄} \\ \verb+\color{green}{绿色}+ & \color{green}{绿色} \\ \verb+\color{aqua}{水绿}+ & \color{aqua}{水绿} \\ \verb+\color{cyan}{青色}+ & \color{cyan}{青色} \\ \verb+\color{teal}{靛青}+ & \color{teal}{靛青} \\ \verb+\color{blue}{蓝色}+ & \color{blue}{蓝色} \\ \verb+\color{navy}{海蓝}+ & \color{navy}{海蓝} \\ \verb+\color{violet}{罗兰}+ & \color{purple}{罗兰} \\ \verb+\color{purple}{紫色}+ & \color{purple}{紫色} \\ \hline \end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7B%7Clc%7C%7D+%5Chline+%5Cverb%2B%5Ccolor%7Bblack%7D%7B%E9%BB%91%E8%89%B2%7D%2B+%26+%5Ccolor%7Bblack%7D%7B%E9%BB%91%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bdarkgray%7D%7B%E6%B7%B1%E7%81%B0%7D%2B+%26+%5Ccolor%7Bdarkgray%7D%7B%E6%B7%B1%E7%81%B0%7D%5C%5C+%5Cverb%2B%5Ccolor%7Bgray%7D%7B%E7%81%B0%E8%89%B2%7D%2B+%26+%5Ccolor%7Bgray%7D%7B%E7%81%B0%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bsilver%7D%7B%E9%93%B6%E8%89%B2%7D%2B+%26+%5Ccolor%7Bsilver%7D%7B%E9%93%B6%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Blightgray%7D%7B%E6%B5%85%E7%81%B0%7D%2B%26%5Ccolor%7Blightgray%7D%7B%E6%B5%85%E7%81%B0%7D%5C%5C+%5Cverb%2B%5Ccolor%7Bwhite%7D%7B%E7%99%BD%E8%89%B2%7D%2B+%26+%5Ccolor%7Bwhite%7D%7B%E7%99%BD%E8%89%B2%7D+%5C%5C+%5Chline+%5Cverb%2B%5Ccolor%7Bbrown%7D%7B%E6%A3%95%E8%89%B2%7D%2B+%26+%5Ccolor%7Bbrown%7D%7B%E6%A3%95%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bmaroon%7D%7B%E6%A0%97%E8%89%B2%7D%2B+%26+%5Ccolor%7Bmaroon%7D%7B%E6%A0%97%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bred%7D%7B%E7%BA%A2%E8%89%B2%7D%2B+%26+%5Ccolor%7Bred%7D%7B%E7%BA%A2%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bfuchsia%7D%7B%E6%A1%83%E7%BA%A2%7D%2B+%26+%5Ccolor%7Bfuchsia%7D%7B%E6%A1%83%E7%BA%A2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bmagenta%7D%7B%E5%93%81%E7%BA%A2%7D%2B+%26+%5Ccolor%7Bmagenta%7D%7B%E5%93%81%E7%BA%A2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bpink%7D%7B%E7%B2%89%E7%BA%A2%7D%2B+%26+%5Ccolor%7Bpink%7D%7B%E7%B2%89%E7%BA%A2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Borange%7D%7B%E6%A9%99%E8%89%B2%7D%2B+%26+%5Ccolor%7Borange%7D%7B%E6%A9%99%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Byellow%7D%7B%E9%BB%84%E8%89%B2%7D%2B+%26+%5Ccolor%7Byellow%7D%7B%E9%BB%84%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Blime%7D%7B%E9%9D%92%E6%9F%A0%7D%2B+%26+%5Ccolor%7Blime%7D%7B%E9%9D%92%E6%9F%A0%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bolive%7D%7B%E6%A9%84%E6%A6%84%7D%2B+%26+%5Ccolor%7Bolive%7D%7B%E6%A9%84%E6%A6%84%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bgreen%7D%7B%E7%BB%BF%E8%89%B2%7D%2B+%26+%5Ccolor%7Bgreen%7D%7B%E7%BB%BF%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Baqua%7D%7B%E6%B0%B4%E7%BB%BF%7D%2B+%26+%5Ccolor%7Baqua%7D%7B%E6%B0%B4%E7%BB%BF%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bcyan%7D%7B%E9%9D%92%E8%89%B2%7D%2B+%26+%5Ccolor%7Bcyan%7D%7B%E9%9D%92%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bteal%7D%7B%E9%9D%9B%E9%9D%92%7D%2B+%26+%5Ccolor%7Bteal%7D%7B%E9%9D%9B%E9%9D%92%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bblue%7D%7B%E8%93%9D%E8%89%B2%7D%2B+%26+%5Ccolor%7Bblue%7D%7B%E8%93%9D%E8%89%B2%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bnavy%7D%7B%E6%B5%B7%E8%93%9D%7D%2B+%26+%5Ccolor%7Bnavy%7D%7B%E6%B5%B7%E8%93%9D%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bviolet%7D%7B%E7%BD%97%E5%85%B0%7D%2B+%26+%5Ccolor%7Bpurple%7D%7B%E7%BD%97%E5%85%B0%7D+%5C%5C+%5Cverb%2B%5Ccolor%7Bpurple%7D%7B%E7%B4%AB%E8%89%B2%7D%2B+%26+%5Ccolor%7Bpurple%7D%7B%E7%B4%AB%E8%89%B2%7D+%5C%5C+%5Chline+%5Cend%7Barray%7D)

感觉很多颜色一样的话...

你可能是色盲(完了就是我)

```
\begin{array}{|rrrrrrrr|}
\hline
\verb+#000+ & \color{#000}{text} & \verb+#005+ & \color{#005}{text} & \verb+#00A+ & \color{#00A}{text} & \verb+#00F+ & \color{#00F}{text}  \\
\verb+#500+ & \color{#500}{text} & \verb+#505+ & \color{#505}{text} & \verb+#50A+ & \color{#50A}{text} & \verb+#50F+ & \color{#50F}{text}  \\
\verb+#A00+ & \color{#A00}{text} & \verb+#A05+ & \color{#A05}{text} & \verb+#A0A+ & \color{#A0A}{text} & \verb+#A0F+ & \color{#A0F}{text}  \\
\verb+#F00+ & \color{#F00}{text} & \verb+#F05+ & \color{#F05}{text} & \verb+#F0A+ & \color{#F0A}{text} & \verb+#F0F+ & \color{#F0F}{text}  \\
\hline
\verb+#080+ & \color{#080}{text} & \verb+#085+ & \color{#085}{text} & \verb+#08A+ & \color{#08A}{text} & \verb+#08F+ & \color{#08F}{text}  \\
\verb+#580+ & \color{#580}{text} & \verb+#585+ & \color{#585}{text} & \verb+#58A+ & \color{#58A}{text} & \verb+#58F+ & \color{#58F}{text}  \\
\verb+#A80+ & \color{#A80}{text} & \verb+#A85+ & \color{#A85}{text} & \verb+#A8A+ & \color{#A8A}{text} & \verb+#A8F+ & \color{#A8F}{text}  \\
\verb+#F80+ & \color{#F80}{text} & \verb+#F85+ & \color{#F85}{text} & \verb+#F8A+ & \color{#F8A}{text} & \verb+#F8F+ & \color{#F8F}{text}  \\
\hline
\verb+#0F0+ & \color{#0F0}{text} & \verb+#0F5+ & \color{#0F5}{text} & \verb+#0FA+ & \color{#0FA}{text} & \verb+#0FF+ & \color{#0FF}{text}  \\
\verb+#5F0+ & \color{#5F0}{text} & \verb+#5F5+ & \color{#5F5}{text} & \verb+#5FA+ & \color{#5FA}{text} & \verb+#5FF+ & \color{#5FF}{text}  \\
\verb+#AF0+ & \color{#AF0}{text} & \verb+#AF5+ & \color{#AF5}{text} & \verb+#AFA+ & \color{#AFA}{text} & \verb+#AFF+ & \color{#AFF}{text}  \\
\verb+#FF0+ & \color{#FF0}{text} & \verb+#FF5+ & \color{#FF5}{text} & \verb+#FFA+ & \color{#FFA}{text} & \verb+#FFF+ & \color{#FFF}{text}  \\
\hline
\end{array}
```

![\begin{array}{|rrrrrrrr|} \hline \verb+#000+ & \color{#000}{text} & \verb+#005+ & \color{#005}{text} & \verb+#00A+ & \color{#00A}{text} & \verb+#00F+ & \color{#00F}{text} \\ \verb+#500+ & \color{#500}{text} & \verb+#505+ & \color{#505}{text} & \verb+#50A+ & \color{#50A}{text} & \verb+#50F+ & \color{#50F}{text} \\ \verb+#A00+ & \color{#A00}{text} & \verb+#A05+ & \color{#A05}{text} & \verb+#A0A+ & \color{#A0A}{text} & \verb+#A0F+ & \color{#A0F}{text} \\ \verb+#F00+ & \color{#F00}{text} & \verb+#F05+ & \color{#F05}{text} & \verb+#F0A+ & \color{#F0A}{text} & \verb+#F0F+ & \color{#F0F}{text} \\ \hline \verb+#080+ & \color{#080}{text} & \verb+#085+ & \color{#085}{text} & \verb+#08A+ & \color{#08A}{text} & \verb+#08F+ & \color{#08F}{text} \\ \verb+#580+ & \color{#580}{text} & \verb+#585+ & \color{#585}{text} & \verb+#58A+ & \color{#58A}{text} & \verb+#58F+ & \color{#58F}{text} \\ \verb+#A80+ & \color{#A80}{text} & \verb+#A85+ & \color{#A85}{text} & \verb+#A8A+ & \color{#A8A}{text} & \verb+#A8F+ & \color{#A8F}{text} \\ \verb+#F80+ & \color{#F80}{text} & \verb+#F85+ & \color{#F85}{text} & \verb+#F8A+ & \color{#F8A}{text} & \verb+#F8F+ & \color{#F8F}{text} \\ \hline \verb+#0F0+ & \color{#0F0}{text} & \verb+#0F5+ & \color{#0F5}{text} & \verb+#0FA+ & \color{#0FA}{text} & \verb+#0FF+ & \color{#0FF}{text} \\ \verb+#5F0+ & \color{#5F0}{text} & \verb+#5F5+ & \color{#5F5}{text} & \verb+#5FA+ & \color{#5FA}{text} & \verb+#5FF+ & \color{#5FF}{text} \\ \verb+#AF0+ & \color{#AF0}{text} & \verb+#AF5+ & \color{#AF5}{text} & \verb+#AFA+ & \color{#AFA}{text} & \verb+#AFF+ & \color{#AFF}{text} \\ \verb+#FF0+ & \color{#FF0}{text} & \verb+#FF5+ & \color{#FF5}{text} & \verb+#FFA+ & \color{#FFA}{text} & \verb+#FFF+ & \color{#FFF}{text} \\ \hline \end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7B%7Crrrrrrrr%7C%7D+%5Chline+%5Cverb%2B%23000%2B+%26+%5Ccolor%7B%23000%7D%7Btext%7D+%26+%5Cverb%2B%23005%2B+%26+%5Ccolor%7B%23005%7D%7Btext%7D+%26+%5Cverb%2B%2300A%2B+%26+%5Ccolor%7B%2300A%7D%7Btext%7D+%26+%5Cverb%2B%2300F%2B+%26+%5Ccolor%7B%2300F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23500%2B+%26+%5Ccolor%7B%23500%7D%7Btext%7D+%26+%5Cverb%2B%23505%2B+%26+%5Ccolor%7B%23505%7D%7Btext%7D+%26+%5Cverb%2B%2350A%2B+%26+%5Ccolor%7B%2350A%7D%7Btext%7D+%26+%5Cverb%2B%2350F%2B+%26+%5Ccolor%7B%2350F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23A00%2B+%26+%5Ccolor%7B%23A00%7D%7Btext%7D+%26+%5Cverb%2B%23A05%2B+%26+%5Ccolor%7B%23A05%7D%7Btext%7D+%26+%5Cverb%2B%23A0A%2B+%26+%5Ccolor%7B%23A0A%7D%7Btext%7D+%26+%5Cverb%2B%23A0F%2B+%26+%5Ccolor%7B%23A0F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23F00%2B+%26+%5Ccolor%7B%23F00%7D%7Btext%7D+%26+%5Cverb%2B%23F05%2B+%26+%5Ccolor%7B%23F05%7D%7Btext%7D+%26+%5Cverb%2B%23F0A%2B+%26+%5Ccolor%7B%23F0A%7D%7Btext%7D+%26+%5Cverb%2B%23F0F%2B+%26+%5Ccolor%7B%23F0F%7D%7Btext%7D+%5C%5C+%5Chline+%5Cverb%2B%23080%2B+%26+%5Ccolor%7B%23080%7D%7Btext%7D+%26+%5Cverb%2B%23085%2B+%26+%5Ccolor%7B%23085%7D%7Btext%7D+%26+%5Cverb%2B%2308A%2B+%26+%5Ccolor%7B%2308A%7D%7Btext%7D+%26+%5Cverb%2B%2308F%2B+%26+%5Ccolor%7B%2308F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23580%2B+%26+%5Ccolor%7B%23580%7D%7Btext%7D+%26+%5Cverb%2B%23585%2B+%26+%5Ccolor%7B%23585%7D%7Btext%7D+%26+%5Cverb%2B%2358A%2B+%26+%5Ccolor%7B%2358A%7D%7Btext%7D+%26+%5Cverb%2B%2358F%2B+%26+%5Ccolor%7B%2358F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23A80%2B+%26+%5Ccolor%7B%23A80%7D%7Btext%7D+%26+%5Cverb%2B%23A85%2B+%26+%5Ccolor%7B%23A85%7D%7Btext%7D+%26+%5Cverb%2B%23A8A%2B+%26+%5Ccolor%7B%23A8A%7D%7Btext%7D+%26+%5Cverb%2B%23A8F%2B+%26+%5Ccolor%7B%23A8F%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23F80%2B+%26+%5Ccolor%7B%23F80%7D%7Btext%7D+%26+%5Cverb%2B%23F85%2B+%26+%5Ccolor%7B%23F85%7D%7Btext%7D+%26+%5Cverb%2B%23F8A%2B+%26+%5Ccolor%7B%23F8A%7D%7Btext%7D+%26+%5Cverb%2B%23F8F%2B+%26+%5Ccolor%7B%23F8F%7D%7Btext%7D+%5C%5C+%5Chline+%5Cverb%2B%230F0%2B+%26+%5Ccolor%7B%230F0%7D%7Btext%7D+%26+%5Cverb%2B%230F5%2B+%26+%5Ccolor%7B%230F5%7D%7Btext%7D+%26+%5Cverb%2B%230FA%2B+%26+%5Ccolor%7B%230FA%7D%7Btext%7D+%26+%5Cverb%2B%230FF%2B+%26+%5Ccolor%7B%230FF%7D%7Btext%7D+%5C%5C+%5Cverb%2B%235F0%2B+%26+%5Ccolor%7B%235F0%7D%7Btext%7D+%26+%5Cverb%2B%235F5%2B+%26+%5Ccolor%7B%235F5%7D%7Btext%7D+%26+%5Cverb%2B%235FA%2B+%26+%5Ccolor%7B%235FA%7D%7Btext%7D+%26+%5Cverb%2B%235FF%2B+%26+%5Ccolor%7B%235FF%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23AF0%2B+%26+%5Ccolor%7B%23AF0%7D%7Btext%7D+%26+%5Cverb%2B%23AF5%2B+%26+%5Ccolor%7B%23AF5%7D%7Btext%7D+%26+%5Cverb%2B%23AFA%2B+%26+%5Ccolor%7B%23AFA%7D%7Btext%7D+%26+%5Cverb%2B%23AFF%2B+%26+%5Ccolor%7B%23AFF%7D%7Btext%7D+%5C%5C+%5Cverb%2B%23FF0%2B+%26+%5Ccolor%7B%23FF0%7D%7Btext%7D+%26+%5Cverb%2B%23FF5%2B+%26+%5Ccolor%7B%23FF5%7D%7Btext%7D+%26+%5Cverb%2B%23FFA%2B+%26+%5Ccolor%7B%23FFA%7D%7Btext%7D+%26+%5Cverb%2B%23FFF%2B+%26+%5Ccolor%7B%23FFF%7D%7Btext%7D+%5C%5C+%5Chline+%5Cend%7Barray%7D)
- 连分数

```
x = a_0 + \cfrac{1^2}{a_1
        + \cfrac{2^2}{a_2
        + \cfrac{3^2}{a_3 
        + \cfrac{4^4}{a_4 + \cdots}}}}
```

![x = a_0 + \cfrac{1^2}{a_1 + \cfrac{2^2}{a_2 + \cfrac{3^2}{a_3 + \cfrac{4^4}{a_4 + \cdots}}}}](https://www.zhihu.com/equation?tex=x+%3D+a_0+%2B+%5Ccfrac%7B1%5E2%7D%7Ba_1+%2B+%5Ccfrac%7B2%5E2%7D%7Ba_2+%2B+%5Ccfrac%7B3%5E2%7D%7Ba_3+%2B+%5Ccfrac%7B4%5E4%7D%7Ba_4+%2B+%5Ccdots%7D%7D%7D%7D)

```
\underset{j=1}{\overset{\infty}{\LARGE\mathrm K}}\frac{a_j}{b_j}
=\cfrac{a_1}{b_1
+\cfrac{a_2}{b_2
+\cfrac{a_3}{b_3
+\ddots}}}
```

![\underset{j=1}{\overset{\infty}{\LARGE\mathrm K}}\frac{a_j}{b_j} =\cfrac{a_1}{b_1 +\cfrac{a_2}{b_2 +\cfrac{a_3}{b_3 +\ddots}}}](https://www.zhihu.com/equation?tex=%5Cunderset%7Bj%3D1%7D%7B%5Coverset%7B%5Cinfty%7D%7B%5CLARGE%5Cmathrm+K%7D%7D%5Cfrac%7Ba_j%7D%7Bb_j%7D+%3D%5Ccfrac%7Ba_1%7D%7Bb_1+%2B%5Ccfrac%7Ba_2%7D%7Bb_2+%2B%5Ccfrac%7Ba_3%7D%7Bb_3+%2B%5Cddots%7D%7D%7D)

讲道理这么写和Lisp一样少个括号就爽了...

```
x = a_0 + \frac{1^2}{a_1+}
          \frac{2^2}{a_2+}
          \frac{3^2}{a_3 +} 
          \frac{4^4}{a_4 +} \cdots
```

![x = a_0 + \frac{1^2}{a_1+} \frac{2^2}{a_2+} \frac{3^2}{a_3 +} \frac{4^4}{a_4 +} \cdots](https://www.zhihu.com/equation?tex=x+%3D+a_0+%2B+%5Cfrac%7B1%5E2%7D%7Ba_1%2B%7D+%5Cfrac%7B2%5E2%7D%7Ba_2%2B%7D+%5Cfrac%7B3%5E2%7D%7Ba_3+%2B%7D+%5Cfrac%7B4%5E4%7D%7Ba_4+%2B%7D+%5Ccdots)
- 线性代数

```
\begin{array}{ll}
 \texttt{\overline}&\overline{AAA}\\
 \texttt{\underline}&\underline{BBB}\\
 \texttt{\widetilde}&\widetilde{CCC}\\
 \texttt{\widehat}&\widehat{DDD}\\
 \texttt{\fbox}&\fbox{EEE}\\
\hline
 \texttt{\vec}&\vec{x}\ \mathrm{or}\ \vec{AB}\\
 \texttt{\check}&\check{x}\\
 \texttt{\acute}&\acute{x}\\
 \texttt{\grave}&\grave{x}\\
 \texttt{\bar}&\bar{x}\\
 \texttt{\hat}&\hat{x}\\
 \texttt{\tilde}&\tilde{x}\\
 \texttt{\mathring}&\mathring{x}\\
 \texttt{\dot}&\dot{x}\\
 \texttt{\ddot}&\ddot{x}\\
 \texttt{\dddot}&\dddot{x}\\
\end{array}
```

![\begin{array}{ll} \texttt{\overline}&\overline{AAA}\\ \texttt{\underline}&\underline{BBB}\\ \texttt{\widetilde}&\widetilde{CCC}\\ \texttt{\widehat}&\widehat{DDD}\\ \texttt{\fbox}&\fbox{EEE}\\ \hline \texttt{\vec}&\vec{x}\ \mathrm{or}\ \vec{AB}\\ \texttt{\check}&\check{x}\\ \texttt{\acute}&\acute{x}\\ \texttt{\grave}&\grave{x}\\ \texttt{\bar}&\bar{x}\\ \texttt{\hat}&\hat{x}\\ \texttt{\tilde}&\tilde{x}\\ \texttt{\mathring}&\mathring{x}\\ \texttt{\dot}&\dot{x}\\ \texttt{\ddot}&\ddot{x}\\ \texttt{\dddot}&\dddot{x}\\ \end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bll%7D+%5Ctexttt%7B%5Coverline%7D%26%5Coverline%7BAAA%7D%5C%5C+%5Ctexttt%7B%5Cunderline%7D%26%5Cunderline%7BBBB%7D%5C%5C+%5Ctexttt%7B%5Cwidetilde%7D%26%5Cwidetilde%7BCCC%7D%5C%5C+%5Ctexttt%7B%5Cwidehat%7D%26%5Cwidehat%7BDDD%7D%5C%5C+%5Ctexttt%7B%5Cfbox%7D%26%5Cfbox%7BEEE%7D%5C%5C+%5Chline+%5Ctexttt%7B%5Cvec%7D%26%5Cvec%7Bx%7D%5C+%5Cmathrm%7Bor%7D%5C+%5Cvec%7BAB%7D%5C%5C+%5Ctexttt%7B%5Ccheck%7D%26%5Ccheck%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cacute%7D%26%5Cacute%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cgrave%7D%26%5Cgrave%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cbar%7D%26%5Cbar%7Bx%7D%5C%5C+%5Ctexttt%7B%5Chat%7D%26%5Chat%7Bx%7D%5C%5C+%5Ctexttt%7B%5Ctilde%7D%26%5Ctilde%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cmathring%7D%26%5Cmathring%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cdot%7D%26%5Cdot%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cddot%7D%26%5Cddot%7Bx%7D%5C%5C+%5Ctexttt%7B%5Cdddot%7D%26%5Cdddot%7Bx%7D%5C%5C+%5Cend%7Barray%7D)

```
\begin{array}{lc}
 \texttt{smallmatrix}&\bigl(\begin{smallmatrix} a & b \\ c & d \end{smallmatrix}\bigr)\\
 \texttt{matrix} &\begin{matrix} 1&2\\3&4\\ \end{matrix} \\
 \texttt{pmatrix}&\begin{pmatrix}1&2\\3&4\\ \end{pmatrix}\\
 \texttt{bmatrix}&\begin{bmatrix}1&2\\3&4\\ \end{bmatrix}\\
 \texttt{Bmatrix}&\begin{Bmatrix}1&2\\3&4\\ \end{Bmatrix}\\
 \texttt{vmatrix}&\begin{vmatrix}1&2\\3&4\\ \end{vmatrix}\\
 \texttt{Vmatrix}&\begin{Vmatrix}1&2\\3&4\\ \end{Vmatrix}\\
\end{array}
```

![\begin{array}{lc} \texttt{smallmatrix}&\bigl(\begin{smallmatrix} a & b \\ c & d \end{smallmatrix}\bigr)\\ \texttt{matrix} &\begin{matrix} 1&2\\3&4\\ \end{matrix} \\ \texttt{pmatrix}&\begin{pmatrix}1&2\\3&4\\ \end{pmatrix}\\ \texttt{bmatrix}&\begin{bmatrix}1&2\\3&4\\ \end{bmatrix}\\ \texttt{Bmatrix}&\begin{Bmatrix}1&2\\3&4\\ \end{Bmatrix}\\ \texttt{vmatrix}&\begin{vmatrix}1&2\\3&4\\ \end{vmatrix}\\ \texttt{Vmatrix}&\begin{Vmatrix}1&2\\3&4\\ \end{Vmatrix}\\ \end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Blc%7D+%5Ctexttt%7Bsmallmatrix%7D%26%5Cbigl%28%5Cbegin%7Bsmallmatrix%7D+a+%26+b+%5C%5C+c+%26+d+%5Cend%7Bsmallmatrix%7D%5Cbigr%29%5C%5C+%5Ctexttt%7Bmatrix%7D+%26%5Cbegin%7Bmatrix%7D+1%262%5C%5C3%264%5C%5C+%5Cend%7Bmatrix%7D+%5C%5C+%5Ctexttt%7Bpmatrix%7D%26%5Cbegin%7Bpmatrix%7D1%262%5C%5C3%264%5C%5C+%5Cend%7Bpmatrix%7D%5C%5C+%5Ctexttt%7Bbmatrix%7D%26%5Cbegin%7Bbmatrix%7D1%262%5C%5C3%264%5C%5C+%5Cend%7Bbmatrix%7D%5C%5C+%5Ctexttt%7BBmatrix%7D%26%5Cbegin%7BBmatrix%7D1%262%5C%5C3%264%5C%5C+%5Cend%7BBmatrix%7D%5C%5C+%5Ctexttt%7Bvmatrix%7D%26%5Cbegin%7Bvmatrix%7D1%262%5C%5C3%264%5C%5C+%5Cend%7Bvmatrix%7D%5C%5C+%5Ctexttt%7BVmatrix%7D%26%5Cbegin%7BVmatrix%7D1%262%5C%5C3%264%5C%5C+%5Cend%7BVmatrix%7D%5C%5C+%5Cend%7Barray%7D)

```
\begin{pmatrix}
 1 & a_1 & a_1^2 & \cdots & a_1^n \\
 1 & a_2 & a_2^2 & \cdots & a_2^n \\
 \vdots  & \vdots& \vdots & \ddots & \vdots \\
 1 & a_m & a_m^2 & \cdots & a_m^n \\
\end{pmatrix}
```

![\begin{pmatrix} 1 & a_1 & a_1^2 & \cdots & a_1^n \\ 1 & a_2 & a_2^2 & \cdots & a_2^n \\ \vdots & \vdots& \vdots & \ddots & \vdots \\ 1 & a_m & a_m^2 & \cdots & a_m^n \end{pmatrix}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bpmatrix%7D+1+%26+a_1+%26+a_1%5E2+%26+%5Ccdots+%26+a_1%5En+%5C%5C+1+%26+a_2+%26+a_2%5E2+%26+%5Ccdots+%26+a_2%5En+%5C%5C+%5Cvdots+%26+%5Cvdots%26+%5Cvdots+%26+%5Cddots+%26+%5Cvdots+%5C%5C+1+%26+a_m+%26+a_m%5E2+%26+%5Ccdots+%26+a_m%5En+%5Cend%7Bpmatrix%7D)

增广矩阵就只能用array了

```
\left[
\begin{array}{cc|c}
  1&2&3\\
  4&5&6\\
\end{array}
\right]
```

![\left[ \begin{array}{cc|c} 1&2&3\\ 4&5&6\\ \end{array} \right]](https://www.zhihu.com/equation?tex=%5Cleft%5B+%5Cbegin%7Barray%7D%7Bcc%7Cc%7D+1%262%263%5C%5C+4%265%266%5C%5C+%5Cend%7Barray%7D+%5Cright%5D)

线性方程组加\\[2ex],不然会挤在一起.

```
\begin{cases}
 a_1x+b_1y+c_1z=\frac{p_1}{q_1} \\[2ex] 
 a_2x+b_2y+c_2z=\frac{p_2}{q_2} \\[2ex] 
 a_3x+b_3y+c_3z=\frac{p_3}{q_3}
\end{cases}
```

![\begin{cases} a_1x+b_1y+c_1z=\frac{p_1}{q_1} \\[2ex] a_2x+b_2y+c_2z=\frac{p_2}{q_2} \\[2ex] a_3x+b_3y+c_3z=\frac{p_3}{q_3} \end{cases}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bcases%7D+a_1x%2Bb_1y%2Bc_1z%3D%5Cfrac%7Bp_1%7D%7Bq_1%7D+%5C%5C%5B2ex%5D+a_2x%2Bb_2y%2Bc_2z%3D%5Cfrac%7Bp_2%7D%7Bq_2%7D+%5C%5C%5B2ex%5D+a_3x%2Bb_3y%2Bc_3z%3D%5Cfrac%7Bp_3%7D%7Bq_3%7D+%5Cend%7Bcases%7D)
- 约去符,删除线

```
\require{cancel}
\require{enclose}
\begin{array}{ll}
 \verb|y+\cancel{x}| & y+\cancel{x}\\
 \verb|y+\bcancel{x}| & y+\bcancel{x}\\
 \verb|y+\xcancel{x}| & y+\xcancel{x}\\
 \verb|y+\cancelto{0}{x}| & y+\cancelto{0}{x}\\
 \verb+\frac{1\cancel9}{\cancel95} = \frac15+& \frac{1\cancel9}{\cancel95} = \frac15 \\
 \verb|\enclose{horizontalstrike}{x+y}| & \enclose{horizontalstrike}{x+y}\\
 \verb|\enclose{verticalstrike}{\frac xy}| & \enclose{verticalstrike}{\frac xy}\\
 \verb|\enclose{updiagonalstrike}{x+y}| & \enclose{updiagonalstrike}{x+y}\\
 \verb|\enclose{downdiagonalstrike}{x+y}| & \enclose{downdiagonalstrike}{x+y}\\
\end{array}
```

![\require{cancel} \require{enclose} \begin{array}{ll} \verb|y+\cancel{x}| & y+\cancel{x}\\ \verb|y+\bcancel{x}| & y+\bcancel{x}\\ \verb|y+\xcancel{x}| & y+\xcancel{x}\\ \verb|y+\cancelto{0}{x}| & y+\cancelto{0}{x}\\ \verb+\frac{1\cancel9}{\cancel95} = \frac15+& \frac{1\cancel9}{\cancel95} = \frac15 \\ \verb|\enclose{horizontalstrike}{x+y}| & \enclose{horizontalstrike}{x+y}\\ \verb|\enclose{verticalstrike}{\frac xy}| & \enclose{verticalstrike}{\frac xy}\\ \verb|\enclose{updiagonalstrike}{x+y}| & \enclose{updiagonalstrike}{x+y}\\ \verb|\enclose{downdiagonalstrike}{x+y}| & \enclose{downdiagonalstrike}{x+y}\\ \end{array}](https://www.zhihu.com/equation?tex=%5Crequire%7Bcancel%7D+%5Crequire%7Benclose%7D+%5Cbegin%7Barray%7D%7Bll%7D+%5Cverb%7Cy%2B%5Ccancel%7Bx%7D%7C+%26+y%2B%5Ccancel%7Bx%7D%5C%5C+%5Cverb%7Cy%2B%5Cbcancel%7Bx%7D%7C+%26+y%2B%5Cbcancel%7Bx%7D%5C%5C+%5Cverb%7Cy%2B%5Cxcancel%7Bx%7D%7C+%26+y%2B%5Cxcancel%7Bx%7D%5C%5C+%5Cverb%7Cy%2B%5Ccancelto%7B0%7D%7Bx%7D%7C+%26+y%2B%5Ccancelto%7B0%7D%7Bx%7D%5C%5C+%5Cverb%2B%5Cfrac%7B1%5Ccancel9%7D%7B%5Ccancel95%7D+%3D+%5Cfrac15%2B%26+%5Cfrac%7B1%5Ccancel9%7D%7B%5Ccancel95%7D+%3D+%5Cfrac15+%5C%5C+%5Cverb%7C%5Cenclose%7Bhorizontalstrike%7D%7Bx%2By%7D%7C+%26+%5Cenclose%7Bhorizontalstrike%7D%7Bx%2By%7D%5C%5C+%5Cverb%7C%5Cenclose%7Bverticalstrike%7D%7B%5Cfrac+xy%7D%7C+%26+%5Cenclose%7Bverticalstrike%7D%7B%5Cfrac+xy%7D%5C%5C+%5Cverb%7C%5Cenclose%7Bupdiagonalstrike%7D%7Bx%2By%7D%7C+%26+%5Cenclose%7Bupdiagonalstrike%7D%7Bx%2By%7D%5C%5C+%5Cverb%7C%5Cenclose%7Bdowndiagonalstrike%7D%7Bx%2By%7D%7C+%26+%5Cenclose%7Bdowndiagonalstrike%7D%7Bx%2By%7D%5C%5C+%5Cend%7Barray%7D)
- 奇妙应用:

```
\begin{align}
& ???????\\
& ???????\\
\end{align}
```

![\begin{align} & ???????\\ & ???????\\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+%3F%3F%3F%3F%3F%3F%3F%5C%5C+%26+%3F%3F%3F%3F%3F%3F%3F%5C%5C+%5Cend%7Balign%7D)

```
\bbox[#EFF,5px,border:2px solid red]
{e^x=\lim_{n\to\infty} \left( 1+\frac{x}{n} \right)^n\qquad (1)}
```

![\bbox[#EFF,5px,border:2px solid red] {e^x=\lim_{n\to\infty} \left( 1+\frac{x}{n} \right)^n\qquad (1)}](https://www.zhihu.com/equation?tex=%5Cbbox%5B%23EFF%2C5px%2Cborder%3A2px+solid+red%5D+%7Be%5Ex%3D%5Clim_%7Bn%5Cto%5Cinfty%7D+%5Cleft%28+1%2B%5Cfrac%7Bx%7D%7Bn%7D+%5Cright%29%5En%5Cqquad+%281%29%7D)

