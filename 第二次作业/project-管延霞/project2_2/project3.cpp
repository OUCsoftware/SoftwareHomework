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
	GByte *buffTmp,*bufftmp1,*bufftmp2;
	//ͼ�񲨶���
	int i, bandNum;
	//������ʼλ������
	int startx1 = 300;
	int starty1 = 300;

	//����Ŀ�Ⱥ͸߶�
	int tmpx1len = 100;
	int tmpy1len = 50;
	//������ʼλ��2����
	int startx2 = 500;
	int starty2 = 500;

	//����Ŀ�Ⱥ͸߶�
	int tmpx2len = 50;
	int tmpy2len = 100;
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
	bufftmp1 = (GByte*)CPLMalloc(tmpx1len*tmpy1len * sizeof(GByte));
	bufftmp2 = (GByte*)CPLMalloc(tmpx2len*tmpy2len * sizeof(GByte));
	//��ȡ��ɫͨ��������bufftmp��
	
	
	// ��������, ��������Ϊ 255
	for (int k = 0; k < 3; k++)
	{
		posrc->GetRasterBand(k+1)->RasterIO(GF_Read, startx1, starty1, tmpx1len, tmpy1len, bufftmp1, tmpx1len, tmpy1len, GDT_Byte, 0, 0);
		for (int j = 0; j <tmpy1len; j++)
			{
				for (int i = 0; i <tmpx1len; i++)
				{
					bufftmp1[j*tmpx1len + i] = (GByte)255;
				}
			}

		podes->GetRasterBand(k+1)->RasterIO(GF_Write, startx1, starty1, tmpx1len, tmpy1len, bufftmp1, tmpx1len, tmpy1len, GDT_Byte, 0, 0);
	}
	for (int k = 0; k < 3; k++)
	{
		posrc->GetRasterBand(k+1)->RasterIO(GF_Read, startx2, starty2, tmpx2len, tmpy2len, bufftmp2, tmpx2len, tmpy2len, GDT_Byte, 0, 0);
		for (int j = 0; j <tmpy2len; j++)
		{
			for (int i = 0; i <tmpx2len; i++)
			{
				bufftmp2[j*tmpx2len + i] = (GByte)0;
			}
		}
		podes->GetRasterBand(k+1)->RasterIO(GF_Write, startx2, starty2, tmpx2len, tmpy2len, bufftmp2, tmpx2len, tmpy2len, GDT_Byte, 0, 0);

	}

	CPLFree(buffTmp);
	CPLFree(bufftmp1);
	CPLFree(bufftmp2);
	//�ر�dataset
	GDALClose(posrc);
	GDALClose(podes);


	system("PAUSE");
	return 0;
}

