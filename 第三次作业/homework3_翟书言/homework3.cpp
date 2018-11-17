// homework3.cpp : 定义控制台应用程序的入口点。
//
#include"stdafx.h"
#include <iostream>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")


int main()
{
	GDALDataset* background;                   //输入图像
	GDALDataset* superman;
	GDALDataset* poDstDS;			           //输出图像
	char* srcPath1 = "background.jpg";         //输入图像路径
	char* srcPath2 = "superman.jpg";
	char* dstPath = "res.tif";		           //输出图像路径
	GByte* buffTmp;					           //图像内存存储
	GByte* bgBuffTmp[3];
	GByte* supBuffTmp[3];
	GByte* tmp;                                //抠图结果
	int imgXlen, imgYlen, bandNum;
	int supXlen, supYlen, supBandNum;
	int i, j, k;

	GDALAllRegister();
	//打开图像
	background = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	superman = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);
	//获取背景图图像参数
	imgXlen = background->GetRasterXSize();                          //获取宽高
	imgYlen = background->GetRasterYSize();
	bandNum = background->GetRasterCount();
	//分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("Gtiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//设置背景
	for (i = 0; i < bandNum; i++)
	{
		background->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//获取前景图图像参数
	supXlen = superman->GetRasterXSize();
	supYlen = superman->GetRasterYSize();
	supBandNum = superman->GetRasterCount();
	for (i = 0; i < 3; i++)
	{
		bgBuffTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
		supBuffTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
	}
	tmp = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));

	//把superman写入背景
	for (i = 0; i < supBandNum; i++)
	{
		background->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bgBuffTmp[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		superman->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, supXlen, supYlen, supBuffTmp[i], supXlen, supYlen, GDT_Byte, 0, 0);
	}

	for (i = 0; i < 3; i++)
	{
		//逐个通道操作
		for (j = 0; j < supYlen; j++)
		{
			for (k = 0; k < supXlen; k++)
			{
				//符合条件即为背景元素，把背景拷贝到目标对象
				if ((supBuffTmp[0][j*supXlen + k] > (GByte)10 && supBuffTmp[0][j*supXlen + k] < (GByte)160) && (supBuffTmp[1][j*supXlen + k] > (GByte)100 && supBuffTmp[1][j*supXlen + k] < (GByte)220) && (supBuffTmp[2][j*supXlen + k] > (GByte)10 && supBuffTmp[2][j*supXlen + k] < (GByte)160))
				{
					tmp[j*supXlen + k] = bgBuffTmp[i][j*supXlen + k];
				}
				//否则即为超人，把超人的像素点拷贝到目标对象
				else
				{
						tmp[j*supXlen + k] = supBuffTmp[i][j*supXlen + k];
				}
			}
		}
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, supXlen, supYlen, tmp, supXlen, supYlen, GDT_Byte, 0, 0);
	}
	
	CPLFree(tmp);
	for (i = 0; i < 3; i++)
	{
		CPLFree(supBuffTmp[i]);
		CPLFree(bgBuffTmp[i]);
	}
	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(superman);
	GDALClose(background);

    return 0;
}

