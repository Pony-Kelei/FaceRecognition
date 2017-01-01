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
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
#include "detect_recog.h"

using namespace std;
using namespace cv;
#ifdef _EiC
#define WIN32
#endif

CvMemStorage* storage = 0;
CvHaarClassifierCascade* cascade = 0;
CvHaarClassifierCascade* cascade1 = 0;

CvHaarClassifierCascade* nested_cascade = 0;
int use_nested_cascade = 0;
const char* cascade_name =
    "./data/haarcascade_frontalface_alt.xml";//�����Ѿ�ѵ���õ��������xml����
const char* nested_cascade_name =
    "./data/haarcascade_eye_tree_eyeglasses.xml";
const char* cascade_name1 = "./data/haarcascade_profileface.xml";

CvCapture* capture = 0;
IplImage *frame, *frame_copy = 0;
IplImage *image = 0;
const char* scale_opt = "--scale="; // ������ѡ��ָʾ���� 
int scale_opt_len = (int)strlen(scale_opt);
const char* cascade_opt = "--cascade=";
int cascade_opt_len = (int)strlen(cascade_opt);
const char* nested_cascade_opt = "--nested-cascade";
int nested_cascade_opt_len = (int)strlen(nested_cascade_opt);
double scale = 2.5;
int num_components = 9;
double facethreshold = 9.0;
//opencv��FaceRecogizerĿǰ��������ʵ����������������fisherface��������ѵ��ͼ��Ϊ���ţ���LBP���Ե���ͼ��ѵ��
//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP����������ڵ���������֤����Ч�����

vector<Mat> images;//��������images,labels�����ͼ�����ݺͶ�Ӧ�ı�ǩ
vector<int> labels;
map<int, string> names;


int main( int argc, char** argv )
{
	string fn_csv1 = string("./einfacedata/name.txt");
	readFile(fn_csv1, names);

	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //���ط����� 
	cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //���ط����� 
	nested_cascade = (CvHaarClassifierCascade*)cvLoad(nested_cascade_name, 0, 0, 0);

    if(!cascade||!cascade1) 
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		getchar();
        return -1;
    }
	model->set("threshold", 2100.0);
	string output_folder;
	output_folder = string("./einfacedata");

	//��ȡ���CSV�ļ�·��
	string fn_csv = string("./einfacedata/at.txt");
	try
	{
		//ͨ��./einfacedata/at.txt����ļ���ȡ�����ѵ��ͼ�������ǩ
		read_csv(fn_csv, images, labels);	
	}
	catch(cv::Exception &e)
	{
		cerr<<"Error opening file "<<fn_csv<<". Reason: "<<e.msg<<endl;
		exit(1);
	}
	/*
	//read_img�������ֱ�Ӵ�einfacedata/trainingdataĿ¼�¶�ȡͼ�����ݲ�Ĭ�Ͻ�ͼ����Ϊ0
	//����������������ֻ����������������֤
	if(!read_img(images, labels))
	{
		cout<< "Error in reading images!";
		images.clear();
		labels.clear();
		return 0;
	}
	*/
	cout << images.size() << ":" << labels.size()<<endl;
	//���û�ж����㹻��ͼƬ�����˳�
	if(images.size() <= 2)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//�õ���һ����Ƭ�ĸ߶ȣ��������ͼ����ε�����ԭʼ��Сʱ��Ҫ
	//int height = images[0].rows;
	//�Ƴ����һ��ͼƬ������������
	//Mat testSample = images[images.size() - 1];
	//cv::imshow("testSample", testSample);
	//int testLabel = labels[labels.size() - 1];
	//images.pop_back();
	//labels.pop_back();

	//���洴��һ��������ģ����������ʶ��
	// ͨ��CSV�ļ���ȡ��ͼ��ͱ�ǩѵ������

	//����ѵ��
	model->train(images, labels);

    storage = cvCreateMemStorage(0); // �����ڴ�洢��   
    //capture = cvCaptureFromCAM(0); // ������Ƶ��ȡ�ṹ 
	//capture=cvCaptureFromFile("./mp4/index.mp4");
	capture = cvCaptureFromFile("C://Users//kelei//Desktop//faceRecgnition//mp4//index2.mp4");
    cvNamedWindow( "result", 1 );
    if(capture) // �������Ƶ������ͷ�ɼ�ͼ����ѭ������ÿһ֡ 
    {
		int numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		int totalFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
		cout << "֡�ʣ�" << numFrames << endl;
		cout << "��֡����" << totalFrames << endl;
     // #pragma omp parallel for
        for(int i=0;i<totalFrames;i++)
        {
			cout <<"��"<< i<<"֡"<< endl;
			//����cvGrabFrame, �õײ�api����һ֡ͼ��
		    // �������ʧ�ܣ����˳�ѭ��
			// ����ɹ��������ͼ�񱣴��ڵײ�api�Ļ�����
            if( !cvGrabFrame( capture ))
                break;
			// ������õ�ͼ����Ϣ�ӻ�����ת����IplImage��ʽ����frame��
            frame = cvRetrieveFrame( capture );
            if( !frame )
                break;

			if (frame->width > 1000){
				scale = 6;
			}
			//�˴���Ҫ����Ƶͼ��ķֱ���
            if( !frame_copy )
                frame_copy = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );

            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            else
                cvFlip( frame, frame_copy, 0 );
            
			cvShowImage("result", frame);
            //detect_and_draw( frame_copy ); // ����������������ֻ��ʵ���������
			//cout << frame_copy->width << "x" << frame_copy->height << endl;
			recog_and_draw( frame_copy );//�ú���ʵ����������ʶ��
            if( cvWaitKey(1) >= 0 )//esc��ֵ������100
                goto _cleanup_;
        }
        cvWaitKey(0);
		_cleanup_: // ���ʹ�ã��ڻ�����ù���C���ԣ��һ�û���ù� 
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }    
   // cvDestroyWindow("result");
    return 0;
}

