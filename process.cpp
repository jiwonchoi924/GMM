#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

void KillProcess(TCHAR* TargetProcess) {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//현재 모든 프로세스들의 상태를 스냅

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf(("CreateToolhelp32Snapshot error! \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//프로세스 정보를 담을 구조체 선언

	//Process32First 시스템 스냅 샷에 기록된 첫번째 프로세스 정보 검색
	if (!Process32First(hProcessSnap, &pe32)) {
		printf(("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	HANDLE hProcess = NULL;
	BOOL isKill = FALSE;

	//Process32Next 다음 프로세스에 대한 정보 검색 계속
	do {
		if (_tcscmp(pe32.szExeFile, TargetProcess) == 0)   //종료 대상 프로세스를 찾았을 때
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);   //핸들값을 가져옴

		if (hProcess != NULL) {
			TerminateProcess(hProcess, -1);   //프로세스 종료
			isKill = TRUE;
			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	if (isKill == FALSE && hProcess != NULL)   //종료 대상 프로세스가 열려있으나 종료되지 않았을 때
		printf(("Kill process fail. Try again! \n"));
}