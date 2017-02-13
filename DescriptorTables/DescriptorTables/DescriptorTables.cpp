// DescriptorTables.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>
#include <intrin.h>

extern "C" void get_idtr(unsigned char *out);
extern "C" BYTE get_tss_selector(void);

int main()
{
	
#ifdef _WIN64
	unsigned char idtr[10] = { 0 };
	get_idtr(idtr);
	unsigned long long idtrBase = (unsigned long long)idtr[0] << 56 |
		(unsigned long long)idtr[1] << 48 |
		(unsigned long long)idtr[2] << 40 |
		(unsigned long long)idtr[3] << 32 |
		(unsigned long long)idtr[4] << 24 |
		(unsigned long long)idtr[5] << 16 |
		(unsigned long long)idtr[6] << 8 |
		(unsigned long long)idtr[7];
	unsigned short idtrLimit = (unsigned int)idtr[8] << 8 |
		(unsigned int)idtr[9];
	printf("Interrupt Descriptor Table Register base: 0x%llx, limit: 0x%X\r\n", idtrBase, idtrLimit);
#else
	unsigned char idtr[6] = { 0 };
	__asm {
		sidt idtr;
	}
	unsigned int idtrBase = (unsigned int)idtr[0] << 24 |
		(unsigned int)idtr[1] << 16 |
		(unsigned int)idtr[2] << 8 |
		(unsigned int)idtr[3];
	unsigned short idtrLimit = (unsigned int)idtr[4] << 8 |
		(unsigned int)idtr[5];
	printf("Interrupt Descriptor Table Register base: 0x%X, limit: 0x%X\r\n", idtrBase, idtrLimit);
#endif
	
	
	
#ifdef _WIN64
	unsigned char gdtr[10] = { 0 };
	_sgdt(gdtr);
	unsigned long long gdtrBase = (unsigned long long)gdtr[0] << 56 |
		(unsigned long long)gdtr[1] << 48 |
		(unsigned long long)gdtr[2] << 40 |
		(unsigned long long)gdtr[3] << 32 |
		(unsigned long long)gdtr[4] << 24 |
		(unsigned long long)gdtr[5] << 16 |
		(unsigned long long)gdtr[6] << 8 |
		(unsigned long long)gdtr[7];
	unsigned short gdtrLimit = (unsigned int)gdtr[8] << 8 |
		(unsigned int)gdtr[9];
	printf("Global Descriptor Table Register base: 0x%llx, limit: 0x%X\r\n", gdtrBase, gdtrLimit);
#else
	unsigned char gdtr[6] = { 0 };
	__asm {
		sgdt gdtr;
	}
	unsigned int gdtrBase = (unsigned int)gdtr[0] << 24 |
		(unsigned int)gdtr[1] << 16 |
		(unsigned int)gdtr[2] << 8 |
		(unsigned int)gdtr[3];
	unsigned short gdtrLimit = (unsigned int)gdtr[4] << 8 |
		(unsigned int)gdtr[5];
	printf("Global Descriptor Table Register base: 0x%X, limit: 0x%X\r\n", gdtrBase, gdtrLimit);
#endif
	
	WORD tr;
#ifdef _WIN64
	tr = get_tss_selector();
#else
	__asm str tr
#endif

#ifdef _WIN64
	WOW64_LDT_ENTRY tss;
	Wow64GetThreadSelectorEntry(GetCurrentThread(), tr, &tss);
#else
	LDT_ENTRY tss;
	GetThreadSelectorEntry(GetCurrentThread(), tr, &tss);
#endif
	unsigned int  tssBase = (tss.HighWord.Bits.BaseHi << 24) +
		(tss.HighWord.Bits.BaseMid << 16) +
		tss.BaseLow;
	unsigned int tssLimit = tss.LimitLow;
	printf("TSS base: 0x%X, limit: 0x%X\r\n", tssBase, tssLimit);

	return 0;
}
