#include"stdafx.h"
#include <iostream>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

#define FILTERNUM 6

int filter1(float*inTmp, float *outTmp, int imgXlen, int imgYlen);
int filter2(float*inTmp, float *outTmp, int imgXlen, int imgYlen);
int filter3(float*inTmp, float *outTmp, int imgXlen, int imgYlen);
int filter4(float*inTmp, float *outTmp, int imgXlen, int imgYlen);
int filter5(float*inTmp, float *outTmp, int imgXlen, int imgYlen);
int filter6(float*inTmp, float *outTmp, int imgXlen, int imgYlen);

int main()
{
	GDALDataset* poSrcDS;           //输入图像
	GDALDataset* poDstDS[FILTERNUM];//输出图像
	float* inTmp;					//图像内存存储
	float* outTmp;
	int imgXlen, imgYlen;			//图像的宽度和高度
	char* srcPath = "model.jpg";	//输入图像路径
	char* dstPath[FILTERNUM];		//输出图像路径
	int i, j, bandNum;			    //图像波段数
	dstPath[0] = "res1.tif";
	dstPath[1] = "res2.tif";
	dstPath[2] = "res3.tif";
	dstPath[3] = "res4.tif";
	dstPath[4] = "res5.tif";
	dstPath[5] = "res6.tif";

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽高和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽高分配内存
	inTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	outTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	for (i = 0; i < FILTERNUM; i++)
	{
		poDstDS[i] = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath[i], imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	}
	//波段的输入与输出
	for (j = 0; j < FILTERNUM; j++)
	{
		for (i = 0; i < bandNum; i++)
		{
			poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, inTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
			if (j == 0)
				filter1(inTmp, outTmp, imgXlen, imgYlen);
			else if (j == 1)
			{
				filter2(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 2)
			{
				filter3(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 3)
			{
				filter4(inTmp, outTmp, imgXlen, imgYlen);
			}
			else if (j == 4)
			{
				filter5(inTmp, outTmp, imgXlen, imgYlen);
			}
			else
			{
				filter6(inTmp, outTmp, imgXlen, imgYlen);
			}
			poDstDS[j]->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, outTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
			printf("...... band %d processing ......\n", i);
		}
	}

	//清除内存
	CPLFree(outTmp);
	CPLFree(inTmp);
	//关闭dataset，先打开的后关
	for (i = 0; i < FILTERNUM; i++)
	{
		GDALClose(poDstDS[i]);
	}
	GDALClose(poSrcDS);
	system("PAUSE");
	return 0;
}

int filter1(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i<imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (inTmp[(j - 1)*imgXlen + i] +
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] +
				inTmp[j*imgXlen + i + 1] +
				inTmp[(j + 1)*imgXlen + i]) / 5.0f;
		}
	}
	return 0;
}
int filter2(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 2; j < imgYlen - 2; j++)
	{
		for (i = 2; i < imgXlen - 2; i++)
		{
			outTmp[j*imgXlen + i] = (inTmp[(j - 2)*imgXlen + i - 2] +
				inTmp[(j - 1)*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] +
				inTmp[(j + 1)*imgXlen + i + 1] +
				inTmp[(j + 2)*imgXlen + i + 2]) / 5.0f;
		}
	}
	return 0;
}
int filter3(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i<imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1]
				- inTmp[(j - 1)*imgXlen + i]
				- inTmp[(j - 1)*imgXlen + i + 1]
				- inTmp[j*imgXlen + i - 1]
				+ inTmp[j*imgXlen + i] * 8
				- inTmp[j*imgXlen + i + 1]
				- inTmp[(j + 1)*imgXlen + i - 1]
				- inTmp[(j + 1)*imgXlen + i]
				- inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
int filter4(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j < imgYlen - 1; j++)
	{
		for (i = 1; i < imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1] -
				inTmp[(j - 1)*imgXlen + i] -
				inTmp[(j - 1)*imgXlen + i + 1] -
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i] * 8 -
				inTmp[j*imgXlen + i + 1] -
				inTmp[(j + 1)*imgXlen + i - 1] -
				inTmp[(j + 1)*imgXlen + i] -
				inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
int filter5(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 1; j<imgYlen - 1; j++)
	{
		for (i = 1; i < imgXlen - 1; i++)
		{
			outTmp[j*imgXlen + i] = (-inTmp[(j - 1)*imgXlen + i - 1] -
				inTmp[(j - 1)*imgXlen + i] -
				inTmp[j*imgXlen + i - 1] +
				inTmp[j*imgXlen + i + 1] +
				inTmp[(j + 1)*imgXlen + i] +
				inTmp[(j + 1)*imgXlen + i + 1]);
		}
	}
	return 0;
}
int filter6(float*inTmp, float *outTmp, int imgXlen, int imgYlen)
{
	int i, j;
	for (j = 2; j<imgYlen - 2; j++)
	{
		for (i = 2; i<imgXlen - 2; i++)
		{
			outTmp[j*imgXlen + i] = (0.0120*inTmp[(j - 2)*imgXlen + i - 2] +
				0.1253*inTmp[(j - 2)*imgXlen + i - 1] +
				0.2736*inTmp[(j - 2)*imgXlen + i] +
				0.1253*inTmp[(j - 2)*imgXlen + i + 1] +
				0.0120*inTmp[(j - 2)*imgXlen + i + 2] +
				0.1253*inTmp[(j - 1)*imgXlen + i - 2] +
				1.3054*inTmp[(j - 1)*imgXlen + i - 1] +
				2.8514*inTmp[(j - 1)*imgXlen + i] +
				1.3054*inTmp[(j - 1)*imgXlen + i + 1] +
				0.1253*inTmp[(j - 1)*imgXlen + i + 2] +
				0.2763*inTmp[j*imgXlen + i - 2] +
				2.8514*inTmp[j*imgXlen + i - 1] +
				6.2279*inTmp[j*imgXlen + i] +
				2.8514*inTmp[j*imgXlen + i + 1] +
				0.2763*inTmp[j*imgXlen + i + 2] +
				0.1253*inTmp[(j + 1)*imgXlen + i - 2] +
				1.3054*inTmp[(j + 1)*imgXlen + i - 1] +
				2.8514*inTmp[(j + 1)*imgXlen + i] +
				1.3054*inTmp[(j + 1)*imgXlen + i + 1] +
				0.1253*inTmp[(j + 1)*imgXlen + i + 2] +
				0.0120*inTmp[(j + 2)*imgXlen + i - 2] +
				0.1253*inTmp[(j + 2)*imgXlen + i - 1] +
				0.2736*inTmp[(j + 2)*imgXlen + i] +
				0.1253*inTmp[(j + 2)*imgXlen + i + 1] +
				0.0120*inTmp[(j + 2)*imgXlen + i + 2]) / 25.0f;
		}
	}
	return 0;
}
