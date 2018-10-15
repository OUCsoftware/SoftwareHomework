## SecondDemo实验报告

姓名：王刚

学号：16020031075

#### 实验过程：

1. 在源程序目录下新建data文件夹，用于存放数据图片和结果图片。把gdal文件夹和gdal_i.lib拷贝到项目目录下，把gdal18.dll拷贝到Release文件夹下。然后在代码块中添加如下代码：

    ```c++
    #include "./gdal/gdal_priv.h"
    #pragma comment(lib, "gdal_i.lib")
    ```

2. 输入输出图像路径：

   ```
   	/// 输入图像路径
   	char* srcPath = "./data/Input.jpg";
   	/// 输出图像1(任务1)
   	char* dstPath1 = "./data/Output1.tif";
   	/// 输出图像2(任务2)
   	char* dstPath2 = "./data/Output2.tif";
   ```

3. 创建Output1.tif和Output2.tif，然后声明一个临时变量Temp并为之分配内存，用该变量实现复制Input.jpg到Output1.tif和Output2.tif。这一过程和FirstDemo实验大同小异。最后一定要使用CPLFree()函数清除Temp临时变量！

   核心代码展示：

   ```
   	/// 创建输出图像1
   	poDstDs1 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
   	/// 创建输出图像2
   	poDstDs2 = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
   	/// 临时变量暂存图像内存
   	GByte* Temp;
   	/// 为临时变量Temp分配内存
   	Temp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
   	/// 复制输入图像到输出图像1和输出图像2
   	for (int i = 0; i < bandNum; i++) {
   		poSrcDs->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
   		poDstDs1->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
   		poDstDs2->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, Temp, imgXlen, imgYlen, GDT_Byte, 0, 0);
   	}
   	/// 清除Temp内存
   	CPLFree(Temp);
   ```

4. 任务一：把图像指定区域的红色通道置为255
   任务描述：把输入图像中起始点（100，100），宽度为200像素，高度为150像素的区域红色通道置为255。

5. 任务一实现思路：

    利用GDAL中的GetRasterBand()函数得到图像的通道：

    GetRasterBand(1)得到的是红色通道；

    GetRasterBand(2)得到的是绿色通道；

    GetRasterBand(3)得到的是蓝色通道。

    那么获取红色通道就可以使用GetRasterBand(1)，然后使用RasterIO()函数读取Input.jpg中的起点（100，100），宽度为200像素，高度为150像素的区域，并将其赋值给临时变量buffTmp；遍历buffTmp，将其每一处都赋值成(GByte)255，即把红色通道中的每一个值都变成最大值（通常，三基色的每个数值也是在0到255之间，0表示相应的基色在该像素中没有，而255则代表相应的基色在该像素中取得最大值）。在这个过程中要注意对图像横纵坐标的理解和转化；最后，再次使用RasterIO()函数将buffTmp写入到输出图像poDstDs1的红色通道中。
    核心代码展示：

    ```c++
    	cout << "Running Task one ..." << endl;
    	/// 起始位置坐标
    	int startX = 100;
    	int startY = 100;
    	/// 区域宽度高度
    	int tmpXlen = 200;
    	int tmpYlen = 150;
    	/// 截取图像内存存储
    	GByte* buffTmp;
    	/// 分配内存
    	buffTmp = (GByte*)CPLMalloc(tmpXlen * tmpYlen * sizeof(GByte));
    	/// 读取红色通道到buffTmp中
    	poSrcDs->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
    	/// 遍历区域，逐像素置为255
    	for (int i = 0; i < tmpYlen; i++) {
    		for (int j = 0; j < tmpXlen; j++) {
    			buffTmp[tmpXlen * i + j] = (GByte)255;
    		}
    	}
    	/// 数据写入poDstDs1
    	poDstDs1->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
    	cout << "Ended Task one !" << endl;
    ```

6. 任务一结果展示：

   打开data文件夹：

      ![1539062733325](https://github.com/Histra/SecondDemo/blob/master/1539062726091.png)

   打开OutPut1.tif文件(截图，非截图在data文件夹下)：

   ![1539062672022](https://github.com/Histra/SecondDemo/blob/master/1539062672022.png)

7. 任务2：把图像指定区域置为白色和黑色
   任务描述：把输入图像中起始点（300，300），宽度为100像素，高度为50像素的区域置为白色，同时，把输入图像中起始点为（500，500），宽度为50像素，高度为100像素的区域置为黑色。

8. 任务二思路：
   对于将特定区域置为白色的任务：我们知道，红黄蓝三色混合以后就是白色，即只需要将红黄蓝三个颜色通道中的值均置为255，便可把该区域置为白色。那么，先重置初始位置坐标和区域宽度高度，然后遍历三个颜色通道，对于每一个颜色通道执行和任务一类似的操作，即对于每一个颜色通道，先将图片特定部分读入buffTmp中，然后把buffTmp中的值置为最大值255，然后再写入poDstDs2中。这样就实现了把图像指定区域置为白色的任务了。

   核心代码展示：

   ```c++
   	cout << "Running Task two ..." << endl;
   	/// 重置初始位置坐标
   	startX = 300;
   	startY = 300;
   	/// 重置区域宽度高度
   	tmpXlen = 100;
   	tmpYlen = 50;
   	/// 区域变白
   	for (int k = 0; k < bandNum; k++) {
   		/// 读取颜色通道到buffTmp中
   		poSrcDs->GetRasterBand(k + 1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
   		/// 遍历区域，逐像素置为255
   		for (int i = 0; i < tmpYlen; i++) {
   			for (int j = 0; j < tmpXlen; j++) {
   				buffTmp[tmpXlen * i + j] = (GByte)255;
   			}
   		}
   		/// 数据写入poDstDs2
   		poDstDs2->GetRasterBand(k + 1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
   	}
   ```

   对于将特定区域置为黑色的任务：此任务和置白的任务大同小异，我们知道，只需要将红黄蓝三个颜色通道中的值均置为0，便可把该区域置为黑色。那么，再次重置起始位置坐标和指定区域的宽度和高度，然后遍历三个颜色通道，对于每一个颜色通道，先将图片特定部分读入buffTmp中，然后把buffTmp中的值置为最小值0，然后再写入poDstDs2中。这样就实现了把图像指定区域置为黑色的任务了。

   注意：一定要清除内存！

   核心代码展示：

   ```c++
   	/// 重置初始位置坐标
   	startX = 500;
   	startY = 500;
   	/// 重置区域宽度高度
   	tmpXlen = 50;
   	tmpYlen = 100;
   	/// 区域变黑
   	for (int k = 0; k < bandNum; k++) {
   		/// 读取颜色通道到buffTmp中
   		poSrcDs->GetRasterBand(k + 1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
   		/// 遍历区域，逐像素置为0
   		for (int i = 0; i < tmpYlen; i++) {
   			for (int j = 0; j < tmpXlen; j++) {
   				buffTmp[tmpXlen * i + j] = (GByte)0;
   			}
   		}
   		/// 数据写入poDstDs2
   		poDstDs2->GetRasterBand(k + 1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
   	}
   	cout << "Ended Task two !" << endl;
   
   	/// 清除内存
   	CPLFree(buffTmp);
   	GDALClose(poSrcDs);
   	GDALClose(poDstDs1);
   	GDALClose(poDstDs2);
   ```

9. 任务二结果展示：
   打开Output2.tif文件(截图，非截图在data文件夹下)：![1539092728024](https://github.com/Histra/SecondDemo/blob/master/1539092728024.png)

#### 实验中遇到的问题以及解决方法：

1. 刚刚写完代码后没有清除内存，导致运行结果出错。发现后及时改正。

2. 对于RasterIO函数的参数有疑问，上网搜索后，整理如下：

   函数原型：

   ```c++
   CPLErr GDALRasterBand::RasterIO(GDALRWFlageRWFlag,int nXOff,int nYOff,int nXSize,int nYSize,void *pData,int nBufXSize,int nBufYSize,GDALDataTypeeBufType,int nPixelSpace,int nLineSpace)
   ```

   eRWFlag：读写标记，指定读取图像或写入图像。

   nXOff,nYOff,nXSize,nYSize：四个参数用来指定读写图像的范围，其中前两个用来说明读取的起始行号和列号，后两个是读写图像的宽度和高度。

   *pData：用来存储图像的元素值。

   nBufXSize和nBufYSize：用来指定*pData的大小，常用于缩放图像。

   nBufType：数据读取的数据类型，这个参数和pData有密切的关系。

   nPixelSpace,nLineSpace：这两个参数是用来控制参数pData中像元的存储顺序的，nPixelSpace表示的是当前像素值和下一个像素值之间的间隔，nLineSpace表示当前行和下一行的间隔，单位都是按照字节为单位计算。这两个值默认给0。

#### 实验反思总结：

​	这个实验是第二个实验，再次接触GDAL中的函数，对它们有了一个更加深刻的印象，尤其是对RasterIO函数，身为感触GDAL对图像处理能力的强大。这个实验还可以改进，对于图像的操作起始有很多的共同点，可以将代码封装成函数，鉴于将其封装成函数会使参数列表过长，就不再展示封装后的代码。
​	希望在以后的实验中能再接再厉，更上一层楼！



