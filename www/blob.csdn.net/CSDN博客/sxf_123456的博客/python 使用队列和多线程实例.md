# python 使用队列和多线程实例 - sxf_123456的博客 - CSDN博客
2019年02月18日 16:13:17[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：28
from threading import Thread
from queue import Queue
queues = Queue()
def pro_que():
    """
    产生队列
    :return: 
    """
    for i in range(10):
        queues.put(i)
    queues.put(None)
def sum(num_list):
    """
    消费队列 求和
    :param num_list: 
    :return: 
    """
    sum = 0
    for i in num_list:
        sum += i
    print("sum=" + str(sum))
def sub(num_list):
    """
    消费队列 求差
    :param num_list: 
    :return: 
    """
    sub = 0
    for i in num_list:
        sub -= i
    print("sub=" + str(sub))
def con_que():
    """
    创建两个线程，
    一个求和，一个求差
    :return: 
    """
    data_list = []
    while True:
        data = queues.get()
        if data is None:
            break
        data_list.append(data)
    th_sum = Thread(target=sum, args=(data_list,))
    th_sub = Thread(target=sub, args=(data_list,))
    th_sum.start()
    th_sub.start()
    th_sum.join()
    th_sub.join()
if __name__ == '__main__':
    pro_que()
    con_que()
