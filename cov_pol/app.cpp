#include "stdafx.h"
#include<iostream>
#include<Eigen/Dense>
#include<unsupported/Eigen/FFT>
#define M_PI       3.14159265358979323846   // pi

#include <time.h>      //���ͷ�ļ�
#include <sys/timeb.h>
using namespace Eigen;
using namespace std;
#define Type double
#define EPS 1.0e-14
typedef unsigned char byte;

typedef struct {
	int W;//ͼ��Ŀ�
	int H;//ͼ��ĸ�
	int num;//ͼ�����
	byte ***data;//��������
}Data;



void init_output(Data &output, Data input, int out_num, int use_filter, bool border)
{

	



}
/*
���ܣ����
���룺
Data input:Ҫ���о����ͼ�����ݵ��ܺ�
out_num:ָ�����֮��ͼ��ĸ���
start_filter:ָ������һ��filter��ʼ���о��
border:true������ԭͼ��ı߽磬ͼ���С���䣻false���Ա߽磬ͼ���С�����仯
���:
Data &output:���֮���ͼ����ܺ�
*/
bool convolution(Data input, Data &output, int out_num, int start_filter, bool border)
{

}





void main()
{



	//���ģ��
	Matrix<double, 3, 3> filter0, filter1, filter2, filter3, filter4, filter5, filter6, filter7, filter8, filter9, filter10;
	Matrix<double, 3, 3> filter11, filter12, filter13, filter14, filter15, filter16, filter17, filter18, filter19, filter20;
	Matrix<double, 3, 3> filter21, filter22, filter23, filter24, filter25;

	filter0 << 0, 0, 0,
		0, 1, 0,
		0, 0, 0;
	filter1 << 1 / 16.0, 1 / 16.0, 1 / 16.0,
		1 / 16.0, 8 / 16.0, 1 / 16.0,
		1 / 16.0, 1 / 16.0, 1 / 16.0;
	filter2 << -1, 0, 0,
		0, 1, 0,
		0, 0, 0;
	filter3 << 0, -1, 0,
		0, 1, 0,
		0, 0, 0;
	filter4 << 0, 0, 0,
		-1, 0, 1,
		0, 0, 0;
	filter5 << 1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9;
	filter6 << 0, 0, 0,
		-1, 2, -1,
		0, 0, 0;
	filter7 << 0, 1, 0,
		1, -4, 1,
		0, 1, 0;
	filter8 << 0, 0, -1,
		0, 1, 0,
		0, 0, 0;
	filter9 << 0, 0, 0,
		-1, 1, 0,
		0, 0, 0;
	filter10 << 0, -1, 0,
		0, 0, 0,
		0, 1, 0;
	filter11<< 1 / 16.0, 2 / 16.0, 1 / 16.0,
		2 / 16.0, 4 / 16.0, 2 / 16.0,
		1 / 16.0, 2 / 16.0, 1 / 16.0;
	filter12 << 0, -1, 0,
		0, 2, 0,
		0, -1, 0;
	filter13 << 1, 1, 1,
		1, -7, 1,
		1, 1, 1;
	filter14 << 0, 0, 0,
		0, 1, -1,
		0, 0, 0;
	filter15 << 0, 0, 0,
		0, 1, 0,
		-1, 0, 0;
	filter16 << -1, 0, 0,
		0, 0, 0,
		0, 0, 1;
	filter17 << 1 / 16.0, 1 / 16.0, 1 / 16.0,
		2 / 16.0, 6 / 16.0, 2 / 16.0,
		1 / 16.0, 1 / 16.0, 1 / 16.0;
	filter18 << -1, 0, 0,
		0, 2, 0,
		0, 0, -1;
	filter19 << -1, -1, -1,
		-1, 8, -1,
		-1, -1, -1;
	filter20 << 0, 0, 0,
		0, 1, 0,
		0, -1, 0;
	filter21 << 0, 0, 0,
		0, 1, 0, 
		0, -1, 0;
	filter22 << 0, 0, -1,
		0, 0, 0,
		1, 0, 0;
	filter23 << 1 / 16.0, 2 / 16.0, 1 / 16.0,
		1 / 16.0, 6 / 16.0, 1 / 16.0,
		1 / 16.0, 2 / 16.0, 1 / 16.0;
	filter24 << 0, 0, -1,
		0, 2, 0,
		-1, 0, 0;
	filter25 << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0;




	system("pause");

}




							