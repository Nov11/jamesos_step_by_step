COBJ=${patsubst %.c,%.o, $(wildcard *.c)}
SOBJ+=${patsubst %.s,%.o, $(wildcard *.s)}
#nostdlib 不连接标准启动文件和标准库文件
#nostdinc 不在标准系统目录中搜索头文件，只在-I指定的目录中搜索
#fno-builtin 不使用内置的函数
#no-stack-protector 不使用堆栈保护
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -g
LDFLAGS=-T link.ld
ASFLAGS=-felf

all: $(COBJ) $(SOBJ) link
.PHONY:clean 
clean:
	-rm *.o kernel *~
link:$(SOBJ)
	ld $(LDFLAGS) -o kernel $(COBJ) $(SOBJ)
.s.o:
	nasm $(ASFLAGS) $<
