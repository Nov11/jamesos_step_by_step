[global idt_flush]
idt_flush:
	mov eax, [esp + 4]
	lidt [eax]			;LIDT m16&32 
	ret

