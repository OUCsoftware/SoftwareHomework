## 任务一：把图像指定区域的红色图像置为255

### 1. 任务描述

提供两张图：space.jpg和superman.jpg，两张图的大小均为640*480像素。分别为：

![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwg8crwsl4j20hs0dcju4.jpg)

![](http://ww1.sinaimg.cn/large/6deb72a3ly1fwg8dir8dlj20hs0dcn23.jpg)

要求为：把superman.jpg中的超人抠出，加到 space.jpg 的太空背景中。

### 2. 核心代码

```c++
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

	//生成目标图片，并写入目的地址
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
```
### 3.实验结果

![目标图片](http://ww1.sinaimg.cn/large/006zdbpcly1fxb4v0ir6kj30m90goajf.jpg) 

### 4. 困难和解决办法

（1）第一次尝试的时候，我把每一个通道的值域分开作为条件筛选像素点

```c++
for (i = 0; i < 3; i++)
	{
		//逐个通道操作
		for (j = 0; j < supYlen; j++)
		{
			for (k = 0; k < supXlen; k++)
			{
				//符合条件即为背景元素，把背景拷贝到目标对象
				if (supBuffTmp[0][j*supXlen + k] > (GByte)10 && supBuffTmp[0][j*supXlen + k] < (GByte)160) 
					tmp[j*supXlen + k] = bgBuffTmp[j*supXlen + k];
				else if(supBuffTmp[1][j*supXlen + k] > (GByte)100 && supBuffTmp[1][j*supXlen + k] < (GByte)220) 
					tmp[j*supXlen + k] = bgBuffTmp[j*supXlen + k];
				else if(supBuffTmp[2][j*supXlen + k] > (GByte)10 && supBuffTmp[2][j*supXlen + k] < (GByte)160)
				{
					tmp[j*supXlen + k] = bgBuffTmp[j*supXlen + k];
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
```

导致的结果就是小超人并不能完全被识别出来。

![不能被完全识别的小超人](http://ww1.sinaimg.cn/large/006zdbpcly1fxb5dpsngij30m80gn0sy.jpg)

解决方法：把所有的限制条件写在同一个if语句里，否则筛选出的像素点会有很多，不只是绿色的点

2.背景的颜色无法显示出来

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxb5gk9y9mj30m70gn3yx.jpg)

解决方法：分析我的问题，是我没有把每一个通道的背景单独输出到一个对象上。后来我把三个通道分别输出就解决了问题。

![](http://ww1.sinaimg.cn/large/006zdbpcly1fxb4v0ir6kj30m90goajf.jpg)