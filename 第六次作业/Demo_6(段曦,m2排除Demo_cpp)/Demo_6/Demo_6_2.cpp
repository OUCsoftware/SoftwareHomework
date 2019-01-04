#include "iostream"
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;


int main()

{

	const char* mulPath = "Mul_large.tif";
	const char* panPath = "Pan_large.tif";
	const char* resPath = "ans_2.tif";

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

	const int BlockXSize = Xlen;
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
	int YBlockCount = Ylen / BlockYSize;
	//BE CAREFUL
	int YBlockLeft = Ylen % BlockYSize;


	bandR = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandG = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandB = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandI = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandH = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandS = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));
	bandP = (float*)CPLMalloc(BlockYSize * BlockXSize * sizeof(float));

	for (int y = 0; y < YBlockCount; y++) {

		poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, y*BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(2)->RasterIO(GF_Read, 0, y*BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
		poMulDs->GetRasterBand(3)->RasterIO(GF_Read, 0, y*BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
		poPanDs->GetRasterBand(1)->RasterIO(GF_Read, 0, y*BlockYSize, BlockXSize, BlockYSize, bandP, BlockXSize, BlockYSize, GDT_Float32, 0, 0);

		for (int i = 0; i < BlockYSize* BlockXSize; i++) {

			//RGB => IHS
			bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
			bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];

			//IHS => RGB
			bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
			bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
			bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
		
		}

		poResDs->GetRasterBand(1)->RasterIO(GF_Write, 0, y*BlockYSize, BlockXSize, BlockYSize, bandR, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(2)->RasterIO(GF_Write, 0, y*BlockYSize, BlockXSize, BlockYSize, bandG, BlockXSize, BlockYSize, GDT_Float32, 0, 0);
		poResDs->GetRasterBand(3)->RasterIO(GF_Write, 0, y*BlockYSize, BlockXSize, BlockYSize, bandB, BlockXSize, BlockYSize, GDT_Float32, 0, 0);

	}


	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);


	bandR = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandG = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandB = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandI = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandH = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandS = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));
	bandP = (float*)CPLMalloc(YBlockLeft * BlockXSize * sizeof(float));

	//calculate botton row
	poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
	poMulDs->GetRasterBand(2)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
	poMulDs->GetRasterBand(3)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
	poPanDs->GetRasterBand(1)->RasterIO(GF_Read, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandP, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);

	for (int i = 0; i < YBlockLeft* BlockXSize; i++) {

		//RGB => IHS
		bandH[i] = -sqrt(2.0) / 6.0*bandR[i] + -sqrt(2.0) / 6.0*bandG[i] + sqrt(2.0) / 3.0*bandB[i];
		bandS[i] = 1.0 / sqrt(2.0)*bandR[i] + -1.0 / sqrt(2.0)*bandG[i];

		//IHS => RGB
		bandR[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + 1.0 / sqrt(2.0)*bandS[i];
		bandG[i] = 1.0*bandP[i] + -1.0 / sqrt(2.0)*bandH[i] + -1.0 / sqrt(2.0)*bandS[i];
		bandB[i] = 1.0*bandP[i] + sqrt(2.0)*bandH[i];
	}

	poResDs->GetRasterBand(1)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandR, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
	poResDs->GetRasterBand(2)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandG, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);
	poResDs->GetRasterBand(3)->RasterIO(GF_Write, 0, YBlockCount*BlockYSize, BlockXSize, YBlockLeft, bandB, BlockXSize, YBlockLeft, GDT_Float32, 0, 0);

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