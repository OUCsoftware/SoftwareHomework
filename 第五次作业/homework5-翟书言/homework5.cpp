// homework5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int posMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen);
int negMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen);

int main()
{
	GDALDataset* poSrcMul;
	GDALDataset* poSrcPan;
	GDALDataset* poDstDS;
	float* mulTmp[3], *panTmp, *dstTmp[3], *panDstTmp[3];
	int mulXlen, mulYlen, panXlen, panYlen, mulBandNum, panBandNum;
	char* srcMul = "American_MUL.bmp";
	char* srcPan = "American_PAN.bmp";
	char* dstPath = "1.tif";
	int i, j;
	GDALAllRegister();

	poSrcMul = (GDALDataset*)GDALOpenShared(srcMul, GA_ReadOnly);
	poSrcPan = (GDALDataset*)GDALOpenShared(srcPan, GA_ReadOnly);
	mulXlen = poSrcMul->GetRasterXSize();
	mulYlen = poSrcMul->GetRasterYSize();
	panXlen = poSrcPan->GetRasterXSize();
	panYlen = poSrcPan->GetRasterYSize();
	mulBandNum = poSrcMul->GetRasterCount();
	panBandNum = poSrcPan->GetRasterCount();
	for (i = 0; i < 3; i++)
	{
		mulTmp[i] = (float*)CPLMalloc(mulXlen*mulYlen * sizeof(float));
		dstTmp[i] = (float*)CPLMalloc(mulXlen*mulYlen * sizeof(float));
		panDstTmp[i] = (float*)CPLMalloc(mulXlen*mulYlen * sizeof(float));
	}
	panTmp = (float*)CPLMalloc(mulXlen*mulYlen * sizeof(float));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, mulXlen, mulYlen, mulBandNum, GDT_Byte, NULL);

	// RGB ==> IHS 正变换
	for (i = 0; i < 3; i++)
	{
		poSrcMul->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, mulXlen, mulYlen, mulTmp[i], mulXlen, mulYlen, GDT_Float32, 0, 0);
		posMatrix(mulTmp, panDstTmp, mulXlen, mulYlen);
	}
	poSrcPan->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, mulXlen, mulYlen, panTmp, mulXlen, mulYlen, GDT_Float32, 0, 0);

	//用 PAN 替换 I 分量
	for(i=0;i<mulXlen;i++)
		for (j = 0; j < mulYlen; j++)
		{
			panDstTmp[0][i*mulYlen + j] = panTmp[i*mulYlen + j];
		}
	
	// IHS ==> RGB 逆变换
	negMatrix(panDstTmp, dstTmp, mulXlen, mulYlen);

	//写入目标地址
	for (i = 0; i < 3; i++)
	{
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, mulXlen, mulYlen, dstTmp[i], mulXlen, mulYlen, GDT_Float32, 0, 0);
	}

	for (i = 0; i < 3; i++)
	{
		CPLFree(panDstTmp[i]);
		CPLFree(dstTmp[i]);
		CPLFree(mulTmp[i]);
	}
	CPLFree(panTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcPan);
	GDALClose(poSrcMul);
	system("PAUSE");
    return 0;
}

//正变换函数
int posMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen)  
{
	int i, j;

	float tran[3][3] = { {1.0f / 3.0f,        1.0f / 3.0f,        1.0f / 3.0f      },
						  {-sqrt(2.0f) / 6.0f, -sqrt(2.0f) / 6.0f, sqrt(2.0f) / 3.0f},
						  {1.0f / sqrt(2.0f),  -1.0f / sqrt(2.0f), 0          } };

	for (i = 1; i < imgXlen - 1; i++)
		for (j = 1; j < imgYlen - 1; j++)
		{
			outTmp[0][i*imgXlen + j] = tran[0][0] * inTmp[0][i*imgXlen + j] + tran[0][1] * inTmp[1][i*imgXlen + j] + tran[0][2] * inTmp[2][i*imgXlen + j];
			outTmp[1][i*imgXlen + j] = tran[1][0] * inTmp[0][i*imgXlen + j] + tran[1][1] * inTmp[1][i*imgXlen + j] + tran[1][2] * inTmp[2][i*imgXlen + j];
			outTmp[2][i*imgXlen + j] = tran[2][0] * inTmp[0][i*imgXlen + j] + tran[2][1] * inTmp[1][i*imgXlen + j] + tran[2][2] * inTmp[2][i*imgXlen + j];
		}
	return 0;
}

//逆变换函数
int negMatrix(float*inTmp[3], float*outTmp[3], int imgXlen, int imgYlen)   
{
	int i, j;
	float tran[3][3] = { {1.0f, -1.0f / sqrt(2.0f), 1.0f / sqrt(2.0f)},
	{1.0f, -1.0f / sqrt(2.0f), -1.0f / sqrt(2.0f) },
	{	1.0f, sqrt(2.0f), 0} };
	for (i = 1; i < imgXlen - 1; i++)
		for (j = 1; j < imgYlen - 1; j++)
		{
			outTmp[0][i*imgXlen + j] = tran[0][0] * inTmp[0][i*imgXlen + j] + tran[0][1] * inTmp[1][i*imgXlen + j] + tran[0][2] * inTmp[2][i*imgXlen + j];
			outTmp[1][i*imgXlen + j] = tran[1][0] * inTmp[0][i*imgXlen + j] + tran[1][1] * inTmp[1][i*imgXlen + j] + tran[1][2] * inTmp[2][i*imgXlen + j];
			outTmp[2][i*imgXlen + j] = tran[2][0] * inTmp[0][i*imgXlen + j] + tran[2][1] * inTmp[1][i*imgXlen + j] + tran[2][2] * inTmp[2][i*imgXlen + j];
		}
	return 0;
}