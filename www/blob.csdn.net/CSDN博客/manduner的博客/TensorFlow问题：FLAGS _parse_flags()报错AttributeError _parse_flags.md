
# TensorFlow问题：FLAGS._parse_flags()报错AttributeError:_parse_flags - manduner的博客 - CSDN博客


2018年07月18日 19:40:02[Manduner_TJU](https://me.csdn.net/manduner)阅读数：4857


# 1，前言
俗话说，难者不会，会者不难。当时解决这个“AttributeError:_parse_flags”时，真真是耗费我一些时间，所以在我解决后，我的第一个想法就是赶紧把TensorFlow这个坑给添上，话不多说了，直接写原因和解决方案
# 2，原因
哎呀、、、、还是TensorFlow的版本问题了，TensorFlow版本升级后，它就无情的抛弃了**FLAGS._parse_flags()**这种用法，改成了用**FLAGS.flag_values_dict()**
# 3，解决办法
聪明如你，应该知道咋解决了吧，就是用**FLAGS.flag_values_dict()**替换**FLAGS._parse_flags()**，问题完美解决，不再出现报错

