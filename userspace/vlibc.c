#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "syscalls.h"

int putchar(int c) {

	return write(1,&c,1);
}

int getchar(void) {

	int c=0;

	read(0,&c,1);

	return c;

}


unsigned int sleep(unsigned int seconds) {

	struct timespec t;

	t.tv_sec=seconds;
	t.tv_nsec=0;

	nanosleep(&t, NULL);

	return 0;
}




#define MAX_PRINT_SIZE 256

int printf(char *string,...) {

	va_list ap;

	char buffer[MAX_PRINT_SIZE];
	char int_buffer[10];
	int int_pointer=0;

	int buffer_pointer=0;
	int i;
	unsigned long x;

	va_start(ap, string);

	while(1) {
		if (*string==0) break;

		if (*string=='%') {
			string++;
			if (*string=='d') {
				string++;
				x=va_arg(ap, unsigned long);
				if (x&0x80000000) {
					x=~x;
					x+=1;
					buffer[buffer_pointer]='-';
					buffer_pointer++;
				}
				int_pointer=9;
				do {
					int_buffer[int_pointer]=(x%10)+'0';
					int_pointer--;
					x/=10;
				} while(x!=0);
				for(i=int_pointer+1;i<10;i++) {
					buffer[buffer_pointer]=int_buffer[i];
					buffer_pointer++;
				}

			}
			else if (*string=='x') {
				string++;
				x=va_arg(ap, unsigned long);
				int_pointer=9;
				do {
					if ((x%16)<10) {
						int_buffer[int_pointer]=(x%16)+'0';
					}
					else {
						int_buffer[int_pointer]=(x%16)-10+'a';
					}
					int_pointer--;
					x/=16;
				} while(x!=0);
				for(i=int_pointer+1;i<10;i++) {
					buffer[buffer_pointer]=int_buffer[i];
					buffer_pointer++;
				}
			}
			else if (*string=='c') {
				string++;
				x=va_arg(ap, unsigned long);
				buffer[buffer_pointer]=x;
				buffer_pointer++;
			}
			else if (*string=='s') {
				char *s;
				string++;
				s=(char *)va_arg(ap, long);
				while(*s) {
					buffer[buffer_pointer]=*s;
					s++;
					buffer_pointer++;
				}
			}
		}
		else {
			buffer[buffer_pointer]=*string;
			buffer_pointer++;
			string++;
		}
		if (buffer_pointer==MAX_PRINT_SIZE-1) break;
	}

	va_end(ap);

	write(1,buffer,buffer_pointer);

	return buffer_pointer;
}


int strncmp(const char *s1, const char *s2, uint32_t n) {

	int i=0,r;

	while(1) {

		if (i==n) return 0;

		r=s1[i]-s2[i];
		if (r!=0) return r;

		i++;
	}

	return 0;
}

void cfmakeraw(struct termios *termios_p) {

	termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP |INLCR|IGNCR|ICRNL|IXON);
	termios_p->c_oflag &= ~OPOST;
	termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	termios_p->c_cflag &= ~(CSIZE|PARENB);
	termios_p->c_cflag |= CS8;
}

int tcgetattr(int fd, struct termios *termios_p) {

	return ioctl3(fd,TCGETS,(long)termios_p);

}

int tcsetattr(int fd, int optional_actions,
                     const struct termios *termios_p) {

        return ioctl3(fd,TCSETS,(long)termios_p);
}

int time(int *t) {

	int our_time;

	our_time=sys_time();

	if (t!=NULL) *t=our_time;

	return our_time;
}

int reboot(void) {
	sys_reboot();
}
