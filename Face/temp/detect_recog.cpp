#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "detect_recog.h"
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <io.h>  
#include <direct.h> 

using namespace std;
using namespace cv;

//��ⲢȦ��������������⵽�����������ж�����ѵ��ͼ���е���һ��
void recog_and_draw( IplImage* img ) 
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage(cvSize(img->width,img->height), 8, 1);
    small_img = cvCreateImage(cvSize(cvRound(img->width/scale),
                         cvRound(img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img ); // ֱ��ͼ���⻯ 
    cvClearMemStorage(storage);
    if(cascade&&cascade1)
    {
        double t = (double)cvGetTickCount(); 
		CvSeq* faces = cvHaarDetectObjects(small_img, cascade, storage,
			1.1, 2,
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			CV_HAAR_DO_CANNY_PRUNING
			// |CV_HAAR_SCALE_IMAGE
			);
                                          //  cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // ͳ�Ƽ��ʹ��ʱ�� 
		cout << "��ʱ��"<<t << endl;
        printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		if (!(faces->total)){
			faces = cvHaarDetectObjects(small_img, cascade1, storage,
				1.1, 2,
				//|CV_HAAR_FIND_BIGGEST_OBJECT
				//|CV_HAAR_DO_ROUGH_SEARCH
				CV_HAAR_DO_CANNY_PRUNING
				/*CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH*/
				//|CV_HAAR_SCALE_IMAGE
				);
				//cvSize(30, 30));
		}

     //   #pragma omp parallel for
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem(faces, i ); // ��faces���ݴ�CvSeqתΪCvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center;
            CvScalar color = colors[i%8]; // ʹ�ò�ͬ��ɫ���Ƹ���face��������ɫ 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // �ҳ�faces���� 
            center.y = cvRound((r->y + r->height*0.5)*scale);

            //radius = cvRound((r->width + r->height)*0.25*scale); 
			//center.x = cvRound((r->x + r->width*0.5)*1); // �ҳ�faces���� 
			//center.y = cvRound((r->y + r->height*0.5)*1);
			radius = cvRound((r->width + r->height)*0.25*scale);

			cvGetSubRect( small_img, &small_img_roi, *r );
			
		//	cvShowImage("test",small_img);
		//	cvShowImage("test1", &small_img_roi);
		//	IplImage dst1 = &small_img_roi;
			//��ȡ��⵽������������Ϊʶ���ͼ��

			/*IplImage* dst1 = cvCreateImage(cvGetSize(&small_img_roi), img->depth, img->nChannels);
			
			cvConvert(&small_img_roi, dst1);*/

			IplImage* imgd = cvCreateImage(cvGetSize(&small_img_roi), 8, 1);
			cvGetImage(&small_img_roi, imgd);

		/*	IplImage *result;*/
			CvRect roi;
			roi = *r;
		/*	roi.width *= roi.width*scale;
			roi.height *= roi.height*scale;*/
			//result = cvCreateImage(cvSize(r->width, r->height), img->depth, img->nChannels );
			cvSetImageROI(img,roi);
			// ������ͼ��
			//cvCopy(img,result);
			cvResetImageROI(img);
			
			IplImage *resizeRes;
			CvSize dst_cvsize;
			dst_cvsize.width=(int)(100);
			dst_cvsize.height=(int)(100);
		/*	resizeRes=cvCreateImage(dst_cvsize,result->depth,result->nChannels);*/
			resizeRes = cvCreateImage(dst_cvsize, imgd->depth, imgd->nChannels);
			//��⵽��������ܲ���100x100��С��������Ҫ��ֵ����ͳһ��С��ͼ��Ĵ�С�����Լ�ָ����
			cvResize(imgd, resizeRes, CV_INTER_NN);
			//cvResize(&small_img_roi, resizeRes, CV_INTER_NN);
			//IplImage* img1 = cvCreateImage(cvGetSize(resizeRes), IPL_DEPTH_8U, 1);//����Ŀ��ͼ��	
		//	cvCvtColor(resizeRes,img1,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
            cvShowImage( "resize", resizeRes );
			cvCircle( img, center, radius, color, 3, 8, 0 ); // ������λ�û�Բ��Ȧ����������
			int predictedLabel = -1;
			//Mat test = img1;
			Mat test = resizeRes;
			//cvShowImage("test", result);
			//images[images.size() - 1] = test;
		/*	model->train(images, labels);*/
			
			//�������read_img����ʱ chdir��Ĭ��Ŀ¼���˸��ģ�����output.jpg�Լ���һ�°�
	//		imwrite("./einfacedata/trainingdata/ouput.jpg",test);

			//����������������б�
			double predicted_confidence = 10.0;
			model->predict(test,predictedLabel,predicted_confidence);
			/*if(predictedLabel == 0)
				cvText(img, "yes", r->x+r->width*0.5, r->y); 
			else
				cvText(img, "no", r->x+r->width*0.5, r->y); */
			/*char str[510];
			sprintf(str, "%s", names[predictedLabel]);*/
			//const char* c_s = names[predictedLabel].c_str();
		/*	wchar_t *WStr = new wchar_t[50];
			wcspcpy(WStr, WideString(CStr).c_bstr());*/

		//	cvText(img, names[predictedLabel].c_str(), r->x + r->width*0.5, r->y);
			//cout << "predict:"<<model->predict(test) << endl;
			
			cout << "predict:"<<names[predictedLabel] << "\nconfidence:" << predicted_confidence << endl;

            if( !nested_cascade )
                continue;
            
            nested_objects = cvHaarDetectObjects(&small_img_roi, nested_cascade, storage,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        |CV_HAAR_DO_CANNY_PRUNING
                                      //  |CV_HAAR_SCALE_IMAGE
                                        ,
                                        cvSize(30, 30));

            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
            {
                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
                radius = cvRound((nr->width + nr->height)*0.25*scale);
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
        }
    }
    cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

void cvText(IplImage* img, const char* text, int x, int y)  
{  
    CvFont font;  
    double hscale = 1.0;  
    double vscale = 1.0;  
    int linewidth = 2;  
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC,hscale,vscale,0,linewidth);  
    CvScalar textColor =cvScalar(0,255,255);  
    CvPoint textPos =cvPoint(x, y);  
    cvPutText(img, text, textPos, &font,textColor);  
}

Mat norm_0_255(cv::InputArray _src)
{
	Mat src = _src.getMat();
	Mat dst;

	switch(src.channels())
	{
	case 1:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}

	return dst;
}

//��ȡ�ļ��е�ͼ�����ݺ���𣬴���images��labels����������
void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator)
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel, name;
	while(getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);  //�����ֺžͽ���
		getline(liness, classlabel);     //�����ӷֺź��濪ʼ���������н���
//		getline(liness, name);
		if(!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

void readFile(const string &filename, map<int, string> &a, char separator){
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while (getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);  //�����ֺžͽ���
		getline(liness, classlabel);     //�����ӷֺź��濪ʼ���������н���
		//		getline(liness, name);
		if (!path.empty() && !classlabel.empty())
		{
		/*	images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));*/
			stringstream ss;
			ss << path;
			int i;
			ss >> i;
			a.insert(pair<int, string>(i, classlabel));
		}
	}
}

bool read_img(vector<Mat> &images, vector<int> &labels)
{
	
	long file;  
    struct _finddata_t find;  
  
    _chdir("./einfacedata/trainingdata/");  
    if((file=_findfirst("*.*", &find))==-1L) {  
        //printf("�հ�!/n");  
        return false;  
    }  
    //fileNum = 0;  
    //strcpy(fileName[fileNum], find.name);
	int i = 0;
    while(_findnext(file, &find)==0)  
    {  
		if(i == 0)
		{
			i++;
			continue;
		}
        images.push_back(imread(find.name, 0));
		labels.push_back(0);  
		cout << find.name << endl;
    }  
    _findclose(file);
	return true;
}
// ֻ�Ǽ����������������Ȧ�� 
void detect_and_draw( IplImage* img ) 
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
    IplImage *gray, *small_img;
    int i, j;
    gray = cvCreateImage(cvSize(img->width,img->height), 8, 1);
    small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
    cvCvtColor( img, gray, CV_BGR2GRAY ); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
    cvResize(gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist(small_img, small_img); // ֱ��ͼ���⻯ 
    cvClearMemStorage(storage);
    if(cascade)
    {
        double t = (double)cvGetTickCount(); 
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0
                                            //|CV_HAAR_FIND_BIGGEST_OBJECT
                                            //|CV_HAAR_DO_ROUGH_SEARCH
                                            |CV_HAAR_DO_CANNY_PRUNING
                                            //|CV_HAAR_SCALE_IMAGE
                                            ,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t; // ͳ�Ƽ��ʹ��ʱ�� 
        printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem(faces, i); // ��faces���ݴ�CvSeqתΪCvRect 
            CvMat small_img_roi;
            CvSeq* nested_objects;
            CvPoint center;
            CvScalar color = colors[i%8]; // ʹ�ò�ͬ��ɫ���Ƹ���face��������ɫ 
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale); // �ҳ�faces���� 
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale); 
            cvCircle(img, center, radius, color, 3, 8, 0); // ������λ�û�Բ��Ȧ���������� 
            if( !nested_cascade )
                continue;
            cvGetSubRect( small_img, &small_img_roi, *r );
            nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
                                        1.1, 2, 0
                                        //|CV_HAAR_FIND_BIGGEST_OBJECT
                                        //|CV_HAAR_DO_ROUGH_SEARCH
                                        //|CV_HAAR_DO_CANNY_PRUNING
                                        //|CV_HAAR_SCALE_IMAGE
                                        ,cvSize(0, 0) );
            for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
            {
                CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
                center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
                center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
                radius = cvRound((nr->width + nr->height)*0.25*scale);
                cvCircle( img, center, radius, color, 3, 8, 0 );
            }
        }
    }
    cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}