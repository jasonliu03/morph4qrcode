#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

const int SIZE = 3;
const int THRESHOLD = 200;
const double RATIO_STADEV = 1;

string dir_path = "./src/";  

int Image_Stat(Mat img)
{
    
    int i,j,width,height,step,chanel;
    unsigned char *dataX;
 
    width = img.cols;
    height = img.rows;
 
    //�����������
    dataX = (unsigned char *)img.data;
 
    //���㲽��
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
    int diffCount = 0;
    if(argc > 1)
    {
        dir_path = argv[1];
    }

    Directory dir;  
    vector<string> fileNames = dir.GetListFiles(dir_path, "*", false);  
    double dArrData[1000] = {0};
    int i=0;
    for(i=0; i < fileNames.size(); i++){
        string fileFullName = dir_path + fileNames[i];
        cout << "fileFullName:" << fileFullName << endl;

        Mat srcImg = imread(fileFullName);
        Mat dstImg;
        Mat element = getStructuringElement(MORPH_RECT, Size(SIZE, SIZE));

        morphologyEx(srcImg, dstImg, MORPH_OPEN, element);  //������
        //morphologyEx(dstImg, dstImg, MORPH_OPEN, element);  //������
        //morphologyEx(srcImg, dstImg, MORPH_CLOSE, element);  //������
        //morphologyEx(srcImg, dstImg, MORPH_GRADIENT, element);  //��̬ѧ�ݶ�����
        //morphologyEx(srcImg, dstImg, MORPH_TOPHAT, element);  //��ñ����
        //morphologyEx(srcImg, dstImg, MORPH_BLACKHAT, element);  //??????����
        //morphologyEx(srcImg, dstImg, MORPH_ERODE, element);  //��̬ѧEROAD

        int srcCount = Image_Stat(srcImg);
        int dstCount = Image_Stat(dstImg);
        diffCount += srcCount - dstCount;
        
        dArrData[i] = srcCount - dstCount; 
        cout << "diffCount:" << srcCount - dstCount << endl;

        cvNamedWindow("src", CV_WINDOW_NORMAL| CV_WINDOW_KEEPRATIO| CV_GUI_EXPANDED);
        cvNamedWindow(fileFullName.c_str(), CV_WINDOW_NORMAL| CV_WINDOW_KEEPRATIO| CV_GUI_EXPANDED);
        imshow("src", srcImg);
        imshow(fileFullName.c_str(), dstImg);
        //waitKey(0);
    }

    int DATA_CNT = i;
    double dSum = diffCount;

    // ��ƽ���� 
    double dAverageData = dSum / DATA_CNT; 
 
    // �������е����ķ���(�������ݷֱ�����͵�ƽ����֮���ƽ���ĺ͵�ƽ����) 
    double dVariance = 0.0; 
    for (int nIdx = 0; nIdx < DATA_CNT; nIdx++) 
    { 
        double dDeviate = dArrData[nIdx] - dAverageData; 
        dVariance += pow(dDeviate, 2); 
        cout << "dArrData:" << dArrData[nIdx] << endl;
    } 
    dVariance /= DATA_CNT; 
 
    // �����׼��(���������ƽ����,��ӳһ�����ݵ���ɢ����) 
    double dStandardDeviation = sqrt(dVariance);

    cout << "avg diffCount:" << dAverageData << endl;
    cout << "dStandardDeviation:" << dStandardDeviation << endl;

    double dRstArr[1000] = {0};
    int index = 0;
    for (int nIdx = 0; nIdx < DATA_CNT; nIdx++) 
    { 
        double dDeviate = dArrData[nIdx] - dAverageData; 
        if (abs(dDeviate) <= RATIO_STADEV*dStandardDeviation) 
        { 
            dRstArr[index++] = dArrData[nIdx];
        } 
    }
    
    double rstSum = 0;
    for (int nIdx = 0; nIdx < index; nIdx++)
    {
        rstSum += dRstArr[nIdx];
    }
    double finalAvg = rstSum/index;
    cout << "finalAvg:" << finalAvg << endl;
        

    return 0;
}
