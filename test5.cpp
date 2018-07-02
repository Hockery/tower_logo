#include <stdio.h>
#include <cv.h>
#include <highgui.h>
 
// #pragma comment(lib, "cv.lib")
// #pragma comment(lib, "cxcore.lib")
// #pragma comment(lib, "highgui.lib")
 
int main( int argc, char** argv )  
{
	IplImage* src = cvLoadImage(".\\wind.png", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	cvThreshold(src, src,120, 255, CV_THRESH_BINARY);	// 二值化
	cvNamedWindow("Source", 1);
	cvShowImage("Source", src);
	// 提取轮廓
	int contour_num = cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cvZero(dst);		// 清空数组
	CvSeq *_contour = contour; 
	double maxarea = 0;
	double minarea = 100;
	int m = 0;
	for( ; contour != 0; contour = contour->h_next )  
	{  
 
		double tmparea = fabs(cvContourArea(contour));
		if(tmparea < minarea)   
		{  
			cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
			continue;
		}  
		CvRect aRect = cvBoundingRect( contour, 0 ); 
		if ((aRect.width/aRect.height)<1)  
		{  
			cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
			continue;
		}  
		if(tmparea > maxarea)  
		{  
			maxarea = tmparea;
		}  
		m++;
		// 创建一个色彩值
		CvScalar color = CV_RGB( 0, 255, 255 );
 
		//max_level 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓
		//如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种
		//如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓
		cvDrawContours(dst, contour, color, color, -1, 1, 8);	//绘制外部和内部的轮廓
	}  
	contour = _contour;
	int count = 0;
	for(; contour != 0; contour = contour->h_next)
	{  
		count++;
		double tmparea = fabs(cvContourArea(contour));
		if (tmparea == maxarea)  
		{  
			CvScalar color = CV_RGB( 255, 0, 0);
			cvDrawContours(dst, contour, color, color, -1, 1, 8);
		}  
	}  
	printf("The total number of contours is:%d", count);
	cvNamedWindow("Components", 1);
	cvShowImage("Components", dst);
	cvWaitKey(0);
	cvDestroyWindow("Source");
	cvReleaseImage(&src);
	cvDestroyWindow("Components");
	cvReleaseImage(&dst);
 
	return 0;
}