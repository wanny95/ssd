#include "gmock/gmock.h"
#include <iostream>
#include "ssd.h"

int main()
{
	SSD ssd;

	ssd.Write(1, 0x12345678);
	printf("0x%x\n", ssd.Read(1));
	return 0;
}
