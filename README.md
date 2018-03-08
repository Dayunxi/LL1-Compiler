# LL1-Compiler
编译原理课程期末设计，类C/Java文法，LL(1)递归下降分析，最后产生中间代码(四元式)

------

### 文法
![syntax](http://cloud.adamyt.com/extern-reference/syntax.jpg)

网上很多关于编译原理的讲解貌似就是这个文法

------

### 词法
![syntax](http://cloud.adamyt.com/extern-reference/word.jpg)

------

### 编译
Windows下：

        g++ -o parser.exe parser.cpp word.cpp table.cpp syntax.cpp semantic.cpp common.cpp
Linux下：

        make

### 测试结果

        parser < test/input.txt
        
        
-------

详细的语法分析过程可以在上传的Word文档中查看，正在上编译原理的同学们可能用得上
