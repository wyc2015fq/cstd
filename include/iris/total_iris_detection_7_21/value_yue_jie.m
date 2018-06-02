function [x1, x2, y1, y2] = value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize)
if x1 < 1
    x1 = 1;
end
if x2 < 2
    x2 = 2;
end
if y1 < 1
    y1 = 1;
end
if y2 < 2
    y2 = 2;
end
if x1 > pic_xsize
    x1 = pic_xsize - 1 ;
end
if x2 > pic_xsize
    x2 = pic_xsize;
end
if y1 > pic_ysize
    y1 = pic_ysize - 1;
end
if y2 > pic_ysize
    y2 = pic_ysize;
end