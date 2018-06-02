// 演示程序功能要求：
//
// 1. 流程
// 1.1 程序启动自动进入识别界面，现实视频，现实识别相关信息
// 1.2 长按屏幕 -> 没有特征，第一个采集的是管理员 , 采集完进入高级设置界面
// -> 有管理员特征 ，进入管理员认证 ，认证成功进入高级设置界面
//
// 1.3 高级设置界面包含
// <用户管理> ， 显示用户注册照片 ，可以删除用户
// <记录管理> ， 显示用户识别的照片和时间
// <采集人脸> ， 自动生成编号，注册人脸
void imput_strf(img_t* im, int x, int y, COLOR color, const char* fmt, ...)
{
  char buf[256];
  va_list ARGLIST;
  va_start(ARGLIST, fmt);
  _vsnprintf(buf, countof(buf), fmt, ARGLIST);
  imput_str(im, x, y, buf, -1, color);
  va_end(ARGLIST);
}

