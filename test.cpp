#include "opencv2/opencv.hpp"
#define PI 3.1415926
using namespace cv;
#include <time.h>

#define RAND150T200 (rand()%100 + 150)
#define RAND0T100 (rand()%100)

int main(int argc, char *argv[])
{
	srandom(time(NULL));
    cv::Mat image = cv::imread("test4.png");
    cv::Mat image2 = image.clone();
    cv::Mat result;

    cv::GaussianBlur(image, image, Size(3, 3), 0, 0);
    cv::cvtColor(image, result, CV_BGRA2GRAY);


	// 局部二值化
	int blockSize = 41;
	int constValue = 10;
	cv::Mat local;
	cv::adaptiveThreshold(result, result, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);

    // Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    // // 高级形态学处理，调用这个函数就可以了，具体要选择哪种操作，就修改第三个参数就可以了。这里演示的是形态学梯度处理
    // morphologyEx(result, result, MORPH_ERODE, element);

    cv::imshow("result",result);
    // image = ~image;
    // cv::cvtColor(image2, image2, CV_BGRA2GRAY);
    cv::Mat contours;
    cv::Canny(result, contours, 125, 350); //边缘检测
    cv::imshow("contours",contours);
    std::vector<cv::Vec2f> lines;
    /*霍夫变换,获得一组极坐标参数（rho，theta）,每一对对应一条直线，保存到lines
      第3,4个参数表示在（rho，theta)坐标系里横纵坐标的最小单位，即步长*/
    cv::HoughLines(contours, lines, 1, PI / 180, 40); //步长自己设置
    std::vector<cv::Vec2f>::const_iterator iter = lines.begin();
    std::cout << lines.size() << std::endl;

   Point2f srcTri[3];
   Point2f dstTri[3];
   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( image.cols - 1, 0 );
   srcTri[2] = Point2f( 0, image.rows - 1 );

   dstTri[0] = Point2f( image.cols*0.0, image.rows*0.33 );
   dstTri[1] = Point2f( image.cols*0.85, image.rows*0.25 );
   dstTri[2] = Point2f( image.cols*0.15, image.rows*0.7 );

   Mat warp_mat( 2, 3, CV_32FC1 );
   warp_mat = getAffineTransform( srcTri, dstTri );
//    warpAffine( image, image, warp_mat, image.size());
    // std::vector<cv::Vec2f> vline;
    // std::vector<cv::Vec2f> hline;
    // std::vector<cv::Vec2f> vline_count;
    // std::vector<cv::Vec2f> hline_count;
    // while (iter != lines.end())
    // {
    //     float rho = (*iter)[0];
    //     float theta = (*iter)[1];
    //     if (theta < PI / 4. || theta > 3. * PI / 4)
    //     { //画交点在上下两边的直线
    //         cv::Point pt1(rho / cos(theta), 0);
    //         cv::Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
    //         cv::line(image, pt1, pt2, cv::Scalar(255,0,0), 1);
    //         vline.push_back(*iter);
    //         circle(image, pt1, 2, Scalar(100, 100, 0));
    //         circle(image, pt2, 2, Scalar(100, 100, 0));
    //         // for(std::vector<cv::Vec2f>::const_iterator viter; viter = vline_count)
    //     }
    //     else
    //     { //画交点在左右两边的直线
    //         hline.push_back(*iter);
    //         cv::Point pt1(0, rho / sin(theta));
    //         cv::Point pt2(result.cols, (rho - result.cols * cos(theta) / sin(theta)));
    //         cv::line(image, pt1, pt2, cv::Scalar(0,0,255), 1);
    //         circle(image, pt1, 2, Scalar(0, 255, 100));
    //         circle(image, pt2, 2, Scalar(0, 255, 100));
    //     }
    //     ++iter;
    // }

    // for(std::vector<cv::Vec2f>::const_iterator viter = vline.begin(); viter == vline.end(); viter++){
    //     for(std::vector<cv::Vec2f>::const_iterator hiter = hline.begin(); hiter == hline.end(); hiter++){

    //     }
    // }

    std::vector<cv::Vec2f> vline;
    std::vector<cv::Vec2f> hline;
    std::vector<Vec4i> lines2;
    HoughLinesP(contours, lines2, 1, CV_PI / 180, 50, 75, 40);
    printf("lines2.size():%ld\n",lines2.size());
    for (size_t i = 0; i < lines2.size(); i++)
    {
        Vec4i l = lines2[i];
        // line(image2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(RAND0T100, RAND150T200, RAND0T100), 2, 2);
        double k = (l[1] - l[3]) / (l[0] - l[2]);
        if (k >= -1 && k <= 1){ //横线
            line(image2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, RAND150T200, 0), 2, 2);
            int is_push = 1;
            for(std::vector<cv::Vec2f>::iterator viter = vline.begin(); viter == vline.end(); viter++){
                if( ((*viter)[0]-k)*((*viter)[0]-k) < 1){
                    (*viter)[1] += 1;
                    is_push = 0;
                    break; 
                }
            }
            if(is_push){
                cv::Vec2f vec;
                vec[0] = k;
                vec[1] = 1;
                vline.push_back(vec);
            }
        }
        else{//竖线
            line(image2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, RAND150T200), 2, 2);
            int is_push = 1;
            for(std::vector<cv::Vec2f>::iterator hiter = hline.begin(); hiter == hline.end(); hiter++){
                if( ((*hiter)[0]-k)*((*hiter)[0]-k) < 1){
                    (*hiter)[1] += 1;
                    is_push = 0;
                    break; 
                }
            }
            if(is_push){
                cv::Vec2f vec;
                vec[0] = k;
                vec[1] = 1;
                hline.push_back(vec);
            }
        }
    }
    
    cv::namedWindow("hough");
    cv::imshow("hough", image);
    cv::namedWindow("hough2");
    cv::imshow("hough2", image2);
    cv::waitKey(0);
}