# DSL与自动化测试 – 用Python实现简单的DSL - 右上角同学的专栏 - CSDN博客
2016年08月05日 16:09:37[右上角徐](https://me.csdn.net/u011032983)阅读数：2210
# DSL与自动化测试 – 用Python实现简单的DSL
本文转自：http://magustest.com/blog/automationtesting/dsl-automation-testing-using-python/comment-page-1/
自动化测试，一个在测试领域中被广为熟知，也是被谈论最多的概念之一。DSL (Domain Specific Language)，一种高度抽象，用于某个特定领域下编程语言。软件测试在大多数情况下都是对某个特定行业的软件系统进行测试，所以这两者应该可以很好的结合起来，事实上也是这样的，QTP里面的keyword view，其实就是DSL的一个实现。DSL一般可以分为两个大的类型，分别是External DSL 和 Internal DSL （引用自Martin Fowler）。External DSL 一般来说是跟其实现语言不一样的
 DSL，常见的External DSL 有：SQL和XML配置文件；而Internal DSL 一般来说就是该DSL使用某个现成的编程语言（就是所谓的host language)，然后对host language进行一些改造而成。
我们在测试中会遇到很多问题，其中一些问题，几乎是所有公司所有团队都会遇到的，例如测试覆盖率不够，测试的时间不够等等。面对这些问题，自动化测试自然而然地成为解决这些问题的首选方法。但是自动化测试真的就是银弹麽？不见得！以前曾经在ASP.NET QA 的博客中给他们留言，请教过关于自动化测试的事情，我记得其中有一个回复是说，在某个release中过度地使用自动化测试，一切东西都想实现自动化测试，而忽略了产品本身的功能、特性的关注，结果就是超高的自动化测试覆盖率，但是很差的产品质量。大家都去实现自动化测试了，谁来做功能点的覆盖呢？某些领域的专家（SME），他们可能对测试技术是一无所知的，要把这些领域专家和测试实施结合起来，DSL就是一个比较好的桥梁。
我在工作中遇到的问题是，我需要测试一个类似UV（独立用户访问数）统计的系统，统计UV的方法其实就是根据_uid cookie的值来判断这个用户在某段时间内访问过我们的系统多少次，访问了哪些站点，进行了什么样的行为。其中有2个地方比较麻烦，第一就是在测试过程中要不断地拷贝cookie，这样拷来拷去两三次以后很容易就混乱，出错；第二就是需要记录访问哪些站点，这些站点都只是ID，也是需要不断地修改请求，测试时间长了也是很容易出错。所以我就打算在原来的测试工具基础上，实现一个简单的Internal DSL。先看成品：
```python
@tc
def uniq_inventory_case01():
    test= testTool()
    test.user('a').view('asset55100002').anetwork('55100').onsite('site55100503').snetwork('55100').dnetwork('55100').times(1).go()
    test.user('b').view('asset55100002').anetwork('55100').onsite('site55100503').snetwork('55100').dnetwork('55100').times(2).go()
    test.user('b').view('asset55100002').anetwork('55100').onsite('site55100504_noad').snetwork('55100').dnetwork('55100').times(4).go()
```
|||
实例化一个testTool对象，然后就是指定哪个用户：user(‘a’)或者user(‘b’)，看的视频的ID：view(‘asset55100002’)，这个视频属于哪个CRO呢？anetwork(‘55100’)；放在哪个网站呢？onsite(‘site55100503’)；网站是谁的呢？snetwork(‘55100’)；谁是分发者呢？dnetwork(‘55100’)；看了多少次呢？times(4)；最后一个有点儿丑陋的go()。
像这样子一句话里面N个方法连着用，就叫Method Chaining，Method Chaining通常可以让代码变得更加人性化，读起来更加容易。但是使用Method Chaining通常会遇到一个问题，就是很难判断就是到了哪个方法才是终结呢？是不是有些方法的调用是可选的，有些是必选的呢？其中一个解决方法就是我用到的，放一个.go()方法在最后，作为终结方法。要实现Method Chaining，其实只需要顶一个类，对于需要做连接的方法，最后都返回这个类的实例。例如：
```python
def view(self, assetid):
    if assetid:     self.asset_id = assetid
    return self
 
def anetwork(self, networkid):
    if networkid:   self.a_network_id = networkid
    return self
 
def snetwork(self, networkid):
    if networkid:   self.s_network_id = networkid
    return self
 
def dnetwork(self, networkid):
    if networkid:   self.d_network_id = networkid
    return self
 
def onsite(self, sectionid):
    if sectionid:   self.site_section_id = sectionid
    return self
 
def times(self, times):
    if times>0:       self.request_times = times
    return self
```
最后一个终结方法go()，就做真正的处理
```python
def go(self):
    if self.asset_id and self.site_section_id and self.times and self.a_network_id and self.s_network_id:
        self.prepareRequest()
        for i in range(self.request_times):
            self.sendRequest()
 
        self.cleanup()
    else:
        info = 'Required information missing, abort running.'
        logging.debug(info)
        print info
```
如果是实现一个External DSL 的话，的确难度不小；但是Internal DSL其实并不是很高深，也不是很难实现，在它的帮助下，可以把工作完成的更好，对自己以后维护测试用例也带来了不少方便。
