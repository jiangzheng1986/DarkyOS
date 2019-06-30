#ifndef MEMORY_H
#define MEMORY_H

#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000

#define MEMMAN_FREES		4090			//大概32KB
#define MEMMAN_ADDR			0x003c0000

typedef struct
{
	unsigned int addr, size;
}FreeInfo;

typedef struct
{
	int frees, maxfrees, lostsize, losts;
	FreeInfo free[MEMMAN_FREES];
}MemoryManager;

void InitMemoryManager();
unsigned int GetTotalMemory();
unsigned int MemoryAllocate(unsigned int size);
int MemoryFree(unsigned int addr, unsigned int size);
unsigned int MemoryAlloc4K(unsigned int size);
int MemoryFree4K(unsigned int addr, unsigned int size);

#endif
