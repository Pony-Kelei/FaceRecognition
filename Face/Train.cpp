#include "stdafx.h"
#include "Train.h"
//#include "Output.h"
using namespace std;
using namespace cv;

cv::Ptr<cv::FaceRecognizer> model;

std::vector<cv::Mat> images; //��������images,labels�����ͼ�����ݺͶ�Ӧ�ı�ǩ
std::vector<int> labels;
std::map<int, string> names;//��¼������ֵID�Ķ�Ӧ��ϵ

const char* cascade_name =
"./Data/Model/haarcascade_frontalface_alt.xml";//�����Ѿ�ѵ���õ��������xml����
const char* cascade_name1 = "./Data/Model/haarcascade_profileface.xml";
extern string OutPutModelFile = "./Data/Temp/testTrain.xml";
extern string OriginalNameFile = "./Data/Train/name.txt";
extern string OriginalIndexFile = "./Data/Train/index.txt";

string tempNameFile = "./Data/Temp/name.txt";
string tempIndexFile = "./Data/Temp/index.txt";

extern void Error(string msg);

template<class T>
string to_string(T t)
{
	ostringstream oss;//����һ����
	oss << t;//��ֵ����������
	return oss.str();//��ȡת������ַ�ת������д��result

}

void setModelPath(string modelPath)
{
	OutPutModelFile = modelPath;
}

void load(string namePath)
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	readFile(namePath, names);
	model->load(OutPutModelFile);
}
void load()
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	initWorkLoading();
}
void train()
{
	if (!model)
		model = cv::createEigenFaceRecognizer();
	initWorkTraining(tempNameFile, tempIndexFile);
}

//��ȡ��name.txt����ֵ���˵Ķ�Ӧ��ϵ���������ֵ��names��
void readFile(const string &filename, std::map<int, string> &a, char separator){
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, separator);  //�����ֺžͽ���
		getline(liness, classlabel);     //�����ӷֺź��濪ʼ���������н���

		if (!path.empty() && !classlabel.empty())
		{
			std::stringstream ss;
			ss << path;
			int i;
			ss >> i;
			a.insert(std::pair<int, string>(i, classlabel));
		}
	}
}

//��ȡ�ļ��е�ͼ�����ݺ���𣬴���images��labels����������
void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator){
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		string error_message = "No valid input file was given.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line, path, classlabel, name;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, separator);  //�����ֺžͽ���
		getline(liness, classlabel);     //�����ӷֺź��濪ʼ���������н���
		//		getline(liness, name);
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
void initWorkLoading(){
	readFile(OriginalNameFile, names);
	model->load(OutPutModelFile);
}


void initWorkTraining(string nametxtPath, string attxtPath){
	//string fn_csv1 = string("./einfacedata/name.txt");
	string fn_csv1 = nametxtPath;
	readFile(fn_csv1, names);

	//��ȡ���CSV�ļ�·��
	string fn_csv = attxtPath;
	try
	{
		//ͨ��./einfacedata/at.txt����ļ���ȡ�����ѵ��ͼ�������ǩ
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception &e)
	{
#ifdef DEBUG
		std::cerr << "Error opening file " << fn_csv << ". Reason: " << e.msg << std::endl;
		Error("Error opening file " + fn_csv + ". Reason: " + e.msg);
#endif
		exit(1);
	}
#ifdef DEBUG
	std::cout << images.size() << ":" << labels.size() << std::endl;
	Error(to_string<int>((int)images.size()) + ":" + to_string<int>((int)labels.size()));
#endif
	//���û�ж����㹻��ͼƬ�����˳�
	if (images.size() <= 2)
	{
		string error_message = "This demo needs at least 2 images to work.";
		CV_Error(CV_StsError, error_message);
	}

	//ģ��ѵ��������
	model->train(images, labels);
	model->save(OutPutModelFile);
	//model->save("MyEigenFaceModel1.xml");
    //model->load(OutPutModelFile);
}

void imageTransform(IplImage *img, string outPath){
	CvMemStorage* storage = 0;
	CvHaarClassifierCascade* cascade = 0;
	CvHaarClassifierCascade* cascade1 = 0;

	if (!cascade || !cascade1){
		cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0); //���ط����� 
		cascade1 = (CvHaarClassifierCascade*)cvLoad(cascade_name1, 0, 0, 0); //���ط����� 
	}

	IplImage *gray, *small_img;
	int i;
	storage = cvCreateMemStorage(0);
	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	small_img = cvCreateImage(cvSize(cvRound(img->width / 2), cvRound(img->height / 2)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY); // ��ɫRGBͼ��תΪ�Ҷ�ͼ�� 
	cvResize(gray, small_img, CV_INTER_LINEAR);
	cvEqualizeHist(small_img, small_img); // ֱ��ͼ���⻯ 

	if (cascade&&cascade1)
	{
		CvSeq* faces = cvHaarDetectObjects(small_img, cascade, storage,
			1.1, 3,
			CV_HAAR_DO_CANNY_PRUNING
			);

		if (!(faces->total)){
			faces = cvHaarDetectObjects(small_img, cascade1, storage,
				1.1, 3,
				CV_HAAR_DO_CANNY_PRUNING
				);
		}

		if (!faces->total){
#ifdef DEBUG
			Error("δ��⵽����");
#endif
			CvSize dst_cvsize;
			dst_cvsize.width = (int)(80);
			dst_cvsize.height = (int)(80);
			IplImage *resizeRes;
			resizeRes = cvCreateImage(dst_cvsize, small_img->depth, small_img->nChannels);
			cvResize(small_img, resizeRes, CV_INTER_NN);
			imwrite(outPath, (Mat)resizeRes);
		}
		else
		{
			for (i = 0; i < (faces ? faces->total : 0); i++)
			{
				CvRect* r = (CvRect*)cvGetSeqElem(faces, i); // ��faces���ݴ�CvSeqתΪCvRect 
				CvMat small_img_roi;

				cvGetSubRect(small_img, &small_img_roi, *r);

				IplImage* imgd = cvCreateImage(cvGetSize(&small_img_roi), 8, 1);
				cvGetImage(&small_img_roi, imgd);

				IplImage *resizeRes;
				CvSize dst_cvsize;
				dst_cvsize.width = (int)(80);
				dst_cvsize.height = (int)(80);

				resizeRes = cvCreateImage(dst_cvsize, imgd->depth, imgd->nChannels);
				//��⵽��������ܲ���80x80��С��������Ҫ��ֵ����ͳһ��С��ͼ��Ĵ�С�����Լ�ָ����
				cvResize(imgd, resizeRes, CV_INTER_NN);

				//ת�������ɵ�80*80��Ŀ��ͼƬ
				imwrite(outPath, (Mat)resizeRes);
			}
		}
	}
	cvClearMemStorage(storage);
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
}

