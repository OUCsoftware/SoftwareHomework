#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset* poSrcDS1;//超人
	GDALDataset* poSrcDS2;//太空

	//输出图像
	GDALDataset* poDstDS;

	//输入图像的宽度和高度
	int supXlen, supYlen, spaXlen, spaYlen;

	//输入图像的路径
	const char* srcPath1 = "superman.jpg";
	const char* srcPath2 = "space.jpg";

	//输出图像的路径
	const char* dstPath = "res.tif";

	//图像内存存储（RGB）
	GByte* supTmp[3];
	GByte* spaTmp[3];
	GByte* buffTmp;//结果
	//图像波段数
	int i, j,k, supNum, spaNum;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	poSrcDS2 = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);

	//获取图像，高度和波段数量
	supXlen = poSrcDS1->GetRasterXSize();
	supYlen = poSrcDS1->GetRasterYSize();
	supNum = poSrcDS1->GetRasterCount();//超人

	spaXlen = poSrcDS2->GetRasterXSize();
	spaYlen = poSrcDS2->GetRasterYSize();
	spaNum = poSrcDS2->GetRasterCount();//太空

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, spaXlen, spaYlen, spaNum, GDT_Byte, NULL);

	//根据图像高度和宽度分配内存
	for (i = 0; i < 3; i++)
	{
		supTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
		spaTmp[i] = (GByte*)CPLMalloc(spaXlen*spaYlen * sizeof(GByte));
	}

	//波段输入然后输出

	buffTmp = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
	
	//RGB像素读入
	for (i = 0; i < 3; i++)
	{
		poSrcDS1->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, supXlen, supYlen, supTmp[i], supXlen, supYlen, GDT_Byte, 0, 0);
		poSrcDS2->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, spaXlen, spaYlen, spaTmp[i], spaXlen, spaYlen, GDT_Byte, 0, 0);
	}
	for (k = 0; k < 3; k++)
	{
	for (j = 0; j < supYlen; j++)
	{
		for (i = 0; i < supXlen; i++)
		{
			//符合条件即为背景元素，把背景拷贝到目标对象
			if ((supTmp[0][j*supXlen + i] > (GByte)10 && supTmp[0][j*supXlen + i] < (GByte)160) && (supTmp[1][j*supXlen + i] > (GByte)100 && supTmp[1][j*supXlen + i] < (GByte)220) && (supTmp[2][j*supXlen + i] > (GByte)10 && supTmp[2][j*supXlen + i] < (GByte)160))
			{
				buffTmp[j*supXlen + i] = spaTmp[k][j*supXlen + i];
			}
			else
			{
				buffTmp[j*supXlen + i] = supTmp[k][j*supXlen + i];//覆盖超人
			}
		}
	}
	
		poDstDS->GetRasterBand( + 1)->RasterIO(GF_Write, 0, 0, supXlen, supYlen, buffTmp, supXlen, supYlen, GDT_Byte, 0, 0);
	}
		//清除内存
		for(i=0;i<3;i++)
		{ 
			CPLFree(supTmp[i]);
			CPLFree(spaTmp[i]);
		}
		CPLFree(buffTmp);

		//关闭dataset
		GDALClose(poDstDS);
		GDALClose(poSrcDS1);
		GDALClose(poSrcDS2);

		system("PAUSE");
		return 0;
}