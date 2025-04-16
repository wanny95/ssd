#pragma once
#include <stdint.h>

#define SSD_RETURN_SUCCESS 0
#define SSD_RETURN_ERROR 1

class SSD {
public:
	SSD();
	uint32_t Read(uint32_t lba);
	void Write(uint32_t lba, uint32_t value);
	int handleError();
private:
};
