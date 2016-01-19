/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef MYSHELL_H_
#define MYSHELL_H_

// Include your relevant functions declarations here they must start with the
// extern keyword such as in the following example:
// extern void display_help(void);

extern void refresh(void); //Refresh the CWD then display the CWD
extern void environ(void); // print environment variables to screen
extern void tokenize(char *str, char *cmd, char *arg);

#endif /* MYSHELL_H_ */
