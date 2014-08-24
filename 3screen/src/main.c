#include "monitor.h"
int kmain(struct multiboot* mboot_ptr)//name is mentioned in boot.s
{
	monitor_write("qhello!!@#$%^&*()[]+= bcdef:wworld! 1234");
	monitor_write("\n");
	monitor_write_hex(256);
	monitor_write("finished");
	monitor_write_dec(256);
	monitor_write("done");
	return 0xdeadbeef;
}
