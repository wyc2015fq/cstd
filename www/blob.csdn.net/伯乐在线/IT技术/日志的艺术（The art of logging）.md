# 日志的艺术（The art of logging） - 文章 - 伯乐在线
原文出处： [xybaby](http://www.cnblogs.com/xybaby/p/7954610.html)
程序员学习每一门语言都是从打印“hello world”开始的，日志也是新手程序员学习、调试程序的一大利器。当项目上线之后，也会有各种各样的日志，比如记录用户的行为、服务器的状态、异常情况等等。打印日志似乎是一件很简单、不值得一提的事情。但是，当看到线上项目居然有这样的日志的时候，不禁想问问我们真的会打印日志吗？
> 
***********************************
Error Happened, F*ck
User xxx can not login
在我看来，打印日志是一门艺术，而且长期被程序员所忽视，本文尝试回答以下几个问题，分享我对打印日志的一些思考：
(1) why or when to log
(2) what to log
(3) how to log and tips
注意：本文中会用**“打印”**日志来统一表示日志的输出，但不是我们狭义理解的输出到终端或者stdout、stderr， 而是泛指将日志内容输出到任何目标，包括但不限于终端、文件、网络传输。
# why or when to log
为什么时候要打印日志，或者什么时候打印日志，在我看来是同一个问题，那就是打印日志的目的。不同的目的，决定了日志内容的格式，输出的频度，输出的目的地。那可能有哪些打印日志的原因？
（1）调试开发
目的是开发期调试程序使用，这种日志量比较大，且没有什么实质性的意义，只应该出现在开发期，而不应该在项目上线之后输出。如何控制这种类型的日志是否输出，后面也会详细讨论。
（2）用户行为日志
这种类型的日志，记录用户的操作行为，用于大数据分析，比如监控、风控、推荐等等。这种日志，一般是给其他团队分析使用，而且可能是多个团队，因此一般会有一定的格式要求，开发者应该按照这个格式来记录，便于其他团队的使用。当然，要记录哪些行为、操作，一般也是约定好的，因此，开发者主要是执行的角色。
（3）程序运行日志
记录程序的运行状况，特别是非预期的行为、异常情况，这种日志，主要是给开发、维护人员使用。什么时候记录，记录什么内容，完全取决于开发人员，开发者具有高度自主性。本文讨论的主要也是指这种类型的日志，因为作为一个服务端开发、运维人员，程序运行日志往往是解决线上问题的救命稻草。
（4）记录系统或者机器的状态
比如网络请求、系统CPU、内存、IO使用情况等等，这种日志主要是给运维人员使用，生成各种更直观的展现形式，在系统出问题的时候报警。
# what to log
一条日志要包含哪些内容，我觉的应该包含 when、where、how、what、who、context，具体含义会在下面一个小节介绍。
我们要记录日志，总是要在某个时机，比如用户的某个请求、某个网络调用、或者内部状态发生了改变。在后文中，统称为事件（event），即日志内容是对某个事件的描述。
## when: the time event happens
when，就是我们打印日志的时间（时间戳），注意这里的时间指的是日志记录的事件的发生时间，而不是日志被最终输出的时间。比如如果日志的输出目标是文件，那么这里的when不是写入到文件的时间，因为往往有延迟。
时间的重要性，在《[Distributed systems for fun and profit](http://book.mixu.net/distsys/time.html)》中有很好的讲述。
首先，时间可以被解读（Interpretaion）成具体的、现实的日期，让我们可以知道事件发生时的时间环境，比如出问题的时候是不是有什么活动、结合系统日志分析当时服务器的网络、CPU、IO是怎么样。具体的时间点也可以帮助我们分析事件的发生是不是存在某种规律，比如是不是每天、每周、每月的固定时间点都会出问题。
其次，时间可以表示一个（一组）事件的持续时间（duration）。比如，可以监控处一段代码的执行时间，也可以记录一个网络请求的耗时。这个时间差也能给出我们很多信息，比如根据经验预判当时程序的运行状态，是否比较‘卡’。
最后，时间代表了事件发生的顺序（order），我们将多个日志按照时间排序，这个顺序能帮助我们debug到底问题是怎么产生的，是按照什么样的时序。这对于多进程（多线程）、异步、分布式程序而言非常重要。虽然我们知道在分布式系统维护全局的时间（global clock）是很复杂的一件事情，但我们使用NTP协议，基本上能解决大部分的问题。
## where: where the event happens
where，就是指日志是在哪里的被记录的，本质上来说，是事件的产生地点。根据情况，可以具体到是哪个模块、哪个文件，甚至是哪一个函数、哪一行代码。实践告知，至少应该包含模块信息。
where的意义在于能够让自己或者其他程序员一眼就看出这条日志是在哪里产生的，这样就能大致定位问题处在哪里，而不用从日志内容全局grep代码
## how：how importance of the event
how important，代表了事件的重要性，我们会打印很多各种类型的日志，但是不同的日志的重要性是不一样的。比如，调试日志是最不重要的，是不应该出现在线上项目的，但是程序运行报错日志却需要认真对待，因为代表程序已经出现了异常。即使是程序的报错日志，也有不同的紧急程度，一次网络请求超时跟子进程异常退出份量完全不一样。
因此，每一条日志都应该有log level，log level代表了日志的重要性、紧急程度。不同的语言、框架的level细分有一定的差异，但都会包括debug，info，warn，error，fatal（critical）。其重要程度从名字就可以看出。
当然，有时候，warn与error，或者error与fatal之间的界限不那么明显，这个需要在团队之间达成共识。在我们的线上项目，对于error、fatal级别的日志都会报警，如果出现了error日志，那么最迟需要第二天去处理，如果是fatal日志，即使是在大半夜，也得立刻起来分析、处理。
## what：what is the log message
what是日志的主体内容，应该简明扼要的描述发生的什么事情。要求可以通过日志本身，而不是重新阅读产生日志的代码，来大致搞清楚发生了什么事情。所以，下面这个日志是不合格的：

```
def user_login(username, password):
    if not valid_username(username):
        logger.warn('Error Happened')
        return
    # some others code
```
是的，我知道，出了问题了，但是日志应该告诉我出了什么问题，所以日志至少应该是这样的：

```
def user_login(username, password):
    if not valid_username(username):
        logger.warn('user_login failed due to unvalid_username')
        return
    # some others code
```
## who：the uniq identify
who代表了事件产生者的唯一标识（identity），用于区分同样的事件。特别是在服务器端，都是大量用户、请求的并发，如果日志内容不包含唯一标识信息，那么这条日志就会淹没在茫茫大海中，比如下面这条日志：

```
def user_login(username, password):
    # some code has check the username
    if not valid_password(password) or not check_password(username, password):
        logger.warn('user_login failed due to password')
        return
    # some others code
```
上面的代码给出了出了什么问题，但是没有包含是谁出了问题，至少应该是

```
def user_login(username, password):
    # some code has check the username
    if not valid_password(password) or not check_password(username, password):
        logger.warn('user_login failed due to password, username %s', username)
        return
```
日志的唯一标识也能帮助我们检索、过滤，找出该唯一标识的最近一段时间的所有日志，再按照时间排序，就能还原日志主体的活动轨迹，比如一个用户在网站上的操作会发散到多个进程、服务。当通过用户名（用户的唯一标识）搜索日志时，就能还原该玩家的操作记录，这个在查线上问题的时候非常有效。
当然，这个唯一标识是很广泛的，需要根据具体情况决定，如果网络请求，可能更好的是requestid、sessionid；如果是系统日志，那么可能是进程、线程ID；如果是分布式集群，那么可能是副本的唯一id
## context: environment when event happens
日志记录的事件发生的上下文环境直观重要，能告知事件是在什么样的情况发生的。当然，上面提到的when、where、who都属于上下文，这些都是固定的，通用的。而在本小节，context专指高度依赖于具体的日志内容的信息，这些信息，是用于定位问题的具体原因。比如

```
def user_login(username, password):
    # some code has check the username
    if not valid_password(password) or not check_password(username, password):
        logger.warn('user_login failed due to password, username %s', username)
        return
    # some others code
```
阅读代码，当password不符合规范，或者校验password失败的时候都会出错，但是到底是那种情况呢，日志内容并没有指出，所以这条日志仍然不合格。
在我看来，这部分是弹性最大，但是又最重要的部分，造成异常的情况很多，记录哪些信息，完全取决于写代码的程序员。但事实上也很简单，遵循一个原则即可：**想想加上哪些信息能定位问题发生的原因**！
# how to log and tips
## use logging framework
早期的编程语言提供的日志功能都比较初级，一般都是输出到终端或者文件，如C++、Java，而且不支持统一的输出格式的配置。对于这些语言，一般会有单独的log框架，如glog，log4cpp，log4j，elf4j等等。而更新的一些语言，一般都内置强大的logging模块，如python。
在前面在讲述“what to log”的时候，我们指出要记录日志发生的时间、地点、等级等信息。那么这些信息是不是每次都在日志内容里面打印呢，这些内容确实是一条完整的日志所必须的，但是如果要程序员写每条日志的时候都得手动加上这些内容，那么无疑是一种负担。框架、模块的作用就是将程序员从这些繁文缛节中解放出来。比如在很多logging框架模块（thon logging、log4j）中，都有named logger这一概念，这个name就可以是module， filename， classname或者instance，这就解决了上一章节提到了“where the event happen”这个问题。
在这里，对logging框架的介绍就不详细展开，只是总结一下logging框架、模块的功能
（1）设置log的输出等级
这样可以不改程序代码，仅仅修改log的输出等级，就能够控制哪些日志输出，哪些日志不输出。比如我们在开发期的调试日志，都可以设置为DEBUG，上线的时候设置输出等级为INFO，那么这些调试日志就不会被输出。
（2）设置每条日志默认包含的内容
以[Python logging](https://docs.python.org/2/library/logging.html#logrecord-attributes)为例，可以通过formatter设置每条日志默认包含哪些信息，比如时间、文件、行号、进程、线程信息。
（3）设置日志的输出目标
通过配置，可以指定日志是输出到stdout，还是文件，还是网络。特别是在linux服务器上，将日志输出到syslog，再使用syslog强大的处理、分发功能，配合elk系统进行分析，是很多应用程序的通用做法。
## log never throw
我们打印日志，是为了记录事故发生的现场，以便发现问题，解决问题。那么就得保证，打印日志这一行为本身不能引入新的问题，比如，不能出错抛异常。这就好比，处理车祸的消防车不能冲进现场一样。
但是，越重要的日志，比如Error、 Fatal级别的日志，出现的概率应该是越低的，我们也不希望产生这样的日志，因为一旦出现就意味着异常或者线上事故。这样的日志，就一定要做好单元测试、覆盖率测试，保证日志本身是能够正常工作的。
## log when u think something never happen
这一点，应该是针对why or when to log而言。就是说，当你认为某种情况一定不会发生，按照墨菲定律，它还是很可能会发生，那么就应该在万一发生的情况记录log，Error（Fatal）级别的log。尤其是在异步，并发的情况下，程序员任务的不可能都会成为可能。
比如下面的伪代码

```
def magic_func():
    if check_ok():
        do_something()
    return
```
代码很简单，如果条件满足，那么就做相应处理，否则直接返回。这里有两种可能，第一种是条件不满足是可能的、可预期的情况，只是说，在这种情况下什么都不用作；第二种情况是程序员觉得条件一定会满足，一定不会出现else的情况，所以什么都没做。如果是第二种情况，那么就一定得加日志，因为一切都可能发生。
## lazy logging
日志的内容，在最终输出的时候，应该是一个字符串，那么这个字符串是什么时候产生呢？前面提到，我们可以通过log level来控制一条日志是否被输出，那么字符串的生成越迟越好，因为有可能日志根本就无需输出，也就无需生成这个字符串，这也是我之前提到过的[lazy思想](https://www.cnblogs.com/xybaby/p/6425735.html)。
至于lazy logging，各个语言、框架都是大同小异的，在[这篇文章](https://garygregory.wordpress.com/2015/10/08/the-art-of-logging-advanced-message-formatting/)中，举出了log4j的例子，在这里还是用我比较熟悉的python讲解这个例子

```
#coding=utf-8
import logging
logger = logging.getLogger('LazyLogging')
logger.setLevel(logging.DEBUG)
hander = logging.StreamHandler()
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s') 
hander.setFormatter(formatter)  
logger.addHandler(hander)
def getUserCount():
    logger.info('getUserCount is called')
    return 1
logger.debug("There are " + str(getUserCount()) + " users logged in now.")
```
上面的代码可谓麻雀虽小五脏俱全，包含了使用logging框架的方方面面。第4行是named logger，第5行设置log level，第6行指定输出目标，第7行指定输出格式（包括时间，logger name，log level，具体的日志内容）。
运行结果如下：
> 
2017-12-02 17:52:20,049 – LazyLogging – DEBUG – getUserCount is called
2017-12-02 17:52:20,049 – LazyLogging – INFO – There are 1 users logged in now.
由于当前的log level是DEBUG，那么第15行的日志会输出，这是正常的。
但是将log level改成INFO，*“getUserCount is called”*这条日志仍然会被输出。这就暴露出两个问题，第一，即使日志内容最终不会被输出，仍然会生成一个字符串；第二，一定会调用getUserCount 这个函数，而如果根据log lebel不需要输出日志，这两步都是没有必要的
要解决第一个问题很简单，将第15行改成
> 
logger.debug(“There are %s users logged in now.”, getUserCount())
即可，但是*“getUserCount is called”*这条日志仍然会被输出，即第二个问题仍然没有解决
按照stackoverflow上的[这篇文章](https://stackoverflow.com/questions/4148790/lazy-logger-message-string-evaluation)，也是可以解决第二个问题的，方案如下

```
class lazyEval:
    def __init__(self, f, *args):
        self.func = f
        self.args = args
    def __str__(self):
        return str(self.func(*self.args))
logger.debug("There are %s users logged in now.", lazyEval(getUserCount))
```
在log lever为INFO的时候，可以发现*“getUserCount is called”*也不会被输出了，即不会调用getUserCount函数，做到了真正的lazy logging。
但是，这是非常不好的写法！除非你们保证所调用的函数是没有副作用的（side－effect），即函数的调用无状态，不会改变程序的状态，否则程序的状态居然会依赖于log level，这是一个陷阱，一个坑，一点都不pythonic。
## consider async logging
日志的记录也是要消耗时间的，这也是我们不能到处打印日志的原因。一般来说，logging框架的效率都是比较高的，但是如果profile发现日志的输出确实带来了不容忽视的消耗，那么也是值得优化的，其中的一个方法就是异步log，以避免日志输出阻塞线程
# conclusion
在网上看到这么一句话
> 
Logs are like car insurance. Nobody wants to pay for it, but when something goes wrong everyone wants the best available
log就像车辆保险，没人愿意为保险付钱，但是一旦出了问题谁都又想有保险可用。
我们打印日志的时候都很随意，于是在差bug的时候，就会骂别人、或者几天前的自己：SB！
从今天起，写好每一条日志吧，与君共勉！
# 参考
- [Distributed systems for fun and profit](http://book.mixu.net/distsys/time.html)
- [python logging#logrecord-attributes](https://docs.python.org/2/library/logging.html#logrecord-attributes)
- [the-art-of-logging-advanced-message-formatting](https://garygregory.wordpress.com/2015/10/08/the-art-of-logging-advanced-message-formatting/)
