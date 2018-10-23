#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//����ͼ��
	GDALDataset* poSrcDS;

	//���ͼ��
	GDALDataset* poDstDS;

	//��ʼλ�õ�����
	int StartX = 100, StartY = 100;

	//��ȡͼ��Ŀ�Ⱥ͸߶�
	int tmpXlen = 200, tmpYlen = 150;

	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;

	//�ڴ�
	GByte* buffTmp;

	//����ͼ���·��
	const char* srcPath = "qingdao.jpg";

	//���ͼ���·��
	const char* dstPath = "qingdao_1.tif";

	//����
	int i, j, k, bandNum;

	//ע������
	GDALAllRegister();

	//��ͼ��
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//��ȡͼ����,�߶ȺͲ�������
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//�����ڴ�
	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	
	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//һ�������ε�����,Ȼ��һ�������ε����
	for (i = 0; i < bandNum; i++)
	{
		//д�뻺����
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//��ȡ��ɫͨ����buffTmp��
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	//��������,��������Ϊ255
	for (j = 0; j < tmpYlen; j++)
	{
		for (i = 0; i < tmpXlen; i++)
		{
			buffTmp[j*tmpXlen + i] = (GByte)255;
		}
	}
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	for (k = 1; k < 4; k++)
	{
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 300, 300, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);

		//��������,��������Ϊ0
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)255;
			}
		}
		//д�뻺����
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 300, 300, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);
	}
	
	for (k = 1; k < 4; k++)
	{
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 500, 500, 100, 50, buffTmp, 100, 50, GDT_Byte, 0, 0);

		//��������,��������Ϊ0
		for (j = 0; j < tmpYlen; j++)
		{
			for (i = 0; i < tmpXlen; i++)
			{
				buffTmp[j*tmpXlen + i] = (GByte)0;
			}
		}
		//д�뻺����
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 500, 500, 50, 100, buffTmp, 50, 100, GDT_Byte, 0, 0);
	}

	//����ڴ�
	CPLFree(buffTmp);

	//�ر�dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	system("PAUSE");
	return 0;

}