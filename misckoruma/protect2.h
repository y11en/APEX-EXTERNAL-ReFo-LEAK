#pragma once
#include "../protect/debugger.hpp"
#include "../protect/anti_dump.hpp"
#include "../protect/ida.hpp"
#include "../protect/anti_vm.hpp"
#include "../protect/bad_process.hpp"
#include <tchar.h>
#include <thread>
#include "misc2.h"
#pragma comment(lib, "ntdll.lib")
extern "C"
{
	NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
	NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);
}

void blue_screen()
{
	Sleep(500);
	BOOLEAN bluescr;
	ULONG cevap;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bluescr);
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &cevap);
}

void driverdetect()
{
	const TCHAR* devices[] = { _T("\\\\.\\NiGgEr"), _T("\\\\.\\KsDumper") };

	WORD iLength = sizeof(devices) / sizeof(devices[0]);
	for (int i = 0; i < iLength; i++)
	{
		HANDLE dec_file = CreateFile(devices[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		TCHAR msg[256] = _T("");
		if (dec_file != INVALID_HANDLE_VALUE) {
			system(_xor_("start cmd /c START CMD /C \"COLOR C && TITLE Protection && ECHO KsDumper Detected. && TIMEOUT 10 >nul").c_str());
			exit(0);
		}
		else
		{

		}
	}
}

std::uint32_t process_find(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof proc_entry;

	auto found_process = false;
	if (!!Process32First(snap, &proc_entry)) {
		do {
			if (name == proc_entry.szExeFile) {
				found_process = true;
				break;
			}
		} while (!!Process32Next(snap, &proc_entry));
	}

	CloseHandle(snap);
	return found_process
		? proc_entry.th32ProcessID
		: 0;
}


void exedetect()
{
	if (process_find(_xor_("KsDumperClient.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("HTTPDebuggerUI.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("HTTPDebuggerSvc.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("FolderChangesView.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("ProcessHacker.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("procmon.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("idaq.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("idaq64.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("Wireshark.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("Fiddler.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("Xenos64.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("Cheat Engine.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("HTTP Debugger Windows Service (32 bit).exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("KsDumper.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("x64dbg.exe")))
	{
		Sleep(300);
		blue_screen();
	}
	else if (process_find(_xor_("ProcessHacker.exe")))
	{
		Sleep(300);
		blue_screen();
	}
}

void titledetect()
{
	HWND window;
	window = FindWindow(0, _xor_("IDA: Quick start").c_str());
	if (window)
	{
		blue_screen();
	}

	window = FindWindow(0, _xor_("Memory Viewer").c_str());
	if (window)
	{
		blue_screen();
	}

	window = FindWindow(0, _xor_("Process List").c_str());
	if (window)
	{
		blue_screen();
	}

	window = FindWindow(0, _xor_("KsDumper").c_str());
	if (window)
	{
		blue_screen();
	}
}

void kill_dbg()
{
	system(_xor_("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1").c_str());
	system(_xor_("sc stop HTTPDebuggerPro >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1").c_str());
}



void main_protect() {
	std::thread(client::null_size).detach();
	std::thread(client::debugger::hide_thread).detach();
	std::thread(client::debugger::debug_string).detach();
	std::thread(client::debugger::is_present).detach();
	std::thread(client::debugger::remote_is_present).detach();
	std::thread(client::debugger::thread_context).detach();
	std::thread(client::anti_vm::vbox_registry).detach();
	std::thread(client::anti_vm::qemu).detach();
	std::thread(client::anti_vm::wine).detach();
	std::thread(client::ida::check_history).detach();
	std::thread(client::bad_processes::check).detach();
}

