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
	//���� ��� ���μ������� ���¸� ����

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf(("CreateToolhelp32Snapshot error! \n"));
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//���μ��� ������ ���� ����ü ����

	//Process32First �ý��� ���� ���� ��ϵ� ù��° ���μ��� ���� �˻�
	if (!Process32First(hProcessSnap, &pe32)) {
		printf(("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	HANDLE hProcess = NULL;
	BOOL isKill = FALSE;

	//Process32Next ���� ���μ����� ���� ���� �˻� ���
	do {
		if (_tcscmp(pe32.szExeFile, TargetProcess) == 0)   //���� ��� ���μ����� ã���� ��
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);   //�ڵ鰪�� ������

		if (hProcess != NULL) {
			TerminateProcess(hProcess, -1);   //���μ��� ����
			isKill = TRUE;
			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	if (isKill == FALSE && hProcess != NULL)   //���� ��� ���μ����� ���������� ������� �ʾ��� ��
		printf(("Kill process fail. Try again! \n"));
}