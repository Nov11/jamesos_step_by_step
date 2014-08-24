MBOOT_PAGE_ALIGN	equ 	1<<0
MBOOT_MEM_INFO		equ 	1<<1
MBOOT_HEADER_MAGIC	equ		0x1badb002
MBOOT_HEADER_FLAGS	equ		MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHEKCSUM		equ		-(MBOOT_HEADER_FLAGS + MBOOT_HEADER_MAGIC)

[bits 32]
[extern code]
[extern bss]
[extern end]


mboot:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHEKCSUM

	dd mboot
	dd code
	dd bss    	;end of data
	dd end		;end of bss
	dd start
[global start]
[extern kmain]
start:
	push ebx

	cli
	call kmain
	jmp $


