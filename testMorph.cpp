#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

const int SIZE = 3;
const int THRESHOLD = 200;

int POS_THRESHOLD = 696;
int NEG_THRESHOLD = 308;


int Image_Stat(Mat img)
{
    
    int i,j,width,height,step,chanel;
    unsigned char *dataX;
 
    width = img.cols;
    height = img.rows;
 
    //存入矩阵数据
    dataX = (unsigned char *)img.data;
 
    //计算步长
    step = img.step/sizeof(char);
    chanel = img.channels();
 
    int count=0;
    int index=0;

    for(i=0; i<height; i++)
        for(j=0; j<width*chanel; j++){
            if(dataX[i*step+j] >= THRESHOLD)
                index++;
        }

    for(i=0; i<height; i++)
        for(j=0; j<width; j++){
            if(img.at<Vec3b>(i, j)[0] >= THRESHOLD)
                count++;
        }

    //Mat_<Vec3f>::const_iterator rit=img.begin<Vec3f>();
    //Mat_<Vec3f>::const_iterator ritend=img.end<Vec3f>();
    //for(;rit!=ritend;++rit)
    //{
    //    if((*rit)[0] >= 200)
    //        count++;
    //}
 
    int area=width*height;
    cout<<"step, chanel:"<<step<<" "<<chanel<<endl;
    cout<<"width == "<<width<<" height == "<<height<<endl;
    cout<<"area == "<<area<<endl;
    cout<<"count == "<<count<<endl;
    cout<<"index == "<<index<<endl;
    cout<<"ratio == "<<(double)count/area<<endl;
    return count;
}

int main(int argc, char** argv)
{
    char * filename = "pos1.jpg";
    if(argc == 2)
    {
        filename = argv[1];
    }

    if(argc == 4)
    {
        filename = argv[1];
        POS_THRESHOLD = atoi(argv[2]);
        NEG_THRESHOLD = atoi(argv[3]);
    }

    Mat srcImg = imread(filename);
    Mat dstImg;
    Mat element = getStructuringElement(MORPH_RECT, Size(SIZE, SIZE));

    morphologyEx(srcImg, dstImg, MORPH_OPEN, element);  //开运算
    //morphologyEx(dstImg, dstImg, MORPH_OPEN, element);  //开运算
    //morphologyEx(srcImg, dstImg, MORPH_CLOSE, element);  //闭运算
    //morphologyEx(srcImg, dstImg, MORPH_GRADIENT, element);  //形态学梯度运算
    //morphologyEx(srcImg, dstImg, MORPH_TOPHAT, element);  //顶帽运算
    //morphologyEx(srcImg, dstImg, MORPH_BLACKHAT, element);  //??????运算
    //morphologyEx(srcImg, dstImg, MORPH_ERODE, element);  //形态学EROAD

    int srcCount = Image_Stat(srcImg);
    int dstCount = Image_Stat(dstImg);
    
    cout << "src - dst count:" << srcCount - dstCount << endl;
    
    if ((srcCount-dstCount) > ((POS_THRESHOLD+NEG_THRESHOLD)/2))
        cout << "result:" << "True" << endl;
    else
        cout << "result:" << "False" << endl;

    cvNamedWindow("src", CV_WINDOW_NORMAL| CV_WINDOW_KEEPRATIO| CV_GUI_EXPANDED);
    cvNamedWindow(filename, CV_WINDOW_NORMAL| CV_WINDOW_KEEPRATIO| CV_GUI_EXPANDED);
    imshow("src", srcImg);
    imshow(filename, dstImg);
    //waitKey(0);
    return 0;
}
