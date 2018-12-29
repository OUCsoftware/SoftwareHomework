// homework6-2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	char* mulPath = "Mul_large.tif";
	char* panPath = "Pan_large.tif";
	char* fusPath = "Fus.tif";

	GDALAllRegister();

	//参数声明
	GDALDataset *poMulDS, *poPanDS, *poFusDS;
	int imgXlen, imgYlen;
	int i, m, n;
	float *bandR, *bandG, *bandB;
	float *bandI, *bandH, *bandS;
	float *bandP;
	int slice = 256;
	int numx, numy, fragx, fragy;

	//打开datasets
	poMulDS = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	poPanDS = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);
	imgXlen = poMulDS->GetRasterXSize();
	imgYlen = poMulDS->GetRasterYSize();
	poFusDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		fusPath, imgXlen, imgYlen, 3, GDT_Byte, NULL);


	printf(" ... ... image xlen : %d pixels ... ...\n", imgXlen);
	printf(" ... ... image ylen : %d pixles ... ...\n", imgYlen);
	printf(" ... ... Mul image band number : %d ...\n", poMulDS->GetRasterCount());
	printf(" ... ... Pan image band number : %d ...\n", poPanDS->GetRasterCount());

	//开辟空间
	bandR = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandG = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandB = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandP = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandI = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandH = (float*)CPLMalloc(slice * slice * sizeof(float));
	bandS = (float*)CPLMalloc(slice * slice * sizeof(float));

	numx = imgXlen / slice;
	numy = imgYlen / slice;
	fragx = imgXlen - slice*numx;
	fragy = imgYlen - slice*numy;

	for (n = 0; n < numy; n++)
	{
		for (m = 0; m < numx; m++)
		{
			poMulDS->GetRasterBand(1)->RasterIO(GF_Read, slice * m, slice * n, slice, slice,
				bandR, slice, slice, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(2)->RasterIO(GF_Read, slice * m, slice * n, slice, slice,
				bandG, slice, slice, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(3)->RasterIO(GF_Read, slice * m, slice * n, slice, slice,
				bandB, slice, slice, GDT_Float32, 0, 0);
			poPanDS->GetRasterBand(1)->RasterIO(GF_Read, slice * m, slice * n, slice, slice,
				bandP, slice, slice, GDT_Float32, 0, 0);

			for (i = 0; i < slice * slice; i++)
			{
				bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
				bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

				bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
				bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
				bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
			}

			poFusDS->GetRasterBand(1)->RasterIO(GF_Write, slice * m, slice * n, slice, slice,
				bandR, slice, slice, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(2)->RasterIO(GF_Write, slice * m, slice * n, slice, slice,
				bandG, slice, slice, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(3)->RasterIO(GF_Write, slice * m, slice * n, slice, slice,
				bandB, slice, slice, GDT_Float32, 0, 0);
		}
		
		if (fragx > 0)
		{
			poMulDS->GetRasterBand(1)->RasterIO(GF_Read, slice * numx, slice * n, fragx, slice,
				bandR, fragx, slice, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(2)->RasterIO(GF_Read, slice * numx, slice * n, fragx, slice,
				bandG, fragx, slice, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(3)->RasterIO(GF_Read, slice * numx, slice * n, fragx, slice,
				bandB, fragx, slice, GDT_Float32, 0, 0);
			poPanDS->GetRasterBand(1)->RasterIO(GF_Read, slice * numx, slice * n, fragx, slice,
				bandP, fragx, slice, GDT_Float32, 0, 0);

			for (i = 0; i < slice * (fragx - 1); i++)
			{
				bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
				bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

				bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
				bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
				bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
			}

			poFusDS->GetRasterBand(1)->RasterIO(GF_Write, slice * numx, slice * n, fragx, slice,
				bandR, fragx, slice, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(2)->RasterIO(GF_Write, slice * numx, slice * n, fragx, slice,
				bandG, fragx, slice, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(3)->RasterIO(GF_Write, slice * numx, slice * n, fragx, slice,
				bandB, fragx, slice, GDT_Float32, 0, 0);
		}
		printf("%d in %d, %d in %d\n", n, numy, m, numx);
	}

	if (fragy > 0)
	{
		for (m = 0; m < numx; m++)
		{
			poMulDS->GetRasterBand(1)->RasterIO(GF_Read, slice * m, slice * numy, slice, fragy,
				bandR, slice, fragy, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(2)->RasterIO(GF_Read, slice * m, slice * numy, slice, fragy,
				bandG, slice, fragy, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(3)->RasterIO(GF_Read, slice * m, slice * numy, slice, fragy,
				bandB, slice, fragy, GDT_Float32, 0, 0);
			poPanDS->GetRasterBand(1)->RasterIO(GF_Read, slice * m, slice * numy, slice, fragy,
				bandP, slice, fragy, GDT_Float32, 0, 0);

			for (i = 0; i < slice * (fragy - 1); i++)
			{
				bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
				bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

				bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
				bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
				bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
			}

			poFusDS->GetRasterBand(1)->RasterIO(GF_Write, slice * m, slice * numy, slice, fragy,
				bandR, slice, fragy, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(2)->RasterIO(GF_Write, slice * m, slice * numy, slice, fragy,
				bandG, slice, fragy, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(3)->RasterIO(GF_Write, slice * m, slice * numy, slice, fragy,
				bandB, slice, fragy, GDT_Float32, 0, 0);
		}

		if (fragx > 0)
		{
			poMulDS->GetRasterBand(1)->RasterIO(GF_Read, slice * numx, slice * numy, fragx, fragy,
				bandR, fragx, fragy, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(2)->RasterIO(GF_Read, slice * numx, slice * numy, fragx, fragy,
				bandG, fragx, fragy, GDT_Float32, 0, 0);
			poMulDS->GetRasterBand(3)->RasterIO(GF_Read, slice * numx, slice * numy, fragx, fragy,
				bandB, fragx, fragy, GDT_Float32, 0, 0);
			poPanDS->GetRasterBand(1)->RasterIO(GF_Read, slice * numx, slice * numy, fragx, fragy,
				bandP, fragx, fragy, GDT_Float32, 0, 0);

			for (i = 0; i < (fragy - 1) * (fragx - 1); i++)
			{
				bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
				bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

				bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
				bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
				bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
			}

			poFusDS->GetRasterBand(1)->RasterIO(GF_Write, slice * numx, slice * numy, fragx, fragy,
				bandR, fragx, fragy, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(2)->RasterIO(GF_Write, slice * numx, slice * numy, fragx, fragy,
				bandG, fragx, fragy, GDT_Float32, 0, 0);
			poFusDS->GetRasterBand(3)->RasterIO(GF_Write, slice * numx, slice * numy, fragx, fragy,
				bandB, fragx, fragy, GDT_Float32, 0, 0);
		}
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

