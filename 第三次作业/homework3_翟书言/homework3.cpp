// homework3.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include"stdafx.h"
#include <iostream>
using namespace std;
#include "../gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")


int main()
{
	GDALDataset* background;                   //����ͼ��
	GDALDataset* superman;
	GDALDataset* poDstDS;			           //���ͼ��
	char* srcPath1 = "background.jpg";         //����ͼ��·��
	char* srcPath2 = "superman.jpg";
	char* dstPath = "res.tif";		           //���ͼ��·��
	GByte* buffTmp;					           //ͼ���ڴ�洢
	GByte* bgBuffTmp[3];
	GByte* supBuffTmp[3];
	GByte* tmp;                                //��ͼ���
	int imgXlen, imgYlen, bandNum;
	int supXlen, supYlen, supBandNum;
	int i, j, k;

	GDALAllRegister();
	//��ͼ��
	background = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	superman = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);
	//��ȡ����ͼͼ�����
	imgXlen = background->GetRasterXSize();                          //��ȡ���
	imgYlen = background->GetRasterYSize();
	bandNum = background->GetRasterCount();
	//�����ڴ�
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("Gtiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//���ñ���
	for (i = 0; i < bandNum; i++)
	{
		background->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//��ȡǰ��ͼͼ�����
	supXlen = superman->GetRasterXSize();
	supYlen = superman->GetRasterYSize();
	supBandNum = superman->GetRasterCount();
	for (i = 0; i < 3; i++)
	{
		bgBuffTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
		supBuffTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
	}
	tmp = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));

	//��supermanд�뱳��
	for (i = 0; i < supBandNum; i++)
	{
		background->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, bgBuffTmp[i], imgXlen, imgYlen, GDT_Byte, 0, 0);
		superman->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, supXlen, supYlen, supBuffTmp[i], supXlen, supYlen, GDT_Byte, 0, 0);
	}

	for (i = 0; i < 3; i++)
	{
		//���ͨ������
		for (j = 0; j < supYlen; j++)
		{
			for (k = 0; k < supXlen; k++)
			{
				//����������Ϊ����Ԫ�أ��ѱ���������Ŀ�����
				if ((supBuffTmp[0][j*supXlen + k] > (GByte)10 && supBuffTmp[0][j*supXlen + k] < (GByte)160) && (supBuffTmp[1][j*supXlen + k] > (GByte)100 && supBuffTmp[1][j*supXlen + k] < (GByte)220) && (supBuffTmp[2][j*supXlen + k] > (GByte)10 && supBuffTmp[2][j*supXlen + k] < (GByte)160))
				{
					tmp[j*supXlen + k] = bgBuffTmp[i][j*supXlen + k];
				}
				//����Ϊ���ˣ��ѳ��˵����ص㿽����Ŀ�����
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

