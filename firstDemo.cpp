// firstDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;


int main()
{
	//����ͼ��
	GDALDataset* posrc;
	//���ͼ��
	GDALDataset* podes;

	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//ͼ���·��
	char *srcpath = "trees.jpg";
	//���ͼ���·��,ֻ�ܱ���Ϊtif���͵�
	char *despath = "res,tif";
	//ͼ���ڴ�洢
	GByte *buffTmp;
	//ͼ�񲨶���
	int i, bandNum;

	//ע������
	GDALAllRegister();
	//��ͼ��
	posrc = (GDALDataset*)GDALOpenShared(srcpath, GA_ReadOnly);

	//��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen = posrc->GetRasterXSize();
	imgYlen = posrc->GetRasterYSize();
	bandNum = posrc->GetRasterCount();
	cout << "Image X Length: " << imgXlen << endl;
	cout << "Image Y Length: " << imgYlen << endl;
	cout << "Band number:" << bandNum << endl;

	//����ͼ���С���з���ռ�
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//�������ͼ��
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//һ�������ε����룬Ȼ��һ�������ε����
	for (i = 0; i < bandNum; i++)
	{
		posrc->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("... ... band %d processing ... ...\n", i);
	}
	//����ڴ�
	CPLFree(buffTmp);
	//�ر�dataset
	GDALClose(posrc);
	GDALClose(podes);

	system("PAUSE");
    return 0;
}

