# 生成全局唯一Id - Arthur的随笔 - CSDN博客
2013年09月29日 00:38:01[largetalk](https://me.csdn.net/largetalk)阅读数：6744
# 生成全局唯一Id[](#id)
参考了: [http://www.cnblogs.com/heyuquan/p/global-guid-identity-maxId.html](http://www.cnblogs.com/heyuquan/p/global-guid-identity-maxId.html)
- GUID
importuuiduuid.uuid1()
优点： 确保唯一， 速度快
缺点： 太长， 不友好, 不好索引
- 数据库唯一索引
时间戳加上随机数，然后通过数据库做唯一性校验
importtimeimportrandomimportstringm=time.strftime('%y%m%d%H%M%S')+''.join([random.choice(string.lowercase+string.digits)for_inrange()])#检查m在数据库中是否存在，存在则重复上述过程，不存在则存入数据库并返回
优点：适合简单应用，id较短，有一定亲和力
缺点：每秒id总数有限制，并发越大性能越低, 加大数据库访问压力，需要锁表
优化：将时间戳转成62进制数
digit62='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'#整数转化为62进制字符串#入口：#   x : 整数#返回： 字符串defint_to_str62(x):try:x=int(x)except:x=ifx<:x=-xifx==:return"0"s=""whilex>=:x1=x%s=digit62[x1]+sx=x//ifx>:s=digit62[x]+sreturns#62进制字符串转化为整数#入口：#   s : 62进制字符串#返回： 整数defstr62_to_int(s):x=s=str(s).strip()ifs=="":returnxforyins:k=digit62.find(y)ifk>=:x=x*+kreturnximporttimeimportrandomimportstringt=time.strftime('%y%m%d%H%M%S')cut=[t[i:i+]foriinrange(,len(t),)]t=''.join([int_to_str62(int(x))forxincut])m=t+''.join([random.choice(string.lowercase+string.digits)for_inrange()])
再ps. 有人说random.choice慢而且随机不均匀，我就写了两个小程序测试一下
importrandomimporttimeimportstringimporttimeitimporthashlibimportuuidimportthreadingdefrandomchoice():return''.join([random.choice(string.lowercase+string.digits)for_inrange()])def_time(f,n=):print'start timeit function ',ft=timeit.timeit(f,number=n)print'repeat %s times and used %ss'%(n,t)print'end timeit function ',fprint_time(randomchoice)
result
start timeit function  <function randomchoice at 0x2a7d6e0>
repeat 1000000 times and used 3.97338795662s
end timeit function  <function randomchoice at 0x2a7d6e0>
随机分布
fromrandomimportchoiceimportstringimportcollectionsfrommatplotlib.pyplotimportplot,show,barh,yticks,xlabel,title,figureimportnumpyasnptables=string.ascii_letters+string.digitscounter=collections.Counter()for_inrange():counter[choice(tables)]+=alphats=counter.keys()y_pos=np.arange(len(alphats))freq=counter.values()figure(figsize=(,))barh(y_pos,freq,align='edge',alpha=,height=0.05)yticks(y_pos,alphats)xlabel('frequence')title('random choice')show()
结果图:
![](https://img-blog.csdn.net/20130929003721156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFyZ2V0YWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可见分布还是比较平均的
- like mongo objectid
      时间 ＋ md5(hostname) + pid + 递增id
importstructimportsocketimportosimporttimefromhashlibimportmd5importthreadingimportrandomimportbinascii_inc=random.randint(,0xFFFFFF)_inc_lock=threading.Lock()oid=""oid+=struct.pack(">i",int(time.time()))m=md5()m.update(socket.gethostname())oid+=m.digest()[:]oid+=struct.pack(">H",os.getpid()%0xFFFF)_inc_lock.acquire()oid+=struct.pack(">i",_inc)[:]_inc=(_inc+)%0xFFFFFF_inc_lock.release()printlen(oid)printbinascii.hexlify(oid)
