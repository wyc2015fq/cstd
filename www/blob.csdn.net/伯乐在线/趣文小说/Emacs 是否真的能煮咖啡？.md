# Emacs 是否真的能煮咖啡？ - 文章 - 伯乐在线
原文出处： [XGuru](http://www.cppblog.com/xguru/archive/2014/10/03/121416.html)
![](http://upload.wikimedia.org/wikipedia/commons/thumb/6/69/Vacpot3.jpg/467px-Vacpot3.jpg)
坊间有传言曰:“emacs 无所不能, 甚至能够用来煮咖啡!”
煮咖啡何解？勾起了我的考究欲望。
上网搜索之, 得出几种初步结论如下:
1. 这只是一种好玩的说法，只是用来形容 emacs 功能无所不包而已。
2.Java 的标志就一杯咖啡, 用来形象的表示写代码
![](http://www.cppblog.com/images/cppblog_com/xguru/make_coffee/java.png)
3.emacs 的确具[有煮咖啡的功能](http://people.ku.edu/~syliu/shredderyin/emacs_power.html)，有脚本能够控制自动咖啡机运行。
觉得第一种说法比较符合逻辑; 第二种说法感觉有点唐突，毕竟 Emacs 是 Richard Stallman(GNU 创始人) 所写，而 Java 是 Bill Joy(vi 作者) 等人完成的，这两派都差点上升到宗教冲突了, 这种解释有点差强人意; 第三种的如果是真的话就会变得很有趣。
于是就开始了探究，首先追本溯源，找到这段脚本代码的源头。发现已经地址已经失效, 终于在在 debian 的一个软件包里找到了副本。这是 emacs 常用脚本的一个打包。
代码如下：


```
;;; coffee.el --- Submit a BREW request to an RFC2324-compliant coffee device
;;;
;;; Author: Eric Marsden <emarsden@laas.fr>
;;; Version: 0.2
;;; Copyright: (C) 1999 Eric Marsden
;;; Keywords: coffee, brew, kitchen-sink, can't
;;
;;     This program is free software; you can redistribute it and/or
;;     modify it under the terms of the GNU General Public License as
;;     published by the Free Software Foundation; either version 2 of
;;     the License, or (at your option) any later version.
;;    
;;     This program is distributed in the hope that it will be useful,
;;     but WITHOUT ANY WARRANTY; without even the implied warranty of
;;     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;;     GNU General Public License for more details.
;;    
;;     You should have received a copy of the GNU General Public
;;     License along with this program; if not, write to the Free
;;     Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
;;     MA 02111-1307, USA.
;;
;; Please send suggestions and bug reports to <emarsden@laas.fr>.
;; The latest version of this package should be available at
;;
;;     <URL:http://purl.org/net/emarsden/home/downloads/>
  
;;; Commentary:
;;
;; This module provides an Emacs interface to RFC2324-compliant coffee
;; devices (Hyper Text Coffee Pot Control Protocol, or HTCPCP). It
;; prompts the user for the different additives, then issues a BREW
;; request to the coffee device.
;;
;; coffee.el requires a special BREW-capable version of Emacs/W3 to be
;; installed.
;;
;; Reference: <URL:ftp://ftp.isi.edu/in-notes/rfc2324.txt>
;;
;;
;; Thanks to Giacomo Boffi <giacomo.boffi@polimi.it> for some typos
;; and the addition of the "Brown-Coffee" sweetener type.
  
;;; Code:
  
(require 'cl)
  
 (defvar coffee-host "coffee"
   "*The host which provides the coffee service.")
  
(defvar coffee-pot-designator 1
   "*On machines with multiple pots, the number of the pot to brew in")
 
(defvar coffee-brew-hook nil
   "*Hook executed before issuing a BREW request")
 
 (defconst coffee-milk-types
   '("Cream""Half-and-Half" "Whole-Milk" "Part-Skim" "Skim" "Non-Dairy"))
 
 (defconst coffee-syrup-types '("Vanilla""Almond" "Raspberry" "Chocolate"))
  
 (defconst coffee-sweetener-types '("White-Sugar""Brown-Sugar" "Artificial-Sweetener"))
 
(defconst coffee-alcohol-types '("Whiskey""Rum" "Kahula" "Aquavit"))
  
(defconst coffee-addition-types
   `(("Milk"      . ,coffee-milk-types)
    ("Syrup"     . ,coffee-syrup-types)
    ("Sweetener" . ,coffee-sweetener-types)
    ("Alcohol"   . ,coffee-alcohol-types)))
 
;;;###autoload
(defun coffee ()
"Submit a BREW request to an RFC2324-compliant coffee device"
   (interactive)
   (require 'url)
   (let* ((additions-list
           (append coffee-milk-types
                   coffee-syrup-types
                  coffee-sweetener-types
                  coffee-alcohol-types))
         (additions-string
           (mapconcat #'identity additions-list","))
          (url (coffee-url))
         (url-request-method "BREW")
         (url-request-extra-headers
           `(("Content-type"     . "message-coffeepot")
             ("Accept-Additions" . ,additions-string)))         
         (url-request-data "START"))
        (run-hooks 'coffee-brew-hook)
     (url-retrieve url)))
  
 (defun coffee-additions ()
   (let* ((type-name
           (completing-read "Coffee addition:" coffee-addition-types nil t))
        (type (cdr (assoc type-name coffee-addition-types)))
          (ingredients (mapcar #'(lambda (a) (cons a a)) type))
         (ingredient
          (completing-read "Addition type:" ingredients nil t)))
     ingredient))
           
 (defun coffee-url ()
   (require 'w3-forms)
   (concat "coffee://" coffee-host "/"
           (int-to-string coffee-pot-designator)
           "?" (w3-form-encode-xwfu (coffee-additions))))
 
  
 (provide 'coffee)
  
;; coffee.el ends here
```
这个脚本看起来还是煞有其事的，文中提到 “Submit a BREW request to an RFC2324-compliant coffee device”
能够向与 RFC2324 协议兼容的咖啡设备提交 BREW 请求 , 即兼容 Hyper Text Coffee Pot Control Protocol (HTCPCP/1.0) 协议。超文本咖啡壶协议, 光看这名字就够喜庆了，但这份协议写得很规范, 看不出一丝破绽。有细心的朋友 shrek.wang 提醒了我，注意看日期:


```
Network Working Group                                       L. Masinter
Request for Comments: 2324                                 1 April 1998
```
1998 年 4 月 1 号，愚人节！这个就让人产生了疑问。
通过查 wiki 发现，原来互联网国际标准机构也是很有才的恶搞高手。
这里列举其中几个好玩的
- 2001 年，RFC 1149 由一班挪威 Linux 使用者协会的成员实现了。他们传送了 9 个封包到约 5 公里外的地方，每个封包由不同的鸽子携带，并有一个 ICMP 应答要求封包（ping）。他们收到 4 个回应，封包流失率是 55%，回应时间是 3000 至 6000 秒。[1]
- RFC 1607 — 来自 21 世纪的观点 Vint Cerf 1994.
- RFC 3091 — 圆周率数字产生协定 . H. Kennedy 2001 年
更多的可以看这里 wiki。
这里可以大胆地作出推断，这个协议只是 IETF 开的一个善意的 joke，而 coffee.el 的作者 Eric Marsden 也是一个幽默的程序员，于是就做了一个兼容 RFC2324 的脚本, 他也没想过要真正的控制咖啡机，所以这整个事情都源于程序员的冷幽默。
![](http://www.cppblog.com/images/cppblog_com/xguru/make_coffee/coffee.jpg)
然而远程控制咖啡机还是有可能的，这里有一个开源咖啡机; 这里还有一个允许网络控制的咖啡机（据说还是兼容 RFC2324 的）。
估计 IETF 应该做梦也没想到自己开的一个玩笑竟然还真的有人做出了实物。 国外的牛人们还真是闲得蛋疼阿。可见 geek 们还是极富幽默感的。
朋友们，你的心中已经有答案了么？你们想要一台这样的咖啡机不?
