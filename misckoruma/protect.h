#pragma once

#include <stdlib.h>
#include <TlHelp32.h>
#include <string>
#include <filesystem>
#include "../þifreleme/xor.h"
#include <debugapi.h>
#include <WinUser.h>
#include <synchapi.h>
#include "misc2.h"
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <consoleapi3.h>
#include <WinBase.h>
#include <consoleapi2.h>
using namespace std;
char title[100];
void __fastcall DebugChecker()
{
	if (IsDebuggerPresent())
	{
		exit(1);
	}
}





void getrandomnumber(char* sStr, unsigned int iLen)
{
	char Syms[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned int Ind = 0;
	srand(time(NULL) + rand());
	while (Ind < iLen)
	{
		sStr[Ind++] = Syms[rand() % 62];
	}
	sStr[iLen] = '\0';
}

std::string path()
{
	char shitter[_MAX_PATH];
	GetModuleFileNameA(NULL, shitter, _MAX_PATH);
	return std::string(shitter);
}

std::string randomstringtorenameexe(const size_t length)
{
	std::string r;
	static const char bet[] = { "abcdefghijklmnopqrstuvwxyzz" };
	srand((unsigned)time(NULL) * 5);
	for (int i = 0; i < length; ++i)
		r += bet[rand() % (sizeof(bet) - 1)];
	return r;
}


void __fastcall goster() {
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void __fastcall gizle() {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void duzen21()
{
	getrandomnumber(title, 32);
	SetConsoleTitle(title);
	int horizontal = 0, vertical = 0;
	int x = 400, y = 400;
	HWND hwnd = GetConsoleWindow();
	MoveWindow(hwnd, 0, 0, x, y, FALSE);
	SetWindowPos(hwnd, HWND_TOPMOST, 10, 10, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
	lStyle &= ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLong(hwnd, GWL_STYLE, lStyle);
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);
	COORD scrollbar = {
		csbi.srWindow.Right - csbi.srWindow.Left + 1,
		csbi.srWindow.Bottom - csbi.srWindow.Top + 1
	};
	if (console == 0)
		throw 0;
	else
		SetConsoleScreenBufferSize(console, scrollbar);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, (255 * 85) / 100, LWA_ALPHA);
}


bool __fastcall IsProcessRun(const char* const processName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	while (1) {
		if (strcmp(pe.szExeFile, processName) == 0) return true;
		if (!Process32Next(hSnapshot, &pe)) return false;
	}
}






bool __fastcall build_date()
{
	return __DATE__;
}

bool __fastcall build_time()
{
	return __TIME__;
}
