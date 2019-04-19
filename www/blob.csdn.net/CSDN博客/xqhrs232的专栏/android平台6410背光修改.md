# android平台6410背光修改 - xqhrs232的专栏 - CSDN博客
2011年09月19日 17:41:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：941标签：[android																[平台																[struct																[module																[methods																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=methods&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/zhandoushi1982/article/details/5068626](http://blog.csdn.net/zhandoushi1982/article/details/5068626)
相关文章网页::[Android 背光驱动](http://www.eoeandroid.com/thread-68952-1-1.html)[http://www.eoeandroid.com/thread-68952-1-1.html](http://www.eoeandroid.com/thread-68952-1-1.html)
昨日装机的MID亮度不够，特别是看些视频文件时很不清晰。实际测得输出PWM的GPF15电压不够，仅0.4伏，要求上应该要达到跟WINCE一样的2.8V，才适合供给U7位的MP1528。
      在内核CODE中查找PWM相关文件，找到s3c6410_timer_setup函数是设置PWM通道和分频比等相关函数的调用。恰好该函数在s3cfb_set_brightness中被调用，设置tcnt的值由1000到500，即可达效果。
      补充一点：关于如何让系统不会进入待机睡眠状态的一种机制：终端中用以下命令echo "1" > /sys/power/wake_lock。
=============================================================================
      顺便分析下android平台中是如何从上到下修改到背光的。
（1）寻找到背光调节的CODE：在packages/apps/Settings/src/com/android/settings的BrightnessPreference.java中：
        private void setBrightness(int brightness) {
                 ........
                hardware.setBacklights(brightness);
         }
（2）寻找到硬件服务的CODE：frameworks/base/services/java/com/android/server/HardwareService.java中：
         public void setBacklights(int brightness) {
                 ........
            setLightBrightness_UNCHECKED(LIGHT_ID_BACKLIGHT, brightness);  //注意其中的ID号
        }
其中函数定义：
        void setLightBrightness_UNCHECKED(int light, int brightness) 
       {
              int b = brightness & 0x000000ff;
              b = 0xff000000 | (b << 16) | (b << 8) | b;
              setLight_native(mNativePointer, light, b, LIGHT_FLASH_NONE, 0, 0);
        }
调用的是native的设备light的函数，并传入参数LIGHT_ID_BACKLIGHT，和亮度。
（3）本地设定的native用于设定亮度的函数在：/hardware/s3c6410/liblights/lights.c中：
      static struct hw_module_methods_t lights_module_methods = {
               .open =  open_lights,
      };
其中：
      static int open_lights(const struct hw_module_t* module, char const* name,
        struct hw_device_t** device)
      {
              ........
                  if (0 == strcmp(LIGHT_ID_BACKLIGHT, name)) {  //注意其ID号与上面是一致的
                           set_light = set_light_backlight;
                  }
       }
实现函数是：
     set_light_backlight(struct light_device_t* dev, struct light_state_t const* state)
     {
          int err = 0;
          int brightness = rgb_to_brightness(state);
          pthread_mutex_lock(&g_lock);
          g_backlight = brightness;
          err = write_int("/sys/class/leds/lcd-backlight/brightness", brightness);
          if (g_haveTrackballLight) {
                handle_trackball_light_locked(dev);
          }
          pthread_mutex_unlock(&g_lock);
          return err;
      }
其中write_int的参数sys/class/leds/lcd-backlight/brightness是linux系统默认的设备驱动模型的参数。可以通过在MID终端里面查看到。比如：cd /sys/class/leds/lcd-backlight，再用ls -l 可以看到好几个属性。这些属性都由class_create来创建的。对于背光，我们可以在sys/class和/sys/devices/都找到对应的背光路径，这两个路径下还可以找得到关联的符号链接；但我们不会在/dev下找到背光的节点，因为一般不会那样做。
（4）在linux层执行改变亮度的函数是：
     static struct led_classdev smdk_backlight_led  = {
          .name  = "lcd-backlight",
          .brightness = SMDK_DEFAULT_BACKLIGHT_BRIGHTNESS,
          .brightness_set = smdk_brightness_set,
       };
其中：
      static void smdk_brightness_set(struct led_classdev *led_cdev, enum led_brightness value)
     {
           mutex_lock(&smdk_backlight_lock);
           smdk_backlight_brightness = value;
           if(!smdk_backlight_off)
                 smdk_set_backlight_level(smdk_backlight_brightness);
           mutex_unlock(&smdk_backlight_lock);
      }
其中：
      static void smdk_set_backlight_level(u8 level)
     {
            if (smdk_backlight_last_level == level)
                  return;
            s3cfb_set_brightness((int)(level/3)); 
             smdk_backlight_last_level = level;
      }
其中：
      void s3cfb_set_brightness(int val)
     {
          int channel = 1; /* must use channel-1 */
          int usec = 0;  /* don't care value */
          unsigned long tcnt = 500;  //1000; //jeff.
          unsigned long tcmp = 0;
          if (val < 0)
              val = 0;
          if (val > S3C_FB_MAX_BRIGHTNESS)
              val = S3C_FB_MAX_BRIGHTNESS;
          s3c_fimd.brightness = val;
          tcmp = val * 5;
          s3c6410_timer_setup (channel, usec, tcnt, tcmp);
      }
（5）疑问在于：A，上层调节的brightness，linux下层怎么样转到哪个函数上去的？就算都以lcd-backlight来匹配，问题是brightness怎么匹配到的？B，smdk_brightness_set的参数是enum类型的，而亮度是纯粹的INT类型，是怎么样传过来的？
===============================================================================
       补充一个关于背光的BUG解决记录。在MTK6516平台调节背光亮度时，耳机听筒可以听到电流声。初步估计是背光调节的PWM频率造成的干扰，测得目前的PWM频率是1.57KHZ，也小于背光驱动芯片的最大值2K。在代码数组blSettingsCustom中改动PWM_CLOCK_DIV_BY_2成PWM_CLOCK_DIV_BY_8，即使得PWM频率变为1.57KHZ*2/8=393HZ，烧录完毕测试电流声没有了。实测频率与计算的一致，可见改变PWM频率并不会影响驱动电压，只取决于占空比。
