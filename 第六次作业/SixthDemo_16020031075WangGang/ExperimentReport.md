## SixthDemo线性滤波实验报告

姓名：王刚

学号：16020031075

#### 实验中遇到的问题以及解决方法：

1. 本实验是在实验五的基础上做的延伸，所以做起来比较容易，花了一个晚上就解决了。但是还是碰到了不少问题，真让人头大！

2. 在完成代码运行 的时候出现了下述图片中的运行错误：

   ![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_01.png) 

   ![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_02.png) 
   出现上述现象的原因是访问了不可访问的存储空间。仔细检查后发现，自己的循环变量写错了，真是不应该啊啊啊！

   ```c++
   for (int x = 0; x < XBlockCount; x++) /// 遍历x方向上的不足的一块
       /// 正确代码
   ```

   写成了

   ```c++
   for (int x = 0; x < BlockXSize ; x++) {/// 遍历x方向上的不足的一块
   /// 错误代码
   ```

   于是就报如上错误，真的是太大意了！！明明应该遍历块个数，我却写成了遍历块X方向！！！

#### 实验结果展示：

​	1.时间比较：

​	![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_06.png) 
​	显然，第一种分块方式的运行效率远低于第二种分块方式！

​	2.细节比较：

​	放大运动场：

​	Mul_large.tif:

​	![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_05.png) 

​	Pan_large.tif:

​	![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_10.png) 

​	Res_large.tif（第一种分块方式）:

​	![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_03.png) 

​	Ans_large.tif（第二种分块方式）:

​	![img](https://github.com/Histra/SixthDemo/blob/master/PicturesOfExperimentReport/SixthDemoExperimentReport_04.png) 

​	3.图像结果：

​	由于结果太大，详见data文件夹下的图片文件和ReadMe.txt。

#### 实验反思总结：

​	这个实验是第六个实验。本次实验在循环变量上卡住了，由此可见，调代码一定要细心专心，还有变量的命名一定要有所意义，这样区分起来也比较容易，而且对人很友好！
​	由本实验可以得出结论，一个好的分块方法可以在根本上影响程序的执行时间，这是我以前没有接触的。同时，本实验的数据很大，很大，很大（因为是真的大，在我的电脑上data文件夹占用了1.75G！）。图像处理不仅耗时，而且耗机器啊~但是很有趣呢~Interesting！
​	希望自己能够再接再厉，严谨治学！



