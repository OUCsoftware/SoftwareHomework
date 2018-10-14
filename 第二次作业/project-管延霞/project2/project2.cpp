// project2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc;
	//���ͼ��
	GDALDataset* podes;

	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//ͼ���·��
	char *srcpath = "qingdao.jpg";
	//���ͼ���·��,ֻ�ܱ���Ϊtif���͵�
	char *despath = "qing.tif";
	//ͼ���ڴ�洢
	GByte *buffTmp,*bufftmp;
	//ͼ�񲨶���
	int i, bandNum;
	//������ʼλ������
	int startx = 100;
	int starty = 100;

	//����Ŀ�Ⱥ͸߶�
	int tmpxlen = 200;
	int tmpylen = 150;

	//ע������
	GDALAllRegister();
	//��ͼ��
	posrc = (GDALDataset*)GDALOpenShared(srcpath, GA_ReadOnly);

	//��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen = posrc->GetRasterXSize();
	imgYlen = posrc->GetRasterYSize();
	bandNum = posrc->GetRasterCount();

	
	//����ͼ���С���з���ռ�
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//�������ͼ��
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//һ�������ε����룬Ȼ��һ�������ε����
	for (i = 0; i < bandNum; i++)
	{
		posrc->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	
	}
	// �����ڴ�
	bufftmp = (GByte*)CPLMalloc(tmpxlen*tmpylen * sizeof(GByte));

	//��ȡ��ɫͨ��������bufftmp��
	posrc->GetRasterBand(1)->RasterIO(GF_Read, startx, starty, tmpxlen, tmpylen, bufftmp, tmpxlen, tmpylen, GDT_Byte, 0, 0);
	// ��������, ��������Ϊ 255
	for (int j = 0; j <tmpylen; j++)
	{
		for (int i = 0; i <tmpxlen; i++)
		{
			bufftmp[j*tmpxlen + i] = (GByte)255;
		}
	}

	podes->GetRasterBand(1)->RasterIO(GF_Write, startx, starty, tmpxlen, tmpylen, bufftmp, tmpxlen, tmpylen, GDT_Byte, 0, 0);

	CPLFree(buffTmp);
	CPLFree(bufftmp);
	//�ر�dataset
	GDALClose(posrc);
	GDALClose(podes);


	system("PAUSE");
	return 0;
}

