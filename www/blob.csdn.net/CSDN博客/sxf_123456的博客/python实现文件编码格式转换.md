# python实现文件编码格式转换 - sxf_123456的博客 - CSDN博客
2018年05月16日 09:36:22[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3107标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
方法一def _read(from_filename, from_encode):
    with open(from_filename, "r", encoding=from_encode) as f:
        for lines in iter(lambda: tuple(islice(f, 1000000)), ()):
            _queue.put(lines)
    _queue.put(None)
def convert_file_to_utf8(p_task, **kwargs):
    *"""**    转化ftp下载的文件格式****:param**** p_task:  不同下载文件的task_id****:param**** kwargs:****:return****:**    """*local_file = kwargs['ti'].xcom_pull(task_ids=p_task)
    convert_file = local_file[0:len(local_file) - 3] + "csv"
th = Process(target=_read, args=(local_file, "GB18030"))
    th.start()
    with open(convert_file, "w", encoding="utf-8") as f:
        while True:
            lines = _queue.get()
            if lines is None:
                break
f.write(''.join(lines))
    th.join()方法二with open(filename, 'r', encoding="GB18030") as f:
    i = 0
for row_ in f:
        row = row_.strip().encode("utf-8").decode("utf-8")
        i += 1性能比较：对1M大小文件进行实验。方法一的消耗时间为44893微秒，方法二消耗时间为49015微秒。使用方法一比使用方法二转换文件格式更快。
