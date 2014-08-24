#include "monitor.h"
#include "idt.h"
#include "gdt.h"

//写个struct mulitboot 省着老有警告 用的时候再改
//grub标准里有这个http://gnu.april.org/software/grub/manual/multiboot/multiboot.html
struct multiboot{};
int kmain(struct multiboot* mboot_ptr)//name is mentioned in boot.s
{
	monitor_write("qhello!!@#$%^&*()[]+= bcdef:wworld! 1234");
	monitor_write("\n");
	monitor_write_hex(256);
	monitor_write("finished");
	monitor_write_dec(256);
	monitor_write("done");
	monitor_put('\n');
	init_gdt();
	init_idt();
	asm volatile("int $0x3");
	asm volatile("int $0x4");
	return 0xdeadbeef;
}
