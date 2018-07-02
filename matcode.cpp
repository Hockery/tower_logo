//  基于局部自适应阈值的图像二值化
//  Author：www.icvpr.com
//  Blog:   http://blog.csdn.net/icvpr  
 
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
int main(int argc, char** argv)
{
	cv::Mat image = cv::imread("./test4.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty())
	{
		std::cout<<"read image failure"<<std::endl;
		return -1;
	}
 
	cv::imshow("aa",image);
	// 全局二值化
	int th = 200; 
	cv::Mat global;
	cv::threshold(image, global, th, 255, CV_THRESH_BINARY_INV);
	cv::imshow("global",global);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
        
    //高级形态学处理，调用这个函数就可以了，具体要选择哪种操作，就修改第三个参数就可以了。这里演示的是形态学梯度处理
    morphologyEx(global, global, MORPH_ERODE, element);
	// 局部二值化
	int blockSize = 41;
	int constValue = 10;
	cv::Mat local;
	cv::adaptiveThreshold(image, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
 
	// global = ~global;
	// cv::imwrite("global.jpg", global);
	// cv::imwrite("local.jpg", local);
 
	cv::imshow("globalThreshold", global);
    // cv::imwrite("globalThreshold.jpeg", global);
	cv::imshow("localThreshold", local);
	cv::waitKey(0);
 
	return 0;
}
