    #include <iostream>  
    #include <malloc.h>  
    #include <CL/cl.h>//包含CL的头文件  
      
    using namespace std;  
      
    //根据参数，判断设备类别。是CPU、GPU、ACCELERATOR或其他设备  
    const char* GetDeviceType(cl_device_type it)  
    {  
        if (it == CL_DEVICE_TYPE_CPU)  
            return "CPU";  
        else if (it == CL_DEVICE_TYPE_GPU)  
            return "GPU";  
        else if (it == CL_DEVICE_TYPE_ACCELERATOR)  
            return "ACCELERATOR";  
        else  
            return "DEFAULT";  
      
    }  
      
    int main()  
    {  
        char dname[512];  
        cl_device_id devices[20];  
        cl_platform_id* platform_id = NULL;  
        cl_uint num_devices;  
        cl_device_type int_type;  
        cl_ulong long_entries;  
        cl_uint num_platform;  
        cl_int err;  
      
        //查询系统上可用的计算平台，可以理解为初始化  
        err = clGetPlatformIDs(0, NULL, &num_platform);  
      
        if (err != CL_SUCCESS)  
        {  
            cout << "clGetPlatformIDs error" << endl;  
            return 0;  
        }  
      
        cout << "PlatForm num:" << num_platform << endl;  
      
        int st = 0;  
      
        platform_id = new cl_platform_id[num_platform];  
      
        err = clGetPlatformIDs(num_platform, platform_id, NULL);  
      
        if (err != CL_SUCCESS)  
        {  
            cout << "clGetPlatformIDs error" << endl;  
            return 0;  
        }  
      
        for (st = 0; st<num_platform; st++)  
        {  
            cout << "----------------------------------" << endl;  
            cout << "Platform " << st + 1 << endl;  
      
            //获取可用计算平台的名称  
            clGetPlatformInfo(platform_id[st], CL_PLATFORM_NAME, 512, dname, NULL);  
            cout << "CL_PLATFORM_NAME:" << dname << endl;  
      
            //获取可用计算平台的版本号,即OpenCL的版本号  
            clGetPlatformInfo(platform_id[st], CL_PLATFORM_VENDOR, 512, dname, NULL);  
            cout << "CL_PLATFORM_VERSION:" << dname << endl;  
      
            //获取可用计算平台的设备数目  
            clGetDeviceIDs(platform_id[st], CL_DEVICE_TYPE_ALL, 20, devices, &num_devices);  
            cout << "Device num:" << num_devices << endl;  
      
            unsigned int n = 0;  
      
            //循环两次，检测两个设备的属性  
            for (n = 0; n<num_devices; n++)  
            {  
                cout << endl << "Device " << n + 1 << endl;  
                //获取设备名称  
                clGetDeviceInfo(devices[n], CL_DEVICE_NAME, 512, dname, NULL);  
                cout << "Device :" << dname << endl;  
      
                //获取设备类别  
                clGetDeviceInfo(devices[n], CL_DEVICE_TYPE, sizeof(cl_device_type),  
                    &int_type, NULL);  
                cout << "Device Type:" << GetDeviceType(int_type) << endl;  
      
                //获取设备版本号  
                clGetDeviceInfo(devices[n], CL_DRIVER_VERSION, 512, dname, NULL);  
                cout << "Device version:" << dname << endl;  
      
                //获取设备全局内存大小  
                clGetDeviceInfo(devices[n], CL_DEVICE_GLOBAL_MEM_SIZE,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device global mem(MB):" <<  
                    long_entries / 1024 / 1024 << endl;  
      
                //获取设备CACHE内存大小  
                clGetDeviceInfo(devices[n], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device global mem cache(KB):" <<  
                    long_entries / 1024 << endl;  
      
                //获取本地内存大小  
                clGetDeviceInfo(devices[n], CL_DEVICE_LOCAL_MEM_SIZE,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device Locale mem(KB) :" << long_entries / 1024 << endl;  
      
                //获取设备频率  
                clGetDeviceInfo(devices[n], CL_DEVICE_MAX_CLOCK_FREQUENCY,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device Max clock(MHz) :" << long_entries << endl;  
      
                //获取最大工作组数  
                clGetDeviceInfo(devices[n], CL_DEVICE_MAX_WORK_GROUP_SIZE,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device Max Group size :" << long_entries << endl;  
      
                //获取最大计算核心数  
                clGetDeviceInfo(devices[n], CL_DEVICE_MAX_COMPUTE_UNITS,  
                    sizeof(cl_ulong), &long_entries, NULL);  
                cout << "Device Max parallel cores:" << long_entries << endl;  
      
            }  
        }  
      
        return 0;  
    }  