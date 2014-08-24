#include "gdt.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;
extern void gdt_flush(u32int);
static void gdt_set_gate(s32int index, u32int base, u32int limit, u8int access, u8int gran)
{
	gdt_entries[index].base_low = base & 0xffff;//16bits
	gdt_entries[index].base_mid = base >> 16 & 0xff;//8bits
	gdt_entries[index].base_high = base >> 24 & 0xff;//8bits

	gdt_entries[index].limit_low = limit & 0xffff;//16bits
	gdt_entries[index].granularity = (limit >> 16) | (gran & 0xf0); 
	gdt_entries[index].access = access;
}


void init_gdt()
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (u32int)gdt_entries;//&gdt_entries is the addr of array

	gdt_set_gate(0, 0, 0, 0, 0);
	//acc 7	6	5	4	3		2	1	0 for data and code seg
	//    p [dpl]   1   0:data  e   w   a
	//                  1:code  c   r   a
	//    1  0  0   1   1       0   1   0  0x9a code
	//    1  0  0   1   0       0   1   0  0x92 data
	gdt_set_gate(1, 0, LIMIT_MAX, 0x9a, 0xc0);//0x9a : 1001 1010
   	gdt_set_gate(2, 0, LIMIT_MAX, 0x92, 0xc0);//0x92 : 1001 0010
	gdt_set_gate(3, 0, LIMIT_MAX, 0xfa, 0xc0);
	gdt_set_gate(4, 0, LIMIT_MAX, 0xf2, 0xc0);
	
	gdt_flush((u32int)&gdt_ptr);
}

