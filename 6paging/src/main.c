#include "monitor.h"
#include "idt.h"
#include "gdt.h"
#include "timer.h"
#include "paging.h"

u16int kss;
u32int kesp;
extern u32int end;
//写个struct mulitboot 省着老有警告 用的时候再改
//grub标准里有这个http://gnu.april.org/software/grub/manual/multiboot/multiboot.html
struct multiboot{};
int kmain(struct multiboot* mboot_ptr)//name is mentioned in boot.s
{
	init_gdt();
	init_idt();
	monitor_write("qhello!!@#$%^&*()[]+= bcdef:wworld! 1234");
	monitor_write("\n");
	monitor_write_hex(256);
	monitor_write("finished");
	monitor_write_dec(256);
	monitor_write("done ss");
	monitor_write_hex(kss);
	monitor_write("done esp");
	monitor_write_hex(kesp);
	
	monitor_put('\n');
	//init_gdt();
	//init_idt();
	asm volatile("int $0x3");
	asm volatile("int $0x4");
	prtf("aa bb %x %u %s 11\t \nbb\n", 10, 10, "str");	
	//asm volatile("sti");
    //init_timer(500);
//	monitor_write_hex((u32int)&end);
	prtf("1\tend is at addr :%x\n", (u32int)&end);


	init_paging();
	prtf("paging enabled!\n");
	u32int* ptr = (u32int*)0xa0000000;
	*ptr = 1;
//	prtf("%x\n", do_page_fault);
	return 0xdeadbeef;
}
