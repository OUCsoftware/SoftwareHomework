## FifthDemo线性滤波实验报告

姓名：王刚

学号：16020031075

#### 实验过程：

​	(1) 将多光谱影像重采样到与全色影像具有相同的分辨率；

​	(2) 将多光谱图像的Ｒ、Ｇ、Ｂ三个波段转换到IHS空间，得到Ｉ、Ｈ、Ｓ三个分量；

​	(3) 以Ｉ分量为参考，对全色影像进行直方图匹配

​	(4) 用全色影像替代Ｉ分量，然后同Ｈ、Ｓ分量一起逆变换到RGB空间，从而得到融合图像。

​	RGB与IHS变换的基本公式：

   ![img](https://github.com/Histra/FifthDemo/blob/master/Pictures_ExperimentReport/OK_I_am_fine_00.png) 

#### 实验中遇到的问题以及解决方法：

1. 由于我于2018.11.24~2018.11.25赴焦作参加ACM-ICPC区域赛，本实验有所耽误。因此，回来后，至今日2018.11.27在参考老师的代码下，独立地完成了代码的编写。

2. 在完成代码运行 的时候出现了下述图片中的运行错误：

   ![img](https://github.com/Histra/FifthDemo/blob/master/Pictures_ExperimentReport/OK_I_am_fine_01.png) 

   ![img](https://github.com/Histra/FifthDemo/blob/master/Pictures_ExperimentReport/OK_I_am_fine_02.png) 
   再三寻找以后，再三寻找以后，再三寻找以后（真的是再三寻找，所以说三遍，加上这遍是四遍qwq）。终于发现了问题所在，原来我把

   ```c++
   poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandR, Xlen, Ylen, GDT_Float64, 0, 0);/// 正确代码
   ```

   写成了

   ```c++
   poMulDs->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, Xlen, Ylen, bandR, Xlen, Ylen, GDT_CFloat64, 0, 0);/// 错误代码
   ```

   于是就报如上错误，真的是太大意了！！
   下面总结一下：

   ![img](https://github.com/Histra/FifthDemo/blob/master/Pictures_ExperimentReport/OK_I_am_fine_03.png) 

#### 实验结果展示：

详见data文件夹下的American_Res.tif文件。

#### 实验反思总结：

​	这个实验是第五个实验。本次实验在GDT_CFloat64这点卡住了，由此可见，敲代码时应该注意严谨性，多了一个字母的代价实在沉重！
​	通过这个实验和实验四，发现图像处理真的越来越有趣了。希望自己能够再接再厉，严谨治学！



