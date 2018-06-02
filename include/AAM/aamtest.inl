/****************************************************************************
* 
* Copyright (c) 2008 by Yao Wei, all rights reserved.
*
* Author:      	Yao Wei
* Contact:     	njustyw@gmail.com
* 
* This software is partly based on the following open source: 
*  
*		- OpenCV 
* 
****************************************************************************/

#include <cstdio>
#include <string>
#include <highgui.h>

#include "AAM_IC.h"
#include "AAM_basic.h"
#include "AAM_VJFaceDetect.h"

using namespace std;

int aamtest( int argc, char **argv )
{
  if( argc != 5 ) {
    printf( "---- string:   aamtest\n"
      "---- string:	trained AAM model\n"
      "---- int:		output mode\n"
      "---- string:	directory name\n"
			"---- string:	collection name\n" );
    return 0;
  }

	AAM *aam;
	int type;
	std::ifstream AAM_Model_File( argv[ 1 ] );
	FILE *Collection_File = fopen( argv[ 4 ], "r" );
	char filename[ 256 ];

	if( AAM_Model_File == 0 )
	{
		fprintf( stderr, "ERROR(%s: %d): Cannot open file %s!\n", __FILE__, __LINE__, argv[ 1 ] );
		exit( 0 );
	}

	AAM_Model_File >> type;

	// aam-basic
	if( type == 0 )
		aam = new AAM_Basic;
	else if( type == 1 )
		aam = new AAM_IC;
	
	// read model from file
	aam->Read( AAM_Model_File );
	AAM_Model_File.close();

	//intial face detector
	AAM_VJFaceDetect fjdetect;
	fjdetect.LoadCascade("haarcascade_frontalface_alt2.xml");

	if( Collection_File == NULL )
	{
		fprintf( stderr, "ERROR(%s: %d): Cannot open file %s!\n", __FILE__, __LINE__, argv[ 4 ] );
		exit( 0 );
	}

	while( fgets( filename, 256, Collection_File ) != NULL )
	{
		cout << filename;

		if( filename[ strlen( filename ) - 1 ] == '\n' )
			filename[ strlen( filename ) - 1 ] = '\0';

		if( strstr( filename, ".txt" ) == NULL )
		{
			//load image
			IplImage *image = cvLoadImage( filename, 1 );
			
			if( image == 0 )
			{
				fprintf( stderr, "ERROR(%s, %d): Cannot open image file %s!\n",
						 __FILE__, __LINE__, filename );
				exit( 0 );
			}

			//detect face for intialization
			AAM_Shape Shape = fjdetect.Detect( image, aam->GetMeanShape() );

			//do image alignment
			aam->Fit( image, Shape, 30, false );

			//show GUI
			cvNamedWindow( "AAMFitting");
			aam->Draw( image, atoi( argv[ 2 ] ) );
			cvShowImage( "AAMFitting", image );
			cvWaitKey( 0 );
	
			cvReleaseImage( &image );
		}
	}

	return 0;
}
