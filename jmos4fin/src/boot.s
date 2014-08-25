MBOOT_PAGE_ALIGN	equ 	1<<0
MBOOT_MEM_INFO		equ 	1<<1
MBOOT_HEADER_MAGIC	equ		0x1badb002
MBOOT_HEADER_FLAGS	equ		MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHEKCSUM		equ		-(MBOOT_HEADER_FLAGS + MBOOT_HEADER_MAGIC)

[bits 32]
[extern code]
[extern bss]
[extern end]

	;The Multiboot header must be contained completely within the first 8192 bytes of the OS image, 
	;and must be longword (32-bit) aligned. 4字节对齐写link.ld了
	;In general, it should come as early as possible, 
	;and may be embedded in the beginning of the text segment after the real executable header. 
mboot:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHEKCSUM
	;多引导标准里说flags 16bit 没设置的话 后面的这些写不写都行
	
	;http://gnu.april.org/software/grub/manual/multiboot/multiboot.html#Header-layout
	;3.1.1 The layout of Multiboot header
    ;The layout of the Multiboot header must be as follows:

	;Offset 	Type 	Field Name 	Note
	;0 			u32 	magic 	required
	;4 			u32 	flags 	required
	;8 			u32 	checksum 	required
	;12 		u32 	header_addr 	if flags[16] is set
	;16 		u32 	load_addr 	if flags[16] is set
	;20 		u32 	load_end_addr 	if flags[16] is set
	;24 		u32 	bss_end_addr 	if flags[16] is set
	;28 		u32 	entry_addr 	if flags[16] is set
	;32 		u32 	mode_type 	if flags[2] is set
	;36 		u32 	width 	if flags[2] is set
	;40 		u32 	height 	if flags[2] is set
	;44 		u32 	depth 	if flags[2] is set 
	dd mboot
	dd code
	dd bss    	;end of data
	dd end		;end of bss
	dd start
[global start]
[extern kmain]
start:
	;没搞起stack呢 现在的esp ss是啥 bochs没搞清楚
	push ebx

	cli
	call kmain
	jmp $


