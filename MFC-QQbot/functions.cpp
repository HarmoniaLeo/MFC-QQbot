#include "pch.h"
#include "QQbot.h"

#ifdef _DEBUG

using namespace std;

const CString windowList[windowNum] = { /*以"会话名1","会话名2"……的方式添加会话名*/};

const CString functions[fnum] = { "查询命令","订阅","取消订阅","查询订阅"/*以"口令名1","口令名2"……的方式添加口令名*/ };
const int values[fnum] = { 0,3,1,0/*以"参数数目1","参数数目2"……的方式添加参数数目*/ };
CString(*funclist[fnum])(CString*, HWND, CString);
const int position[8][2] = { {43,121},{970,121},{43,344},{970,344} };//更改四个定位点的坐标

/*
添加函数的区域
*/

CString checkscribe(CString* valuelist, HWND hwnd, CString caller)
{
	bool suc = 0;
	vector<CString> content;
	CString windowName, ret;
	int count = openFile(".\\订阅\\" + windowName + ".txt", &content);
	windowName = getTitle(hwnd);
	for (int i = 0; i < count; i += plContent)
		ret += cutLeft(content[i], " ") + "\n";
	return "目前的订阅有：\n" + ret;
}

CString unscribe(CString* valuelist, HWND hwnd, CString caller)
{
	bool suc = 0;
	vector<CString> content;
	CString windowName;
	int count = openFile(".\\订阅\\" + windowName + ".txt", &content);
	windowName = getTitle(hwnd);
	for (int i = 0; i < count; i += plContent)
	{
		if ((cutLeft(content[i], " ") == valuelist[0]) && (content[i + 1] == caller))
		{
			for (int k = i; k < count - plContent; k++)
				content[k] = content[k + plContent];
			for (int j = 0; j < plContent; j++)
				content.pop_back();
			count -= plContent;
			suc = 1;
		}
	}
	writeFile(".\\订阅\\" + windowName + ".txt", content, count);
	if (suc)
		return "已经成功删除订阅：" + valuelist[0];
	else
		return "订阅未删除";
}


CString subscribe(CString* valuelist, HWND hwnd, CString caller)
{
	bool suc = 0;
	vector<CString> content;
	CString windowName;
	int count = openFile(".\\订阅\\" + windowName + ".txt", &content), count2 = 0;
	windowName = getTitle(hwnd);
	CString funcName;
	CString addlist[plContent];

	if (valuelist[0].Find(_T("(")) >= 0)
	{
		funcName = cutLeft(valuelist[0], "(");
		valuelist[0] = cutRightBI(valuelist[0], valuelist[0].Find(_T("(")));
		for (int i = 0; i < fnum; i++)
		{
			if (funcName == functions[i])
			{
				if (values[i] != 0)
				{
					addlist[0] = funcName;
					suc = 1;
					for (int i = 0; i < values[i] - 1; i++)
					{
						if (valuelist[0].Find(_T(",")) < 0)
						{
							return "订阅未成功";
						}
						else
						{
							addlist[0] += " " + valuelist[0].Left(valuelist[0].Find(_T(",")));
							valuelist[0] = cutRightBI(valuelist[0], valuelist[0].Find(_T(",")));
							count2++;
						}
					}
					addlist[0] += " " + valuelist[0].Left(valuelist[0].Find(_T(")")));
				}
				else
				{
					return "订阅未成功";
				}
			}
		}
		if (!suc)
			return "订阅未成功";
		suc = 0;
	}
	else
	{
		int i;
		for (i = 0; i < fnum; i++)
		{
			if (valuelist[0] == functions[i])
			{
				suc = 1;
				if (values[i] == 0)
					addlist[0] = valuelist[0] + " ";
				else
				{
					return "订阅未成功";
				}
			}
		}
		if (i == fnum)
		{
			addlist[0] = valuelist[0] + " ";
			suc = 1;
		}
		if (!suc)
			return "订阅未成功";
		suc = 0;
	}

	addlist[1] = caller;

	for (int i = 0; i < count; i += plContent)
	{
		if (content[i] == addlist[0] && content[i + 1] == addlist[1])
			return "订阅未成功";
	}

	aTime timeNow = getATime();
	int period[6];
	memset(period, 0, sizeof(period));
	CString addForm = turnTimeToAddForm(valuelist[1]);
	if (addForm == "")
		return "订阅未成功";
	else if (addForm == "-1")
		addlist[2] = "-1";
	else if (addForm == "0")
		addlist[2] = getTimeStamp();
	else
		addlist[2] = addForm;
	memset(period, 0, sizeof(period));
	periodTOI(valuelist[2], period);
	if (!(period[0] == -1))
		addlist[3] = valuelist[2];
	else if (valuelist[2] == "-1")
		addlist[3] = "-1";
	else
		return "订阅未成功";

	for (int i = 0; i < plContent; i++)
		content.push_back(addlist[i]);
	writeFile(".\\订阅\\" + windowName + ".txt", content, count);
	return "已经成功订阅：" + addlist[0];
}


CString listCommands(CString* valuelist, HWND hwnd, CString caller)
{
	CString commands;
	commands = "\n";
	for (int i = 0; i < fnum; i++)
		commands += functions[i] + "\n";
	return commands;
}

void funcInit()
{
	funclist[0] = &listCommands;
	funclist[1] = &subscribe;
	funclist[2] = &unscribe;
	funclist[3] = &checkscribe;
	//初始化自定义函数
}

#endif