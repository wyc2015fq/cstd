# Android使用文件管理器打开指定文件夹，浏览里面的内容 - weixin_33985507的博客 - CSDN博客
2018年06月20日 17:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：56
Android下可以打开一些文件，带有.doc 等后缀的文件网上一般都有解释，这个写一个使用文件管理器打开指定文件夹的
```
private void openAssignFolder(String path){
        File file = new File(path);
        if(null==file || !file.exists()){
            return;
        }
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.addCategory(Intent.CATEGORY_DEFAULT);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setDataAndType(Uri.fromFile(file), "file/*");
        try {
            startActivity(intent);
//            startActivity(Intent.createChooser(intent,"选择浏览工具"));
        } catch (ActivityNotFoundException e) {
            e.printStackTrace();
        }
    }
```
打开其他文件的方法，基本上和以上差不多，只是inten的一些属性不同而已，现在列表在下：
||Intent intent = new Intent(Intent.ACTION_VIEW);Uri uri = Uri.fromFile(file);intent.addCategory(Intent.CATEGORY_DEFAULT);|
|----|----|
|打开图片文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "image/*");|
|打开PDF文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/pdf");|
|打开文本文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "text/plain");|
|打开音频文件|intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);intent.putExtra("oneshot", 0);intent.putExtra("configchange", 0);intent.setDataAndType(uri, "audio/*");|
|打开视频文件|intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);intent.putExtra("oneshot", 0);intent.putExtra("configchange", 0);intent.setDataAndType(uri, "video/*");|
|打开CHM文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/x-chm");|
|打开apk文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/vnd.android.package-archive");|
|打开PPT文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/vnd.ms-powerpoint");|
|打开Excel文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/vnd.ms-excel");|
|打开Word文件|intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);intent.setDataAndType(uri, "application/msword");|
