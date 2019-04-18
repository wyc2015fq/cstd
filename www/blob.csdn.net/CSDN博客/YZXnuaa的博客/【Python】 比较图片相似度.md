# 【Python】 比较图片相似度 - YZXnuaa的博客 - CSDN博客
2018年10月15日 20:18:34[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：477

# Filename: histsimilar.py
# -*- coding: utf-8 -*-
import Image
def make_regalur_image(img, size = (256, 256)):
    return img.resize(size).convert('RGB')
#几何转变，全部转化为256*256像素大小
def split_image(img, part_size = (64, 64)):
    w, h = img.size
    pw, ph = part_size
    assert w % pw == h % ph == 0
    return [img.crop((i, j, i+pw, j+ph)).copy() \
                for i in xrange(0, w, pw) \
                for j in xrange(0, h, ph)]
#region = img.crop(box)
#将img表示的图片对象拷贝到region中，这个region可以用来后续的操作（region其实就是一个
#image对象，box是个四元组（上下左右））
def hist_similar(lh, rh):
    assert len(lh) == len(rh)
    return sum(1 - (0 if l == r else float(abs(l - r))/max(l, r)) for l, r in zip(lh, rh))/len(lh)
#好像是根据图片的左右间隔来计算某个长度，zip是可以接受多个x,y,z数组值统一输出的输出语句
def calc_similar(li, ri):
#    return hist_similar(li.histogram(), ri.histogram())
    return sum(hist_similar(l.histogram(), r.histogram()) for l, r in zip(split_image(li), split_image(ri))) / 16.0 #256>64
    #其中histogram()对数组x（数组是随机取样得到的）进行直方图统计，它将数组x的取值范围分为100个区间，
        #并统计x中的每个值落入各个区间中的次数。histogram()返回两个数组p和t2，
        #其中p表示各个区间的取样值出现的频数，t2表示区间。
        #大概是计算一个像素点有多少颜色分布的
        #把split_image处理的东西zip一下，进行histogram,然后得到这个值
def calc_similar_by_path(lf, rf):
    li, ri = make_regalur_image(Image.open(lf)), make_regalur_image(Image.open(rf))
    return calc_similar(li, ri)
def make_doc_data(lf, rf):
    li, ri = make_regalur_image(Image.open(lf)), make_regalur_image(Image.open(rf))
    li.save(lf + '_regalur.png')#转换图片格式:img.save('file.jpg'),保存临时的
    ri.save(rf + '_regalur.png')#img对象到硬盘
    fd = open('stat.csv', 'w')#stat模块是做随机变量统计的，stat用来计算随机变量的期望值和方差
        #这句是关键啊，把histogram的结果进行map处理
    fd.write('\n'.join(l + ',' + r for l, r in zip(map(str, li.histogram()), map(str, ri.histogram()))))
#    print >>fd, '\n'
#    fd.write(','.join(map(str, ri.histogram())))
    fd.close()
    import ImageDraw
    li = li.convert('RGB') #与save对象，这是转换格式
    draw = ImageDraw.Draw(li)
    for i in xrange(0, 256, 64):
        draw.line((0, i, 256, i), fill = '#ff0000')
        draw.line((i, 0, i, 256), fill = '#ff0000')
        #从始至终划线!!!!!!!!!!!!!!!通过把每一列刷成红色，来进行颜色的随机分布划分
        #用法：pygame.draw.line(Surface, color, start_pos, end_pos, width=1)
    li.save(lf + '_lines.png')
if __name__ == '__main__':
    path = r'testpic/TEST%d/%d.JPG'
    for i in xrange(1, 7):
        print 'test_case_%d: %.3f%%'%(i, \
            calc_similar_by_path('testpic/TEST%d/%d.JPG'%(i, 1), 'testpic/TEST%d/%d.JPG'%(i, 2))*100)
#    make_doc_data('test/TEST4/1.JPG', 'test/TEST4/2.JPG')
</span>
这个主要的思想是：每张图片都可以生成颜色分布的直方图（color histogram）。如果两张图片的直方图很接近，就可以认为它们很相似。
详细的看这里：google识图
感觉用Wote写的ImgHash更加简洁：
<span style="font-size:14px;">import glob
import os
import sys
from PIL import Image
EXTS = 'jpg', 'jpeg', 'JPG', 'JPEG', 'gif', 'GIF', 'png', 'PNG'
def avhash(im):#通过计算哈希值来得到该张图片的“指纹”
    if not isinstance(im, Image.Image):#判断参数im，是不是Image类的一个参数
        im = Image.open(im)
    im = im.resize((8, 8), Image.ANTIALIAS).convert('L')
    #resize，格式转换，把图片压缩成8*8大小，ANTIALIAS是抗锯齿效果开启，“L”是将其转化为
    #64级灰度，即一共有64种颜色
    avg = reduce(lambda x, y: x + y, im.getdata()) / 64.#递归取值，这里是计算所有
                                                        #64个像素的灰度平均值
    return reduce(lambda x, (y, z): x | (z << y),
                  enumerate(map(lambda i: 0 if i < avg else 1, im.getdata())),
                  0)#比较像素的灰度，将每个像素的灰度与平均值进行比较，>=avg：1；<avg：0
def hamming(h1, h2):#比较指纹，等同于计算“汉明距离”（两个字符串对应位置的字符不同的个数）
    h, d = 0, h1 ^ h2
    while d:
        h += 1
        d &= d - 1
    return h
if __name__ == '__main__':
    if len(sys.argv) <= 1 or len(sys.argv) > 3:#sys.argv是用来获取命令行参数的，[0]是本身路径
        print "Usage: %s image.jpg [dir]" % sys.argv[0]#起码要有>1，才能有2张图比较
    else:
        im, wd = sys.argv[1], '.' if len(sys.argv) < 3 else sys.argv[2]
        h = avhash(im)
        os.chdir(wd)#chdir是更改目录函数
        images = []
        for ext in EXTS:
            images.extend(glob.glob('*.%s' % ext))
         #返回一个含有包含有匹配文件/目录的数组,在比对之前
        seq = []
        prog = int(len(images) > 50 and sys.stdout.isatty())
        for f in images:
            seq.append((f, hamming(avhash(f), h)))
            if prog:
                perc = 100. * prog / len(images)
                x = int(2 * perc / 5)
                print '\rCalculating... [' + '#' * x + ' ' * (40 - x) + ']',
                print '%.2f%%' % perc, '(%d/%d)' % (prog, len(images)),
                sys.stdout.flush()
                prog += 1
        if prog: print
        for f, ham in sorted(seq, key=lambda i: i[1]):
            print "%d\t%s" % (ham, f)
