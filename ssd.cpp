#include "ssd.h"
#include <cstdio>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//#define FILE_STRUCT
#define SSD_NAND_FILE_NAME "./ssd_nand.txt"
#define SSD_OUTPUT_FILE_NAME "./ssd_output.txt"
#define SSD_ERROR_STRING "ERROR"

SSD::SSD()
{
	int result = 0;
	if (_access(SSD_NAND_FILE_NAME, 0) != 0) {
#ifdef FILE_STRUCT
		FILE* fp = nullptr;

		if ((result = fopen_s(&fp, SSD_NAND_FILE_NAME, "wb+")) != 0) {
			return;// error
		}

		uint32_t value = 0;
		for (int i = 0; i < 100; ++i) {
			fwrite(&value, sizeof(uint32_t), 1, fp);
		}

		fflush(fp);

		fclose(fp);
#elif defined(SSD_NAND_RAW_API)
		int fd = 0;

		//if ((result = _open(SSD_NAND_FILE_NAME, O_RDWR | O_CREAT)) != 0) {
		if ((result = _sopen_s(&fd, SSD_NAND_FILE_NAME, O_CREAT | O_RDWR | O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE)) != 0) {
			return;// error
		}

		uint32_t value = 0;
		for (int i = 0; i < 100; ++i) {
			_write(fd, &value, sizeof(uint32_t));
		}

		_close(fd);
#else
		//DWORD attributes = GetFileAttributes(filePath);

		//if (attributes != INVALID_FILE_ATTRIBUTES) {
		HANDLE hFile;
		BOOL bErrorFlag = FALSE;

		hFile = CreateFile(L"./ssd_nand.txt", // name of the write
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			0,                      // do not share
			NULL,                   // default security
			CREATE_NEW,             // create new file only
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}

		uint32_t value = 0;
		DWORD wrteByte;

		for (int i = 0; i < 100; ++i) {
			bErrorFlag = WriteFile(
				hFile,           // open file handle
				&value,      // start of data to write
				sizeof(int),  // number of bytes to write
				&wrteByte, // number of bytes that were written
				NULL);            // no overlapped structure
		}

		if (!FlushFileBuffers(hFile)) {
			CloseHandle(hFile);
			return;
		}

		CloseHandle(hFile);
#endif
	}
}

uint32_t SSD::Read(uint32_t lba)
{
	return 0;
}

void SSD::Write(uint32_t lba, uint32_t value)
{

}

int SSD::handleError()
{
	int result = 0;
	FILE* fp = nullptr;

	if ((result = fopen_s(&fp, SSD_OUTPUT_FILE_NAME, "wb+")) != 0) {
		return SSD_RETURN_ERROR;
	}

	const char* value = SSD_ERROR_STRING;
	fwrite(value, sizeof(uint32_t), 1, fp);

	//fflush(fp);

	fclose(fp);

	return SSD_RETURN_ERROR;
}
