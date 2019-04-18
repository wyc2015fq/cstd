# 2018-09-26Argparse中action的可选参数store_true,store_false - weixin_33985507的博客 - CSDN博客
2018年09月26日 15:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
parser.add_argument('-c', action='store_true', default=false)
# python [test.py](https://link.zhihu.com/?target=http%3A//test.py/) -c => c是true（因为action）
# python [test.py](https://link.zhihu.com/?target=http%3A//test.py/)=> c是false（default）
