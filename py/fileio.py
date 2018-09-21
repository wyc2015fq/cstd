

def file_read(fn, encoding='gbk'):
    fp = open(fn, 'r', encoding=encoding)
    content = fp.read()
    fp.close()
    return content

def file_readlines(fn, encoding='gbk'):
    fp = open(fn, 'r', encoding=encoding)
    content = fp.readlines()
    fp.close()
    return content

