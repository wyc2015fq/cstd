# YOLO修改代码，使得运行demo时能够裁剪识别到的指定类别目标框中的图像 - 站在巨人的肩膀上coding - CSDN博客





2018年07月12日 15:55:18[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1130








1、修改example/image.c中draw_detections函数，或者增加draw_detections_bbox_crop函数

```
// maybe 搞清楚，这个函数在哪里执行，输出框在哪里执行
void draw_detections_bbox_crop(image im, int num, float thresh, box *boxes, float **probs, float **masks, char **names, image **alphabet, int classes, box *bbox_crop)
{
  // classes是类别数，probs是 num*classes的数组
    int i,j,k = 0;
    for(i = 0; i < num; ++i)
    {
        char labelstr[4096] = {0};
        int class = -1;
        for(j = 0; j < classes; ++j)
	{
            if (probs[i][j] > thresh) // 概率大于阈值的时候打印类名和概率
	    {
                if (class < 0) 
		{
                    strcat(labelstr, names[j]);
                    class = j;
                } 
                else 
		{
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], probs[i][j]*100);
		
		// 比较识别的结果，如果是人的话，计数加一
                if(0 == strcmp( names[j], "person" ))
		{
                    k++;
                }
            }
        }
        
        if(class >= 0) 
	{
            int width = im.h * .006;


            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            int offset = class*123457 % classes;
            float red = get_color(2,offset,classes);
            float green = get_color(1,offset,classes);
            float blue = get_color(0,offset,classes);
            float rgb[3];


            //width = prob*20+2;


            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = boxes[i];


            int left  = (b.x-b.w/2.)*im.w;
            int right = (b.x+b.w/2.)*im.w;
            int top   = (b.y-b.h/2.)*im.h;
            int bot   = (b.y+b.h/2.)*im.h;


            if(left < 0) left = 0;
            if(right > im.w-1) right = im.w-1;
            if(top < 0) top = 0;
            if(bot > im.h-1) bot = im.h-1;


	    // 打印输出测试
	    /*
	    printf("im.w is %d\n",im.w);
	    printf("im.h is %d\n",im.h);
	    printf("b.x - b.w/2 is %d\n",b.x - b.w/2.);
	    printf("b.y - b.h/2 is %d\n",b.y - b.h/2.);
	    printf("bbox coordinate is: %d %d %d %d\n",left,right,top,bot);
	    根据左上角和右下角坐标画框(left,top) && (right,bot)
	    */
	    //过滤掉其他物体bbox. maybe
	    bool is_person;
	    bool is_car;
	    is_person = !strcmp(labelstr,"person");
	    is_car = !strcmp(labelstr,"car");
	    if(is_person || is_car)
	    {
	      // TODO 在这里将图像裁剪 maybe
	      // image to iplimage，一维图像转换成为二维图像
	      IplImage* src ;
	      src = image_to_iplImage(im,src);
	      cvShowImage("src",src );
	      int x = left;
	      int y = top;
	      int w = right -left;
	      int h = bot -top;      
	      cvSetImageROI(src,cvRect(left,top,right-left,bot-top)); //这里如果宽或者高越界会导致错误，闪退/ 	     
	      IplImage* crop = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,src->nChannels);
 	      cvCopy(src,crop,0);
	      cvResetImageROI(src);
	      
	      draw_box_width(im, left, top, right, bot, width, red, green, blue); //width为根据目标大小确定框的粗细，三原色合并成框的颜色  
	      save_image(im,"test"); //这里的额图像已经是有框的
	      
	      src = image_to_iplImage(im,src);
	      cvShowImage("src_bbox",src);
	      cvShowImage("crop",crop);
	      cvSaveImage("crop.jpg",crop,0);
	      if (alphabet)
	      {
		  image label = get_label(alphabet, labelstr, (im.h*.03)/10);
		  draw_label(im, top + width, left, label, rgb);
		  free_image(label);
	      }
	      if (masks)
	      {
		  image mask = float_to_image(14, 14, 1, masks[i]);
		  image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
		  image tmask = threshold_image(resized_mask, .5);
		  embed_image(tmask, im, left, top);
		  free_image(mask);
		  free_image(resized_mask);
		  free_image(tmask);
	      }
	    }
        }
    }
   // printf("personNum = %d\n",k); // printf person number
}
```


注意在include/darknet.h中增加draw_detections_bbox_crop的声明

```
void draw_detections(image im, int num, float thresh, box *boxes, float **probs, float **masks, char **names, image **alphabet, int classes);
void draw_detections_bbox_crop(image im, int num, float thresh, box *boxes, float **probs, float **masks, char **names, image **alphabet, int classes,box* bbox_crop);
```


2、在image.c中增加一个一维图像数组转二维图像的函数 void image_to_iplimage(image im,IplImage* img)


```
IplImage*  image_to_iplImage(image p,IplImage* disp)
{
  // 将image结构体中存放的一维数组转换成二维图像。 maybe
  image copy = copy_image(p);
  if(p.c == 3) rgbgr_image(copy);
  int x,y,k;
  disp = cvCreateImage(cvSize(p.w,p.h), IPL_DEPTH_8U, p.c);
  int step = disp->widthStep;
  for(y = 0; y < p.h; ++y){
        for(x = 0; x < p.w; ++x){
            for(k= 0; k < p.c; ++k){
                disp->imageData[y*step + x*p.c + k] = (unsigned char)(get_pixel(copy,x,y,k)*255);
            }
        }
    }
    return disp;
}
```


3、注释 src/demo.c中draw_detections函数，改为draw_detections_bbox_crop函数

```
// draw_detections(display, demo_detections, demo_thresh, boxes, probs, 0, demo_names, demo_alphabet, demo_classes);
    draw_detections_bbox_crop(display, demo_detections, demo_thresh, boxes, probs, 0, demo_names, demo_alphabet, demo_classes,demo_bbox_crop);
    image_to_iplimage(display , demo_img);
```

```
static box* demo_bbox_crop;
static IplImage* demo_img;
```

4、draw_detections_bbox_crop按照draw_detections函数来处理，~~只是增加一个返回值类型box* bbox_crop~~;完整的函数如图1所示。

5、示例

IplImage* demo_img


box *demo_bbox_crop;

draw_detections_bbox_crop()









