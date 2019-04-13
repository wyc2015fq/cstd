
# ffmpeg从视频提取图片序列 - 机器学习的小学生 - CSDN博客


2018年07月20日 20:20:30[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：465


[ffmpeg for windows](https://pan.baidu.com/s/1e7ijrcu1wRWUANjNj4S00g)下载地址：
```python
https:
```
```python
//pan
```
```python
.baidu
```
```python
.com
```
```python
/s/
```
```python
1e7
```
```python
ijrcu1wRWUANjNj4S00g
```
下载完后，将ffmpeg可执行文件所在的目录添加到环境变量中。
下面给出的是利用matlab调用ffmpeg进行批量处理的代码：
```python
exe_cmd =
```
```python
'ffmpeg'
```
```python
;
root_path =
```
```python
'E:/eclipse_workplace/test1/src/test/lstm_activity_recognition/UCF-101'
```
```python
;
speed =
```
```python
'30'
```
```python
;
activity_class_list = dir(root_path);
activity_class_list = activity_class_list(
```
```python
3
```
```python
:
```
```python
end
```
```python
);
```
```python
% remove '.' and '..'
```
```python
nclass =
```
```python
length
```
```python
(activity_class_list);
class_names = cell(nclass,
```
```python
1
```
```python
);
```
```python
for
```
```python
i
```
```python
=
```
```python
1
```
```python
:
```
```python
length
```
```python
(activity_class_list)
    class_names
```
```python
{i}
```
```python
=  activity_class_list(
```
```python
i
```
```python
).name;
    each_class_set = dir(
```
```python
[root_path
```
```python
'/'
```
```python
class_names{i}
```
```python
'/*.avi'
```
```python
]
```
```python
);
```
```python
for
```
```python
j
```
```python
=
```
```python
1
```
```python
:
```
```python
length
```
```python
(each_class_set)
        avi_name = each_class_set(
```
```python
j
```
```python
).name;
        s = strfind(avi_name,
```
```python
'.avi'
```
```python
);
        avi_name_pre = avi_name(
```
```python
1
```
```python
:s-
```
```python
1
```
```python
);
        img_path =
```
```python
[root_path
```
```python
'/'
```
```python
class_names{i}
```
```python
'/'
```
```python
avi_name_pre]
```
```python
;
```
```python
if
```
```python
~exist(img_path,
```
```python
'dir'
```
```python
)
            mkdir(img_path);
```
```python
end
```
```python
avi_input =
```
```python
[root_path
```
```python
'/'
```
```python
class_names{i}
```
```python
'/'
```
```python
each_class_set(j).name]
```
```python
;
        img_output =
```
```python
[img_path
```
```python
'/'
```
```python
avi_name_pre
```
```python
'.%4d.jpg'
```
```python
]
```
```python
;
        cmd_line =
```
```python
[exe_cmd
```
```python
' -i '
```
```python
avi_input
```
```python
' -r '
```
```python
speed
```
```python
' '
```
```python
img_output]
```
```python
;
        system(cmd_line);
```
```python
disp
```
```python
(
```
```python
[num2str(i)
```
```python
':'
```
```python
num2str(j)]
```
```python
);
```
```python
end
```
```python
end
```
```python
% imgpath = 'v_ApplyEyeMakeup_g01_c01/';
```
```python
% imglist = dir([imgpath '/*.jpg']);
```
```python
% disp([num2str(length(imglist)) '张图像！']);
```
```python
% for i = 1:length(imglist)
```
```python
%     imgname = [imgpath imglist(i).name];
```
```python
%     img = imread(imgname);
```
```python
%     imshow(img);
```
```python
%     pause(0.01);
```
```python
%
```
```python
% end
```

