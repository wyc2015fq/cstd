
from datetime import date
from datetime import timedelta
from identity import *

from gen_utils import *
from allcharset import *


def get_image(jj, str1, chars, fonts):
    fontsize = 32
    a = 1.1+rand()*0.5
    b = 1.1+rand()*0.5
    h = int(32*a)
    w = int(280*b)

    while 1:
        b = randint(0, 255)
        f = randint(0, 255)
        if abs(b-f) > 50:
            break
    bgColor = (b, b, b)
    fillColor = (f, f, f)

    img_OpenCV2 = np.ones((h, w, 3), dtype=np.uint8)
    cv2.rectangle(img_OpenCV2, (0, 0), (w, h), bgColor, -1)
    img_OpenCV2 = add_line2(img_OpenCV2, 150, (b+f)//2)
    img_OpenCV2 = cv2.cvtColor(img_OpenCV2, cv2.COLOR_BGR2RGB)
    img_PIL = Image.fromarray(img_OpenCV2)
    draw = ImageDraw.Draw(img_PIL)
    pos_x = randint(-5, 5)
    pos_y0 = randint(0, h-fontsize-4)
    ss = []
    fontindex = randint(0, len(fonts))
    font = fonts[fontindex]
    jjj=jj
    for j in range(18):
        idx = 0
        t = rand()
        tt = 0.00
        if jjj==jj or ((fontsize*2)+pos_x > w):
            tt = 0.5

        if t>tt:
            if jj<len(str1) and (fontsize/2)+pos_x < w:
                pos_y = pos_y0 + randint(-2, 2)
                ch = str1[jj]
                jj = jj + 1
                if not ' '==ch:
                    idx1 = chars.find(ch)
                    size = font.getsize(ch)
                    if size[0]+pos_x < w:
                        idx = idx1 + 1
                        draw.text((pos_x, pos_y), ch, font=font, fill=fillColor)
                        pos_x = pos_x + size[0] + randint(-5, 5)
                else:
                    if (fontsize/2)+pos_x < w:
                        idx = 0
                        pos_x = pos_x + fontsize/2
        else:
            if (fontsize/2)+pos_x < w:
                idx = 0
                pos_x = pos_x + fontsize/2
                
        ss.append(idx)


    im = np.asarray(img_PIL)
    im = cv2.cvtColor(im, cv2.COLOR_RGB2GRAY)

    k = 3
    if rand() > 0.5:
        if rand() > 0.5:
            if b > f:
                im = add_erode(im, randint(1, k))
            else:
                im = add_dilate(im, randint(1, k))
        else:
            if b > f:
                im = add_erode(im, randint(1, k))
            else:
                im = add_dilate(im, randint(1, k))

    im = add_noise(im, 50)

    #if rand() > 0.5:        im = add_line(im, 10)

    if rand() > 0.5:
        im1 = add_shader(im)
        k = 0.4
        im = im1*k + im*(1-k)
    # im = HSVequalizeHist(im)
    if rand() > 0.5:
        im = motion_blur(im, randint(2, 5), randint(0, 360))

    if rand() > 0.5:
        im = gauss_blur(im, 5, randint(5, 10))

    if 1:
        #la = '_'.join(map(str, ss))
        #fn = '%s/%d/%d_%s.jpg' % (imgoutpath, j, i, la)
        im = cv2.resize(im, (280, 32))
        #la = ' '.join(map(str, ss))
        #s = '%s %s' % (fn, la)
        #infos.append(s)
        #print(h, w, fn)
        # cv2.imwrite(fn, im)
        #cv2.imencode('.jpg', im)[1].tofile(fn)
        return ss, im, jj




if 1:
    #random.shuffle(fontfiles)
    #fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    fontfiles = listdir('D:/fonts/ttf/')
    fontfiles = fullpath('D:/fonts/ttf_num/', ['ocrb10bt.ttf'])
    fontfiles = listdir('D:/fonts/ttf_han/')
    outpath = 'E:/OCR_Line/lines/num50w'
    chars = numxs
    other_chars = other+fuhao

fonts = []

fontsize = 32

for fontfile in fontfiles:
    font = ImageFont.truetype(fontfile, fontsize)
    fonts.append(font)


def save_image(imgoutpath, ss, im, i, base):
    j = i//base
    if i%base==0:
        print(j)
    fn = '%s/%d' % (imgoutpath, j)
    if not os.path.exists(fn):
        mkdir(fn)
    la = '_'.join(map(str, ss))
    fn = '%s/%d/%d_%s.jpg' % (imgoutpath, j, i, la)
    la = ' '.join(map(str, ss))
    s = '%s %s' % (fn, la)
    #infos.append(s)
    #print(h, w, fn)
    # cv2.imwrite(fn, im)
    cv2.imencode('.jpg', im)[1].tofile(fn)



if 0:
    for i in range(10):
        random_sex = random.randint(0, 1)  # 随机生成男(1)或女(0)
        idx = IdNumber.generate_id(random_sex)
        print(idx)  # 随机生成身份证号

if 0:
    for i in range(10):
        random_sex = random.randint(0, 1)  # 随机生成男(1)或女(0)
        idx = IdNumber.generate_id(random_sex)
        print(idx)  # 随机生成身份证号

if 0:
    f = open('E:\\data\\ew_id\\t1_0911.txt', 'rb')
    for i in range(10):
        line = f.readline().decode('utf-8').strip().split("\t")
        print(line)

    f.close()

if 0:
    f = open('E:\\data\\ew_id\\t1_0911.txt', 'rb')
    chars = '0123456789X'
    imgout = 'E:\\OCR_Line\\lines\\han100w'
    imgout = imgout + '\\img'
    for i in range(10):
        line = f.readline().decode('utf-8').strip().split("\t")
        #print(line[2], chars.index(line[2][0]), chars.index(line[2][1]))
        ss, im, jj = get_image(0, line[2], chars, fonts)
        save_image(imgout, ss, im, i, 100)

    f.close()


if 0:
    str1 = "this is string example....wow!!!";
    str2 = "exam";
    
    print(str1.find(str2))
    print(str1.index(str2))

if 1:
    f = open('E:\\data\\ew_id\\t1_0911.txt', 'rb')
    chars = nums+ens+hans
    imgout = 'E:\\OCR_Line\\lines\\han200wa'

    if 1:
        print(len(chars))
        labels = ['blank']

        for i in chars:
            labels.append(i)

        savelines(labels, imgout+'\\label.txt')

    imgout = imgout + '\\img'
    i = 0
    base = 10000
    n = 200*base
    while i<n:
        line = f.readline().decode('utf-8').strip().split("\t")
        #print(line[2], chars.index(line[2][0]), chars.index(line[2][1]))
        jj = 0
        #print(line)
        if len(line)>6:
            s1 = line[6]
            while i<n and jj<len(s1):
                ss, im, jj = get_image(jj, s1, chars, fonts)
                save_image(imgout, ss, im, i, base)
                i = i+1

    f.close()

if 0:
    f = open('E:\\data\\ew_id\\t1_0911.txt', 'rb')
    for i in range(10):
        line = f.readline().decode('utf-8').strip().split("\t")
        print(line[6])

    f.close()
