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

	ret = RegOpenKeyEx(HKEY_CURRENT_USER,                                // 키 값
		"Control Panel\\International",                   // 서브 키값   user아래 경로
		0,                                                 // 옵션 항상 0
		KEY_EXECUTE,                                       // 접근 권한  읽기만 할 경우 사용하는 권한
		&hKey                                              // 키 핸들
	);

	if (ret == ERROR_SUCCESS)
	{
		memset(buffer, 0, sizeof(buffer));
		size = sizeof(buffer);
		RegQueryValueEx(hKey,                                      // RegOpenKeyEx에 얻어진 키 핸들
			"sLanguage",                                // 키 안에 값 이름
			0,                                             // 옵션 항상 0
			&type,                                    // 얻어진 데이터 타입
			(BYTE*)buffer,                           // 얻어진 데이터
			(DWORD *)&size
			// 얻어진 데이터 크기
		);
		RegCloseKey(hKey);
	}

	int isSame = strcmp(buffer, "KOR");                 //문자열 비교 변
	if (isSame == 0) {                                //문자열 비교해서 같을시 0
		return 1;
	}
	else return 0;
}