#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
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
#include <vector>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <io.h>  
#include <map>
#include <direct.h> 

using namespace cv;
using namespace cv::gpu;

struct Container
{
	int num;
	string name;
};

extern cv::Ptr<cv::FaceRecognizer> model;

class FaceRecognization{

	CvMemStorage* storage = 0;
	CvHaarClassifierCascade* cascade = 0;
	CvHaarClassifierCascade* cascade1 = 0;

	const char* cascade_name =
		"../data/haarcascade_frontalface_alt.xml";//�����Ѿ�ѵ���õ��������xml����
	const char* nested_cascade_name =
		"../data/haarcascade_eye_tree_eyeglasses.xml";
	const char* cascade_name1 = "../data/haarcascade_profileface.xml";

	CvCapture* capture = 0;

	double scale = 4;

	int numFrames = 0;
	int totalFrames = 0;

	vector<Mat> images; //��������images,labels�����ͼ�����ݺͶ�Ӧ�ı�ǩ
	vector<int> labels;
	vector<Container> con;//��¼ÿ�˵ĳ��ִ����Ա�ѡ���
	IplImage * frame_copy = 0;
	bool repeat = false;
	std::map<int, string> names;//��¼������ֵID�Ķ�Ӧ��ϵ

	string detectedName = "";
	vector<bool>record;
	vector<double>confidenceRecord;

	double maxSim=1.0;

public:

	void startWork(string nameFile, string indexFile);

	void imageTransform(IplImage * image);

	bool readMovieByNum(int num);

	void detectMovie(string movieName, string add);

	double generatePartSim(int begin,int end);

	void setMaxSim();

	void initWorkTraining();

	void initWorkLoading();

	bool recog_and_draw(IplImage* img, int num);

	void refineSearch(int num);

	void decideName();

	Mat norm_0_255(cv::InputArray _src);

	//��ȡ�ļ��е�ͼ�����ݺ���𣬴���images��labels����������
	void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');

	void readFile(const string &filename, std::map<int, string> &a, char separator = ';');

};