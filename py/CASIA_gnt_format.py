# CASIA手写体图片库、gnt格式读取
#!/usr/bin/python
 
import struct
import Image
import os
count = 0
path = 'C:/Users/Administrator/Desktop/HandWriting/test_data/'


for z in xrange(1241,1301):
    ff = './HWDB1.1_test/' + str(z) + '-c.gnt'
    f = open(ff,'rb')
    #ifend = f.read(1)
    while f.read(1) != "":
        f.seek(-1,1)
        global count
        count += 1
        length_bytes = struct.unpack('<I', f.read(4))[0]
        print length_bytes
        tag_code = f.read(2)
        print tag_code
        width = struct.unpack('<H', f.read(2))[0]
        print width
        height = struct.unpack('<H', f.read(2))[0]
        print height 
 
        im = Image.new('RGB',(width,height))
        img_array = im.load()
        #print img_array[0,7]
        for x in xrange(0,height):
            for y in xrange(0,width):
                pixel = struct.unpack('<B',f.read(1))[0]
                img_array[y,x]=(pixel,pixel,pixel)
        
        #print str(count)
        filename = str(count) + '.png'
        #filename = '/'+ tag_code + '/' +filename
        print filename
        if(os.path.exists(path + tag_code)):
            filename = path + tag_code + '/' + filename
            im.save(filename)
            #f.close()
        else:
            os.makedirs(path + tag_code)
            filename = path + tag_code + '/' + filename
            im.save(filename)
    f.close()