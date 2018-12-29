
// project2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc1, *posrc2;
	// 输出图像
	GDALDataset* podes1;
	//RGB == > IHS 正变换矩阵
	/*double tran1[3][3] = {{1 / 3, 1 / 3, 1 / 3},
	{-sqrt(2) / 6, -sqrt(2) / 6, sqrt(2) / 3},
	{1 / sqrt(2), -1 / sqrt(2), 0}};*/
	// IHS == > RGB 逆变换矩阵
	/*double tran2[3][3] = {{1, -1 / sqrt(2), 1 / sqrt(2)},
	{1, -1 / sqrt(2), -1 / sqrt(2)},
	{1, sqrt(2), 0} };*/
	// 图像的宽度和高度
	int imgXlen, imgYlen, bandNum;
	// 图像的路径
	int i, j;
	// 输出图像的路径, 只能保存为 tif 类型的
	char *despath = "2.tif";

	// 图像内存存储
	float *bufftmp[3], *bufftmp2, *bufftmpH, *bufftmpI, *bufftmpS;
	// 图像波段数
	// 定义动态的二维数组

	// 注册驱动
	GDALAllRegister();
	// 打开图像
	char *srcpath1 = "MUL_large.tif";
	char *srcpath2 = "PAN_large.tif";
	posrc1 = (GDALDataset*)GDALOpenShared(srcpath1, GA_ReadOnly);
	posrc2 = (GDALDataset*)GDALOpenShared(srcpath2, GA_ReadOnly);
	// 获取图像的宽度，高度和波段数
	imgXlen = posrc1->GetRasterXSize();
	imgYlen = posrc1->GetRasterYSize();
	bandNum = posrc1->GetRasterCount();

	// 创建输出图像；
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

	// 清理内存

	for (j = 0; j<3; j++)
		CPLFree(bufftmp[j]);
	CPLFree(bufftmp2);
	CPLFree(bufftmpH);
	CPLFree(bufftmpI);
	CPLFree(bufftmpS);

	// 关闭 dataset
	GDALClose(posrc1);
	GDALClose(podes1);
	GDALClose(posrc2);
	system("PAUSE");
	return 0;
}