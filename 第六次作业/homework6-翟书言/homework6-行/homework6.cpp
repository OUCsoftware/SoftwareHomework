// homework6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
#include<stdio.h>
using namespace std;

int main()
{
	char *mulPath = "Mul_large.tif";
	char *panPath = "Pan_large.tif";
	char *fusPath = "Fus.tif";

	GDALAllRegister();

	// 参数
	GDALDataset *poMulDS, *poPanDS, *poFusDS;
	int imgXlen, imgYlen;
	int i, j;
	float *bandR, *bandG, *bandB;
	float *bandI, *bandH, *bandS;
	float *bandP;
	int slice = 256, numY, left;

	// 打开datasets
	poMulDS = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	poPanDS = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);
	imgXlen = poMulDS->GetRasterXSize();
	imgYlen = poMulDS->GetRasterYSize();
	poFusDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		fusPath, imgXlen, imgYlen, 3, GDT_Byte, NULL);

	printf(" ... ... image xlen : %d pixels ... ...\n", imgXlen);
	printf(" ... ... image ylen : %d pixels ... ...\n", imgYlen);
	printf(" ... ... Mul image band number : %d ...\n", poMulDS->GetRasterCount());
	printf(" ... ... Pan image band number : %d ...\n", poPanDS->GetRasterCount());

	// allocating memory
	bandR = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandG = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandB = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandP = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandI = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandH = (float*)CPLMalloc(imgXlen * slice * sizeof(float));
	bandS = (float*)CPLMalloc(imgXlen * slice * sizeof(float));

	//求出分块数量和剩余行数
	numY = imgYlen / slice;
	left = imgYlen - numY * slice;

	for (j = 0; j < numY; j++)
	{
		poMulDS->GetRasterBand(1)->RasterIO(GF_Read, 0, j*slice, imgXlen, slice,
			bandR, imgXlen, slice, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(2)->RasterIO(GF_Read, 0, j*slice, imgXlen, slice,
			bandG, imgXlen, slice, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(3)->RasterIO(GF_Read, 0, j*slice, imgXlen, slice,
			bandB, imgXlen, slice, GDT_Float32, 0, 0);
		poPanDS->GetRasterBand(1)->RasterIO(GF_Read, 0, j*slice, imgXlen, slice,
			bandP, imgXlen, slice, GDT_Float32, 0, 0);

		for (i = 0; i < imgXlen * slice; i++)
		{
			bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
			bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

			bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
			bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
			bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
		}

		poFusDS->GetRasterBand(1)->RasterIO(GF_Write, 0, j*slice, imgXlen, slice,
			bandR, imgXlen, slice, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(2)->RasterIO(GF_Write, 0, j*slice, imgXlen, slice,
			bandG, imgXlen, slice, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(3)->RasterIO(GF_Write, 0, j*slice, imgXlen, slice,
			bandB, imgXlen, slice, GDT_Float32, 0, 0);

		printf("%d in %d\n", j,numY);
	}

	if (left > 0)
	{
		poMulDS->GetRasterBand(1)->RasterIO(GF_Read, 0, imgYlen - left, imgXlen, left,
			bandR, imgXlen, left, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(2)->RasterIO(GF_Read, 0, imgYlen - left, imgXlen, left,
			bandG, imgXlen, left, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(3)->RasterIO(GF_Read, 0, imgYlen - left, imgXlen, left,
			bandB, imgXlen, left, GDT_Float32, 0, 0);
		poPanDS->GetRasterBand(1)->RasterIO(GF_Read, 0, imgYlen - left, imgXlen, left,
			bandP, imgXlen, left, GDT_Float32, 0, 0);

		for (i = 0; i < imgXlen * (left-1); i++)
		{
			bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
			bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

			bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
			bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
			bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
		}

		poFusDS->GetRasterBand(1)->RasterIO(GF_Write, 0, imgYlen - left-1, imgXlen, left,
			bandR, imgXlen, left, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(2)->RasterIO(GF_Write, 0, imgYlen - left-1, imgXlen, left,
			bandG, imgXlen, left, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(3)->RasterIO(GF_Write, 0, imgYlen - left-1, imgXlen, left,
			bandB, imgXlen, left, GDT_Float32, 0, 0);
	}

	//释放
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	GDALClose(poMulDS);
	GDALClose(poPanDS);
	GDALClose(poFusDS);


	return 0;
}