#pragma once

#include "resource.h"
#include <afxdb.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <iostream>

using namespace std;

#define fnum 19//更改函数数目
#define TIME 100
#define plContent 4
#define secNum 11
#define windowNum 2//更改窗口数目
extern const CString functions[fnum];
extern const int values[fnum];
extern CString(*funclist[fnum])(CString*, HWND, CString);
extern const int position[8][2];
extern const CString windowList[windowNum];

void funcInit();
CString cutRight(CString, char);
CString cutRightBI(CString, int);
CString cutLeft(CString, CString);
CString timeAdd(CString, CString);
void periodTOI(CString, int*);
void toClipboard(CString, HWND);
void toClipboardForPython(CString, HWND);
CString unscribe(CString*, HWND, CString);
void exec(CString);
int openFile(CString, vector<CString>*);
void writeFile(CString, vector<CString>, int);
CString getTitle(HWND hwnd);

class aTime
{
public:
	int year, month, day, hour, min, sec;

	aTime()
	{
		year = 1970;
		month = 1;
		day = 1;
		hour = 0;
		min = 0;
		sec = 0;
	}

	void renew(CString time)
	{
		year = _ttoi(cutLeft(time, "/"));
		time = cutRightBI(time, time.Find(_T("/")));
		month = _ttoi(cutLeft(time, "/"));
		time = cutRightBI(time, time.Find(_T("/")));
		day = _ttoi(cutLeft(time, " "));
		time = cutRightBI(time, time.Find(_T(" ")));
		hour = _ttoi(cutLeft(time, ":"));
		time = cutRightBI(time, time.Find(_T(":")));
		min = _ttoi(cutLeft(time, ":"));
		time = cutRightBI(time, time.Find(_T(":")));
		sec = _ttoi(time);
	}

	bool isOBI(aTime* alter)
	{
		if (year > (alter->year))
			return 1;
		else if (year < (alter->year))
			return 0;
		else if (year == (alter->year))
		{
			if (month > (alter->month))
				return 1;
			else if (month < (alter->month))
				return 0;
			else if (month == (alter->month))
			{
				if (day > (alter->day))
					return 1;
				else if (day < (alter->day))
					return 0;
				else if (day == (alter->day))
				{
					if (hour > (alter->hour))
						return 1;
					else if (hour < (alter->hour))
						return 0;
					else if (hour == (alter->hour))
					{
						if (min > (alter->min))
							return 1;
						else if (min < (alter->min))
							return 0;
						else if (min == (alter->min))
						{
							if (sec > (alter->sec))
								return 1;
							else
								return 0;
						}
					}
				}
			}
		}
		return 0;
	}

	bool equal(aTime* alter)
	{
		if ((year == alter->year) && (month == alter->month) && (day == alter->day) && (hour == alter->hour) && (min == alter->min) && (sec == alter->sec))
			return 1;
		else
			return 0;
	}

	void change(aTime* alter)
	{
		year = alter->year;
		month = alter->month;
		day = alter->day;
		hour = alter->hour;
		min = alter->min;
		sec = alter->sec;
	}

	CString printTime()
	{
		CString yearSTR, monthSTR, daySTR, hourSTR, minSTR, secSTR;
		yearSTR.Format(_T("%d"), year);
		monthSTR.Format(_T("%d"), month);
		daySTR.Format(_T("%d"), day);
		hourSTR.Format(_T("%d"), hour);
		minSTR.Format(_T("%d"), min);
		secSTR.Format(_T("%d"), sec);
		return yearSTR + "/" + monthSTR + "/" + daySTR + " " + hourSTR + ":" + minSTR + ":" + secSTR;
	}
};

aTime getATime();
CString getTimeStamp();
CString lastSpeak(CString, CString, aTime*, HWND);
CString get(HWND);
void send(HWND);
void pasteIMG(CString, HWND);
void pasteMessage(CString, HWND);
CString turnTimeToAddForm(CString);

CString manaInterface(CString*, HWND, CString);