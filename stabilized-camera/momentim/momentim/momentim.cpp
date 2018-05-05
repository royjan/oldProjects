/*
Ort Hermelin
Modified By Roy Jan
Original By David Stavens
*/

#include "stdafx.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>
static const double pi = 3.14159265358979323846;
//Linking to OpenCV's Folder and Define pi
inline static double square(int a) {
  return a * a;
}
//Calcute the hypotenuse
inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels )	{
  if ( *img != NULL ) return;
  *img = cvCreateImage( size, depth, channels );
  if ( *img == NULL ) {
    fprintf(stderr, "Error: Couldn't allocate image. Out of memory?\n");
    exit(-1);
  }
}


	IplImage *rotateImage(const IplImage *src, int angleDegrees) {    
    IplImage *imageRotated = cvCloneImage(src);

    if(angleDegrees!=0){
        CvMat* rot_mat = cvCreateMat(2,3,CV_32FC1);
    
        // Compute rotation matrix
        CvPoint2D32f center = cvPoint2D32f( cvGetSize(imageRotated).width/2, cvGetSize(imageRotated).height/2 );
        cv2DRotationMatrix( center, angleDegrees, 1, rot_mat );

        // Do the transformation
        cvWarpAffine( src, imageRotated, rot_mat );
    }

    return imageRotated;
	}


//Checking if there isn't camera , show an error
int main(void) {
	clock_t start = clock();
  CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  if (capture == NULL) {
    fprintf(stderr, "Error: Can't open video.\n");
    return -1;
  }
  //Open camera else show error
  cvQueryFrame( capture );
  CvSize frame_size;
  frame_size.height =
  (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT );
  frame_size.width =
  (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH );
  long number_of_frames;
  cvSetCaptureProperty( capture, CV_CAP_PROP_POS_AVI_RATIO, 1. );
  number_of_frames = (int) cvGetCaptureProperty( capture,  
  CV_CAP_PROP_POS_FRAMES );
  cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES, 0. );
  cvNamedWindow("Optical Flow", CV_WINDOW_AUTOSIZE);
  //cvNamedWindow("Optical Flow Rotated", CV_WINDOW_AUTOSIZE);
  //Setting window and properties for showing 
  char key;
  long current_frame = 0;
  int count=0;
  double zavit=0;
  double zavitsum=0;
  double zaviterror=0;
  double hypotenusesum=0;
  double hypotenuseavg=0;
  int changey=0;
  int k=1;
  int r=1;
  int p;
  CvPoint p2,q2,q3,reng1,reng2;
  reng2.y = 140;
  reng1.y = 340;
  reng1.x = 220;
  reng2.x = 420;
  p2.x = 320;
  p2.y = 240;
  double zavit_yahas=0;
  int width;
  int height;
  int depth;
  int nchannels;
  int ex=0;
  int changeyc=0;
  int changexc=0;
  int changex=0;
  //Setting variables

  while(true) {
  //CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  static IplImage *frame = NULL, *frame_1C = NULL,
  *frame2_1C = NULL, *eig_image = NULL, *temp_image = NULL,
  *pyramid1 = NULL, *pyramid2 = NULL ,*test=NULL ,*test2=NULL ,*rotated_img=NULL;
  cvSetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES,current_frame);
  frame = cvQueryFrame( capture );
  if (frame == NULL) {
    fprintf(stderr, "Error: Hmm. The end came sooner than we thought.\n");
    return -1;
  }
  width     = frame->width;
  height    = frame->height;
  depth     = frame->depth;
  nchannels = frame->nChannels;
  test = cvCreateImage( cvSize( width, height ), depth, nchannels );
  test2 = cvCreateImage( cvSize( width, height ), depth, nchannels );
  cvCopy( frame, test );
  cvCopy( frame, test2 );
  //cvSmooth( frame, frame, CV_GAUSSIAN, 11,11 );
  //Checking if the camera is on
  allocateOnDemand( &frame_1C, frame_size, IPL_DEPTH_8U, 1 );
  cvConvertImage(frame, frame_1C, CV_CVTIMG_FLIP);
  allocateOnDemand( &frame, frame_size, IPL_DEPTH_8U, 3 );
  cvConvertImage(frame, frame, CV_CVTIMG_FLIP);
  frame = cvQueryFrame( capture);
  if (frame == NULL) {
    fprintf(stderr, "Error: Hmm. The end came sooner than we thought.\n");
    return -1;
  }
  //Checking if the camera is on
  allocateOnDemand( &frame2_1C, frame_size, IPL_DEPTH_8U, 1 );
  cvConvertImage(frame, frame2_1C, CV_CVTIMG_FLIP);
  allocateOnDemand( &eig_image, frame_size, IPL_DEPTH_32F, 1 );
  allocateOnDemand( &temp_image, frame_size, IPL_DEPTH_32F, 1 );
  CvPoint2D32f frame_features[400];
  int number_of_features;
  number_of_features = 400;
  cvGoodFeaturesToTrack(frame_1C, eig_image, temp_image,
  frame_features, & number_of_features, .01, .01, NULL);
  CvPoint2D32f frame2_features[400];
  char optical_flow_found_feature[400];
  float optical_flow_feature_error[400];
  CvSize optical_flow_window = cvSize(3,3);
  CvTermCriteria optical_flow_termination_criteria
  = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );
  allocateOnDemand( &pyramid1, frame_size, IPL_DEPTH_8U, 1 );
  allocateOnDemand( &pyramid2, frame_size, IPL_DEPTH_8U, 1 );
  cvCalcOpticalFlowPyrLK(frame_1C, frame2_1C, pyramid1, pyramid2,
  frame_features, frame2_features, number_of_features, 
  optical_flow_window, 5, optical_flow_found_feature, 
  optical_flow_feature_error,	optical_flow_termination_criteria, 0);
  //Optical Flow Calcs
  count=0;
  zavit=0;
  hypotenusesum=0;
  k=0;
  //Reset conts








  for(int i = 0; i <number_of_features; i++) {
    if ( optical_flow_found_feature[i] == 0 ) continue;
    int line_thickness = 1;
    CvPoint p,q;
    p.x = (int) frame_features[i].x;
    p.y = (int) frame_features[i].y;
    q.x = (int) frame2_features[i].x;
    q.y = (int) frame2_features[i].y;
    double angle;
	angle = atan2((double) p.y-q.y,(double) p.x-q.x);
    double hypotenuse;
    hypotenuse = sqrt( square(p.y - q.y) + square(p.x - q.x) );
	changex=(int)(p.x - q.x);
	changey=(int)(p.y - q.y);
	//printf("%d) changey : %d\n\n",i,changey);
    CvScalar line_color;
    if (hypotenuse>hypotenuseavg*2) {
      line_color = CV_RGB(0,0,20);
    }
    else {
      line_color = CV_RGB(255,0,0);
	  count=count+1;
      zavit=((int)(angle*180/pi)+zavit);
      k=(int) frame_features[i].y+k;
  	  hypotenusesum=hypotenuse+hypotenusesum;
    }
    //RANSAC
    q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
    q.y = (int) (p.y - 3 * hypotenuse * sin(angle));
    cvLine( frame, p, q, line_color, line_thickness, CV_AA, 0 );
    p.x = (int) (q.x + 9 * cos(angle + pi / 4));
    p.y = (int) (q.y + 9 * sin(angle + pi / 4));
    cvLine( frame, p, q, line_color, line_thickness, CV_AA, 0 );
    p.x = (int) (q.x + 9 * cos(angle - pi / 4));
    p.y = (int) (q.y + 9 * sin(angle - pi / 4));
    cvLine( frame, p, q, line_color, line_thickness, CV_AA, 0 );
  } 
  //Draw points
  hypotenuseavg=hypotenusesum/count;
  zavitsum = zavit/(double)count;
  zavitsum = 360.0-zavitsum;
  zavit_yahas=zavitsum-zaviterror;
  //printf("zavit sofi = %lf, zaviterror=%lf,hypotenuse=%lf,yahas=%lf\n",zavitsum,zaviterror-zavitsum,hypotenuseavg,zavit_yahas);
  zaviterror=zavitsum; 
  q2.x = (int) (p2.x + 90 * cos(zavitsum*pi/180));
  q2.y = (int) (p2.y + 90 * sin(zavitsum*pi/180));
  q3.x = (int) (p2.x + 90 * cos(zavit_yahas*pi/180));
  q3.y = (int) (p2.y + 90 * sin(zavit_yahas*pi/180));
  






  int line_thickness_arrow;
  line_thickness_arrow = 2; 
  CvScalar line_color2;
  line_color2 = CV_RGB(0,255,0);
  cvLine( frame, p2, q2, line_color2, line_thickness_arrow, CV_AA, 0 );
  CvScalar line_color3;
  line_color3 = CV_RGB(150,150,150);
  cvLine( frame, p2, q3, line_color3, line_thickness_arrow, CV_AA, 0 );
  cvCircle(frame,p2,5,line_color2,line_thickness_arrow,CV_AA,0);   
  //Draw Angle
  IplImage *roiimg = test;
  IplImage *roiimg2 = test2;
  printf("changexc : %d\n\n",changexc);
  changeyc=changeyc+changey;
  changexc=changexc+changex;
  reng2.x = 220-changexc;
  reng1.x = 420-changexc;
  reng2.y = 140+changeyc;
  reng1.y = 340+changeyc;
  printf("\n\n%d  %d\n\n",changey,changeyc);
  cvSetImageROI(roiimg, cvRect(220-changexc, 120+changeyc , 250, 250));
  cvSetImageROI(roiimg2, cvRect(220, 120 , 250, 250));
  //Show ROI stabled and original
  cvRectangle(frame,reng1,reng2,line_color3,line_thickness_arrow);
  cvShowImage("Optical Flow", frame); 
  IplImage *img2 = cvCreateImage(cvGetSize(roiimg),roiimg->depth,roiimg->nChannels);
  IplImage *img3 = cvCreateImage(cvGetSize(roiimg2),roiimg->depth,roiimg->nChannels);
  /* copy subimage */
  cvCopy(roiimg, img2, NULL);
  cvCopy(roiimg2, img3, NULL);
  /* always reset the Region of Interest */
  cvResetImageROI(roiimg);
  cvResetImageROI(roiimg2);
  cvShowImage("stabled", img2);
  cvShowImage("original", img3);
  key=cvWaitKey (10);
  if (key==32) { //If SPACE Pressed , reset ROI to middle
	changeyc=0;
	changexc=0;
	printf("key pressed\n");
  }
  rotated_img=rotateImage(roiimg,(int)(360-zavitsum));
  //cvShowImage("Optical Flow Rotated", rotated_img); 
printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC); //Print time calc
  cvWaitKey(1);
  } 
}

