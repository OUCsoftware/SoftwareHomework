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
	GByte *buffTmp,*bufftmp;
	//图像波段数
	int i, bandNum;
	//定义起始位置坐标
	int startx = 100;
	int starty = 100;

	//区域的宽度和高度
	int tmpxlen = 200;
	int tmpylen = 150;

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
	bufftmp = (GByte*)CPLMalloc(tmpxlen*tmpylen * sizeof(GByte));

	//读取红色通道缓存在bufftmp中
	posrc->GetRasterBand(1)->RasterIO(GF_Read, startx, starty, tmpxlen, tmpylen, bufftmp, tmpxlen, tmpylen, GDT_Byte, 0, 0);
	// 遍历区域, 逐像素置为 255
	for (int j = 0; j <tmpylen; j++)
	{
		for (int i = 0; i <tmpxlen; i++)
		{
			bufftmp[j*tmpxlen + i] = (GByte)255;
		}
	}

	podes->GetRasterBand(1)->RasterIO(GF_Write, startx, starty, tmpxlen, tmpylen, bufftmp, tmpxlen, tmpylen, GDT_Byte, 0, 0);

	CPLFree(buffTmp);
	CPLFree(bufftmp);
	//关闭dataset
	GDALClose(posrc);
	GDALClose(podes);


	system("PAUSE");
	return 0;
}

