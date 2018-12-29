
// project2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc1, *posrc2;
	// ���ͼ��
	GDALDataset* podes1;
	//RGB == > IHS ���任����
	/*double tran1[3][3] = {{1 / 3, 1 / 3, 1 / 3},
	{-sqrt(2) / 6, -sqrt(2) / 6, sqrt(2) / 3},
	{1 / sqrt(2), -1 / sqrt(2), 0}};*/
	// IHS == > RGB ��任����
	/*double tran2[3][3] = {{1, -1 / sqrt(2), 1 / sqrt(2)},
	{1, -1 / sqrt(2), -1 / sqrt(2)},
	{1, sqrt(2), 0} };*/
	// ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen, bandNum;
	// ͼ���·��
	int i, j;
	// ���ͼ���·��, ֻ�ܱ���Ϊ tif ���͵�
	char *despath = "2.tif";

	// ͼ���ڴ�洢
	float *bufftmp[3], *bufftmp2, *bufftmpH, *bufftmpI, *bufftmpS;
	// ͼ�񲨶���
	// ���嶯̬�Ķ�ά����

	// ע������
	GDALAllRegister();
	// ��ͼ��
	char *srcpath1 = "MUL_large.tif";
	char *srcpath2 = "PAN_large.tif";
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	// ��ȡͼ��Ŀ�ȣ��߶ȺͲ�����
	imgXlen = posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();

	// �������ͼ��
	podes1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i <bandNum; i++)
	{
		bufftmp[i] = (float*)CPLMalloc(256 * 256 * sizeof(float));
	}
	bufftmpH = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmpI = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmpS = (float*)CPLMalloc(256 * 256 * sizeof(float));
	bufftmp2 = (float*)CPLMalloc(256 * 256 * sizeof(float));
	int m1 = imgYlen / 256 + 1;
	int n1 = 1;
	int m2 = imgXlen / 256 + 1;
	int n2 = 1;
	while (n1 <m1)
	{
		n2 = 1;
		while (n2 < m2)
		{
			for (i = 0; i <bandNum; i++)
			{
				posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read, 256 * (n2 - 1), 256 * (n1 - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}

			posrc2->GetRasterBand(1)->RasterIO(GF_Read, 256 * (n2 - 1), 256 * (n1 - 1), 256, 256, bufftmp2, 256, 256, GDT_Float32, 0, 0);
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
				podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 256 * (n2 - 1), 256 * (n1 - 1), 256, 256, bufftmp[i], 256, 256, GDT_Float32, 0, 0);
			}
			n2++;
		}

		n1++;

	}
	int mode= imgYlen % 256;
	int start = imgYlen - mode-1;
	n2= 1;
	while (n2 < m2)
	{
		for (i = 0; i <bandNum; i++)
		{
			posrc1->GetRasterBand(i + 1)->RasterIO(GF_Read,  256 * (n2 - 1),start,  256,mode, bufftmp[i],  256,mode, GDT_Float32, 0, 0);
		}

		posrc2->GetRasterBand(1)->RasterIO(GF_Read,  256 * (n2 - 1),start, 256, mode, bufftmp2,  256,mode, GDT_Float32, 0, 0);
		for (i = 0; i <  256*(mode-1) ; i++)
		{
			bufftmpH[i] = -sqrt(2.0f) / 6.0f* bufftmp[0][i] - sqrt(2.0f) / 6.0f*bufftmp[1][i] + sqrt(2.0f) / 3.0f*bufftmp[2][i];
			bufftmpS[i] = 1.0f / sqrt(2.0f)* bufftmp[0][i] - 1 / sqrt(2.0f)*bufftmp[1][i];

			bufftmp[0][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] + 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[1][i] = bufftmp2[i] - 1.0f / sqrt(2.0f)*bufftmpH[i] - 1.0f / sqrt(2.0f)*bufftmpS[i];
			bufftmp[2][i] = bufftmp2[i] + sqrt(2.0f)*bufftmpH[i];

		}

		for (i = 0; i < 3; i++)
		{
			podes1->GetRasterBand(i + 1)->RasterIO(GF_Write,  256 * (n2 - 1),start,  256,mode, bufftmp[i],  256,mode, GDT_Float32, 0, 0);
		}
		n2++;
	}

	cout << "123" << endl;

	// �����ڴ�

	for (j = 0; j<3; j++)
		CPLFree(bufftmp[j]);
	CPLFree(bufftmp2);
	CPLFree(bufftmpH);
	CPLFree(bufftmpI);
	CPLFree(bufftmpS);

	// �ر� dataset
	GDALClose(posrc1);
	GDALClose(podes1);
	GDALClose(posrc2);
	system("PAUSE");
	return 0;
}