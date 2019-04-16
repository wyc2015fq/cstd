# How to decompile Google Android .apk file as readable dump【原创】 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2010年02月06日 12:13:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：3833








Background ：google android SDK platform provide emulator & ADB utility tools,it's very useful to


decompile the apk file to dump byte readable file.



Summary:


if you are ready for it, the sequences as following,




   - create/launch emulator from command line

   - launch the ADB utility tool from command line

   - install apk file from local directory from command line

   - list all system application apk file from command line

   - dexdump selected .dex file from emulator

   - pull dump file to local directory



Detail Section:



    step 1: create your own emulator if you still not create one and then launch the emulator

                launch the windows command line console, then chnage root directory to your android

                SDK Platform/tools directory, for me

C:/>cd /d D:/android-sdk-windows/tools

D:/android-sdk-windows/tools>android create -avd -n my_android1.5 -t 2

D:/android-sdk-windows/tools>emulator -avd  my_android1.5

    step 2: after emulator launched,
 go to windowns console to start ADB Shell utility tool

                make sure your
 emulator is running before you start ADB. if it's successful

D:/android-sdk-windows/tools>adb shell

                # cd /system/app

                cd /system/app

                # ls

                ls

                Mms.apk

                ApplicationsProvider.apk

                GlobalSearch.apk

                Calculator.apk

                SpareParts.apk

                GestureBuilder.apk

                Music.apk

                SdkSetup.apk

                PackageInstaller.apk

                Fallback.apk

                SettingsProvider.apk

                DrmProvider.apk

                Development.apk

                TelephonyProvider.apk

                PinyinIME.apk

                HTMLViewer.apk

                Settings.apk

                NetSpeed.apk

              
 if you want to see how many user applications have been installed on your emulator, just


              
 cd /data/app,and then ls



               
 step 3: install your apk file using adb shell commands, if you still not install your application


                on
 emulator

D:/android-sdk-windows/tools>adb
 install D:/android-sdk-windows/tools/a.apk

               
 after install successfully, you will see your apk file using above command line



              
 step 4: get the cached dex file from your emulator, we have to enter dalvik VM cache using command

# cd /data/dalvik-cache

#
 ls

..........

[data@app@a.apk@classes.dex](mailto:data@app@a.apk@classes.dex)



              
 step 5: use dexdump file to generate the dump file then use pull command to copy to local directory

#dexdump
 -d -f -h data@app@a.apk@classes.dex >> a.apk.dump



finally
 you will get a bit-readable dump file, based on dalvik class format knowledge, the more you dig


the
 more you
 get. normally *.apk file is a .zip file, could be open by any winzip tool.







Reference: SDK platform documentation





