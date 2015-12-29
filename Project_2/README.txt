Authors:
1) Ashok Marannan
CS ID: marannan
Wisc ID: marannan
Mail: marannan@wisc.edu 

2) Kavin Mani
CS ID: kavin
Wisc ID: mani4
Mail: mani4@wisc.edu
 
Project 2a: mysh
Build No: v1.0
Release Date: 10-07-2015


Introduction: 
	This program implements a custom shell similar to unix shell environments.
mysh takes user inputs and creates a child using fork() and execute the user input commands using execvp().
It also keeps track of upto 20 user inputs in history and executes history commands with !<command no> input command.
Redirection is supported by duplicating the STDOUT_FILENO with the user specified redirection file. 
This shell can be run in interactive and batch mode.  

Usage: 
	./mysh  		- launches interactive shell
	./mysh batch_file	- executes batch file in batch mode
	./mysh < batch_file	- execute batch file in interactive mode

Known issues: 
	Test cases available at ~cs537-1/ta/tests/2a/ need to run with -c -n arguments to avoid temp directory permission issues
	Example ~cs537-1/ta/tests/2a/runtests -c -n 

Bug reporting: 
	File bugs at marannan@wisc.edu & mani4@wisc.edu



Project 2b: MLFQ scheduler in xv6
Build No: v1.0

Introduction: 
	This project invloves implementing Multi Level Feedback Queue (MLFQ) scheduling in xv6 and implementing a system call named sysgetpinfo(). 
The MLFQ scheduling is implemented as per the project requirements. There are four priority queues available (Q0, Q1, Q2 and Q3) in the system. 
The scheduler will scan for runnable process available in highest priority queue (Q0) and schedule it if found. If no process is to be found in the highest priority queue (Q0), 
the scheduler will look at the next highest priority queue (Q1) and so on. This is implemented as a function called find_runnable(queue) (proc.c) that returns runnable process highest priority queue and so on.
Once the process is scheduled, it is allowed to run for (ticks = 1 for Q0, ticks = 2 for Q1, ticks = 4 for Q2, and ticks = 8 for Q3 ) amount of time before it is demoted to lower priority queue 
and pre-empted to schedule another process suitable for running, starting from highest priority queue. 
Once the process is demoted to Q0, it has to stay in Q0 until it is completed or killed. This is implemented in function trap() (trap.c) under timer interrupt. 
All the process in the same priority queue are scheduled in round robin fashion. This is implemented in function scheduler() (proc.c). 
sysgetpinfo() will retrieve the process information containing process id, no of ticks run and queue no. 

Bug reporting:
	File bugs at marannan@wisc.edu & mani4@wisc.edu

