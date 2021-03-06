/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#ifndef UTILITY_H_
#define UTILITY_H_

extern void clr(void);
extern void dir(char *directory); // Display contents of directory
extern void echo(char *comment);
extern void help(void);
extern void halt(); // pause is already defined in unistd.h, Using Halt instead
extern void quit(void);
extern void exec(void); // Both fork and execute the program
// This might help -> http://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1044654269&id=1043284392
extern void execAsync(void); // OPTIONAL FOR BONUS MARKS

// Include your relevant functions declarations here they must start with the
// extern keyword such as in the following example:
// extern void display_help(void);

#endif /* UTILITY_H_ */
