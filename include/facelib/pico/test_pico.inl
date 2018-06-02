
#include "cstd.h"
#include "picornt.inl"
#include "img/color.inl"
#include "img/imgopt.inl"
#include "cv1/drawing.inl"
#include "img/imgio.inl"
#include "cap.h"

void* cascade = 0;

int minsize;
int maxsize;

float angle;

float scalefactor;
float stridefactor;

float qthreshold;

int usepyr;
int noclustering;
int verbose;

void process_image(img_t* frame, int draw)
{
  IM_BEGIN(im, 10);
	int i, j, h, w;

	uint8_t* pixels;
	int nrows, ncols, ldim;

	#define MAXNDETECTIONS 2048
	int ndetections;
	float rcsq[4*MAXNDETECTIONS];

	img_t* gray = im;
	img_t* pyr[5] = {0, 0, 0, 0, 0};
  w=frame->width, h=frame->height;
  for (i=0; i<countof(pyr); ++i) {
    pyr[i] = im++;
		imsetsize(pyr[i], h, w, 1, 1);
    h/=2;
    w/=2;
  }

	// get grayscale image
  if(frame->c == 3) {
		imcolorcvt(frame, gray, T_RGB, T_GRAY);
  } else {
		imclone2(frame, gray);
  }

	// perform detection with the pico library

	if(usepyr)
	{
		int nd;

		//
		pyr[0] = gray;

		pixels = (uint8_t*)pyr[0]->data;
		nrows = pyr[0]->height;
		ncols = pyr[0]->width;
		ldim = pyr[0]->s;

		ndetections = find_objects(rcsq, MAXNDETECTIONS, cascade, angle, pixels, nrows, ncols, ldim, scalefactor, stridefactor, MAX(16, minsize), MIN(128, maxsize));

  w=frame->width, h=frame->height;
		for(i=1; i<5; ++i)
		{
      h/=2;
      w/=2;
			imresize(pyr[i-1], h, w, pyr[i]);

			pixels = (uint8_t*)pyr[i]->data;
			nrows = pyr[i]->height;
			ncols = pyr[i]->width;
			ldim = pyr[i]->step;

			nd = find_objects(&rcsq[4*ndetections], MAXNDETECTIONS-ndetections, cascade, angle, pixels, nrows, ncols, ldim, scalefactor, stridefactor, MAX(64, minsize>>i), MIN(128, maxsize>>i));

			for(j=ndetections; j<ndetections+nd; ++j)
			{
				rcsq[4*j+0] = (1<<i)*rcsq[4*j+0];
				rcsq[4*j+1] = (1<<i)*rcsq[4*j+1];
				rcsq[4*j+2] = (1<<i)*rcsq[4*j+2];
			}

			ndetections = ndetections + nd;
		}
	}
	else
	{
		//
		pixels = (uint8_t*)gray->data;
		nrows = gray->height;
		ncols = gray->width;
		ldim = gray->s;

		//
		ndetections = find_objects(rcsq, MAXNDETECTIONS, cascade, angle, pixels, nrows, ncols, ldim, scalefactor, stridefactor, minsize, MIN(nrows, ncols));
	}

	if(!noclustering)
		ndetections = cluster_detections(rcsq, ndetections);

	// if the flag is set, draw each detection
  if(draw) {
		for(i=0; i<ndetections; ++i)
			if(rcsq[4*i+3]>=qthreshold) // check the confidence threshold
				cvCircle(frame, cPoint(rcsq[4*i+1], rcsq[4*i+0]), rcsq[4*i+2]/2, cScalar(255, 0, 0), 4, 8, 0); // we draw circles here since height-to-width ratio of the detected face regions is 1.0f
  }

	// if the `verbose` flag is set, print the results to standard output
	if(verbose)
	{
		//
		for(i=0; i<ndetections; ++i)
			if(rcsq[4*i+3]>=qthreshold) // check the confidence threshold
				printf("%d %d %d %f\n", (int)rcsq[4*i+0], (int)rcsq[4*i+1], (int)rcsq[4*i+2], rcsq[4*i+3]);

		//
		//printf("# %f\n", 1000.0f*t); // use '#' to ignore this line when parsing the output of the program
	}
  IM_END(im);
}

int test_pico(int argc, char* argv[])
{
	//
	int arg;
	//
	//if(argc<2 || 0==strcmp("-h", argv[1]) || 0==strcmp("--help", argv[1]))
	if (0) {
		printf("Usage: pico <path/to/cascade> <options>...\n");
		printf("Detect objects in images.\n");
		printf("\n");

		// command line options
		printf("Mandatory arguments to long options are mandatory for short options too.\n");
		printf("  -i,  --input=PATH          set the path to the input image\n");
		printf("                               (*.jpg, *.png, etc.)\n");
		printf("  -o,  --output=PATH         set the path to the output image\n");
		printf("                               (*.jpg, *.png, etc.)\n");
		printf("  -m,  --minsize=SIZE        sets the minimum size (in pixels) of an\n");
		printf("                               object (default is 128)\n");
		printf("  -M,  --maxsize=SIZE        sets the maximum size (in pixels) of an\n");
		printf("                               object (default is 1024)\n");
		printf("  -a,  --angle=ANGLE         cascade rotation angle:\n");
		printf("                               0.0 is 0 radians and 1.0 is 2*pi radians\n");
		printf("                               (default is 0.0)\n");
		printf("  -q,  --qthreshold=THRESH   detection quality threshold (>=0.0):\n");
		printf("                               all detections with estimated quality\n");
		printf("                               below this threshold will be discarded\n");
		printf("                               (default is 5.0)\n");
		printf("  -c,  --scalefactor=SCALE   how much to rescale the window during the\n");
		printf("                               multiscale detection process (default is 1.1)\n");
		printf("  -t,  --stridefactor=STRIDE how much to move the window between neighboring\n");
		printf("                               detections (default is 0.1, i.e., 10%%)\n");
		printf("  -u,  --usepyr              turns on the coarse image pyramid support\n");
		printf("  -n,  --noclustering        turns off detection clustering\n");
		printf("  -v,  --verbose             print details of the detection process\n");
		printf("                               to `stdout`\n");

		//
		printf("Exit status:\n");
		printf(" 0 if OK,\n");
		printf(" 1 if trouble (e.g., invalid path to input image).\n");

	}
	
const char* path_to_cascade = "E:/code/cstd/include/facelib/pico/facefinder";
	if (path_to_cascade) {
		int size;
		FILE* file;

		//
		file = fopen(path_to_cascade, "rb");

		if(!file)
		{
			printf("# cannot read cascade from '%s'\n", argv[1]);
			return 1;
		}

		//
		fseek(file, 0L, SEEK_END);
		size = ftell(file);
		fseek(file, 0L, SEEK_SET);

		//
		cascade = malloc(size);

		if(!cascade || size!=fread(cascade, 1, size, file))
			return 1;

		//
		fclose(file);
	}

	// set default parameters
	minsize = 30;
	maxsize = 1024;

	angle = 0.f;

	scalefactor = 1.1f;
	stridefactor = 0.1f;

	qthreshold = 5.0f;

	usepyr = 0;
	noclustering = 0;
	verbose = 0;

	// parse command line arguments
	arg = 2;

  IM_BEGIN(im, 4);

	if(verbose)
	{
		//
		printf("# Copyright (c) 2013, Nenad Markus\n");
		printf("# All rights reserved.\n\n");

		printf("# cascade parameters:\n");
		printf("#	tsr = %f\n", ((float*)cascade)[0]);
		printf("#	tsc = %f\n", ((float*)cascade)[1]);
		printf("#	tdepth = %d\n", ((int*)cascade)[2]);
		printf("#	ntrees = %d\n", ((int*)cascade)[3]);
		printf("# detection parameters:\n");
		printf("#	minsize = %d\n", minsize);
		printf("#	maxsize = %d\n", maxsize);
		printf("#	scalefactor = %f\n", scalefactor);
		printf("#	stridefactor = %f\n", stridefactor);
		printf("#	qthreshold = %f\n", qthreshold);
		printf("#	usepyr = %d\n", usepyr);
	}

	{
    const char* filename;
    filename = "C:/code/testc/feat/0/0.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171025041558_2.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171026030548_0.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171026041207_0.jpg";
    filename = "D:/rywclm/aaa_20171031053339_9.jpg";
    filename = "F:/dataset/facedetect/hztest.jpg";
    const char* input = filename;
		imread(input, 3, 1, im);
		process_image(im, 1);
    
    imshow_(im);
    WaitKey(0);
	}

if (1) {
  capdev cap[1] = { 0 };
  int h = 720, w = 1280;
  int ch, fram=0;
  
  const char* windowname = "--------------------";
  
  if (capdshow_open(cap, 0, w, h) > 0) {
    double avg=0;
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      img_t* im1 = im;
      
      {
        utime_start(_start_time);
        tictoc0;
        process_image(im, 1);
        tictoc;
        double t = utime_elapsed(_start_time);
        avg += t;
        printf("%f %f ", t, avg / (fram + 1));
      }
      
      printf("\n");
      imshow_(im1);
      ch = WaitKey(10);
    }
    
    cap_close(cap);
    imfrees2(im);
  }
}
  IM_END(im);

	return 0;
}
