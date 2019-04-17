# 作业五：RE 模块模拟计算器 - lincappu - 博客园







# [作业五：RE 模块模拟计算器](https://www.cnblogs.com/lincappu/p/8253070.html)





```
# !/usr/bin/env python3
# _*_coding:utf-8_*_
'''
实现模拟计算器的功能：
公式：1 - 2 * ( (60-30 +(-40/5) * (9-2*5/3 + 7 /3*99/4*2998 +10 * 568/14 )) - (-4*3)/ (16-3*2) )
思路：
括号的计算优先级最高，应该优先计算括号内的式子，然后将值代替会原式子，循环，接着做惩处运算，最后是加减运算，然后弹出结果，
# print(eval('1-2*((60-30+(-40/5)*(9-2*5/3+7/3*99/4*2998+10*568/14))-(-4*3)/(16-3*2))'))
'''
import re

symbol = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+', '*', '/', '(', ')']


def check(formula):
    '''
    检查输入的公式的正确性：
    除了十个数字和六个符号，其他的一律判错，但是这样会限制运算规则，目前先是这样规划
    :param formula:
    :return:
    '''
    symbol_flag = True
    formula = re.sub(r'\s*', '', formula)
    cal_list = list(formula)
    for item in cal_list:
        if item not in symbol:
            symbol_flag = False
    if not symbol_flag:
        return 1
    else:
        return formula


def change(formula):
    '''
    替换出现两个运算符在一起的情况：
    :param formula:
    :return:
    '''
    formula = formula.replace('--', '+')
    formula = formula.replace('++', '+')
    formula = formula.replace('**', '*')
    formula = formula.replace('//', '=')
    formula = formula.replace('*+', '*')
    formula = formula.replace('/+', '/')
    return formula


def chengchu(arg):
    val = arg[0]
    match = re.search(r'\d+\.*\d*[\*\/]+[\+\-]?\d+\.*\d*', val)
    if not match:
        return

    content = re.search(r'\d+\.*\d*[\*\/]+[\+\-]?\d+\.*\d*', val).group()
    if len(content.split('*')) > 1:
        n1, n2 = content.split('*')
        value = float(n1) * float(n2)
    else:
        n1, n2 = content.split('/')
        value = float(n1) / float(n2)

    before, after = re.split(r'\d+\.*\d*[\*\/]+[\+\-]?\d+\.*\d*', val, 1)
    new_str = '%s%s%s' % (before, value, after)
    arg[0] = new_str
    print(arg)
    chengchu(arg)


def jiajian(arg):
    while True:
        if arg[0].__contains__('+-') or arg[0].__contains__("++") or arg[0].__contains__('-+') or arg[0].__contains__(
                "--"):
            arg[0] = arg[0].replace('++', '+')
            arg[0] = arg[0].replace('+-', '-')
            arg[0] = arg[0].replace('--', '+')
            arg[0] = arg[0].replace('-+', '-')
        else:
            break

    if arg[0].startswith('-'):
        arg[1] += 1
        arg[0] = arg[0].replace('-', '&')
        arg[0] = arg[0].replace('+', '-')
        arg[0] = arg[0].replace('&', '+')
        arg[0] = arg[0][1:]

    val = arg[0]

    match = re.search(r'\d+\.?\d*[-+]{1}\d+\.?\d*', val)
    if not match:
        return

    content = re.search(r'\d+\.?\d*[-+]{1}\d+\.?\d*', val).group()

    if len(content.split('+')) > 1:
        n1, n2 = content.split('+')
        value = float(n1) + float(n2)
    else:
        n1, n2 = content.split('-')
        value = float(n1) - float(n2)

    before, after = re.split(r'\d+\.?\d*[-+]{1}\d+\.?\d*', val, 1)
    new_str = '%s%s%s' % (before, value, after)
    arg[0] = new_str
    jiajian(arg)


def compute(formula):
    '''
    操作加减乘除：根据操作
    :param formula:
    :return:
    '''
    inp = [formula, 0]  #
    chengchu(inp)
    jiajian(inp)
    if divmod(inp[1], 2)[1] == 1:
        result = float(inp[0])
        result = float(result) * -1
    else:
        result = float(inp[0])
    return result


def calcultor(formula):
    '''
    递归处理括号，并计算括号内的式子，将结果返回做替换，
    :param formula:
    :return:
    '''
    # 没有括号了
    if not re.search(r'\(([-+*/]*\d+\.*\d*){2,}\)', formula):
        final = compute(formula)
        return final
    # 取出其中的一个括号，然后计算
    content = re.search(r'\(([-+*/]*\d\.*\d*){2,}\)', formula).group()
    # 分裂表达式
    before, nothing, after = re.split(r'\(([-+*/]*\d+\.*\d*){2,}\)', formula, 1)
    # 计算括号里的内容
    content = content[1:len(content) - 1]
    ret = compute(content)
    # 计算完后然后拿到结果，然后重新拼接成一个新的式子
    formula = '%s%s%s' % (before, ret, after)
    return calcultor(formula)


# s='1-2*((60-30+(-40/5)*(9-2*5/3+7/3*99/4*2998+10*568/14))-(-4*3)/(16-3*2))'
# calcultor(s)



def main():
    print('欢迎使用计算器'.center(60, '#'))
    while True:
        formula = input('计算式>>:').strip()
        if formula == 'q':
            exit()
        elif len(formula) == 0:
            continue
        else:
            formula=check(formula)
            if formula == 1:
                continue
            else:
                result = calcultor(formula)
                print('计算结果为：\033[1;31;1m %s\033[0m' % (result))
                try:
                    print('正确答案是：\033[1;31;1m %s\033[0m' %(eval(formula)))
                except:
                    print('无法计算')



if __name__ == '__main__':
    main()
```














