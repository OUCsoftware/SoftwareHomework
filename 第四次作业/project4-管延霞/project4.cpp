// project2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* posrc;
	//输出图像
	GDALDataset* podes1, *podes2, *podes3, *podes4, *podes5, *podes6;
	double matirx(double a[3][3], double b[3][3]);
	double matirx(double a[5][5], double b[5][5]);
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//图像的路径

	//输出图像的路径,只能保存为tif类型的
	char *despath1 = "1.tif";
    char *despath2 = "2.tif";
	char *despath3 = "3.tif";
	char *despath4 = "4.tif";
	char *despath5 = "5.tif";
	char *despath6 = "6.tif";
	//图像内存存储
	GByte *bufftmp[3], *bufftmp1[6][3];
	//图像波段数
	int i, bandNum, m = 0, n = 0, result[6];
	//注册驱动
	GDALAllRegister();
	//打开图像
	char *srcpath = "linda.jpg";

	posrc = (GDALDataset*)GDALOpenShared(srcpath, GA_ReadOnly);
	//获取图像的宽度，高度和波段数
	imgXlen = posrc->GetRasterXSize();
	imgYlen = posrc->GetRasterYSize();
	bandNum = posrc->GetRasterCount();
	//创建输出图像；
	podes1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	podes2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	podes3 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath3, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	podes4 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath4, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	podes5 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath5, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	podes6 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath6, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	
	//---matrix1
	double  matrix1[3][3] = { { 0,1,0 },{ 1,1,1 },{ 0,1,0 } };
	double  matrix2[5][5] = { { 1,0,0,0,0 },{ 0,1,0,0,0 },{ 0,0,1,0,0 },{ 0,0,0,1,0 },{ 0,0,0,0,1 } };
	double  matrix3[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };
	double  matrix4[3][3] = { { -1,-1,-1 },{ -1,9,-1 },{ -1,-1,-1 } };
	double  matrix5[3][3] = { { -1,-1,0 },{ -1,0,1 },{ 0,1,1 } };
	double  matrix6[5][5] = { { 0.0120,0.1253,0.2736,0.1253,0.0120 },{ 0.1253,1.3054,2.8514,1.3054,0.1253 },
	{ 0.2736,2.8514,6.2279,2.8514,0.2736 },{ 0.1253,1.3054,2.8514,1.3054,0.1253 },{ 0.0120,0.1253,0.2736,0.1253,0.0120 } };
	// 根据图像的宽度和高度分配内存
	for (i = 0; i <bandNum; i++)
	{
		bufftmp[i] = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
		for(int j=0;j<6;j++)
		bufftmp1[j][i] = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	}
	double example1[3][3];
	double example2[5][5];

	for (i = 0; i < bandNum; i++)
	{
		posrc->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bufftmp[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		for (m = 0; m < imgYlen; m++)
		{

			for (n = 0; n < imgXlen; n++)
			{
				if (m >= 1 && m <= imgYlen - 2 && n >= 1 && n <= imgXlen - 2)
				{
					for (int l1 = 0; l1 < 3; l1++)
					{
						for (int l2 = 0; l2<3; l2++)
							example1[l1][l2] = bufftmp[i][(m + (l1 - 1))*imgXlen + n + (l2 - 1)];
					}
					result[0] = (int)(0.2*matirx(example1, matrix1));
					result[2] = (int)(matirx(example1, matrix3));
					result[3] = (int)(matirx(example1, matrix4));
					result[4] = (int)(matirx(example1, matrix5));
				}
				else
				{
					result[0] = 0;
					result[2] = 0;
					result[3] = 0;
					result[4] = 0;
				}
				if ( m>=2 && m<=imgYlen-3 && n>=2 && n <= imgXlen - 3)
				{
					for (int l1 = 0; l1 < 5; l1++)
					{
						for(int l2=0;l2<5;l2++)
						example2[l1][l2]=bufftmp[i][(m +(l1-2))*imgXlen + n +(l2-2)];
					}
					result[1] = (int)(0.2*matirx(example2, matrix2));
					result[5] = (int)(matirx(example2, matrix6)/25);
				}
				else
				{
					result[1] = 0;
					result[5] = 0;
				}
				for (int l3 = 0; l3 < 6; l3++)
				{
					if (result[l3] < 0)    result[l3] = 0;
					else if (result[l3] > 255) result[l3] = 255;
					bufftmp1[l3][i][m*imgXlen + n] = (GByte)result[l3];
				}
			}
		}
		podes1->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[0][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes2->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[1][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes3->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[2][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes4->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[3][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes5->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[4][i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes6->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, bufftmp1[5][i], imgXlen, imgYlen, GDT_Byte, 0, 0);

	}

	//清理内存

	CPLFree(bufftmp[0]);
	CPLFree(bufftmp[1]);
	CPLFree(bufftmp[2]);
	for ( i = 0; i < 6; i++)
	{
		CPLFree(bufftmp1[i][0]);
		CPLFree(bufftmp1[i][1]);
		CPLFree(bufftmp1[i][2]);
	}

	//关闭dataset
	GDALClose(posrc);
	GDALClose(podes1);
	GDALClose(podes2);
	GDALClose(podes3);
	GDALClose(podes4);
	GDALClose(podes5);
	GDALClose(podes6);
	system("PAUSE");
	return 0;
}
double matirx(double a[5][5], double b[5][5])
{
	double result = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
				result += a[i][j] * b[i][j];
		}
	}
	return result;
}
double matirx(double a[3][3], double b[3][3])
{
	double result = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
				result += a[i][j] * b[i][j];
		}
	}
	return result;
}
