#pragma
#include"ErrorOutput.h"
#include<sstream>

ErrorOutput eo("../error.txt");

void Error(string msg)
{
	eo.Print(msg);
}

//string to_string(int t)
//
//{
//	ostringstream oss;//����һ����
//	oss << t;//��ֵ����������
//	return oss.str();//��ȡת������ַ�ת������д��result
//}
//
//string to_string(double t)
//
//{
//	ostringstream oss;//����һ����
//	oss << t;//��ֵ����������
//	return oss.str();//��ȡת������ַ�ת������д��result
//}
//
//
//string to_string(float t)
//{
//	ostringstream oss;//����һ����
//	oss << t;//��ֵ����������
//	return oss.str();//��ȡת������ַ�ת������д��result
//}
//
