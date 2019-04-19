# LLVM每日谈之二十八 I am leaving llvm - SHINING的博客 - CSDN博客
2018年05月08日 13:28:55[snsn1984](https://me.csdn.net/snsn1984)阅读数：546标签：[LLVM																[开源社区																[贡献																[Rafael 																[社区环境](https://so.csdn.net/so/search/s.do?q=社区环境&t=blog)](https://so.csdn.net/so/search/s.do?q=Rafael &t=blog)](https://so.csdn.net/so/search/s.do?q=贡献&t=blog)](https://so.csdn.net/so/search/s.do?q=开源社区&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
Rafael 于当地时间五月二日宣称离开LLVM社区，在网络引发了广泛的讨论。Rafael 作为LLVM贡献排名第五的资深贡献者，对LLVM社区贡献极大，他一共提交了4,344个节点，占LLVM提交节点总数的2.65%，共提交了157,679行代码。
Rafael 离开的根本原因还是社区环境的变化，最终的触发事件是社区与Outreachy的合作。不管怎么说，Rafael 的离开让人觉得惋惜。感谢他一直以来为LLVM社区所做的贡献。
邮件全文：
Summary:
I am leaving llvm effectively immediately. I am sorry for any
inconvenience this may cause.
Practicalities:
I can unsubscribe myself from the email lists and I disabled email
notification on bugzilla and phabricator. Could someone please disable
my account on phabricator and delete my svn access? Thanks.
The long story:
I first became aware of llvm during a compiler course at university. I
wanted to write a toy scheme frontend to a real compiler. To my shame
I missed that llvm had a mem2reg pass and selected gcc to avoid having
to compute ssa form myself.
After contributing a few patches to gcc it was clear that the frontend
interface needed some cleanup. At the time llvm was being considered
as a potential new gcc architecture and the idea of a well defined IR
with a textual representation was a revolution.
On my first job (indt) we were using arm cpus and I was able to sell
the idea of starting an llvm backend for arm. My first commit was on
May 14, 2006. I am incredibly grateful to both indt and the llvm
developers for trusting and helping such an inexperienced and unknown
developer with such a large task.
It is only in May 2007 in the dev meeting that I got to meet the other
developers in person. It was an incredibly fun event and people were
as friendly in person as on the list.
In the next few years I was working at google. First as an sre and
then a compiler developer on gcc. During that time I kept llvm as my
20% project as much as possible. Working on it was always a refreshing
experience. It was far easier to change and far less political than gcc
at the time.
My opportunity to be back full time on llvm came with portable native
client (pnacl). They needed to be able to emit elf objects from llvm ir
and so I went to work on elf support for mc.
Unfortunately another job change (mozilla) made llvm a side project
again after that. I still managed to contribute to llvm/clang as I
helped mozilla transition away from gcc 4.2 on OS X.
It is only about 5 years ago that I started working on llvm full time
again. The big item this time was elf support in lld. I was really
excited when Rui posted a new design for a coff linker and did my best
to find a corresponding design for elf.
Unfortunately the last few years haven't been the same. On the
technical side llvm now feels far bigger and slower to change. There
are many incomplete transitions. That, by itself, would not be
sufficient reason to leave. llvm still seems better than the
competition and lld itself is still awesome.
The reason for me leaving are the changes in the community. The
current license change discussions unfortunately bring to memory the
fsf politics when I was working on gcc. That would still not be
sufficient reason to leave. As with the code, llvm will still have the
best license and if the only community change was the handling of the
license change I would probably keep going.
The community change I cannot take is how the social injustice
movement has permeated it. When I joined llvm no one asked or cared
about my religion or political view. We all seemed committed to just
writing a good compiler framework.
Somewhat recently a code of conduct was adopted. It says that the
community tries to welcome people of all "political belief". Except
those whose political belief mean that they don't agree with the code
of conduct. Since agreement is required to take part in the
conferences, I am no longer able to attend.
The last drop was llvm associating itself with an organization that
openly discriminates based on sex and ancestry (1,2). This goes
directly against my ethical views and I think I must leave the project
to not be associated with this.
So long, and thanks for all the bugs,
Rafael
地址：[[llvm-dev] I am leaving llvm](https://link.zhihu.com/?target=http%3A//lists.llvm.org/pipermail/llvm-dev/2018-May/122922.html)
参考文献：
1、https://www.phoronix.com/scan.php?page=news_item&px=LLVM-Rafael-Espindola
2、http://lists.llvm.org/pipermail/llvm-dev/2018-May/122922.html
3、https://www.cnbeta.com/articles/soft/722947.htm
