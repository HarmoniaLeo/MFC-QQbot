// QQbot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "QQbot.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

bool flag = 0;
aTime last[windowNum];

CString cutRight(CString source, char obj)
{
	return source.Right(source.GetLength() - source.ReverseFind(obj) - 1);
}

CString cutRightBI(CString source, int index)
{
	return source.Right(source.GetLength() - index - 1);
}

CString cutLeft(CString source, CString obj)
{
	return source.Left(source.Find(obj, 0));
}

CString getTitle(HWND hwnd)
{
	char windowname[999];
	GetWindowText(hwnd, (LPTSTR)windowname, 999);
	CString windowName;
	windowName.Format(_T("%s"),windowname);
	CString title;
	title.Format(_T("%s"), windowname);
	return title;
}

int openFile(CString dir, vector<CString>* list)
{
	CStdioFile file;
	char dir2[1024];
	strcpy(dir2, dir);
	USES_CONVERSION;
	if (!file.Open(A2T(dir2), CFile::modeRead))
		file.Open(A2T(dir2), CFile::modeCreate | CFile::modeRead);
	CString tmp;
	int count = 0;
	while (true)
	{
		file.ReadString(tmp);
		if (tmp.IsEmpty())
			break;
		else
		{
			list->push_back(tmp);
			count++;
		}
	}
	file.Close();
	return count;
}

void writeFile(CString dir, vector<CString> list, int count)
{
	CStdioFile file;
	char dir2[1024];
	strcpy(dir2, dir);
	USES_CONVERSION;
	file.Open(dir2, CFile::modeCreate | CFile::modeWrite);
	for (int i = 0; i < count; i++)
		file.WriteString(list[i] + "\n");
	file.Close();
}

void exec(CString route)
{
	STARTUPINFO stStartUpInfo = { sizeof(stStartUpInfo) };
	PROCESS_INFORMATION pProcessInfo;
	ZeroMemory(&stStartUpInfo, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow = 1;
	char tmp[256];
	strcpy(tmp, route);
	CreateProcess(NULL, tmp, NULL, NULL, 0, NULL, NULL, NULL, &stStartUpInfo, &pProcessInfo);
}

CString getTimeStamp()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	CString timeNow;
	timeNow.Format(_T("%d/%d/%d %d:%d:%d"), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	return timeNow;
}

aTime getATime()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	CString timeNow;
	timeNow.Format(_T("%d/%d/%d %d:%d:%d"), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	aTime last;
	last.renew(timeNow);
	return last;
}

CString turnTimeToAddForm(CString target)
{
	if (target == "-1")
		return "-1";
	if (target == "0")
		return "0";
	CString target1 = target, tmp;
	int total = 0;
	if (target.Find(_T(":")) < 0)
		return "";
	total += target.Find(_T(":"));
	target = cutRightBI(target, target.Find(_T(":")));
	tmp = target1.Left(total) + "/";
	target1 = cutRightBI(target1, total);
	target1 = tmp + target1;
	total++;
	if (target.Find(_T(":")) < 0)
		return "";
	total += target.Find(_T(":"));
	target = cutRightBI(target, target.Find(_T(":")));
	tmp = target1.Left(total) + "/";
	target1 = cutRightBI(target1, total);
	target1 = tmp + target1;
	total++;
	if (target.Find(_T(":")) < 0)
		return "";
	total += target.Find(_T(":"));
	target = cutRightBI(target, target.Find(_T(":")));
	tmp = target1.Left(total) + " ";
	target1 = cutRightBI(target1, total);
	target1 = tmp + target1;
	total++;
	for (int i = 0; i < 2; i++)
	{
		if (target.Find(_T(":")) < 0)
			return "";
		target = cutRightBI(target, target.Find(_T(":")));
	}
	if (target.IsEmpty())
		return "";
	return target1;
}

void periodTOI(CString period, int* anwser)
{
	CString sym[6] = { "y","m","d","h","m","s" };
	int buf = 2;
	for (int i = 0; i < period.GetLength(); i++)
	{
		if (period[i] >= 48 && period[i] <= 57)
			continue;
		else
		{
			while (true)
			{
				if (period[i] == sym[buf])
				{
					anwser[buf] = _ttoi(cutLeft(period, sym[buf]));
					period = cutRightBI(period, period.Find(sym[buf]));
					break;
				}
				buf++;
				if (buf == 6)
				{
					anwser[0] = -1;
					return;
				}
			}

		}
		i = 0;
	}
}

void timeATOI(int* time, CString TMP)
{
	int target;
	CString CTMP;
	for (int i = 0; i < 2; i++)
	{
		target = TMP.Find(_T("/"), 0);
		CTMP = TMP.Left(target);
		time[i] = _ttoi(CTMP);
		TMP = TMP.Right(TMP.GetLength() - target - 1);
	}
	target = TMP.Find(_T(" "), 0);
	CTMP = TMP.Left(target);
	time[2] = _ttoi(CTMP);
	TMP = TMP.Right(TMP.GetLength() - target - 1);
	for (int i = 3; i < 5; i++)
	{
		target = TMP.Find(_T(":"), 0);
		CTMP = TMP.Left(target);
		time[i] = _ttoi(CTMP);
		TMP = TMP.Right(TMP.GetLength() - target - 1);
	}
	time[5] = _ttoi(TMP);
}

CString timeAdd(CString TMP, CString period)
{
	int time[6], per[6], tmp1, tmp2;
	memset(per, 0, sizeof(per));
	timeATOI(time, TMP);
	periodTOI(period, per);
	tmp1 = (time[5] + per[5]) / 60;
	time[5] = (time[5] + per[5]) % 60;
	tmp2 = (time[4] + per[4] + tmp1) / 60;
	time[4] = (time[4] + per[4] + tmp1) % 60;
	tmp1 = (time[3] + per[3] + tmp2) / 24;
	time[3] = (time[3] + per[3] + tmp2) % 24;
	tmp1 += per[2];
	for (int i = 0; i < tmp1; i++)
	{
		time[2]++;
		if (time[1] == 1 || time[1] == 3 || time[1] == 5 || time[1] == 7 || time[1] == 8 || time[1] == 10 || time[1] == 12)
		{
			if (time[2] == 32)
			{
				time[2] = 1;
				time[1]++;
			}
		}
		else
		{
			if (time[1] == 2)
			{
				if (time[0] % 4 == 0 || time[0] % 400 == 0)
				{
					if (time[2] == 30)
					{
						time[2] = 1;
						time[1]++;
					}
				}
				else
				{
					if (time[2] == 29)
					{
						time[2] = 1;
						time[1]++;
					}
				}
			}
			else
			{
				if (time[2] == 31)
				{
					time[2] = 1;
					time[1]++;
				}
			}
		}
		if (time[1] == 13)
		{
			time[1] = 1;
			time[0]++;
		}
	}
	CString anwser;
	TMP.Format(_T("%d/"), time[0]);
	anwser = TMP;
	for (int j = 1; j < 6; j++)
	{
		if (j < 2)
			TMP.Format(_T("%d/"), time[j]);
		else if (j == 2)
			TMP.Format(_T("%d "), time[j]);
		else if (j > 2 && j <= 4)
			TMP.Format(_T("%d:"), time[j]);
		else
			TMP.Format(_T("%d"), time[j]);
		anwser += TMP;
	}
	return anwser;
}

void click(HWND hwnd, int no)
{
	POINT point;
	point.x = position[no][0];
	point.y = position[no][1];
	ScreenToClient(hwnd, &point);
	for (int i = 0; i < 4; i++)
	{
		point.x = position[i % 4][0];
		point.y = position[i % 4][1];
		ScreenToClient(hwnd, &point);
		SendMessage(hwnd, WM_LBUTTONDOWN, (WPARAM)1, (point.y << 16) + point.x);
		Sleep(TIME);
		SendMessage(hwnd, WM_LBUTTONUP, 0, (point.y << 16) + point.x);
		Sleep(TIME);
	}

}

void pasteIMG(CString path, HWND hwnd)
{
	while (!OpenClipboard(NULL)) {}

	BOOL bRet = FALSE;
	HBITMAP hBitmap = NULL;
	BOOL bClipboardOpened = FALSE;
	hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	bClipboardOpened = TRUE;
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	bRet = TRUE;
	Sleep(TIME * 5);
	CloseClipboard();
	if (hBitmap)
	{
		DeleteObject(hBitmap);
		hBitmap = NULL;
	}
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYDOWN, (WPARAM)0x56, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, (WPARAM)0x56, 0);
	Sleep(TIME);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(TIME);
	pasteMessage("\n", hwnd);
}

void send(HWND hwnd)
{
	SendMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, VK_RETURN, 0);
	Sleep(TIME * 10);
}

void pasteMessage(CString source, HWND hwnd)
{
	while (!OpenClipboard(NULL)) {}
	HGLOBAL clipbuffer;
	char *buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength() + 1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, source);
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT, clipbuffer);
	CloseClipboard();
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYDOWN, (WPARAM)0x56, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, (WPARAM)0x56, 0);
	Sleep(TIME);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(TIME);
}

void toClipboard(CString source, HWND hwnd)
{
	while (!OpenClipboard(NULL)) {}
	HGLOBAL clipbuffer;
	char *buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength() + 1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, source);
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT, clipbuffer);
	CloseClipboard();
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYDOWN, (WPARAM)0x56, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, (WPARAM)0x56, 0);
	Sleep(TIME);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(TIME);
	send(hwnd);
}

CString get(HWND hwnd)
{
	click(hwnd, 0);
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYDOWN, (WPARAM)0x41, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, (WPARAM)0x41, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYDOWN, (WPARAM)0x43, 0);
	Sleep(TIME);
	SendMessage(hwnd, WM_KEYUP, (WPARAM)0x43, 0);
	Sleep(TIME);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	CString fClipboard;
	char * buffer = NULL;
	while (!OpenClipboard(NULL)) {}
	HANDLE hData = GetClipboardData(CF_TEXT);
	buffer = (char*)GlobalLock(hData);
	if (buffer != NULL)
		fClipboard.Format(_T("%s"), buffer);
	else
		fClipboard = "";
	GlobalUnlock(hData);
	EmptyClipboard();
	CloseClipboard();
	return fClipboard;
}

CString call(CString message, int index, HWND hwnd, CString caller)
{
	CString* valuelist;
	int count = 0;
	bool normal = 1;
	valuelist = new CString[values[index]];
	message = cutRightBI(message, message.Find(_T(" ")));
	if (message.IsEmpty())
		normal = 0;
	for (int i = 0; i < values[index] - 1; i++)
	{
		if (message.Find(_T(" ")) < 0)
			normal = 0;
		else
		{
			valuelist[count] = message.Left(message.Find(_T(" ")));
			message = cutRightBI(message, message.Find(_T(" ")));
			count++;
		}
	}
	if (values[index] > 0)
		valuelist[count] = message.Left(message.GetLength() - 1);
	if (normal)
		return funclist[index](valuelist, hwnd, caller);
	else
		return "";
}

int map(CString message)
{
	for (int i = 0; i < fnum; i++)
	{
		if (((message.Find(functions[i]) == 0) && (message.Find(_T(" ")) == functions[i].GetLength())) || (message.Left(message.GetLength() - 1) == functions[i]))
			return i;
	}
	return -1;
}

CString lastSpeak(CString source, CString caller, aTime* last, HWND hwnd)
{
	CString message, message2, name, response, message1, name1;
	aTime now1, now;
	source = source.Left(source.ReverseFind('\n'));
	while (source.ReverseFind('\n') >= 0)
	{
		message = cutRight(source, '\n');
		source = source.Left(source.ReverseFind('\n'));
		message2 = cutRight(source, '\n');
		source = source.Left(source.ReverseFind('\n'));
		name = cutLeft(message2, " ");
		now.renew(message2.Right(message2.GetLength() - message2.Find(_T(" ")) - 1));
		if (caller == name)
		{
			if (!now.isOBI(last))
			{
				if (!(now1.year == 1970))
				{
					last->change(&now1);
					return message1.Left(message1.GetLength() - 1);
				}
				else
					return "";
			}
			else
			{
				message1 = message;
				name1 = name;
				now1.renew(message2.Right(message2.GetLength() - message2.Find(_T(" ")) - 1));
			}
		}
	}
	if (!(now1.year == 1970))
	{
		last->change(&now1);
		return message1.Left(message1.GetLength() - 1);
	}
	return "";
}

void pip(CString source, int id, HWND hwnd)
{
	CString message, message2, name, response;
	int callback;
	aTime now, last1;
	bool first = 1;
	while (source.ReverseFind('\n') >= 0)
	{
		message = cutRight(source, '\n');
		source = source.Left(source.ReverseFind('\n'));
		callback = map(message);
		if (callback >= 0)
		{
			message2 = cutRight(source, '\n');
			source = source.Left(source.ReverseFind('\n'));
			name = cutLeft(message2, " ");
			now.renew(message2.Right(message2.GetLength() - message2.Find(_T(" ")) - 1));
			if (!now.isOBI(&last[id]))
				break;
			else
			{
				if (first)
				{
					last1.change(&now);
					first = 0;
				}
				response = call(message, callback, hwnd, name);
				if (!response.IsEmpty())
					pasteMessage("@" + name + " " + response + "\n", hwnd);
			}
		}
	}
	if (!first && (last1.year != 1970))
		last[id].change(&last1);
	send(hwnd);
}


void put(HWND hwnd)
{
	vector<CString> content;
	CString tmp, windowName;
	char windowname[999];
	int count = 0, id = 0;
	windowName = getTitle(hwnd);
	count = openFile(".\\订阅\\" + windowName + ".txt", &content);
	aTime now, target;
	now = getATime();
	for (int i = 2; i < count; i += plContent)
	{
		if (content[i] != "-1")
		{
			target.renew(content[i]);
			if (now.isOBI(&target))
			{
				id = map(content[i - 2]);
				if (id == -1)
					toClipboard("@" + content[i - 1] + " " + content[i - 2], hwnd);
				else
				{
					tmp = call(content[i - 2], id, hwnd, content[i - 1]);
					if (!tmp.IsEmpty())
						toClipboard("@" + content[i - 1] + " " + tmp, hwnd);
				}
				if (content[i + 1] != "-1")
					content[i] = timeAdd(content[i], content[i + 1]);
				else
				{
					for (int k = i - 2; k < count - plContent; k++)
						content[k] = content[k + plContent];
					for (int j = 0; j < plContent; j++)
						content.pop_back();
					count -= plContent;
				}
			}
		}
	}
	writeFile(".\\订阅\\" + windowName + ".txt", content, count);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, DWORD id)
{
	char szTitle[255];
	setlocale(LC_ALL, "");
	GetWindowText(hwnd, (LPTSTR)szTitle, sizeof(szTitle));
	CString str;
	str.Format(_T("%s"), szTitle);
	if (str == windowList[id%windowNum])
	{
		if (id > windowNum * 3 - 1)
			put(hwnd);
		else
			pip(get(hwnd), id%windowNum, hwnd);
	}
	return 1;
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 在此处为应用程序的行为编写代码。
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
			funcInit();
			int count = 0;
			aTime timeNow;
			timeNow = getATime();
			for (int a = 0; a < windowNum; a++)
				last[a] = timeNow;
			while (flag != 1)
			{
				for (int i = 0; i < windowNum * 4; i++)
					EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)i);
			}
        }
    }
    else
    {
        // TODO: 更改错误代码以符合需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }

    return nRetCode;
}
