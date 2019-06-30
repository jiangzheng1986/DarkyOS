#include "Kernel.h"

unsigned int dummy = 0;

unsigned int RealMemoryTest(unsigned int start, unsigned int end)
{
	unsigned int i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
	
	for ( i = start; i <= end; i+= 0x1000 )
	{
		p = (unsigned int *)(i + 0xffc);
		
		old = *p;
		*p = pat0;
		*p ^= 0xffffffff;
		if ( *p != pat1 )
		{
not_memory:
			*p = old;
			break;
		}
		
		dummy += *p;
		
		*p ^= 0xffffffff;
		if ( *p != pat0 )
		{
			goto not_memory;
		}
		*p = old;
	}
	return i;
}

unsigned int MemoryTest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;
	
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT;
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ( (eflg & EFLAGS_AC_BIT) != 0 )
	{
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT;
	io_store_eflags(eflg);
	
	if ( flg486 != 0 )
	{
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE;
		store_cr0(cr0);
	}
	
	i = RealMemoryTest(start, end);
	
	if ( flg486 != 0 )
	{
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE;
		store_cr0(cr0);
	}
	
	return i;
}

MemoryManager *GetMemoryMananger()
{
    return (MemoryManager *)MEMMAN_ADDR;
}

void InitMemoryManager()
{
    MemoryManager *pMemoryManager = GetMemoryMananger();

    pMemoryManager->frees = 0;
    pMemoryManager->maxfrees = 0;
    pMemoryManager->lostsize = 0;
    pMemoryManager->losts = 0;

    unsigned int uTotalMemory = MemoryTest(0x00400000, 0xbfffffff);
    MemoryFree(0x00001000, 0x0009e000);
    MemoryFree(0x00400000, uTotalMemory - 0x00400000);
}

unsigned int GetTotalMemory()
{
    MemoryManager *pMemoryManager = GetMemoryMananger();

	unsigned int i, t = 0;
	for ( i = 0; i < pMemoryManager->frees; i++ )
	{
		t += pMemoryManager->free[i].size;
	}
	return t;
}

unsigned int MemoryAllocate(unsigned int size)
{
    MemoryManager *pMemoryManager = GetMemoryMananger();

	unsigned int i, a;
	for ( i = 0; i < pMemoryManager->frees; i++ )
	{
		if (pMemoryManager->free[i].size >= size )
		{
			a = pMemoryManager->free[i].addr;
            pMemoryManager->free[i].addr += size;
            pMemoryManager->free[i].size -= size;
			if (pMemoryManager->free[i].size == 0 )
			{
                pMemoryManager->frees--;
				for ( ; i < pMemoryManager->frees; i++ )
				{
                    pMemoryManager->free[i] = pMemoryManager->free[i + 1];
				}
			}
			return a;
		}
	}
	return 0;
}

int MemoryFree(unsigned int addr, unsigned int size)
{
    MemoryManager *pMemoryManager = GetMemoryMananger();

	int i, j;
	
	for ( i = 0; i < pMemoryManager->frees; i++ )
	{
		if (pMemoryManager->free[i].addr > addr )
		{
			break;
		}
	}
	
	if ( i > 0 )
	{
		if (pMemoryManager->free[i - 1].addr + pMemoryManager->free[i - 1].size == addr )
		{
            pMemoryManager->free[i - 1].size += size;
			
			if ( i < pMemoryManager->frees )
			{
				if ( addr + size == pMemoryManager->free[i].addr )
				{
                    pMemoryManager->free[i - 1].size += pMemoryManager->free[i].size;
					
                    pMemoryManager->frees--;
					
					for ( ; i < pMemoryManager->frees; i++ )
					{
                        pMemoryManager->free[i] = pMemoryManager->free[i + 1];
					}
				}
			}
			
			return 0;
		}
	}
	
	if ( i < pMemoryManager->frees )
	{
		if ( addr + size == pMemoryManager->free[i].addr )
		{
            pMemoryManager->free[i].addr = addr;
            pMemoryManager->free[i].size += size;
			return 0;
		}
	}
	
	if (pMemoryManager->frees < MEMMAN_FREES )
	{
		for ( j = pMemoryManager->frees; j > i; j-- )
		{
            pMemoryManager->free[j] = pMemoryManager->free[j - 1];
		}
        pMemoryManager->frees++;
		if (pMemoryManager->maxfrees < pMemoryManager->frees )
		{
            pMemoryManager->maxfrees = pMemoryManager->frees;
		}
        pMemoryManager->free[i].addr = addr;
        pMemoryManager->free[i].size = size;
		return 0;
	}
	
    pMemoryManager->losts++;
    pMemoryManager->lostsize = size;
	return -1;
}

unsigned int MemoryAlloc4K(unsigned int size)
{
	unsigned int a;
	size = (size + 0xfff) & 0xfffff000;
	a = MemoryAllocate(size);
	return a;
}

int MemoryFree4K(unsigned int addr, unsigned int size)
{
	int i;
	size = (size + 0xfff) & 0xfffff000;
	i = MemoryFree(addr, size);
	return i;
}
