// homework6.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
#include<stdio.h>
using namespace std;

int strategySquare();
int strategyRectangle();

int main()
{
	//strategySquare();
	strategyRectangle();
    return 0;
}

//��һ�ַ�ʽ��ʹ��256 * 256 ��С�Ŀ�
int strategySquare()
{
	GDALDataset* posrc1, *posrc2;
	//���ͼ��
	GDALDataset* podes1;
	//RGB == > IHS ���任����
	/*double tran1[3][3] = {{1 / 3, 1 / 3, 1 / 3},
	{-sqrt(2) / 6, -sqrt(2) / 6, sqrt(2) / 3},
	{1 / sqrt(2), -1 / sqrt(2), 0}};*/
	// IHS == > RGB ��任����
	/*double tran2[3][3] = { {1, -1 / sqrt(2), 1 / sqrt(2)},
	{1, -1 / sqrt(2), -1 / sqrt(2)},
	{1, sqrt(2), 0} };*/
	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen, bandNum;
	//ͼ���·��
	//���ͼ���·��,ֻ�ܱ���Ϊtif���͵�
	char *despath = "square.tif";

	//ͼ���ڴ�洢
	float *bufftmp[3], *bufftmp2, *bufftmpH, *bufftmpI, *bufftmpS;
	
	int m = 1, n = 1, i;

	//ע������
	GDALAllRegister();
	//��ͼ��
	char *srcpath1 = "American_MUL.bmp";
	char *srcpath2 = "American_PAN.bmp";
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	//��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen = posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();

	//�������ͼ��
	podes1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i <bandNum; i++)
	{
		bufftmp[i] = (float*)CPLMalloc(256 * 256 * sizeof(float));
	}
	bufftmpH = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmpI = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmpS = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmp2 = (float*)CPLMalloc(256 * 256 * sizeof(float));
	while (n < imgXlen / 256 + 1)
	{
		while (m < imgYlen / 256 + 1)
		{

			for (i = 0; i < bandNum; i++)
			{
				posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}

			posrc2->GetRasterBand(1)->RasterIO(GF_Read, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp2, 256, 256, GDT_Float32, 0, 0);
			for (i = 0; i < 256 * 256; i++)
			{
				bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
				bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

				bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
				bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
				bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

			}

			for (i = 0; i < 3; i++)
			{
				podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 256 * (m - 1), 256 * (n - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}
			m++;
		}
		n++;
	}
	for (i = 0; i<3; i++)
		CPLFree(bufftmp[i]);
	CPLFree(bufftmp2);
	CPLFree(bufftmpH);
	CPLFree(bufftmpI);
	CPLFree(bufftmpS);

	//�ر�dataset
	GDALClose(posrc1);
	GDALClose(podes1);
	GDALClose(posrc2);
	system("PAUSE");
	return 0;
}

//�ڶ��ַ�ʽ��ÿ��Ŀ��Ϊͼ���ȣ��߶�Ϊ256����
int strategyRectangle()
{
	GDALDataset* posrc1, *posrc2;
	//���ͼ��
	GDALDataset* podes1;
	//RGB == > IHS ���任����
	/*double tran1[3][3] = {{1 / 3, 1 / 3, 1 / 3},
	{-sqrt(2) / 6, -sqrt(2) / 6, sqrt(2) / 3},
	{1 / sqrt(2), -1 / sqrt(2), 0}};*/
	// IHS == > RGB ��任����
	/*double tran2[3][3] = { {1, -1 / sqrt(2), 1 / sqrt(2)},
	{1, -1 / sqrt(2), -1 / sqrt(2)},
	{1, sqrt(2), 0} };*/
	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen, bandNum;
	//ͼ���·��
	int i, j, l;
	//���ͼ���·��,ֻ�ܱ���Ϊtif���͵�
	char *despath = "rectangle.tif";

	//ͼ���ڴ�洢
	float *bufftmp[3], *bufftmp2, *bufftmpH, *bufftmpI, *bufftmpS;
	//ͼ�񲨶���
	//���嶯̬�Ķ�ά����

	//ע������
	GDALAllRegister();
	//��ͼ��
	char *srcpath1 = "MUL_large.tif";
	char *srcpath2 = "PAN_large.tif";
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	//��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen = posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();

	//�������ͼ��
	podes1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i <bandNum; i++)
	{
		bufftmp[i] = (float*)CPLMalloc(imgYlen * 256 * sizeof(float));
	}
	bufftmpH = (float*)CPLMalloc(imgYlen * 256 * sizeof(float));
	bufftmpI = (float*)CPLMalloc(imgYlen * 256 * sizeof(float));
	bufftmpS = (float*)CPLMalloc(imgYlen * 256 * sizeof(float));
	bufftmp2 = (float*)CPLMalloc(imgYlen * 256 * sizeof(float));
	int n = 1;
	while (n < imgXlen / 256 + 1)
	{
		for (i = 0; i < bandNum; i++)
		{
			posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 256 * (n - 1), 0, 256, imgYlen, bufftmp[i], 256, imgYlen, GDT_Float32, 0, 0);
		}

		posrc2->GetRasterBand(1)->RasterIO(GF_Read, 256 * (n - 1), 0, 256, imgYlen, bufftmp2, 256, imgYlen, GDT_Float32, 0, 0);
		for (i = 0; i < imgYlen * 256; i++)
		{
			bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
			bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

			bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

		}

		for (i = 0; i < 3; i++)
		{
			podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 256 * (n - 1), 0, 256, imgYlen, bufftmp[i], 256, imgYlen, GDT_Float32, 0, 0);
		}
		n++;

	}

	//�����ڴ�

	for (j = 0; j<3; j++)
		CPLFree(bufftmp[j]);
	CPLFree(bufftmp2);
	CPLFree(bufftmpH);
	CPLFree(bufftmpI);
	CPLFree(bufftmpS);

	//�ر�dataset
	GDALClose(posrc1);
	GDALClose(podes1);
	GDALClose(posrc2);
	system("PAUSE");
	return 0;
}