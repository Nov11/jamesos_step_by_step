#include "paging.h"
#include "kheap.h"
//32bit so 8 * 4
#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))
page_directory_t* kernel_directory;
page_directory_t* current_directory;
u32int* frames;
u32int nframes;

extern u32int placement_address;

void set_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

void clear_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

u32int test_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	return frames[idx] & (0x1 << off);
}

u32int first_frame()
{
	u32int i, j;
	
	for(i = 0; i < INDEX_FROM_BIT(nframes); i++){
		if(frames[i] != 0xffffffff){
			for(j = 0; j < 32; j++){
				u32int mask = 0x1 << j;
				if(!(frames[i] & mask)){
					return i * 32 + j;
				}
			}
		}
	}
	return 0xffffffff;//应该加个参数 否则无法分辨是成功分配了还是满了
}

void alloc_frame(page_t* page, u8int is_kernel, u8int is_writable)
{
	if(page->frame != 0){
		return;
	}
	u32int idx = first_frame();
	if(idx == 0xffffffff){
		//full
		PANIC("full");
	}
	set_frame(idx * 0x1000);
	page->present = 1;
	page->rw = (is_writable) ? 1 : 0;
	page->user = (is_kernel) ? 1 : 0;
	page->frame = idx;
}

void free_frames(page_t* page)
{
	u32int frame;

	if(!(frame = page->frame)){
		return;
	}
	clear_frame(frame);
	page->frame = 0;
	//应该再置表项为无效 p=0
}

void init_paging()
{
	int i;
	u32int mem_end_page = 0x1000000;//假定有着么多内存 按照4k分成nframe页
	nframes = mem_end_page / 0x1000;
	frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;
	
	i = 0;
	while(i < placement_address){
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	spec_handler[14] = page_fault;
	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t* dir)
{
	u32int cr0;

	current_directory = dir;
	asm volatile("mov %0, %%cr3"::"r"(dir->tablesPhysical));
	asm volatile("mov %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0"::"r"(cr0));
}

page_t* get_page(u32int addr, u8int create, page_directory_t* dir)
{
	u32int table_idx;
	addr /= 0x1000;
	table_idx = addr / 1024;
	if(dir->tables[table_idx]){
		return &dir->tables[table_idx]->pages[addr % 1024];
	}else if(create){
		u32int tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, sizeof(page_table_t));
		dir->tablesPhysical[table_idx] = tmp | 0x7;//应该是0x3 0x7踩了reserve bits
		return &dir->tables[table_idx]->pages[addr % 1024];
	}
	return NULL;
}

void page_fault(registers_t regs)
{
	u32int fault_addr;
	asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

	prtf("page fault: %x\n", fault_addr);
	prtf("error code: %x\n", regs.err_code);
	PANIC("!!");
}
