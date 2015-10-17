如果提示缺少dll请下载GCC_Dlls包补全dll，或者自行从CodeBlocks提取。

如何创建dasp包？

1.打开dicgen.exe，它将会创建一个全球唯一的加密字典，名为dic.dasdic。
2.参考dirtree.bat，使用它创建一个文件列表fileList.txt，这个列表中包含相对于dasp.exe工作目录的各个要打包文件的路径。
（注：不能以点号作为路径名，dasp工作目录即为dasp包根目录。）
3.修改fileList.txt，保证它包含的是相对路径后，把fileList.txt和dic.dasdic放入dasp工作目录。
4.创建Warning.txt，它将会作为文件头部的标示写入文件包，当有人用ASCII方式打开文件包时将会看到这个标识，你可以在这里写入法律信息。
5.运行dasp.exe，完成后它将输出out.dasp。

关于如何使用dasp包，请参考DASPReader.h。

--御宅十字 星翼
