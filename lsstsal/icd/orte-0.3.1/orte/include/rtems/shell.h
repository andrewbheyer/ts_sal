/**
 * @file rtems/shell.h
 *
 *  Instantatiate a new terminal shell.
 */

/*
 *  Author:
 *
 *   WORK: fernando.ruiz@ctv.es
 *   HOME: correo@fernando-ruiz.com
 *
 *   Thanks at:
 *    Chris John
 *
 *  $Id: shell.h,v 1.7 2004/12/02 18:20:51 ralf Exp $
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include <rtems.h>
#include <stdio.h>
#include <termios.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*shell_command_t)(int argc,char * argv[]);

struct shell_cmd_tt ;
typedef struct shell_cmd_tt shell_cmd_t;

struct shell_cmd_tt {
 char * name;
 char * usage;
 char * topic;
 shell_command_t command;
 shell_cmd_t * alias;
 shell_cmd_t * next;
};

uint32_t   new_rtems_name(char * rtems_name);
shell_cmd_t * shell_lookup_cmd(char * cmd);
shell_cmd_t * shell_add_cmd(char * cmd,
                            char * topic,
                            char * usage,
                            shell_command_t command);
shell_cmd_t * shell_alias_cmd(char * cmd, char * alias);

int shell_make_args(char * cmd,
                    int  * pargc,
                    char * argv[]);

typedef struct  {
  rtems_name magic; /* 'S','E','N','V': Shell Environment */
  char * devname;
  char * taskname;
  tcflag_t tcflag;
  /* user extensions */
  int  exit_shell; /* logout */
  int  forever   ; /* repeat login */
  int  errorlevel;
  int  mdump_adr;
} shell_env_t;

int shell_scanline(char * line,int size,FILE * in,FILE * out) ;
void cat_file(FILE * out,char *name);
void write_file(char *name,char * content);

rtems_status_code shell_init(char * task_name      ,
                              uint32_t      task_stacksize,/*0 default*/
		              rtems_task_priority task_priority ,
                              char * devname      ,
			      tcflag_t tcflag     ,
			      int forever         );

extern shell_env_t global_shell_env,
                *  current_shell_env;
/*--------*/
/* cmds.c */
/*--------*/
int str2int(char * s);
void register_cmds(void);

#ifdef __cplusplus
}
#endif

#endif
