GitHub项目地址：https://github.com/HarmoniaLeo/MFC-QQbot

## 0x00 这是什么？

欢迎使用本QQ机器人开发框架（如果有人会用的话）。本框架是主要使用MFC中的Windows API制作而成、利用Windows的消息机制以及一些系统级底层架构实现利用TIM客户端自动收发QQ消息的QQ机器人框架，用其制作的QQ机器人程序已在VS2017下编译通过（仅支持ANSI字符集）并可正常运行。

在框架开发时主要考虑了其拓展性，通过拓展该框架，可以实现包括自动回复、定时推送、发送图片、调用其他进程并进行通信甚至连接MySQL的功能，您不仅可以用本框架制作常规的群聊机器人，还可以让QQ机器人成为你访问各种程序的接口，这也是本QQ机器人框架开发的初衷。

本QQ机器人框架由HarmoniaLeo完全独立开发，部分代码参考CSDN论坛和各种博客，在此向所有为本框架诞生提供帮助的前辈们表达真诚的谢意。



## 0x01 我能做出什么样的QQ机器人？



### 1.自动回复

自动回复的本质是通过接收消息记录中特定模式的消息调用对应的CString类函数，特定模式的消息指形如“口令 参数1 参数2……”的消息。调用函数后返回的CString result将会以**@发送消息的用户名 result** 的形式被机器人发送到对应的会话窗口。参数个数需要与函数配置时申请的参数个数一致，若过少则不会回复，若过多则最后一个参数将是带 空 格的字符串（函数的配置会在0x03中介绍）。


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319180726577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzQ0MTc0Mg==,size_16,color_FFFFFF,t_70)


### 2.订阅与定时推送

**functions.cpp**中预置有函数**CString subscribe(CString* valuelist,HWND hwnd,CString caller)**，对应口令为“订阅”，当会话中出现形如**订阅 口令名[(参数1,参数2……)] YYYY:MM:DD:HH:MM:SS [Nd][Nh][Nm][Ns]**（[]内的内容可以省略）的消息时，机器人会自动记录该订阅，回复“@用户名 订阅成功：口令名”，并根据YYYY:MM:DD:HH:MM:SS和[Nd][Nh][Nm][Ns]这两个参数来定时调用口令名对应的函数，并回复消息。假如这个口令名没有对应函数，则机器人会回复该口令原本的内容。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319180845472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzQ0MTc0Mg==,size_16,color_FFFFFF,t_70)

举个例子：我想要订阅口令名为“复读”的函数，想要从1919年11月4日5点14分开始每隔8小时1秒调用一次，我可以在会话中回复“订阅 复读 1919:11:4:5:14:0 8h1s”，如果我想从现在开始调用函数，则YYYY:MM:DD:HH:MM:SS参数可以被替换为-1，如果我只想让它推送一次该函数，之后就将这个订阅删除，我可以把[Nd][Nh][Nm][Ns]参数替换为-1。如果“复读”没有对应函数，那么它就会从1919年11月4日5点14分开始每隔8小时1秒回复一次“复读”。

此外，**functions.cpp**中预置有函数**CString checkscribe(CString* valuelist,HWND hwnd,CString caller)**和**CString unscribe(CString\* valuelist,HWND hwnd,CString caller)**，对应的消息分别为“查询订阅”和“取消订阅 口令名”。试试看吧！

和订阅功能有关的外部储存文件会被储存在和.exe文件相同目录下的“订阅”文件夹下，其储存采用.txt方式。

由于该框架的高度可拓展性，您甚至可以制作出终端类的QQ机器人，这可以通过本框架提供的API实现。


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319180916646.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzQ0MTc0Mg==,size_16,color_FFFFFF,t_70)


## 0x02 听起来不错，那么我怎么开始呢？

首先您需要的是**本项目文件夹中的所有文件，还需要一台装有Windows系统的服务器，以及一个TIM客户端**。

我们接下来以阿里云的Windows Server 2012 R2为例进行介绍。

Step1：打开TIM客户端，登录机器人账号，将需要回复的会话窗口拖出嵌入式的对话框，并将其全屏（注意不要最小化）。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319180941840.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzQ0MTc0Mg==,size_16,color_FFFFFF,t_70)

Step2：测出会话窗口聊天区中4个安全点的x、y坐标（就是点击后只是把焦点设置在消息列表，而不会执行其他操作）（可以使用 https://github.com/HarmoniaLeo/Windows-Screen-Ruler 提供的工具测量），这个坐标主要受windows服务器内部设置的屏幕分辨率影响。将其填入**functions.cpp**中的**extern** **const** **int** **position[8][2]** 数组的初始化值中。如果您使用的是阿里云的Windows Server 2012 R2，则使用默认值即可。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319181000532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MzQ0MTc0Mg==,size_16,color_FFFFFF,t_70)
```c
const int position[8][2] = { {43,121},{970,121},{43,344},{970,344} };//更改四个定位点的坐标
```


Step3：在**functions.cpp**中的**extern const CString windowList[windowNum]** 的初始化值中填入所有要回复的会话窗口的标题，并将**QQbot3.h**中的宏**windowNum**改为会话窗口的总数目。

```c
const CString windowList[windowNum] = { /*以"会话名1","会话名2"……的方式添加会话名*/};
#define windowNum 2//更改窗口数目
```


## 0x03 如何为QQ机器人配置函数呢？

Step1：在**functions.cpp**中的指定位置，以**CString 函数名\(CString\* valuelist,HWND hwnd,CString caller\)** 为函数头添加函数。



函数的参数介绍：

**CString\* valuelist**：参数列表，全部使用MFC内建的CString类来传入参数，用户通过回复“口令 参数1 参数2……”这样的消息将参数传入函数，您可以在函数中对这些参数进行处理并返回不同的结果。

**HWND hwnd**：窗口句柄，Windows API中的概念，根据QQ机器人响应的会话窗口不同而有所不同，通过这个参数可以用来调用本框架提供的各种API。

**CString caller**：调用该函数的用户名。

函数的返回值：

若函数返回了一个CString类字符串，那么调用函数后返回的**CString result**将会以**@发送消息的用户名 result** 的形式被机器人发送到对应的会话窗口。若返回了空串，则机器人不会执行任何动作，可以用来拒绝参数格式不正确的输入。

Step2：在**functions.cpp**中的**void funcInit()**函数中加入语句：**funclist[函数编号（从4开始（0-3为预留））]=&函数名;**
```c
void funcInit()
{
	funclist[0] = &listCommands;
	funclist[1] = &subscribe;
	funclist[2] = &unscribe;
	funclist[3] = &checkscribe;
	//初始化自定义函数
}
```
Step3：在**functions.cpp**中的**extern const CString functions[fnum]** 的初始化值中函数编号对应的位置填入需要响应的口令名（也就是用户用来调用该函数的口令）
```c
const CString functions[fnum] = { "查询命令","订阅","取消订阅","查询订阅"/*以"口令名1","口令名2"……的方式添加口令名*/ };
```


Step4：在**functions.cpp**中的**extern const int values[fnum]** 的初始化值中函数编号对应的位置填入申请的参数个数
```c
const int values[fnum] = { 0,3,1,0/*以"参数数目1","参数数目2"……的方式添加参数数目*/ };
```


Step5：将**QQbot3.h**中的宏**fnum**改为函数的总数目。
```c
#define fnum 19//更改函数数目
```


## 0x04 框架为我提供了哪些API呢？

1.针对一些对于MFC并不是特别熟悉的用户，也为了开发方便起见，以下列出该框架提供的基础API列表：
```c
CString getTimeStamp()//获取当前时间用Y/M/D H:M:S表示的时间戳（TIM消息记录中时间戳的格式 同时也是本框架在通过文件保存时间戳时的标准形式）

void timeATOI(int* time,CString TMP)//将用Y/M/D H:M:S表示的时间戳TMP转换为一个长度至少为6的int型数组anwser

void periodTOI(CString period,int* anwser)//将用NyNmNdNhNmNs（N表示对应数字）表示的时间戳period转换为一个长度至少为6的int型数组anwser（为用户提供更方便的时间输入方式）

CString turnTimeToAddForm(CString target)//将用Y:M:D:H:M:S表示的时间戳target转换为用Y/M/D H:M:S表示的时间戳（为用户提供更方便的时间输入方式）

CString timeAdd(CString TMP,CString period)//将用Y/M/D H:M:S表示的时间戳TMP和用NyNmNdNhNmNs（N表示对应数字）表示的时间戳period相加并输出新的用Y:M:D H:M:S表示的时间戳

CString cutLeft(CString source,CString obj)//获取source字符串第一个obj字符串以前（不包括obj）的字符串

CString cutRightBI(CString source,int index)//获取source字符串index以后（不包括index）的字符串

CString cutRight(CString source,char obj)//获取source字符串最后一个obj字符以后（不包括obj）的字符串

CString getTitle(HWND hwnd)//获取窗口标题

void exec(CString route);//启动外部程序（route为该程序路径以及该程序的命令行参数，用空格分隔）（可以用来执行python脚本等）

int openFile(CString dir,vector<CString>* list)//打开或创建dir所表示的文件，并分行读入所有内容到一个CString类的vector里，返回读入的行数

void writeFile(CString dir,vector<CString> list,int count)//将一个CString类的vector里的前count行内容分行写入dir所示的文件中
```

2.针对一些想要丰富消息发送方式的用户，用户可以在配置函数时随时使用以下API：
```c
CString get(HWND hwnd)//获取会话窗口的消息记录

void pasteMessage(CString source,HWND hwnd)//将source中的消息填入消息发送栏

void toClipboard(CString source,HWND hwnd)//发送source中的消息

void pasteIMG(CString path,HWND hwnd)//将文件路径path所指向的bmp图片复制到消息发送栏

void send(HWND hwnd)//发送消息发送栏里面填入的消息
```

3.针对想要开发终端交互式QQ机器人程序的用户，本框架提供以下API：
```c
aTime getATime()//获取当前时间，需要在终端交互函数的开始初始化时间，用aTime类保存，并在之后将aTime类对象的指针传入lastSpeak以响应特定用户最后发送的消息

CString lastSpeak(CString source,CString caller,aTime* last,HWND hwnd)//获取消息记录source中昵称为caller的用户最后发送的消息，获取失败则返回空串，last是一个指向aTime的指针。可以通过一个while循环，判断返回值是否为空，来等待特定用户的输入
```
实际上终端交互式QQ机器人程序就是让机器人响应一个长周期的函数，利用lastSpeak不断接收特定用户的消息，再利用2中所示的函数即时地发送消息罢了。



## 0x05 上面这些我都做完了，还需要做什么呢？

做完了以上这些，在MFC框架支持下建立控制台程序，编译项目，将生成的.exe文件和（如果使用了MySQL API的话）上传至服务器。运行之后就完成属于你的QQ机器人了！


