#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <omp.h>
#include <time.h>
#include <ctype.h>
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <opencv2\gpu\gpu.hpp>
#include <opencv2\core\internal.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <io.h>  
#include <direct.h> 

using namespace std;
using namespace cv;
using namespace cv::gpu;

class FaceRecognization{
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
	double scale = 2.8;
	int num_components = 9;
	double facethreshold = 9.0;
	//opencv��FaceRecogizerĿǰ��������ʵ����������������fisherface��������ѵ��ͼ��Ϊ���ţ���LBP���Ե���ͼ��ѵ��
	//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
	//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
	cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP����������ڵ���������֤����Ч�����

	vector<Mat> images;//��������images,labels�����ͼ�����ݺͶ�Ӧ�ı�ǩ
	vector<int> labels;
	map<int, string> names;



public:
	const char* windowName = "result";
	void  readMovie(char * address);

	void recog_and_draw(IplImage* img);


	void cvText(IplImage* img, const char* text, int x, int y);

	Mat norm_0_255(cv::InputArray _src);

	//��ȡ�ļ��е�ͼ�����ݺ���𣬴���images��labels����������
	void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');

	void readFile(const string &filename, map<int, string> &a, char separator = ';');

	bool read_img(vector<Mat> &images, vector<int> &labels);

	// ֻ�Ǽ����������������Ȧ�� 
	void detect_and_draw(IplImage* img);
};