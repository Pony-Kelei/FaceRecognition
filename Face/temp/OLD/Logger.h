#pragma
#include<stdio.h>
#include<iostream>

using namespace std;
//typedef char byte;

class Logger
{
private:
	char* readed;
	FILE *log;
	int size;
public:
	Logger(char *path, int videoNum);	//�����ļ���������
	~Logger(){
		delete[] readed;
		fclose(log);
	}
	void WriteLog(int pos);		//�����Ѷ����ļ����,������0��ʼ
	void InitLog(char *path, int videoNum);
	void ReadLog();		//����־�е����ݶ����¼����
	bool IsReaded(int pos);		//��ȡ�ñ����Ƶ�Ƿ��Ѷ���,�Ƿ���true,�񷵻�false
	void Error(char* err);	//���������Ϣ
};