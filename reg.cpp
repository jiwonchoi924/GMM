#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

int KillSwitch()
{
	LONG    ret;

	HKEY    hKey;

	char    buffer[256];
	DWORD   type;
	DWORD   size;

	ret = RegOpenKeyEx(HKEY_CURRENT_USER,                                // Ű ��
		"Control Panel\\International",                   // ���� Ű��   user�Ʒ� ���
		0,                                                 // �ɼ� �׻� 0
		KEY_EXECUTE,                                       // ���� ����  �б⸸ �� ��� ����ϴ� ����
		&hKey                                              // Ű �ڵ�
	);

	if (ret == ERROR_SUCCESS)
	{
		memset(buffer, 0, sizeof(buffer));
		size = sizeof(buffer);
		RegQueryValueEx(hKey,                                      // RegOpenKeyEx�� ����� Ű �ڵ�
			"sLanguage",                                // Ű �ȿ� �� �̸�
			0,                                             // �ɼ� �׻� 0
			&type,                                    // ����� ������ Ÿ��
			(BYTE*)buffer,                           // ����� ������
			(DWORD *)&size
			// ����� ������ ũ��
		);
		RegCloseKey(hKey);
	}

	int isSame = strcmp(buffer, "KOR");                 //���ڿ� �� ��
	if (isSame == 0) {                                //���ڿ� ���ؼ� ������ 0
		return 1;
	}
	else return 0;
}