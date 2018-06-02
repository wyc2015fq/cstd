// cloning_demo.cpp
// 
// Author:
// Siddharth Kherada <siddharthkherada27[at]gmail[dot]com>
// 
// This tutorial demonstrates how to use OpenCV seamless cloning
// module without GUI.
// 
// 1- Normal Cloning
// 2- Mixed Cloning
// 3- Monochrome Transfer
// 4- Color Change
// 5- Illumination change
// 6- Texture Flattening
// 
// The program takes as input a source and a destination image (for 1-3 methods)
// and ouputs the cloned image.
// 
// Download test images from opencv_extra folder @github.


//#include "imgproc/imgproc.inl"
//#include "imgcodecs/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/core.hpp"

//#include "photo/photo.inl"
#include "cv/photo/seamless_cloning.inl"
#include "cstd.h"
#include "img/imgio.inl"

int cloning_demo()
{
     const char* help = "Cloning Module\n"
     "---------------\n"
     "Options: \n"
     "\n"
     "1) Normal Cloning \n"
     "2) Mixed Cloning \n"
     "3) Monochrome Transfer \n"
     "4) Local Color Change \n"
     "5) Local Illumination Change \n"
     "6) Texture Flattening \n"
     "\n"
     "Press number 1-6 to choose from above techniques: \n";
    int num = 1;
    img_t im[10] = {0};
    int i=0;
    printf(help);
    sys_chdir("E:/code/c/AI/OpenCV/opencv_extra-master/testdata/cv");
    sys_chdir("D:/pub/data/Adience/faces/8007224@N07/out");
    if(num == 1)
    {
        const char* folder =  "cloning/Normal_Cloning/";
        const char* original_path1 = "source1.bmp";
        const char* original_path2 = "destination1.bmp";
        const char* original_path3 = "mask.bmp";
        sys_chdir(folder);
        
        img_t* source = im+i++;
        img_t* destination = im+i++;
        img_t* mask = im+i++;
        img_t* result = im+i++;

        if(!imread(original_path1, 3, 1, source))
        {
            printf("Could not load source image %s\n", original_path1 );
            exit (0);
        }
        if(!imread(original_path2, 3, 1, destination))
        {
            printf("Could not load destination image %s\n", original_path2 );
            exit(0);
        }
        if(!imread(original_path3, 3, 1, mask))
        {
            printf("Could not load mask image %s\n", original_path3 );
            exit(0);
        }
        //imwrite("source1.bmp", source);
        //imwrite("destination1.bmp", destination);
        //imwrite("mask.bmp", mask);
        IPOINT p;
        p.x = 400;
        p.y = 100;
p.y+=125;
p.x-=15;
p.x=0, p.y=0;
        cvSeamlessClone(source, destination, mask, p, result, 1);
        
        imshow2("source",source);
        imshow2("Output",result);
        imwrite("cloned1.jpg", result);
    }
#if 0
    else if(num == 2)
    {
        const char* folder = "cloning/Mixed_Cloning/";
        const char* original_path1 = folder + "source1.png";
        const char* original_path2 = folder + "destination1.png";
        const char* original_path3 = folder + "mask.png";

        img_t source = imread(original_path1, IMREAD_COLOR);
        img_t destination = imread(original_path2, IMREAD_COLOR);
        img_t mask = imread(original_path3, IMREAD_COLOR);

        if(source.empty())
        {
            cout << "Could not load source image " << original_path1 << endl;
            exit(0);
        }
        if(destination.empty())
        {
            cout << "Could not load destination image " << original_path2 << endl;
            exit(0);
        }
        if(mask.empty())
        {
            cout << "Could not load mask image " << original_path3 << endl;
            exit(0);
        }

        img_t result;
        Point p;
        p.x = destination.size().width/2;
        p.y = destination.size().height/2;

        seamlessClone(source, destination, mask, p, result, 2);

        imshow("Output",result);
        imwrite(folder + "cloned.png", result);
    }
    else if(num == 3)
    {
        const char* folder = "cloning/Monochrome_Transfer/";
        const char* original_path1 = folder + "source1.png";
        const char* original_path2 = folder + "destination1.png";
        const char* original_path3 = folder + "mask.png";

        img_t source = imread(original_path1, IMREAD_COLOR);
        img_t destination = imread(original_path2, IMREAD_COLOR);
        img_t mask = imread(original_path3, IMREAD_COLOR);

        if(source.empty())
        {
            cout << "Could not load source image " << original_path1 << endl;
            exit(0);
        }
        if(destination.empty())
        {
            cout << "Could not load destination image " << original_path2 << endl;
            exit(0);
        }
        if(mask.empty())
        {
            cout << "Could not load mask image " << original_path3 << endl;
            exit(0);
        }

        img_t result;
        Point p;
        p.x = destination.size().width/2;
        p.y = destination.size().height/2;

        seamlessClone(source, destination, mask, p, result, 3);

        imshow("Output",result);
        imwrite(folder + "cloned.png", result);
    }
    else if(num == 4)
    {
        const char* folder = "cloning/Color_Change/";
        const char* original_path1 = folder + "source1.png";
        const char* original_path2 = folder + "mask.png";

        img_t source = imread(original_path1, IMREAD_COLOR);
        img_t mask = imread(original_path2, IMREAD_COLOR);

        if(source.empty())
        {
            cout << "Could not load source image " << original_path1 << endl;
            exit(0);
        }
        if(mask.empty())
        {
            cout << "Could not load mask image " << original_path2 << endl;
            exit(0);
        }

        img_t result;

        colorChange(source, mask, result, 1.5, .5, .5);

        imshow("Output",result);
        imwrite(folder + "cloned.png", result);
    }
    else if(num == 5)
    {
        const char* folder = "cloning/Illumination_Change/";
        const char* original_path1 = folder + "source1.png";
        const char* original_path2 = folder + "mask.png";

        img_t source = imread(original_path1, IMREAD_COLOR);
        img_t mask = imread(original_path2, IMREAD_COLOR);

        if(source.empty())
        {
            cout << "Could not load source image " << original_path1 << endl;
            exit(0);
        }
        if(mask.empty())
        {
            cout << "Could not load mask image " << original_path2 << endl;
            exit(0);
        }

        img_t result;

        illuminationChange(source, mask, result, 0.2f, 0.4f);

        imshow("Output",result);
        imwrite(folder + "cloned.png", result);
    }
    else if(num == 6)
    {
        const char* folder = "cloning/Texture_Flattening/";
        const char* original_path1 = folder + "source1.png";
        const char* original_path2 = folder + "mask.png";

        img_t source = imread(original_path1, IMREAD_COLOR);
        img_t mask = imread(original_path2, IMREAD_COLOR);

        if(source.empty())
        {
            cout << "Could not load source image " << original_path1 << endl;
            exit(0);
        }
        if(mask.empty())
        {
            cout << "Could not load mask image " << original_path2 << endl;
            exit(0);
        }

        img_t result;

        textureFlattening(source, mask, result, 30, 45, 3);

        imshow("Output",result);
        imwrite(folder + "cloned.png", result);
    }
#endif
    WaitKey(0);
    imfrees(im, 10);
    return 0;
}
