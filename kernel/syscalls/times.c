#include <stdint.h>

#include "syscalls/times.h"

#include "processes/process.h"

int32_t times(struct tms *buf) {

	buf->tms_utime=current_process->user_time;
	buf->tms_stime=current_process->kernel_time;

	/* These are time of children */
	/* To implement this we should add in time at waitpid() */
	buf->tms_cutime=0;
	buf->tms_cstime=0;

	return 0;
}
