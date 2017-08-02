## 1.1. placement new(new)
用于分配指定大小的空间，不带类型。我们知道new分配空间是带类型的，也就是new会调用对应类型的构造函数，把相应的数据放在对应的位置，然而在此之前，new先会使用placement new来分配空间，再在这个空间上使用构造函数。
```c
A* a = new A；
//有以下三个步骤
调用operator new (sizeof(A))
调用A:A()
返回指针
```
p为一个已知分配堆空间，T为一个类，T(v)构造一个类对象放在位置p。使用`new(p) T(v)`可以在已分配空间p上进行对象T构造，即对象T保存在p上。对于内置类型来说不需要构造函数过程。
>[ C++ 内存分配(new，operator new)详解](http://blog.csdn.net/wudaijun/article/details/9273339)
>[new operator、operator new 、placement new三者之间的区别与联系](http://blog.csdn.net/pfgmylove/article/details/8234360)

## 1.2. c++ 内置类型的构造
c++中常见的内置类型int，char等初始化也可以写出对象构造的形式(没有析构形式)：

`int* a = new int(5);  char c = char('a');`

## 1.3. template
1. C++中template类定义和声明不能分开，需要写在一个文件中，模板类继承和一般类继承也有区别，子类不能直接访问父类的成员变量。
2. template的作用域是有限的，对于模板类来说从类定义开始到最后的结束大括号为作用域，模板函数的话作用域为函数的定义到结束大括号。
3. 在模板类或函数内部可以继续定义模板，使用方法相同，不过在作用域在内部而已。
4. 模板中类型也可以使用非型别参数，但是非型别模版参数必须是整型或者和整型兼容的如unsigned int，short等；如：`template<class T, int n>` ；int n 就是非型别模板参数，它是有特定类型的值。

>《C++ templates》

## 1.4. 可变参数宏  \_\_VA_ARGS\_\_
宏定义过程中可以设定可变的参数，像函数一样，并通过__VA_ARGS__ 把参数传递给宏：

```c++
#define debug(...) printf(__VA_ARGS__)
缺省号代表一个可以变化的参数表。使用保留名 __VA_ARGS__ 把参数传递给宏。当宏的调用展开时，实际的参数就传递给 printf()了。例如: 
debug("Y = %d\n", y);
而处理器会把宏的调用替换成: 


printf("Y = %d\n", y);
```
>[C语言 \_\_VA_ARGS\_\_ 宏](http://www.cnblogs.com/alexshi/archive/2012/03/09/2388453.html)

## 1.5. UINION 结构
union在c++称为共用体，与struct区别在于union中的数据成员是共享存储空间的，即同一时刻只能使用其中的一个成员数据，union结构的大小一般为其中最大的数据成员大小（不考虑字节对齐等因素）。union初始化与struct类似，不过union初始化数据放在共享空间的开始位置。


this is new paragrah!!!!!!!!!!!!!!!!!!!!!!11



```c
union test
{
    int a;
    int b[10];
    test* ptr;
};

test t1 = {1};   //初始化a=1; b[0] = 1; ptr = 0x01
for(int i=0;i<10;i++)
    t1.b[i] = i;       //赋值，a=0；ptr = 0x0;

//即不管怎么初始化，赋值，都是在同一个共享空间上操作，其它的成员也能读取数据，但不一定正确；
```
>[C++中的union用法总结](http://www.jb51.net/article/56009.htm)
## 1.6. enum
c++中enum是枚举类型，是被列举的常量的集合，即可以把enum中的枚举成员当作一个个常量，类似于#define 定义的宏。枚举成员默认按照顺序每一个对应一个值0,1,2,3...，也可以用整形常数进行赋值。
```c
enum test{Mon=1,Tue=15,Wed,Thu,Fri=7,Sat,Sun};
int a = Wed;
test b = Mon;
//a的值为16，默认上一个数加1，b这种用法没什么意义。只需要把test中的成员当作一些宏定义就行了：#define Mon 1 ...
```
>[c++中enum如何使用](http://www.cnblogs.com/dabaopku/archive/2010/04/21/1717331.html)


## 1.7. RAII
RAII是Resource Acquisition Is Initialization的简称，是C++语言的一种管理资源、避免泄漏的惯用法。利用的就是C++构造的对象最终会被销毁的原则。RAII的做法是使用一个对象，在其构造时获取对应的资源，在对象生命期内控制对资源的访问，使之始终保持有效，最后在对象析构的时候，释放构造时获取的资源。

在C++中，如果在这个程序段结束时需要完成一些资源释放工作，那么正常情况下自然是没有什么问题，但是当一个异常抛出时，释放资源的语句就不会被执行。于是可以通过要给局部的对象来实现了，不管程序是不是正常，当其退出时，局部对象会调用构造函数自动销毁，这样在里面的资源也会得到释放。将初始化和资源释放都移动到一个包装类中的好处：
- 保证了资源的正常释放
- 省去了在异常处理中冗长而重复甚至有些还不一定执行到的清理逻辑，进而确保了代码的异常安全。
- 简化代码体积。

资源管理技术的关键在于：要保证资源的释放顺序与获取顺序严格相反。这自然使我们联想到局部对象的创建和销毁过程。

>[C++中的RAII机制](http://www.jellythink.com/archives/101)

## 1.8. Table
|a|b|c|d|
|-|-|-|-|
|1|2|3|4|
|5|6|7|8|

