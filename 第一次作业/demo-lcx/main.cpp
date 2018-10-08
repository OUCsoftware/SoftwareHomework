#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
int main()
{

	GDALDataset* poSrcDS;

	GDALDataset* poDstDS;

	int imgXlen, imgYlen;

	const char* srcPath = "trees.jpg";

	const char* dstPath = "res.tif";

	GByte* buffTmp;

	int i, bandNum;

	GDALAllRegister();

	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	imgXlen = poSrcDS->GetRasterXSize();

	imgYlen = poSrcDS->GetRasterYSize();

	bandNum = poSrcDS->GetRasterCount();

	cout << "Image X Length:" << imgXlen << endl;
	cout << "Image Y Length:" << imgYlen << endl;
	cout << "Band number:   " << bandNum << endl;

	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("......band %d processing......\n", i);
	}

	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
	return 0;
}