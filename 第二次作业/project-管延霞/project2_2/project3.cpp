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
	GDALDataset* podes;

	//图像的宽度和高度
	int imgXlen, imgYlen;
	//图像的路径
	char *srcpath = "qingdao.jpg";
	//输出图像的路径,只能保存为tif类型的
	char *despath = "qing.tif";
	//图像内存存储
	GByte *buffTmp,*bufftmp1,*bufftmp2;
	//图像波段数
	int i, bandNum;
	//定义起始位置坐标
	int startx1 = 300;
	int starty1 = 300;

	//区域的宽度和高度
	int tmpx1len = 100;
	int tmpy1len = 50;
	//定义起始位置2坐标
	int startx2 = 500;
	int starty2 = 500;

	//区域的宽度和高度
	int tmpx2len = 50;
	int tmpy2len = 100;
	//注册驱动
	GDALAllRegister();
	//打开图像
	posrc = (GDALDataset*)GDALOpenShared(srcpath, GA_ReadOnly);

	//获取图像的宽度，高度和波段数
	imgXlen = posrc->GetRasterXSize();
	imgYlen = posrc->GetRasterYSize();
	bandNum = posrc->GetRasterCount();

	
	//根据图像大小进行分配空间
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//创建输出图像
	podes = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(despath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//一个个波段的输入，然后一个个波段的输出
	for (i = 0; i < bandNum; i++)
	{
		posrc->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		podes->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	
	}
	// 分配内存
	bufftmp1 = (GByte*)CPLMalloc(tmpx1len*tmpy1len * sizeof(GByte));
	bufftmp2 = (GByte*)CPLMalloc(tmpx2len*tmpy2len * sizeof(GByte));
	//读取红色通道缓存在bufftmp中
	
	
	// 遍历区域, 逐像素置为 255
	for (int k = 0; k < 3; k++)
	{
		posrc->GetRasterBand(k+1)->RasterIO(GF_Read, startx1, starty1, tmpx1len, tmpy1len, bufftmp1, tmpx1len, tmpy1len, GDT_Byte, 0, 0);
		for (int j = 0; j <tmpy1len; j++)
			{
				for (int i = 0; i <tmpx1len; i++)
				{
					bufftmp1[j*tmpx1len + i] = (GByte)255;
				}
			}

		podes->GetRasterBand(k+1)->RasterIO(GF_Write, startx1, starty1, tmpx1len, tmpy1len, bufftmp1, tmpx1len, tmpy1len, GDT_Byte, 0, 0);
	}
	for (int k = 0; k < 3; k++)
	{
		posrc->GetRasterBand(k+1)->RasterIO(GF_Read, startx2, starty2, tmpx2len, tmpy2len, bufftmp2, tmpx2len, tmpy2len, GDT_Byte, 0, 0);
		for (int j = 0; j <tmpy2len; j++)
		{
			for (int i = 0; i <tmpx2len; i++)
			{
				bufftmp2[j*tmpx2len + i] = (GByte)0;
			}
		}
		podes->GetRasterBand(k+1)->RasterIO(GF_Write, startx2, starty2, tmpx2len, tmpy2len, bufftmp2, tmpx2len, tmpy2len, GDT_Byte, 0, 0);

	}

	CPLFree(buffTmp);
	CPLFree(bufftmp1);
	CPLFree(bufftmp2);
	//关闭dataset
	GDALClose(posrc);
	GDALClose(podes);


	system("PAUSE");
	return 0;
}

