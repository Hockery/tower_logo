#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//画圆画点都是使用circle()函数来画，点就是圆，我们平常所说的圆只不过是半径大一点而已。
int main()
{
    Mat img = imread("lol16.jpg");  

    //画空心点
    Point p(20, 20);//初始化点坐标为(20,20)
    circle(img, p, 2, Scalar(0, 255, 0)); //第三个参数表示点的半径，第四个参数选择颜色。这样子我们就画出了绿色的空心点

    //这种初始化点的方式也可以
    Point p2;
    p2.x = 100;
    p2.y = 100;
    //画实心点
    circle(img, p2, 3,Scalar(255,0,0),-1); //第五个参数我设为-1，表明这是个实点。

    //画空心圆
    Point p3(300, 300);
    circle(img,p3,100,Scalar(0,0,255),3);//第五个参数我们调高点，让线更粗

    //画实心圆
    Point p4;
    p4.x = 600;
    p4.y = 600;
    circle(img, p4, 100, Scalar(120, 120, 120), - 1);

    imshow("画点画圆", img);

    waitKey();
    return 0;
}