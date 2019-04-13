
# Linux Shell 生成随机数和随机字符串 - 阳光岛主 - CSDN博客

2019年02月08日 22:54:55[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：800


日常生活中，会经常用到随机数，使用场景非常广泛，例如买彩票、丢骰子、抽签、年会抽奖等。
Shell 下如何生成随机数呢，米扑博客特意写了本文，总结 Linux Shell 产生随机数的多种方法。
本文原文转自米扑博客：[Linux Shell 生成随机数和随机字符串](https://blog.mimvp.com/article/13056.html)
计算机产生的的只是“伪随机数”，不会产生绝对的随机数（是一种理想随机数）。实际上，伪随机数和理想随机数也是相对的概念，例如伪随机数在1万万亿亿亿年内也无法重复，算是理想随机数么？
伪随机数在大量重现时也并不一定保持唯一，但一个好的伪随机产生算法将可以产生一个非常长的不重复的序列，例如 UUID（**通用唯一识别码**）在100亿年内才可用完。

**1. 使用系统的 $RANDOM 变量**（CentOS、Ubuntu、MacOS 都支持，但只有5位数随机）
mimvp@ubuntu:~$ echo $RANDOM 17617
$RANDOM 的范围是 [0, 32767]
**示例：**使用 for 循环来验证：
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
function print_random() { for i in {1…10}; do echo -e “$i \t $RANDOM” done }
print_random
运行结果：
\# sh mimvp_shell_rand.sh 1 20191 2 16817 3 25971 4 1489 5 34 6 25183 7 920 8 315 9 18845 10 29519
如需要生成超过32767的随机数，可以用以下方法实现（有缺陷）
例：生成 40,000,000~50,000,000 的随机数，但最后末尾五位数在随机变化，实现原理有缺陷
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# Linux 系统随机数 + 范围上限值后, 再取余 function mimvp_random_bignum() { min=$1 max=$2 mid=$(($max-$min+1)) num=$(($RANDOM+$KaTeX parse error: Expected 'EOF', got '\#' at position 7: max)) \#̲ 随机数+范围上限, 然后取余…$(($num%$mid+$min)) \# 随机数包含上下限边界数值 echo $randnum }
function print_random_bignum() { for i in {1…10}; do bignum=$(mimvp\_random\_bignum 40000000 50000000) echo -e &quot;$i \t $bignum" done }
print_random_bignum
运行结果：
\# sh mimvp_shell_rand.sh
1 40022422 2 40014261 3 40022712 4 40016695 5 40026575 6 40032198 7 40026667 8 40016024 9 40012010 10 40016143
这里，还可以通过 awk 产生随机数，最大为6位随机数，其跟时间有关，系统时间一致则随机数都相同，没有 $RANDOM 随机性好
\# awk ‘BEGIN{srand(); print rand()}’
0.739505 \# awk ‘BEGIN{srand(); print rand()*1000000}’ 855767

**2. 使用date +%s%N**（CentOS、Ubuntu支持，MacOS不支持纳秒 +%N）
通过 Linux / Unix 的时间戳来获取随机数
\# date +%S \# 获取秒数, 2位数 43 \# date +%s \# 获取时间戳, 10位数, 从 1970-01-01 00:00:00 到当前的间隔的秒数 1548739004 \# date +%N \# 获取纳秒值, 9位数, CentOS、Ubuntu支持, 但 MacOS 不支持 468529240
说明：
如果用时间戳 date +%s 做随机数，相同一秒的数据是一样的。在做循环处理多线程时，基本不能满足要求
如果用纳秒值 date +%N 做随机数，精度达到了亿分之一，相当精确了，在多cpu高并发的循环里，同一秒里也很难出现相同结果，不过也会有重复碰撞的可能性
如果用时间戳+纳秒值 date +%N%s 做组合随机数（10+9=19位数），则比较完美了，重复的概率大大降低，但注意： MacOS 系统不支持纳秒值，不算通用
**示例：**生成 40,000,000~50,000,000 的随机数
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# Linux 时间戳随机数 function mimvp_randnum_date() { min=$1 max=$2 mid=$(($max-$min+1)) num=$(date +%s%N | cut -c1-17) \# 19位数, 截取第1-17位数, 下标从1开始 randnum=$(($num%$mid+$min)) \# 随机数包含上下限边界数值 echo $randnum }
function print_randnum_date() { for i in {1…10}; do randnum=$(mimvp\_randnum\_date 40000000 50000000) echo -e &quot;$i \t $randnum" done }
print_randnum_date
运行结果：
\# sh mimvp_shell_rand.sh
1 42153680 2 42199904 3 42243885 4 42283556 5 42332691 6 42376578 7 42422048 8 42462640 9 42505483 10 42550221
说明：
上面的结果可以看到，当取大数值范围时，高位可能都是相同的，原因是 date +%N%s 是按照 秒数+纳秒 获取的，时间高位具有顺序位，可能相同
那么，有的同学问题，能不能把 date +%s%N 的秒数和纳秒互换下，答案是不可以的，原因是纳秒的第一位可能为0，从第一位截取可能为 09641524615487432 ，shell 会提示错误： value too great for base (error token is “09641524615487432”)
**改进的办法1：互调**date +%N%s （仍然不行）**：**
既然第一位不能为0，那么从纳秒的第2位、第3位… 截取不行吗，答案也是不可以的，因为纳秒的每一位都有可能是0，毕竟纳秒是9位数（毫秒3位数、微秒6位数、纳秒9位数）纳秒本身就在秒数之后，所以纳秒的9位数的每一位都可以为0  另外，纳秒在高位，秒数在低位，截取大数值可能导致高位不相同，但低位数值相同的情况，原因是秒数的值变化非常慢。结论，互换的办法是行不通的，还可能导致新的问题，因此，老老实实的用  date +%s%N 格式吧
**改进的方法2：**直接用 date +%s%N 的19位数（可行）
不要截取 date +%s%N | cut -c1-17 ，充分利用纳秒的快速变化后再取余

**3. 使用 /dev/random 和 /dev/urandom 随机文件**（CentOS、Ubuntu、MacOS 都支持，推荐）
/dev/random 是阻塞的随机数发生器，读取有时需要等待。存储着系统当前运行环境的实时数据，如 CPU、内存、电压、物理信号等
/dev/urandom 是非阻塞随机数发生器，读取操作不会产生阻塞。
说明：
/dev/random 和 /dev/urandom 存储的都是乱码，实际上它们是通过二进制数据保存实时数据的
打开 /dev/random 和 /dev/urandom 文件，推荐用 head，不推荐 cat 命令，因为文件非常大且是乱码，只需要获取前几行文件内容就变了
用到了 cksum 命令，其读取文件内容，生成唯一的整型数据，只有文件内容没变，生成结果就不会变化，与php crc函数类似，一般校验文件是否篡改
其生成随机数的原理是：截取文件的一部分内容，做内容的计算，取第一个数值
\# head -20 /dev/urandom | cksum 3535024891 50260 \# head -20 /dev/urandom | cksum | cut -f1 -d " " 1713554848
**示例：**使用/dev/urandom生成 40,000,000~50,000,000 之间的随机数，使用 /dev/urandom 避免阻塞。
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# Linux 随机文件 function mimvp_randnum_file() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 49: …| cut -f1 -d ' '̲) \# num=$(head -n 20 /dev/urandom | cksum | cut -d ’ ’ -f1) \# ok \# num=$(head -n 20 /dev/urandom | cksum | awk '{print$KaTeX parse error: Expected 'EOF', got '}' at position 2: 1}̲') \# ok \# num=$(head -n 20 /dev/urandom | cksum | awk -F " " '{print$KaTeX parse error: Expected 'EOF', got '}' at position 2: 1}̲') \# ok randnum…$(($num%$mid+$min))
echo $randnum }
function print_randnum_file() { for i in {1…10}; do randnum=$(mimvp\_randnum\_file 40000000 50000000) echo -e &quot;$i \t $randnum" done }
print_randnum_file
运行结果：
\# sh mimvp_shell_rand.sh
1 48894638 2 43078483 3 41678948 4 48987680 5 46095205 6 49650777 7 47144679 8 49003259 9 44562068 10 42014734
由此可见，用随机文件生成的随机数，基本是全随机的，且通用于 CentOS、Ubuntu、MacOS

**4. 使用 linux uuid**（CentOS、Ubuntu支持，MacOS不支持）
UUID（Universally Unique Identifier，通用唯一识别码），格式包含32个16进制数字，以’-'连接号分为5段。
格式为 8-4-4-4-12 的32个字符，例如： 07e73165-1196-4194-98bb-a3bf7c96e34a
\# cat /proc/sys/kernel/random/uuid 07e73165-1196-4194-98bb-a3bf7c96e34a
**UUID 数量**，理论上的总数为216 x 8=2128，约等于3.4 x 1038。 也就是说若每奈秒产生1兆个UUID，要花100亿年才会将所有UUID用完。
**UUID 目的**，是让分布式系统中的所有元素，都能有唯一的辨识信息，而不需要通过中央控制端来做辨识信息的指定。如此一来，每个人都可以创建不与其它人冲突的 UUID。在这样的情况下，就不需考虑数据库创建时的名称重复问题。它会让网络任何一台计算机所生成的uuid码，都是互联网整个服务器网络中唯一的。它的原信息会加入硬件，时间，机器当前运行信息等等。
**UUID 格式：**包含32个16进位数字，以“-”连接号分为五段，形式为8-4-4-4-12的32个字符。范例；550e8400-e29b-41d4-a716-446655440000  ,所以：
与 uuid类似的还有一个guid(**全局唯一标识符**)码，它由微软支持，它们由操作系统内核产生。
**示例：**使用 linux uuid 生成 40,000,000~50,000,000 之间的随机数
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# Linux uuid function mimvp_randnum_uuid() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 65: …| cut -f1 -d ' '̲) randnum=$(($num%$mid+$min))
echo $randnum }
function print_randnum_uuid() { for i in {1…10}; do randnum=$(mimvp\_randnum\_uuid 40000000 50000000) echo -e &quot;$i \t $randnum" done }
print_randnum_uuid
运行结果：
\# sh mimvp_shell_rand.sh
1 44736535 2 43538760 3 40133914 4 41016814 5 49148972 6 40179476 7 48147712 8 45665645 9 40522150 10 44361996
**5. 使用 openssl rand **（CentOS、Ubuntu支持、MacOS 都支持，需安装 openssl，推荐）
openssl rand 用于产生指定长度个bytes的随机字符
\# openssl rand --help Usage: rand [options] num where options are -out file - write to file -engine e - use engine e, possibly a hardware device. -rand file:file:… - seed PRNG from files -base64 - base64 encode output -hex - hex encode output
其中，参数 -base64 或 -hex 对随机字符串进行base64编码或用hex格式显示
结合 cksum 产生整数、md5sum 产生字符串，可以产生随机的整数或字符串（仅含小写字母和数字）
例如：
\# openssl rand -base64 8 \# 第一次执行 Vt4MNFIfzCU= \# openssl rand -base64 8 \# 第二次执行, 随机数不同 uwnovaLKhek= \# openssl rand -base64 8 | cksum \# 生成随机整数 3663376449 13 \# openssl rand -base64 8 | md5sum \# 生成随机字符串 1f36cf340e0a90ccb0d504925c3d7ada - \# openssl rand -base64 8 | cksum | cut -c1-8 \# 截取数字 15997092 \# openssl rand -base64 8 | md5sum | cut -c1-8 \# 截取字符串 f1a972ce
\# openssl rand -hex 8 \# 第一次执行 c5bc62152bddadfb \# openssl rand -hex 8 \# 第二次执行, 随机数不同 156642181b22306a \# openssl rand -hex 8 | cksum \# 生成随机整数 3663376449 13 \# openssl rand -hex 8 | md5sum \# 生成随机字符串 1f36cf340e0a90ccb0d504925c3d7ada - \# openssl rand -hex 8 | cksum | cut -c1-8 \# 截取数字 15997092 \# openssl rand -hex 8 | md5sum | cut -c1-8 \# 截取字符串 f1a972ce
**示例：**使用 openssl rand 生成 40,000,000~50,000,000 之间的随机数
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 5. Linux openssl function mimvp_randnum_openssl() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 48: …| cut -f1 -d ' '̲) \# -base64 \# n…$(openssl rand -hex 8 | cksum | cut -f1 -d ’ ') \# -hex randnum=$(($num%$mid+$min))
echo $randnum }
function print_randnum_openssl() { for i in {1…10}; do randnum=$(mimvp\_randnum\_openssl 40000000 50000000) echo -e &quot;$i \t $randnum" done }
print_randnum_openssl
运行结果：
\# sh mimvp_shell_rand.sh 1 43422505 2 40756492 3 45087076 4 43882168 5 47105153 6 45505018 7 41411938 8 48662626 9 47508094 10 41362566

**6. 自定义数组生成随机数**
自定义一个数组，用于生成一段特定长度（整数最长为18位）的有数字和字母组成的字符串，字符串中元素取自自定义的池子。
array=(0 1 2 3 4 5 6 7 8 9)                   \# 自定义一个数字数组
num=${\#array[*]}                                \# 获取数组的长度（元素个数）
randnum=$KaTeX parse error: Expected '}', got '\[' at position 7: {array\̲[̲$((RANDOM%num))]}   \# 利用Linux系统默认的 $RANDOM 随机数，随机从数组选择一个元素，构成新的长度数组
**示例：**自定义数组生成 40,000,000~50,000,000 之间的随机数（注释有点不好看，但非常有助于理解代码哈）
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 6. custom array, 可以生成整数, 字符串 function mimvp_randnum_array() { NUM_LENGTH=18 \# 整数的位数, 依据取值范围设定, 默认最长为18位整数(取决于正整数的范围) STR_ARRAY=(0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) \# 生成字符串 STR_ARRAY=(0 1 2 3 4 5 6 7 8 9) \# 生成整数
`str\_array\_count=${#STR_ARRAY\[@\]}    # 字符串数组的元素个数, 62 = 10 + 26 + 26`\# echo “str_array_count: ${str_array_count}”
`i=1
while \[ "$i" -le "${NUM_LENGTH}" \];
do
    randnum\_array\[$i\]=${STR\_ARRAY\[$((RANDOM%str\_array\_count))\]}
    let "i=i+1"
done
randnum\_array\_count=${#randnum_array\[@\]}`\# echo “randnum_array_count: ${randnum_array_count}” \# NUM_LENGTH 的长度: 18 \# echo “randnum_array: ${randnum_array[@]}” \# 打印出全部数组元素, 如 B 2 y t z K c Z s N l 9 T b V w j 6
`num='1'             # 整数首位不能是0, 因此直接固定为1, 防止整数时首位为0的异常错误
for item in ${randnum_array\[@\]};
do
    num="${num}${item}"
done`\# echo “num: $num” \# 1B2ytzKcZsNl9TbVwj6
`min=$1
max=$2
mid=$(($max-$min+1))
randnum=$(($num%$mid+$min))        
echo $randnum`}
function print_randnum_array() { for i in {1…10}; do randnum=$(mimvp\_randnum\_array 40000000 50000000) echo -e &quot;$i \t $randnum" done }
print_randnum_array
运行结果：
\# sh mimvp_shell_rand.sh
1 48952205 2 43220726 3 45241774 4 45758327 5 43147638 6 44319391 7 46834434 8 41601915 9 48687238 10 45029848
**7. 生成随机字符串**
上述所有可以生成随机整数的方法，都可以生成随机字符串，原理是对随机整数进行 md5sum 计算
**示例：**生成10位随机字符串
\# 使用date 生成随机字符串 date +%s%N | md5sum | head -c 10
\# 使用 /dev/urandom 生成随机字符串 cat /dev/urandom | head -n 10 | md5sum | head -c 10

**随机数应用一**
随机生成端口号范围为 1025 ~ 65536 (通用于 CentOS, Ubuntu, MacOS)，并支持排除任意添加的端口号
应用的随机数是**方法3. 使用 /dev/random 和 /dev/urandom 随机文件**
**应用代码：**
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 应用一: 随机生成端口号 1025 ~ 65536 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_port() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 49: …| cut -f1 -d ' '̲) randnum=$(($num%$mid+$min))
`# 排除的端口号 1080, 4500, 8080, 58866, 可以任意添加
port_exclude='1080,4500,8080,58866'
flag=\`echo ${port_exclude} | grep ${randnum} | wc -l\`
while \[ "$flag" -eq "1" \]
do
    num=$(head -n 20 /dev/urandom | cksum | cut -f1 -d ' ')
    randnum=$(($num%$mid+$min))        
    flag=\`echo ${port_exclude} | grep ${randnum} | wc -l\`
done
echo $randnum`}
function print_app_port() { for i in {1…10}; do randnum=$(mimvp\_app\_port 1025 65535) echo -e &quot;$i \t $randnum" done }
print_app_port
运行结果：
\# sh mimvp_shell_rand.sh
1 29483 2 61738 3 31935 4 3242 5 19865 6 56677 7 5944 8 28579 9 12510 10 31844

**随机数应用二**
随机生成长度为10的密码字符串 (通用于 CentOS, Ubuntu, MacOS)
应用的随机数是**方法1：使用系统的 $RANDOM 变量**
**应用代码：**
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 应用二: 随机生成长度为10的密码字符串 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_passwd() { user_array=`seq -w 10` echo ${user_array[@]}
`for idx in ${user_array\[@\]}
do
    user_name="user-${idx}"
    passwd=\`echo $RANDOM | md5sum | cut -c11-20\`
    echo -e "${user_name} \\t ${passwd}"
done`}
mimvp_app_passwd
运行结果：
\# sh mimvp_shell_rand.sh
01 02 03 04 05 06 07 08 09 10 user-01 52cf5272cb user-02 40f20d352d user-03 9fe9a7b770 user-04 ff4e20e6e0 user-05 88fc4a3ea3 user-06 6494032261 user-07 6a42732519 user-08 6fc7a25dd5 user-09 f0b6a95608 user-10 49219467fa

**随机数应用三**
统计掷骰子，投掷6000次统计分别为1-6的次数 (通用于 CentOS, Ubuntu, MacOS)
应用的随机数是**方法1：使用系统的 $RANDOM 变量**
**应用代码：**
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 应用三: 统计掷骰子, 投掷6000次统计分别为1-6的次数 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_dice() { MAX=6000 stat_1=0 stat_2=0 stat_3=0 stat_4=0 stat_5=0 stat_6=0
`i=1
while \[ "$i" -le "$MAX" \]
do
    randnum=$(($RANDOM%6))  # 对6取余, 余数为0时记作6点
    case "$randnum" in
        0) stat\_6=\`expr ${stat\_6} + 1\`;;    # 余数为0时记作6点 
        1) stat\_1=\`expr ${stat\_1} + 1\`;;
        2) stat\_2=\`expr ${stat\_2} + 1\`;;
        3) stat\_3=\`expr ${stat\_3} + 1\`;;
        4) stat\_4=\`expr ${stat\_4} + 1\`;;
        5) stat\_5=\`expr ${stat\_5} + 1\`;;
    esac
    let "i=i+1"
done
echo "stat\_1  ${stat\_1}"
echo "stat\_2  ${stat\_2}"
echo "stat\_3  ${stat\_3}"
echo "stat\_4  ${stat\_4}"
echo "stat\_5  ${stat\_5}"
echo "stat\_6  ${stat\_6}"`}
mimvp_app_dice
运行结果：
\# sh mimvp_shell_rand.sh
stat_1 923 stat_2 994 stat_3 977 stat_4 1039 stat_5 1072 stat_6 995
**总结**
random、urandom、uuid、openssl rand、自定义数组（用到了 $RANDOM）产生随机码的伪数据来源，都与 /dev/random 设备有关系，只是它们各自呈现不同。
date 日期生成的随机数，与Linux 系统的随机设备 /dev/random 的关系不大，但系统时间也会影响  /dev/random 设备，两者并非绝对无关系。
所有可以生成随机整数的方法，都可以生成随机字符串，原理是对随机整数进行 md5sum 计算

*最后，附上完整的 shell 代码，方便爱好者研究、调试*
\#!/bin/bash \#[mimvp.com](http://mimvp.com)2016.05.10
\#\# 1. Linux 系统默认随机数 function print_randnum() { for i in {1…10}; do randnum=$KaTeX parse error: Expected 'EOF', got '\#' at position 8: RANDOM \#̲ Linux 内置随机数, 范…$(awk ‘BEGIN{srand(); print rand()*1000000; }’) \# awk 随机种子函数, 最多5位随机数, 跟时间有关 echo -e “$i \t $randnum” done }
\#\# Linux 系统随机数 + 范围上限值后, 再取余 function mimvp_randnum_bignum() { min=$1 max=$2 mid=$(($max-$min+1)) num=$(($RANDOM+$KaTeX parse error: Expected 'EOF', got '\#' at position 7: max)) \#̲ 随机数+范围上限, 然后取余…$(($num%$mid+$min)) \# 随机数包含上下限边界数值 echo $randnum }
function print_randnum_bignum() { for i in {1…10}; do randnum=$(mimvp\_randnum\_bignum 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 2. Linux 时间戳随机数 (CentOS, Ubuntu支持, MacOS不支持纳秒+%N) function mimvp_randnum_date() { min=$1 max=$2 mid=$(($max-$min+1)) num=$(date +%s%N | cut -c1-17) \# 19位数, 截取第1-17位数, 下标从1开始 num=$(date +%s%N) \# 19位数, 截取第1-17位数, 下标从1开始 randnum=$(($num%$mid+$min)) \# 随机数包含上下限边界数值 echo $randnum }
function print_randnum_date() { for i in {1…10}; do randnum=$(mimvp\_randnum\_date 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 3. Linux 随机文件 function mimvp_randnum_file() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 49: …| cut -f1 -d ' '̲) \# num=$(head -n 20 /dev/urandom | cksum | cut -d ’ ’ -f1) \# ok \# num=$(head -n 20 /dev/urandom | cksum | awk '{print$KaTeX parse error: Expected 'EOF', got '}' at position 2: 1}̲') \# ok \# num=$(head -n 20 /dev/urandom | cksum | awk -F " " '{print$KaTeX parse error: Expected 'EOF', got '}' at position 2: 1}̲') \# ok randnum…$(($num%$mid+$min))
echo $randnum }
function print_randnum_file() { for i in {1…10}; do randnum=$(mimvp\_randnum\_file 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 4. Linux uuid function mimvp_randnum_uuid() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 65: …| cut -f1 -d ' '̲) randnum=$(($num%$mid+$min))
echo $randnum }
function print_randnum_uuid() { for i in {1…10}; do randnum=$(mimvp\_randnum\_uuid 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 5. Linux openssl function mimvp_randnum_openssl() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 48: …| cut -f1 -d ' '̲) \# -base64 \# n…$(openssl rand -hex 8 | cksum | cut -f1 -d ’ ') \# -hex randnum=$(($num%$mid+$min))
echo $randnum }
function print_randnum_openssl() { for i in {1…10}; do randnum=$(mimvp\_randnum\_openssl 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 6. custom array, 可以生成整数, 字符串 function mimvp_randnum_array() { NUM_LENGTH=18 \# 整数的位数, 依据取值范围设定, 默认最长为18位整数(取决于正整数的范围) STR_ARRAY=(0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) \# 生成字符串 STR_ARRAY=(0 1 2 3 4 5 6 7 8 9) \# 生成整数
`str\_array\_count=${#STR_ARRAY\[@\]}    # 字符串数组的元素个数, 62 = 10 + 26 + 26`\# echo “str_array_count: ${str_array_count}”
`i=1
randnum_array=()
while \[ "$i" -le "${NUM_LENGTH}" \];
do
    randnum\_array\[$i\]=${STR\_ARRAY\[$((RANDOM%str\_array\_count))\]}
    let "i=i+1"
done
randnum\_array\_count=${#randnum_array\[@\]}`\# echo “randnum_array_count: ${randnum_array_count}” \# NUM_LENGTH 的长度: 18 \# echo “randnum_array: ${randnum_array[@]}” \# 打印出全部数组元素, 如 B 2 y t z K c Z s N l 9 T b V w j 6
`num='1'             # 整数首位不能是0, 因此直接固定为1, 防止整数时首位为0的异常错误
for item in ${randnum_array\[@\]};
do
    num="${num}${item}"
done`\# echo “num: $num” \# 1B2ytzKcZsNl9TbVwj6
`min=$1
max=$2
mid=$(($max-$min+1))
randnum=$(($num%$mid+$min))        
echo $randnum`}
function print_randnum_array() { for i in {1…10}; do randnum=$(mimvp\_randnum\_array 40000000 50000000) echo -e &quot;$i \t $randnum" done }
\#\# 应用一: 随机生成端口号 1025 ~ 65536 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_port() { min=$1 max=$2 mid=$(($max-$min+1)) num=$KaTeX parse error: Double superscript at position 49: …| cut -f1 -d ' '̲) randnum=$(($num%$mid+$min))
`# 排除的端口号 1080, 4500, 8080, 58866, 可以任意添加
port_exclude='1080,4500,8080,58866'
flag=\`echo ${port_exclude} | grep ${randnum} | wc -l\`
while \[ "$flag" -eq "1" \]
do
    num=$(head -n 20 /dev/urandom | cksum | cut -f1 -d ' ')
    randnum=$(($num%$mid+$min))        
    flag=\`echo ${port_exclude} | grep ${randnum} | wc -l\`
done
echo $randnum`}
function print_app_port() { for i in {1…10}; do randnum=$(mimvp\_app\_port 1025 65535) echo -e &quot;$i \t $randnum" done }
\#\# 应用二: 随机生成长度为10的密码字符串 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_passwd() { user_array=`seq -w 10` echo ${user_array[@]}
`for idx in ${user_array\[@\]}
do
    user_name="user-${idx}"
    passwd=\`echo $RANDOM | md5sum | cut -c11-20\`
    echo -e "${user_name} \\t ${passwd}"
done`}
\#\# 应用三: 统计掷骰子, 投掷6000次统计分别为1-6的次数 (通用于 CentOS, Ubuntu, MacOS) function mimvp_app_dice() { MAX=6000 stat_1=0 stat_2=0 stat_3=0 stat_4=0 stat_5=0 stat_6=0
`i=1
while \[ "$i" -le "$MAX" \]
do
    randnum=$(($RANDOM%6))  # 对6取余, 余数为0时记作6点
    case "$randnum" in
        0) stat\_6=\`expr ${stat\_6} + 1\`;;    # 余数为0时记作6点 
        1) stat\_1=\`expr ${stat\_1} + 1\`;;
        2) stat\_2=\`expr ${stat\_2} + 1\`;;
        3) stat\_3=\`expr ${stat\_3} + 1\`;;
        4) stat\_4=\`expr ${stat\_4} + 1\`;;
        5) stat\_5=\`expr ${stat\_5} + 1\`;;
    esac
    let "i=i+1"
done
echo "stat\_1  ${stat\_1}"
echo "stat\_2  ${stat\_2}"
echo "stat\_3  ${stat\_3}"
echo "stat\_4  ${stat\_4}"
echo "stat\_5  ${stat\_5}"
echo "stat\_6  ${stat\_6}"`}
print_randnum
\#print_randnum_bignum
\#print_randnum_date
\#print_randnum_file
\#print_randnum_uuid
\#print_randnum_openssl
\#print_randnum_array
\#print_app_port
\#mimvp_app_passwd
\#mimvp_app_dice \# 循环次数多, 运行时间较长, 大约30秒, 请慎用


**参考推荐**：
[shell 字符串出现的行数](https://blog.mimvp.com/article/11232.html)
[shell 字符串包含关系](https://blog.mimvp.com/article/11574.html)
[Linux Shell 函数返回值](https://blog.mimvp.com/article/11359.html)
[Linux Shell 算术运算](https://blog.mimvp.com/article/5994.html)
[Linux 之 shell 比较运算符](https://blog.mimvp.com/article/7415.html)
[Linux curl 命令模拟 POST/GET 请求](https://blog.mimvp.com/article/25593.html)
[Linux awk 内置函数实例](https://blog.mimvp.com/article/11401.html)
[Linux tcpdump命令详解](https://blog.mimvp.com/article/6341.html)

