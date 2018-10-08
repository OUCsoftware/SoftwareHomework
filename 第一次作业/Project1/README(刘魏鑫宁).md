**1.**首先把代码敲进去,因为老师之前有提示,把lib和gdal文件放到了项目的目录下,然后又把dll放到了release的目录下,所以这个没有发生问题,然后在百度上查到:lib与dll的区别是：dll是运行时需要的，lib是编译时需要的。dll是动态链接库,而lib是静态链接库.

然后运行:

![](http://ww1.sinaimg.cn/large/006y6mwBly1fvseagysygj30wz04lq3p.jpg)

奇怪,然后意识到是tree.jpg它根本不知道在哪里,就把图片也放到了项目下面,

![](http://ww1.sinaimg.cn/large/006y6mwBly1fvsebok6rjj30ke03m3yo.jpg)

结果还是错误,就在char*前面加了const,就好了

![](http://ww1.sinaimg.cn/large/006y6mwBly1fvsepibuobj30pl07874i.jpg)

然后还有一个问题是运行时总是说要加载符号,耗费很多时间,甚至几分钟,百度到:

点击【工具】》【选项】，在弹出的框中再点击【调试】》【符号】，再把【Microsoft符号服务器】前面的√去掉，最后点击【确定】即可.
