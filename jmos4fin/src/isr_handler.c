#include "common.h"
#include "monitor.h"

typedef struct re{
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int int_no, err_code;
	u32int eip, cs, eflags, useresp, ss;
}regsiters_t;

void isr_handler(regsiters_t regs)
{
	monitor_write("recv inte");
	monitor_write_dec(regs.int_no);
	monitor_write("err code");
	monitor_write_dec(regs.err_code);
	monitor_put('\n');
}


