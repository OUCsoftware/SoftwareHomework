## 第一次实验 —— 获得图片主要参数
### 一、实验环境准备

1. 下载VS
2. 下载GDAL库
###二、实验内容
1. 新建win32控制台应用程序
2. 在含main（）函数的文件中写入代码
```
#include"stdafx.h"
#include <iostream>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset* poSrcDS;           //输入图像
	GDALDataset* poDstDS;			//输出图像
	int imgXlen, imgYlen;			//图像的宽度和高度
	char* srcPath = "trees.jpg";	//输入图像路径
	char* dstPath = "res.tif";		//输出图像路径
	GByte* buffTmp;					//图像内存存储
	int i, bandNum;					//图像波段数
									//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽高和波段数量
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//输出获取结果
	cout << "Image X Length: " << imgXlen << endl;
	cout << "Image Y Length: " << imgYlen << endl;
	cout << "Band Number: " << bandNum << endl;

	//根据图像的宽高分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//波段的输入与输出
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("...... band %d processing ......\n", i);
	}

	//清除内存
	CPLFree(buffTmp);
	//关闭dataset，先打开的后关
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
	return 0;
}
```
### 二、实验结果

![](https://wx1.sinaimg.cn/square/006zdbpcly1fw02783uhuj30eh06cwg2.jpg)
### 三、收获
1.` #pragma comment(lib, "gdal_i.lib")`
​	这句代码可以免去配环境的麻烦，并且可以轻松地在不同的环境下运行代码，方便项目合作

2.GDAL库的初次使用
​	
​        GDAL库把很多常用的类型变量封装起来了，比如GDALdataset，GByte等。


