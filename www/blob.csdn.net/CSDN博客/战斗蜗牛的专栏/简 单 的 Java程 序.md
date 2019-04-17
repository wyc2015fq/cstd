# 简 单 的 Java程 序  - 战斗蜗牛的专栏 - CSDN博客





2010年01月15日 11:46:00[vbskj](https://me.csdn.net/vbskj)阅读数：714标签：[java																[applet																[import																[string																[html																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[java](https://blog.csdn.net/vbskj/article/category/643636)





[http://www.sdau.edu.cn/support/html/java03.htm](http://www.sdau.edu.cn/support/html/java03.htm)

1.3 简 单 的 Java程 序 

下 面 我 们 先 介 绍 两 个 简 单 的 Java程 序 ,并 对 其 进 行 分 析。 

例 1.1. 

public class HelloWorldApp { //an application 

public static void main (String args[ ]){ 

System.out.println("Hello World!"); 

} 

} 

本 程 序 的 作 用 是 输 出 下 面 一 行 信 息 : 

Hello World! 

程 序 中 ,首 先 用 保 留 字 class来 声 明 一 个 新 的 类 ,其 类 名 为Hell oWorldApp,它 是 一 个公 共 类 (public)。 整 个 类 定 义 由 大 括 号 {}括起 来 。 在 该 类 中 定义 了 一 个 main ()方 法 ,其中 public表 示 访 问权 限 ,指 明 所 有 的 类 都 可 以 使 用 这 一 方 法 ;static指 明 该 方法 是 一 个 类方 法 ,它 可 以 通 过 类 名 直 接 调 用 ;void则 指 明 main()方法 不 返 回 任何 值 。 对 于 一 个 应 用程 序 来 说 ,main()方 法 是 必需 的 ,而 且 必 须 按 照 如 上 的 格 式 来 定义 。 Jave解 释 器 在 没 有生成 任 何 实 例 的 情 况 下 ,以 main()作 为 入 口 来 执 行 程 序 。 Jave程序中 可 以 定 义 多 个 类 , 每 个 类 中 可 以 定 义 多 个 方 法 ,但 是 最多 只 能 有 一 个 公 共 类,main()方 法 也 只 能 有 一 个 ,作 为 程 序 的入 口 。 main()方 法 定 义 中 ,括 号 ()中 的 String args[ ]是传 递 给 main()方法 的 参 数 ,参 数 名 为 args,它 是 类 String的 一 个 实 例 ,参 数 可 以为 0个 或 多 个,每 个 参 数 用 "类 名 参数 名 "来 指 定 , 多个 参 数 间 用 逗 号 分 隔 。 在 main()方 法 的 实 现(大 括 号 中 ),只有 一 条 语句 : 

System.out.println ("Hello World!"); 它 用 来 实 现 字 符串 的 输 出 ,这 条 语 句 实 现 与 C语 言 中 的printf 语 句 和 C++中 cout＜＜语句 相 同 的 功 能 。 另 外 ,//后 的 内 容 为 注 释 。 

现 在 我 们 可 以 运 行 该 程 序 。 首 先 把 它 放 到 一 个 名 为HelloWor ldApp.java的 文 件 中 ,这 里 ,文 件 名 应 和 类 名 相 同 ,因 为 Java解释 器 要 求 公 共 类 必 须 放在 与 其 同 名 的 文 件 中。 然 后 对 它 进行 编 译 : 

C:/＞javac HelloWorldApp.java 

编 译 的 结 果 是 生 成 字 节 码 文 件 HelloWorldApp.class。最 后 用java 解 释 器 来 运 行 该字 节 码 文 件 : 

C:/＞java HelloWorldApp 

结 果 在 屏 幕 上 显 示 Hello World! 

我 们 再 来 看 下 面 的 一 个 例 子 : 例 1.2 

import java.awr.*; 

import java.applet.*; 

public class HelloWorldApplet extends Applet { //an applet 

public void paint(Graphics g){ 

g.drawString ("Hello World!",20,20); 

} 

} 

这 是 一 个 简 单 的 Applet(小 应 用 程 序 )。 程 序 中 ,首 先 用 import语句 输 入 java.awr和java.applet下 所 有 的 包 ,使 得 该 程 序 可 能 使 用这 些 包 中 所 定 义 的类 , 它 类 似 于 C中 的#include 语 句 。 然 后 声明 一 个 公 共 类 HelloWorldApplet,用 extends指明 它 是 Applet的 子 类。 在类 中 ,我 们 重 写 父 类 Ap-plet的 paint()方 法 ,其 中 参 数 g为 Graphics类,它表 明 当 前 作 画 的 上下 文 。 在 paint()方 法 中 ,调 用 g的 方 法 drawString(),在坐 标 (20,20)处输 出 字 符 串 ""Hello World!",其 中 坐 标是 用 象 素 点 来 表 示 的 。 

这 个 程 序 中 没 有 实 现 main()方 法 ,这 是 Applet与 应 用 程序 Applica tion(如 例 1)的 区 别 之一 。 为 了 运 行 该 程 序 ,首 先 我们 也 要 把它 放 在 文 件HelloWorldAppl et.java中 ,然 后 对 它 进行 编 译 : 

C:/＞javac HelloWorldApplet.java 

得 到 字 节 码 文 件 HelloWorldApplet.class。 由 于 Applet中 没 有main()方法 作 为 Java解 释 器的 入 口 ,我 们 必 须 编 写 HTML文 件 ,把 该 Applet嵌入 其 中 ,然 后 用 appletviewer来 运 行 ,或 在支 持 Java的 浏 览 器 上运 行 。 它 的 ＜HTML＞文 件 如 下 : 

＜HTML＞ 

＜HEAD＞ 

＜TITLE＞ An Applet ＜/TITLE＞ 

＜/HEAD＞ 

＜BODY＞ 

＜applet code="HelloWorldApplet.class" width=200 height=40＞ 

＜/applet＞ 

＜/BODY＞ 

＜/HTML＞ 

其 中 用 ＜applet＞标 记 来 启 动 HelloWorldApplet,code指 明字 节 码所在 的 文 件 ,width和height指 明 applet所 占 的 大 小 ,我 们 把 这 个 HTML文件 存 入Example.html,然 后 运 行 : 

C:/＞appleviewer Example.html 

这 时 屏 幕 上 弹 出 一 个 窗 口 ,其 中 显 示 Hello World!,显 示 结果如 图 : 

从 上 述 例 子 中 可 以 看 出 ,Java程 序 是 由 类 构 成 的 ,对 于 一个应 用 程 序 来 说 ,必 须有 一 个 类 中 定 义 main()方 法 ,而 对 applet来说 ,它 必 须 作 为 Applet的一 个 子 类 。 在 类 的 定 义 中 ,应 包 含 类变 量 的 声 明 和 类 中 方 法 的 实 现 。 Java在 基 本 数据 类 型 、 运算 符 、 表达 式 、 控 制 语 句 等 方 面 与 C、 C++基 本 上 是 相 同 的 ,但 它 同 时 也增 加 了 一 些 新 的 内 容 ,在 以 后 的 各 章 中 ,我 们会 详 细 介 绍 。 本 节 中 ,只 是 使 大 家 对Java程 序 有 一 个 初 步 的了解 。](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=applet&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




