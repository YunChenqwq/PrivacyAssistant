#pragma once
#include <iostream>
#include<string> 
#include<Windows.h>
#include<setupapi.h>
#include"data.h"
#define PHYSICAL_SECTOR_SIZE 512
#define Mbr_sign 0X1A
#define Gpt_sign 0X2A
using namespace std;
unsigned char w_sign[512];
unsigned char Gpt_f[512];
unsigned char Gpt_s[512];
unsigned char Gpt_t[512];
unsigned char OldMbr[512];
using namespace std;
BOOL WriteDiskSector(HANDLE hFile, int Id, unsigned char* buffer) {
	int offset = 0;
	DWORD Writed = 0;
	offset = Id * 512;
	if (buffer == NULL) {
		return FALSE;
	}
	SetFilePointer(hFile, offset, 0, 0);
	WriteFile(hFile, buffer, PHYSICAL_SECTOR_SIZE, &Writed, NULL);
}
BOOL ReadDiskSector(HANDLE hFile, int Id, unsigned char* buffer) {
	int offset = 0;
	int WirteSize = 0;
	DWORD Readed = 0;
	offset = Id * 512;
	if (buffer == NULL) {
		return FALSE;
	}
	SetFilePointer(hFile, offset, 0, 0);
	return ReadFile(hFile, buffer, PHYSICAL_SECTOR_SIZE, &Readed, NULL);
}
string GetBoot(HANDLE hDevice)
{
	int nDiskBufferSize = sizeof(PARTITION_INFORMATION_EX);
	PARTITION_INFORMATION_EX* lpDiskPartinfo_ex = (PARTITION_INFORMATION_EX*)malloc(nDiskBufferSize);
	DWORD nDiskBytesRead = 0;
	memset(lpDiskPartinfo_ex, 0, sizeof(lpDiskPartinfo_ex));
	BOOL Ret = FALSE;
	Ret = DeviceIoControl(hDevice, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, lpDiskPartinfo_ex, nDiskBufferSize, &nDiskBytesRead, NULL);

	if (!Ret)
	{
		int e = GetLastError();
		return strerror(e);
	}
	if (lpDiskPartinfo_ex->PartitionStyle == PARTITION_STYLE_MBR)
	{
		return "MBR";
	}
	else if (lpDiskPartinfo_ex->PartitionStyle == PARTITION_STYLE_GPT)
	{
		return "GPT";
	}
	else if (lpDiskPartinfo_ex->PartitionStyle == PARTITION_STYLE_RAW)
	{
		return "RAW";
	}
	free(lpDiskPartinfo_ex);
	lpDiskPartinfo_ex = NULL;
	CloseHandle(hDevice);
}
string GetPhysicalDriveHandle(HANDLE& handle, int DriveNumber)
{
	char drivestr[512];
	_snprintf_s(drivestr, 512, _TRUNCATE, "\\\\.\\PhysicalDrive%d", DriveNumber);
	handle = CreateFileA(drivestr, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);
	if (handle == INVALID_HANDLE_VALUE) {
		int e = GetLastError();
		return strerror(e);
	}
	return "success";
}
void StringToHex(char* str, unsigned char* strhex)
{
	uint8_t i, cnt = 0;
	char* p = str;             //ֱ��p��ʼ��Ϊָ��str
	uint8_t len = strlen(str); //��ȡ�ַ����е��ַ�����

	while (*p != '\0') {        //�������ж�
		for (i = 0; i < len; i++)  //ѭ���жϵ�ǰ�ַ������ֻ���Сд�ַ����Ǵ�д��ĸ
		{
			if ((*p >= '0') && (*p <= '9')) //��ǰ�ַ�Ϊ����0~9ʱ
				strhex[cnt] = *p - '0' + 0x30;

			if ((*p >= 'A') && (*p <= 'Z')) //��ǰ�ַ�Ϊ��д��ĸA~Zʱ
				strhex[cnt] = *p - 'A' + 0x41;

			if ((*p >= 'a') && (*p <= 'z')) //��ǰ�ַ�ΪСд��ĸa~zʱ
				strhex[cnt] = *p - 'a' + 0x61;

			p++;
			cnt++;
		}
	}
}
bool check(string str)
{
	int badNum = 0;
	int size = str.length();
	if (size <= 0)
	{
		return false;
	}
	char* pStr = new char[size];
	strcpy(pStr, str.c_str());
	for (int i = 0; i < size; i++)
	{
		if (!(pStr[i] >= 0 && pStr[i] <= 127))
			continue;
		if (ispunct(pStr[i]))
		{
			badNum++;
		}
	}
	delete[] pStr;
	bool res = true;
	if (badNum > 0)
	{
		return true;
	}
	else return false;
}
void password(char password[10], string what)
{
	unsigned char last[10] = { 0 };
	//438-447 MBR
	//458-467 Gpt
	if (what == "mbr")
	{
		for (int i = 0; i < 10; i++)
		{
			Mbr[437 + i] = password[i];
		}
	}
	if (what == "gpt")
	{
		for (int i = 0; i < 10; i++)
		{
			Mbr[457 + i] = password[i];
		}
	}
}
int GetPHDnum()
{
	//��������򵥵ķ�ʽ�ȽϺã�doge)
	int drivenumber;
	HANDLE hFile = NULL;
	char drivestr[512] = { 0 };
	for (drivenumber = 0;; drivenumber++)
	{
		_snprintf_s(drivestr, 512, _TRUNCATE, "\\\\.\\PhysicalDrive%d", drivenumber);
		hFile = CreateFileA(drivestr, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			return drivenumber;
			break;
		}
		CloseHandle(hFile);
	}
}
void lockup(char* passwords, int drivernumber)
{
	HANDLE hdisk = INVALID_HANDLE_VALUE;
	string error = GetPhysicalDriveHandle(hdisk, drivernumber);
	if (error != "success")
	{
		error = "�򿪴��̾��ʧ�ܣ��������:" + error;
		MessageBoxA(NULL, error.c_str(), "PrivacyAssistant", MB_OK | MB_ICONERROR);
		return;
	}
	if (GetBoot(hdisk) == "GPT")
	{
		int msgID = MessageBox(NULL, "��⵽���������ΪGPT���ڱ���������£��������������ͨ��UEFI�������أ���Ҫ�ֶ�������ͳģʽ���Ƿ�Ҫ�������� ? ", "PrivacyAssistant", MB_OKCANCEL | MB_ICONEXCLAMATION);
		switch (msgID)
		{
		case IDCANCEL:
			exit(0);
			break;
		case IDOK:
			password(passwords, "gpt");
			ReadDiskSector(hdisk, 0, Gpt_f);
			ReadDiskSector(hdisk, 1, Gpt_s);
			ReadDiskSector(hdisk, 2, Gpt_t);
			ReadDiskSector(hdisk, 10, w_sign);
			w_sign[509] = Gpt_sign;//����unlock.exe�жϷ������ͣ��Է���10������Ϊ�գ����Լ���Ϳд.
			WriteDiskSector(hdisk, 3, Gpt_f);
			WriteDiskSector(hdisk, 4, Gpt_s);
			WriteDiskSector(hdisk, 5, Gpt_t);
			WriteDiskSector(hdisk, 0, Gpt);
			WriteDiskSector(hdisk, 10, w_sign);
			break;
		}
	}
	else
	{
		password(passwords, "mbr");
		ReadDiskSector(hdisk, 0, OldMbr);
		WriteDiskSector(hdisk, 0, Mbr);
		WriteDiskSector(hdisk, 2, OldMbr);
		w_sign[509] = Mbr_sign;
		WriteDiskSector(hdisk, 10, w_sign);
	}
}
void unlock(int drivernumber)
{
	HANDLE hdisk = INVALID_HANDLE_VALUE;
	string error = GetPhysicalDriveHandle(hdisk, drivernumber);
	if (error != "success")
	{
		error = "�򿪴��̾��ʧ�ܣ��������:" + error;
		MessageBoxA(NULL, error.c_str(), "PrivacyAssistant", MB_OK | MB_ICONERROR);
		return;
	}
	ReadDiskSector(hdisk, 10, w_sign);
	if (w_sign[509] == Gpt_sign)
	{
		cout << "Gpt";
		ReadDiskSector(hdisk, 3, Gpt_f);
		ReadDiskSector(hdisk, 4, Gpt_s);
		ReadDiskSector(hdisk, 5, Gpt_t);
		ReadDiskSector(hdisk, 10, w_sign);
		WriteDiskSector(hdisk, 0, Gpt_f);
		WriteDiskSector(hdisk, 1, Gpt_s);
		WriteDiskSector(hdisk, 2, Gpt_t);
		w_sign[509] = 0x00;
		WriteDiskSector(hdisk, 10, w_sign);
		exit(0);
	}
	if (w_sign[509] == Mbr_sign)
	{
		cout << "Mbr";
		ReadDiskSector(hdisk, 2, OldMbr);
		WriteDiskSector(hdisk, 0, OldMbr);
		w_sign[509] = 0x00;
		WriteDiskSector(hdisk, 10, w_sign);
		exit(0);
	}
	else
	{
		MessageBoxA(NULL, "��������δ�ӹ���", "PrivacyAssistant", MB_OK | MB_ICONERROR);
	}
}