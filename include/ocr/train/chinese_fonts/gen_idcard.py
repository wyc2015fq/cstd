
from datetime import date
from datetime import timedelta
from identity import *

from gen_utils import *
from allcharset import *

fontsize = 32

def drawtext(draw, pos_x0, pos_y0, maxw, lineh, str1, font, fill):
    pos_x = pos_x0
    w = pos_x0 + maxw
    for jj in range(len(str1)):
        ch = str1[jj]
        if not ' '==ch:
            size = font.getsize(ch)
            if size[0]+pos_x > w:
                pos_x = pos_x0
                pos_y0 += lineh
                
            pos_y = pos_y0 + randint(-2, 2)
            draw.text((pos_x, pos_y), ch, font=font, fill=fill)
            pos_x = pos_x + size[0] + randint(-5, 5)
        else:
            if (fontsize/2)+pos_x > w:
                pos_x = pos_x0
                pos_y0 += lineh

            pos_x = pos_x + fontsize/2
    
    return draw

def sum(arr):
    s = 0
    for i in arr:
        s += i
    return s

def rand_str(chars, n):
    name = ''
    for i in range(n):
        name += chars[randint(0, len(chars))]
    return name

def rand_name(chars, arr):
    s = sum(arr)
    t = randint(0, s)
    ss = 0
    n = 0
    for i in arr:
        ss += i
        if ss>t:
            break
        n += 1

    name = ''
    for i in range(n):
        name += chars[randint(0, len(chars))]
        if n==2 and i==0:
            name += "  "
    return name



def get_image(str1, chars, fonts):
    fontsize = 32
    a = 1.1+rand()*0.
    b = 1.1+rand()*0.
    h = int(fontsize*16)
    w = int(fontsize*24)

    while 1:
        b = randint(0, 255)
        f = randint(0, 255)
        if abs(b-f) > 50:
            break
    m = (b+f)//2
    bgColor = (b, b, b)
    fillColor = (f, f, f)
    midColor = (m,m,m)

    img_OpenCV2 = np.ones((h, w, 3), dtype=np.uint8)
    cv2.rectangle(img_OpenCV2, (0, 0), (w, h), bgColor, -1)
    img_OpenCV2 = add_line2(img_OpenCV2, 150, (b+f)//2)
    img_OpenCV2 = cv2.cvtColor(img_OpenCV2, cv2.COLOR_BGR2RGB)
    img_PIL = Image.fromarray(img_OpenCV2)
    draw = ImageDraw.Draw(img_PIL)
    ss = []
    fontindex = randint(0, len(fonts))
    font = fonts[fontindex]
    pos_x0 = randint(-3*fontsize, 3*fontsize)
    pos_y0 = randint(-2*fontsize, 2*fontsize)
    maxw = fontsize*10
    lineh = fontsize*1.5
    name = rand_name(chars, [0, 0, 50, 30, 20])
    drawtext(draw, pos_x0+2*fontsize, pos_y0+2*fontsize, maxw, lineh, "姓 名", font, midColor)
    drawtext(draw, pos_x0+5*fontsize, pos_y0+2*fontsize, maxw, lineh, name, font, fillColor)
    gennes = "男女"
    drawtext(draw, pos_x0+2*fontsize, pos_y0+4*fontsize, maxw, lineh, "性 别", font, midColor)
    genne = gennes[randint(0, 2)]
    drawtext(draw, pos_x0+5*fontsize, pos_y0+4*fontsize, maxw, lineh, genne, font, fillColor)
    drawtext(draw, pos_x0+8*fontsize, pos_y0+4*fontsize, maxw, lineh, "民 族", font, midColor)
    minzu = rand_name(chars, [0, 50, 30, 20, 10, 10, 10])
    drawtext(draw, pos_x0+11*fontsize, pos_y0+4*fontsize, maxw, lineh, minzu, font, fillColor)

    drawtext(draw, pos_x0+2*fontsize, pos_y0+6*fontsize, maxw, lineh, "出 生", font, midColor)
    drawtext(draw, pos_x0+5*fontsize, pos_y0+6*fontsize, maxw, lineh, "%4s"%(randint(1900, 2019)) , font, fillColor)
    drawtext(draw, pos_x0+8*fontsize, pos_y0+6*fontsize, maxw, lineh, "年", font, midColor)
    drawtext(draw, pos_x0+9.5*fontsize, pos_y0+6*fontsize, maxw, lineh, "%s"%(randint(1, 13)) , font, fillColor)
    drawtext(draw, pos_x0+11*fontsize, pos_y0+6*fontsize, maxw, lineh, "月", font, midColor)
    drawtext(draw, pos_x0+11.5*fontsize, pos_y0+6*fontsize, maxw, lineh, "%4s"%(randint(1, 30)) , font, fillColor)
    drawtext(draw, pos_x0+14*fontsize, pos_y0+6*fontsize, maxw, lineh, "日", font, midColor)

    drawtext(draw, pos_x0+2*fontsize, pos_y0+8*fontsize, maxw, lineh, "住 址", font, midColor)
    drawtext(draw, pos_x0+5*fontsize, pos_y0+8*fontsize, maxw, lineh, str1, font, fillColor)

    drawtext(draw, pos_x0+2*fontsize, pos_y0+13*fontsize, maxw, lineh, "公民身份证号码", font, midColor)
    idnum = rand_str(nums, 18)
    drawtext(draw, pos_x0+10.5*fontsize, pos_y0+13*fontsize, 2*maxw, lineh, idnum, font, fillColor)


    im = np.asarray(img_PIL)
    im = cv2.cvtColor(im, cv2.COLOR_RGB2GRAY)

    if 1:
        #la = '_'.join(map(str, ss))
        #fn = '%s/%d/%d_%s.jpg' % (imgoutpath, j, i, la)
        im = cv2.resize(im, (300, 200))
        #la = ' '.join(map(str, ss))
        #s = '%s %s' % (fn, la)
        #infos.append(s)
        #print(h, w, fn)
        # cv2.imwrite(fn, im)
        #cv2.imencode('.jpg', im)[1].tofile(fn)
        return ss, im




if 1:
    #random.shuffle(fontfiles)
    #fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    #fontfiles = listdir('D:/fonts/ttf/')
    #fontfiles = fullpath('D:/fonts/ttf_num/', ['ocrb10bt.ttf'])
    fontfiles = listdir('E:/bak/fonts/ttf_han/')
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
    imgout = 'E:\\OCR_Line\\lines\\han50w'
    mkdir(imgout)
    imgout = imgout + '\\img'
    for i in range(10):
        line = f.readline().decode('utf-8').strip().split("\t")
        #print(line[2], chars.index(line[2][0]), chars.index(line[2][1]))
        ss, im, jj = get_image(0, line[2], hans, fonts)
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
    imgout = 'E:\\OCR_Line\\idcard\\han50w'
    mkdir(imgout)

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
    n = 10
    while i<n:
        line = f.readline().decode('utf-8').strip().split("\t")
        #print(line[2], chars.index(line[2][0]), chars.index(line[2][1]))
        jj = 0
        #print(line)
        if len(line)>6:
            s1 = line[6]
            ss, im = get_image(s1, chars, fonts)
            save_image(imgout, ss, im, i, base)
            i = i+1

    f.close()

if 0:
    f = open('E:\\data\\ew_id\\t1_0911.txt', 'rb')
    for i in range(10):
        line = f.readline().decode('utf-8').strip().split("\t")
        print(line[6])

    f.close()
