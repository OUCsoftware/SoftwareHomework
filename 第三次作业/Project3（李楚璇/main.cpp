#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//����ͼ��
	GDALDataset* poSrcDS1;//����
	GDALDataset* poSrcDS2;//̫��

	//���ͼ��
	GDALDataset* poDstDS;

	//����ͼ��Ŀ�Ⱥ͸߶�
	int supXlen, supYlen, spaXlen, spaYlen;

	//����ͼ���·��
	const char* srcPath1 = "superman.jpg";
	const char* srcPath2 = "space.jpg";

	//���ͼ���·��
	const char* dstPath = "res.tif";

	//ͼ���ڴ�洢��RGB��
	GByte* supTmp[3];
	GByte* spaTmp[3];
	GByte* buffTmp;//���
	//ͼ�񲨶���
	int i, j,k, supNum, spaNum;

	//ע������
	GDALAllRegister();

	//��ͼ��
	poSrcDS1 = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	poSrcDS2 = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);

	//��ȡͼ�񣬸߶ȺͲ�������
	supXlen = poSrcDS1->GetRasterXSize();
	supYlen = poSrcDS1->GetRasterYSize();
	supNum = poSrcDS1->GetRasterCount();//����

	spaXlen = poSrcDS2->GetRasterXSize();
	spaYlen = poSrcDS2->GetRasterYSize();
	spaNum = poSrcDS2->GetRasterCount();//̫��

	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, spaXlen, spaYlen, spaNum, GDT_Byte, NULL);

	//����ͼ��߶ȺͿ�ȷ����ڴ�
	for (i = 0; i < 3; i++)
	{
		supTmp[i] = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
		spaTmp[i] = (GByte*)CPLMalloc(spaXlen*spaYlen * sizeof(GByte));
	}

	//��������Ȼ�����

	buffTmp = (GByte*)CPLMalloc(supXlen*supYlen * sizeof(GByte));
	
	//RGB���ض���
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
			//����������Ϊ����Ԫ�أ��ѱ���������Ŀ�����
			if ((supTmp[0][j*supXlen + i] > (GByte)10 && supTmp[0][j*supXlen + i] < (GByte)160) && (supTmp[1][j*supXlen + i] > (GByte)100 && supTmp[1][j*supXlen + i] < (GByte)220) && (supTmp[2][j*supXlen + i] > (GByte)10 && supTmp[2][j*supXlen + i] < (GByte)160))
			{
				buffTmp[j*supXlen + i] = spaTmp[k][j*supXlen + i];
			}
			else
			{
				buffTmp[j*supXlen + i] = supTmp[k][j*supXlen + i];//���ǳ���
			}
		}
	}
	
		poDstDS->GetRasterBand( + 1)->RasterIO(GF_Write, 0, 0, supXlen, supYlen, buffTmp, supXlen, supYlen, GDT_Byte, 0, 0);
	}
		//����ڴ�
		for(i=0;i<3;i++)
		{ 
			CPLFree(supTmp[i]);
			CPLFree(spaTmp[i]);
		}
		CPLFree(buffTmp);

		//�ر�dataset
		GDALClose(poDstDS);
		GDALClose(poSrcDS1);
		GDALClose(poSrcDS2);

		system("PAUSE");
		return 0;
}