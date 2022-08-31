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

#ifndef __PROCESS_H__
#define __PROCESS_H__

void KillProcess(TCHAR* TargetProcess);
#endif // !__PROCESS_H__
