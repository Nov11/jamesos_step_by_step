#include "monitor.h"
#include "common.h"

u8int cursor_x;
u8int cursor_y;
u16int* video_memory = (void*)0xb8000;

static void move_cursor()
{
	u16int cursorLocation = cursor_y * 80 + cursor_x ;
	outb(0x3d4, 14);//发送高字节
	outb(0x3d5, cursorLocation >> 8);
	outb(0x3d4, 15);//发低字节
	outb(0x3d5, cursorLocation);
}
	
static void scroll()
{
	u8int attribute = 0x0f; //黑底白字
	u16int blank = 0x20 | attribute << 8;//黑底白字的空格

	if(cursor_y >= 25){
		int i;
		for(i = 0; i < 24 * 80; i++){
			video_memory[i] = video_memory[i + 80];
		}

		for(i = 24 * 80; i < 25 * 80; i++){
			video_memory[i] = blank;
		}

		cursor_y = 24;
	}
}

void monitor_put(char c)
{
	u8int attribute = 0x0f;

	switch(c){
	case 0x08://backspace
		if(cursor_x > 0){
			//delete char use *((cursor_x + cursor_y * 80) * 2) = 0x2f;
			cursor_x -= 1;
		}
		break;
	case 0x09://tab
		cursor_x = (cursor_x + 8) & ~(8 - 1);
		break;
	case '\r'://0x0d cr
		cursor_x = 0;
		break;
	case '\n'://0x0a line feed
		cursor_x = 0;
		cursor_y ++;
		break;
	default:
		//if(c >= '0' && c <= '9' ||
		//	   	c >= 'a' && c <= 'z' || 
		//		c >= 'A' && c <= 'Z')//0～9 a~z A~Z 只处理ascii
		{u16int* loc = video_memory + (cursor_y * 80 + cursor_x); //video_memory is u16int, don't *2 here
		*loc = c | attribute << 8;
		cursor_x++;
		}
		break;
	}
	if(cursor_x >= 80){
		cursor_x = 0;
		cursor_y++;
	}
	scroll();
	move_cursor();
}

void monitor_clear()
{
	u16int blank = 0x2f;
	int i;

	for(i = 0; i < 25 * 80; i++){
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void monitor_write(const char* c)
{
	if(c == NULL){
		return;
	}
	while(*c){
		monitor_put(*c++);
	}
}

void monitor_write_hex(u32int n)
{
	//32 / 4 = 8
	char hex[9] = {0};
	int index = 7;
	
	for(index = 7; index >= 0; index--){
		u32int tmp = n % 16;
		switch(tmp){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				hex[index] = tmp + '0';
				break;
			case 10:
				hex[index] = 'A';
				break;
			case 11:
				hex[index] = 'B';
				break;
			case 12:
				hex[index] = 'C';
				break;
			case 13:
				hex[index] = 'D';
				break;
			case 14:
				hex[index] = 'E';
				break;
			case 15:
				hex[index] = 'F';
				break;
		}
		if(n < 16){
			break;
		}
		n = n / 16;
	}
	index = index == -1 ? index + 1 : index;
	monitor_write(&hex[index]);
}

void monitor_write_dec(u32int n)
{
	char dec[33] = {0};
	int index = 31;

	for(index = 31; index >= 0; index--){
		u32int tmp = n % 10;

		dec[index] = tmp + '0';
		if(n < 10){
			break;
		}
		n /= 10;
	}
	index = index == -1 ? index + 1 : index;
	monitor_write(&dec[index]);
}

void prtf(const char* format, ...)
{
	const char** arg = &format;//存format的地址
	int c;

	//arg是format这个参数的下一个参数的地址,也就是字符串后第一个参数的地址
	arg++;
	while((c = *format++) != 0){
		if(c == '\\'){
			c = *format++;
			switch(c){
				case 'n':
					monitor_put('\n');
					break;
				case 't':
					monitor_put('\t');
					break;
			}
		}else if(c == '%'){
			c = *format++;
			switch(c){
				case 'u':
					monitor_write_dec(*(u32int*)arg);
					break;
				case 'x':
					monitor_write_hex(*(u32int*)arg);
					break;
				case 's':
					monitor_write(*arg);
					break;
			}
			arg++;
		}else{
			monitor_put(c);
		}
	}
}
