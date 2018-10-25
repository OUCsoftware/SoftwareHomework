// project2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc1;
	GDALDataset* posrc2 ;
	//���ͼ��
	GDALDataset* podes;

	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//ͼ���·��
	char *srcpath1 = "superman.jpg";
	//���ͼ���·��,ֻ�ܱ���Ϊtif���͵�
	char *srcpath2 = "space.jpg";
	char *despath = "perfect.tif";
	//ͼ���ڴ�洢
	GByte *bufftmpR,*bufftmpG,*bufftmpB;
	GByte *bufftmpR1, *bufftmpG1, *bufftmpB1;
	GByte *bufftmpR2, *bufftmpG2, *bufftmpB2;
	//ͼ�񲨶���
	int i, j, bandNum;

	//ע������
	GDALAllRegister();
	//��ͼ��
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2= (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	//��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen= posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();
	//�������ͼ��
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//����ͼ���С���з���ռ�
	bufftmpR1= (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpR2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpB1 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpB2 = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpR = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	bufftmpG = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));	
	bufftmpB = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//���ˣңǣ�ͨ�����صĶ���
	posrc1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc1->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc1->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB1, imgXlen, imgYlen, GDT_Byte, 0, 0);
	//̫��RGBͨ�����صĶ���
	posrc2->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpR2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc2->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpG2, imgXlen, imgYlen, GDT_Byte, 0, 0);
	posrc2->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmpB2, imgXlen, imgYlen, GDT_Byte, 0, 0);

	for (i = 0; i < imgYlen; i++)
		for (j = 0; j < imgXlen; j++)
		{
			if (!(bufftmpR1[i*imgXlen + j] <(GByte)160 && bufftmpR1[i*imgXlen + j] > (GByte)10 && bufftmpG1[i*imgXlen + j] < (GByte)220 && bufftmpG1[i*imgXlen + j] > (GByte)100  && bufftmpB1[i*imgXlen + j] <(GByte)110 && bufftmpB1[i*imgXlen + j] > (GByte)10))
			{
				bufftmpR[i*imgXlen + j] = bufftmpR1[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG1[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB1[i*imgXlen + j];
			}
			else
			{

				bufftmpR[i*imgXlen + j] = bufftmpR2[i*imgXlen + j];
				bufftmpG[i*imgXlen + j] = bufftmpG2[i*imgXlen + j];
				bufftmpB[i*imgXlen + j] = bufftmpB2[i*imgXlen + j];
			}
		}
			

	//д��ͼ��
	podes->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	podes->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	podes->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);
    //�����ڴ�
	CPLFree(bufftmpR);
	CPLFree(bufftmpG);
	CPLFree(bufftmpB);
	CPLFree(bufftmpG1);
	CPLFree(bufftmpR1);
	CPLFree(bufftmpB1);
	CPLFree(bufftmpR2);
	CPLFree(bufftmpG2);	
	CPLFree(bufftmpB2);
	//�ر�dataset
	GDALClose(posrc1);
	GDALClose(posrc2);
	GDALClose(podes);

	system("PAUSE");
	return 0;
}

