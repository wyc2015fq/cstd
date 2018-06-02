
static void help()
{
    printf("\nThis program demonstrated the use of the discrete Fourier transform (dft)\n"
           "The dft of an image is taken and it's power spectrum is displayed.\n"
           "Usage:\n"
            "./dft [image_name -- default ../data/lena.jpg]\n");
}

#include "img/imgio.inl"



int main(int argc, const char ** argv)
{
    help();
    int hashelp = 0;
    char filename[256];
    sys_chdir("C:/code/lib/opencv-3.2.0/samples/data");
    cvCommandLineParser(argc, argv, "{help h|h|}{@image|s|../data/lena.jpg|input image file}", &hashelp, filename);

    if (hashelp)
    {
        help();
        return 0;
    }
    int i = 0;
    img_t im[10] = {0};
    img_t* img = im+i++;
    img_t* padded = im+i++;
    imread(filename, 1, 1, img);
    if( imempty(img) )
    {
        help();
        printf("Cannot read image file: %s\n", filename);
        return -1;
    }
    int M = cGetOptimalDFTSize( img->rows );
    int N = cGetOptimalDFTSize( img->cols );
    
    cvCopyMakeBorder(img, padded, 0, M - img->rows, 0, N - img->cols, CC_BORDER_CONSTANT, cScalarAll(0));

#if 0
    img_t planes[] = {Mat_<float>(padded), img_t::zeros(padded->size(), CC_32F)};
    img_t complexImg;
    merge(planes, 2, complexImg);

    cvDFT(complexImg, complexImg);

    imshow(padded);WaitKey(-1);
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);
    img_t mag = planes[0];
    mag += Scalar::all(1);
    log(mag, mag);

    // crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag->cols & -2, mag->rows & -2));

    int cx = mag->cols/2;
    int cy = mag->rows/2;

    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center
    img_t tmp;
    img_t q0(mag, Rect(0, 0, cx, cy));
    img_t q1(mag, Rect(cx, 0, cx, cy));
    img_t q2(mag, Rect(0, cy, cx, cy));
    img_t q3(mag, Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(mag, mag, 0, 1, NORM_MINMAX);

    imshow("spectrum magnitude", mag);
    waitKey();
    
#endif
    return 0;
}
