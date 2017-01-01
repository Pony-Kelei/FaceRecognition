#pragma once

#include <fstream>  
#include <string>  
#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <map>
using namespace std;

std::vector<std::string> split(const string& input, const string& regex);
class FileReader
{
public:
	FileReader();
	FileReader(string namePath, string indexPath) :nameFilePath(namePath), indexFilePath(indexPath)
	{
		maxIndex = 0;
		imageOutputPath = "./Data/Temp/images";
		readOriginalNameFile();
		readOriginalIndexFile();
	};

	~FileReader();
	void readPeopleList(const char *filename);
	void readVideoList(const char *filename);

	vector<string> getVideoList()
	{
		return videoList;
	};
	vector<int> getFrameList()
	{
		return frameNums;
	};

	string getNameFilePath() {
		return nameFilePath;
	};

	string getIndexFilePath() {
		return indexFilePath;
	}


private:
	int maxIndex;

	void readOriginalNameFile();
	void readOriginalIndexFile();

	string nameFilePath;
	string indexFilePath;
	string imageOutputPath;

	map<string, int> name2id;

	vector<string> nameList;
	vector<string> imageList;
	vector<string> videoList;
	vector<int> frameNums;

	void generateNameFile();
	void generateImageFile();
	void cutImage(string imageFile, string outputName);
	void generateIndexFile();
};
/*
�������£�
1. ��ȡԭʼ��name��at�����ļ�
	���������ļ������ݷ���namelist,imagelist�Լ�name2id��
	readOriginalNameFile
	readOriginalIndexFile

2.��ȡ���ļ����ݣ�׷�ӵ�����������
	readPeopleList

3.д�����ļ���������������
	generateNameFile
	generateIndexFile
	generateImageFile


*/



