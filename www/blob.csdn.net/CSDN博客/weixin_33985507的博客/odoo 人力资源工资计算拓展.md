# odoo 人力资源工资计算拓展 - weixin_33985507的博客 - CSDN博客
2016年11月14日 10:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
默认情况下 odoo工资条的计算只支持一下几种python变量：
# payslip: object containing the payslips
# employee: hr.employee object
# contract: hr.contract object
# rules: object containing the rules code (previously computed)
# categories: object containing the computed salary rule categories (sum of amount of all rules belonging to that category).
# worked_days: object containing the computed worked days.
# inputs: object containing the computed inputs.
# Note: returned value have to be set in the variable 'result'
实际使用过程种，我们可能想要用的到变量要远远多于这些个变量，比如，如果工资条上想要加入对销售订单的提成比例，那么这个公式按道理应该是可以写到工资计算规则里的
可是，我们应该如何获取销售单对象呢？（sale.order)
具体方法，请阅读以下文章：
[http://www.cnblogs.com/post/readauth?url=/kfx2007/p/6060871.html](http://www.cnblogs.com/post/readauth?url=/kfx2007/p/6060871.html)
或者安装qunar_payroll这个模块。
