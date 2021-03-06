#include "iostream"
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;


int main()

{

	const char* mulPath = "Mul_large.tif";
	const char* panPath = "Pan_large.tif";
	const char* resPath = "ans_1.tif";

	GDALAllRegister();
	GDALDataset *poMulDs, *poPanDs, *poResDs;

	int Xlen, Ylen;

	float *bandR;
	float *bandG;
	float *bandB;
	float *bandI;
	float *bandH;
	float *bandS;
	float *bandP;


	poMulDs = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	poPanDs = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);


	Xlen = poMulDs->GetRasterXSize();
	Ylen = poMulDs->GetRasterYSize();

	//output image
	poResDs = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(resPath, Xlen, Ylen, 3, GDT_Byte, NULL);
	
	cout << " ... ... image Xlen : " << Xlen << " pixels ... ...\n";
	cout << " ... ... image Ylen : " << Ylen << " pixels ... ...\n";
	cout << " ... ... Mul image band number : " << poMulDs->GetRasterCount() << endl;
	cout << " ... ... Pan image band number : " << poPanDs->GetRasterCount() << endl;

	//size of block

	const int BlockXSize = 256;
	const int BlockYSize = 256;

	//allocate space

	bandR = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandG = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandB = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandI = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandH = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandS = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));
	bandP = (float*)CPLMalloc(BlockXSize * BlockYSize * sizeof(float));

	//calculate block count
	int XBlockCount = Xlen / BlockXSize;
	int XBlockLeft = Xlen % BlockXSize;
	int YBlockCount = Ylen / BlockYSize;
	int YBlockLeft = Ylen % BlockYSize;

	for (int y = 0; y < YBlockCount; y++) {

		for (int x = 0; x < XBlockCount; x++) {

			poMulDs->GetRasterBand(1)->RasterIO(GF_Read, x * BlockXSize, y * BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
			poMulDs->GetRasterBand(2)->RasterIO(GF_Read, x * BlockXSize, y * BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
			poMulDs->GetRasterBand(3)->RasterIO(GF_Read, x * BlockXSize, y * BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
			poPanDs->GetRasterBand(1)->RasterIO(GF_Read, x * BlockXSize, y * BlockYSize, BlockXSize, BlockYSize, bandP, BlockXSize, BlockYSize, GDT_Float32, 0, 0);

			for (int i = 0; i < BlockXSize* BlockYSize; i++) {

				//RGB => IHS

				bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
				bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];

				//IHS => RGB

				bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
				bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
				bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];

			}

			poResDs->GetRasterBand(1)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
			poResDs->GetRasterBand(2)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
			poResDs->GetRasterBand(3)->RasterIO(GF_Write, x*BlockXSize, y*BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float32, 0, 0);

		}

	}


	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	//calculate right colunm

	bandR = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandG = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandB = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandI = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandH = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandS = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));
	bandP = (float*)CPLMalloc(BlockYSize * XBlockLeft * sizeof(float));

	for (int y = 0; y < YBlockCount; y++) {

		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandR, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandG, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandB, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandP, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);

		for (int i = 0; i < XBlockLeft* BlockYSize; i++) {

			//RGB => IHS

			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];

			//IHS => RGB

			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];

		}

		poResDs->GetRasterBand(1)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandR, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(2)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandG, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(3)->RasterIO(GF_Write, XBlockCount*BlockXSize, y*BlockYSize, XBlockLeft, BlockYSize, bandB, XBlockLeft, BlockYSize, GDT_Float32, 0, 0);

	}

	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	//calculate bottom row

	bandR = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandG = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandB = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandI = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandH = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandS = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));
	bandP = (float*)CPLMalloc(BlockXSize * YBlockLeft * sizeof(float));

	for (int x = 0; x < XBlockCount; x++) {

		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, x * BlockXSize, YBlockCount * BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, x * BlockXSize, YBlockCount * BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, x * BlockXSize, YBlockCount * BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, x * BlockXSize, YBlockCount * BlockYSize, BlockXSize, YBlockLeft, bandP, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);

		for (int i = 0; i < BlockXSize* YBlockLeft; i++) {


			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];

			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];

		}

		poResDs->GetRasterBand(1)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(2)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(3)->RasterIO(GF_Write, x*BlockXSize, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);

	}


	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	GDALClose(poMulDs);
	GDALClose(poPanDs);
	GDALClose(poResDs);

	cout << "END";
	return 0;
}