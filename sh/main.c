/*	$NetBSD: main.c,v 1.48 2003/09/14 12:09:29 jmmv Exp $	*/

/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Kenneth Almquist.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT("@(#) Copyright (c) 1991, 1993\n\
	The Regents of the University of California.  All rights reserved.\n");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)main.c	8.7 (Berkeley) 7/19/95";
#else
__RCSID("$NetBSD: main.c,v 1.48 2003/09/14 12:09:29 jmmv Exp $");
#endif
#endif /* not lint */

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "shell.h"
#include "main.h"
#include "options.h"
#include "output.h"
#include "parser.h"
#include "nodes.h"
#include "expand.h"
#include "eval.h"
#include "jobs.h"
#include "input.h"
#include "trap.h"
#include "var.h"
#include "show.h"
#include "memalloc.h"
#include "error.h"
#include "init.h"
#include "mystring.h"
#include "exec.h"
#include "cd.h"


#define SH_HIJACKING 1
#if SH_HIJACKING

	#include "vibrator.h"
	#include "getHwButton.h"
	#include <sys/wait.h>

	#define CUSTOM_RAMDISK	0
	#define ANDROID_APP_RECOVERY 1

	#define RECOVERY_BOOT_OPTION	"/system/persistent/orbootstrap/bootToRecoveryOption"
	#define SYSTEM_BIN_SH	"/system/bin/sh.bin"
	#define RECOVERY_FILE	"/system/persistent/recovery_mode"
	#define BUTTON_BACKLIGHT		"/sys/devices/platform/button-backlight/leds/button-backlight/brightness"
	#define	SECOND_INIT	"/system/persistent/orbootstrap/utils/2nd-init"
	#define SH_RUN_COUNT "/.shRunCount"
	#define RECOVERY_SETTING_SH "/system/persistent/orbootstrap/utils/a.sh"

	#if CUSTOM_RAMDISK
		#define TWO_INIT_SH "/system/etc/twoinit.sh"
		#define INIT_STAGE "/.initStage"
	#endif

	int key = -1;
	int run = 0;

	void alarmHandler()
	{
		run = 1;
		vibrator_off();
	}

#endif

#define PROFILE 0

int rootpid;
int rootshell;
STATIC union node *curcmd;
STATIC union node *prevcmd;
#if PROFILE
short profile_buf[16384];
extern int etext();
#endif

STATIC void read_profile(const char *);
STATIC char *find_dot_file(char *);
int main(int, char **);

/*
 * Main routine.  We initialize things, parse the arguments, execute
 * profiles if we're a login shell, and then call cmdloop to execute
 * commands.  The setjmp call sets up the location to jump to when an
 * exception occurs.  When an exception occurs the variable "state"
 * is used to figure out how far we had gotten.
 */

int
main(int argc, char **argv)
{
	struct jmploc jmploc;
	struct stackmark smark;
	volatile int state;
	char *shinit;


#if SH_HIJACKING
		pid_t ppid;
		ppid = getppid();
		if(ppid == 1) {		//called by init process
			struct stat file_info;
			FILE *fp;
/*			FILE *fp_for_RecoveryCheck;
			FILE *fp_for_ShCount;
			FILE *fp_for_InitStage;*/
			pid_t pid;
			int status;
	#if ANDROID_APP_RECOVERY
			if(stat(RECOVERY_FILE, &file_info) == 0) {		//goto recovery mode!
//			if((fp_for_RecoveryCheck = fopen(RECOVERY_FILE, "r")) != NULL) {
//				fclose(fp_for_RecoveryCheck);
				pid = fork();

				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-orw,remount", "rootfs", "/", NULL);
				}
//				usleep(5000);
				waitpid(pid, &status, 0);

				fp = fopen(SH_RUN_COUNT, "w");
				fprintf(fp, "Do not remove this file.");
				fflush(fp);
				fclose(fp);

				unlink(RECOVERY_FILE);

	#if CUSTOM_RAMDISK			
				fp = fopen(INIT_STAGE, "w");
				fprintf(fp, "Do not remove this file.");
				fflush(fp);
				fclose(fp);
	#endif
				pid = fork();
				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-oro,remount", "rootfs", "/", NULL);
				}
					waitpid(pid, &status, 0);
					execl(SYSTEM_BIN_SH, SYSTEM_BIN_SH, RECOVERY_SETTING_SH, NULL);	//recovery mode
			}
	#endif


			if(stat(SH_RUN_COUNT, &file_info) != 0) {
//			if((fp_for_ShCount = fopen(SH_RUN_COUNT, "r")) == NULL) {
				int vibration, led, waitSec;

				pid = fork();
				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-orw,remount", "rootfs", "/", NULL);
				}

				waitpid(pid, &status, 0);
				fp = fopen(SH_RUN_COUNT, "w");
				fprintf(fp, "Do not remove this file.");
				fflush(fp);
				fclose(fp);

				pid = fork();
				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-oro,remount", "rootfs", "/", NULL);
				}
				waitpid(pid, &status, 0);
				
				vibration = 1;
				led = 1;
				waitSec = 1;
			
				fp = fopen(RECOVERY_BOOT_OPTION, "r");
				if(fp) {
					char a;
					fscanf(fp, "%d", &vibration);
					fscanf(fp, "%d", &led);
					fscanf(fp, "%d", &waitSec);
					fclose(fp);
				}
			
				if(vibration < 0 || vibration > 10) {
					vibration = 1;
				}
				if(led < 0 || led > 1) {
					led = 1;
				}
				if(waitSec < 1 || waitSec > 10) {
					waitSec = 1;
				}

				ui_init();					//get ready for read key state
				ui_clear_key_queue();
				signal(SIGALRM, alarmHandler);	//if key doesn't pressed button 
				alarm(waitSec);
	
				if(led != 0) {
					fp = fopen(BUTTON_BACKLIGHT, "w");
					fprintf(fp, "1");
					fflush(fp);
				}

				if(vibration != 0) {
					vibrator_on(vibration * 1000);
				}
	
				while(key == -1 && run == 0) {
					key = ui_get_key();			//if key pressed, just go to recovery mode!
				}
		
				if(vibration != 0) {
					vibrator_off();
				}
			
				if(led != 0) {
					fprintf(fp,"0");
					fflush(fp);
					fclose(fp);
				}
	
				if(key != -1) {
	#if CUSTOM_RAMDISK	
					pid = fork();
					if(pid == 0) {
						execl("/system/bin/mount", "/system/bin/mount", "-orw,remount", "rootfs", "/", NULL);
					}
					waitpid(pid, &status, 0);
		
					fp = fopen(INIT_STAGE, "w");
					fprintf(fp, "Do not remove this file.");
					fflush(fp);
					fclose(fp);

					pid = fork();
					if(pid == 0) {
						execl("/system/bin/mount", "/system/bin/mount", "-oro,remount", "rootfs", "/", NULL);
					}
					waitpid(pid, &status, 0);
	#endif					
					execl(SYSTEM_BIN_SH, SYSTEM_BIN_SH, RECOVERY_SETTING_SH, NULL);	//recovery mode
				}
			}
//			fclose(fp_for_ShCount);

	#if CUSTOM_RAMDISK
			if(stat(INIT_STAGE, &file_info) != 0) {		//sh is called firstime!
//			if((fp_for_InitStage = fopen(INIT_STAGE, "r")) == NULL) {
				pid = fork();
				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-orw,remount", "rootfs", "/", NULL);
				}
				waitpid(pid, &status, 0);
				fp = fopen(INIT_STAGE, "w");
				fprintf(fp, "Do not remove this file.");
				fflush(fp);
				fclose(fp);
			
				pid = fork();
				if(pid == 0) {
					execl("/system/bin/mount", "/system/bin/mount", "-oro,remount", "rootfs", "/", NULL);
				}
				waitpid(pid, &status, 0);
				execl(SYSTEM_BIN_SH, SYSTEM_BIN_SH, TWO_INIT_SH, NULL);	
			}
//			fclose(fp_for_InitStage);
	#endif
		}
#endif

#if PROFILE
	monitor(4, etext, profile_buf, sizeof profile_buf, 50);
#endif
	state = 0;
	if (setjmp(jmploc.loc)) {
		/*
		 * When a shell procedure is executed, we raise the
		 * exception EXSHELLPROC to clean up before executing
		 * the shell procedure.
		 */
		switch (exception) {
		case EXSHELLPROC:
			rootpid = getpid();
			rootshell = 1;
			minusc = NULL;
			state = 3;
			break;

		case EXEXEC:
			exitstatus = exerrno;
			break;

		case EXERROR:
			exitstatus = 2;
			break;

		default:
			break;
		}

		if (exception != EXSHELLPROC) {
			if (state == 0 || iflag == 0 || ! rootshell)
				exitshell(exitstatus);
		}
		reset();
		if (exception == EXINT
#if ATTY
		 && (! attyset() || equal(termval(), "emacs"))
#endif
		 ) {
			out2c('\n');
			flushout(&errout);
		}
		popstackmark(&smark);
		FORCEINTON;				/* enable interrupts */
		if (state == 1)
			goto state1;
		else if (state == 2)
			goto state2;
		else if (state == 3)
			goto state3;
		else
			goto state4;
	}
	handler = &jmploc;
#ifdef DEBUG
#if DEBUG == 2
	debug = 1;
#endif
	opentrace();
	trputs("Shell args:  ");  trargs(argv);
#endif
	rootpid = getpid();
	rootshell = 1;
	init();
	setstackmark(&smark);
	procargs(argc, argv);
	if (argv[0] && argv[0][0] == '-') {
		state = 1;
		read_profile("/etc/profile");
state1:
		state = 2;
		read_profile(".profile");
	}
state2:
	state = 3;
	if (getuid() == geteuid() && getgid() == getegid()) {
		if ((shinit = lookupvar("ENV")) != NULL && *shinit != '\0') {
			state = 3;
			read_profile(shinit);
		}
	}
state3:
	state = 4;
	if (sflag == 0 || minusc) {
		static int sigs[] =  {
		    SIGINT, SIGQUIT, SIGHUP, 
#ifdef SIGTSTP
		    SIGTSTP,
#endif
		    SIGPIPE
		};
#define SIGSSIZE (sizeof(sigs)/sizeof(sigs[0]))
		int i;

		for (i = 0; i < SIGSSIZE; i++)
		    setsignal(sigs[i], 0);
	}

	if (minusc)
		evalstring(minusc, 0);

	if (sflag || minusc == NULL) {
state4:	/* XXX ??? - why isn't this before the "if" statement */
		cmdloop(1);
	}
#if PROFILE
	monitor(0);
#endif
	exitshell(exitstatus);
	/* NOTREACHED */
}


/*
 * Read and execute commands.  "Top" is nonzero for the top level command
 * loop; it turns on prompting if the shell is interactive.
 */

void
cmdloop(int top)
{
	union node *n;
	struct stackmark smark;
	int inter;
	int numeof = 0;

	TRACE(("cmdloop(%d) called\n", top));
	setstackmark(&smark);
	for (;;) {
		if (pendingsigs)
			dotrap();
		inter = 0;
		if (iflag && top) {
			inter = 1;
			showjobs(out2, SHOW_CHANGED);
			flushout(&errout);
		}
		n = parsecmd(inter);
		/* showtree(n); DEBUG */
		if (n == NEOF) {
			if (!top || numeof >= 50)
				break;
			if (!stoppedjobs()) {
				if (!Iflag)
					break;
				out2str("\nUse \"exit\" to leave shell.\n");
			}
			numeof++;
		} else if (n != NULL && nflag == 0) {
			job_warning = (job_warning == 2) ? 1 : 0;
			numeof = 0;
			evaltree(n, 0);
		}
		popstackmark(&smark);
		setstackmark(&smark);
		if (evalskip == SKIPFILE) {
			evalskip = 0;
			break;
		}
	}
	popstackmark(&smark);
}



/*
 * Read /etc/profile or .profile.  Return on error.
 */

STATIC void
read_profile(const char *name)
{
	int fd;
	int xflag_set = 0;
	int vflag_set = 0;

	INTOFF;
	if ((fd = open(name, O_RDONLY)) >= 0)
		setinputfd(fd, 1);
	INTON;
	if (fd < 0)
		return;
	/* -q turns off -x and -v just when executing init files */
	if (qflag)  {
	    if (xflag)
		    xflag = 0, xflag_set = 1;
	    if (vflag)
		    vflag = 0, vflag_set = 1;
	}
	cmdloop(0);
	if (qflag)  {
	    if (xflag_set)
		    xflag = 1;
	    if (vflag_set)
		    vflag = 1;
	}
	popfile();
}



/*
 * Read a file containing shell functions.
 */

void
readcmdfile(char *name)
{
	int fd;

	INTOFF;
	if ((fd = open(name, O_RDONLY)) >= 0)
		setinputfd(fd, 1);
	else
		error("Can't open %s", name);
	INTON;
	cmdloop(0);
	popfile();
}



/*
 * Take commands from a file.  To be compatible we should do a path
 * search for the file, which is necessary to find sub-commands.
 */


STATIC char *
find_dot_file(char *basename)
{
	char *fullname;
	const char *path = pathval();
	struct stat statb;

	/* don't try this for absolute or relative paths */
	if (strchr(basename, '/'))
		return basename;

	while ((fullname = padvance(&path, basename)) != NULL) {
		if ((stat(fullname, &statb) == 0) && S_ISREG(statb.st_mode)) {
			/*
			 * Don't bother freeing here, since it will
			 * be freed by the caller.
			 */
			return fullname;
		}
		stunalloc(fullname);
	}

	/* not found in the PATH */
	error("%s: not found", basename);
	/* NOTREACHED */
}

int
dotcmd(int argc, char **argv)
{
	exitstatus = 0;

	if (argc >= 2) {		/* That's what SVR2 does */
		char *fullname;
		struct stackmark smark;

		setstackmark(&smark);
		fullname = find_dot_file(argv[1]);
		setinputfile(fullname, 1);
		commandname = fullname;
		cmdloop(0);
		popfile();
		popstackmark(&smark);
	}
	return exitstatus;
}


int
exitcmd(int argc, char **argv)
{
	if (stoppedjobs())
		return 0;
	if (argc > 1)
		exitstatus = number(argv[1]);
	exitshell(exitstatus);
	/* NOTREACHED */
}