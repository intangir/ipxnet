/*
 *  Copyright (C) 2009-2010  Michał "MasterM" Siejak
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "ipx.h"
#include "ipxserver.h"

static FILE *log = NULL;

static void quit_handler(int signal)
{
  IPX_StopServer();
  LOG_MSG("MAIN: Server shutdown.");
#ifdef LOG_TO_FILE
  fclose(log);
#endif
  exit(0);
}

static void daemon_handler(int signum)
{
  switch(signum) {
  case SIGALRM: exit(1); break;
  case SIGCHLD: exit(1); break;
  case SIGUSR1: exit(0); break;
  }
}

int log_stdout(const char *fmt, ...)
{
  va_list args;
  int ret;

  time_t timestamp;
  struct tm *t;
  char buffer[50];
      
  time(&timestamp);
  t = localtime(&timestamp);
  sprintf(buffer, "%02d-%02d-%d %02d:%02d:%02d", t->tm_mon+1, t->tm_mday, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec);
            
  va_start(args, fmt);
  printf("[%s] ", buffer);
  ret = vprintf(fmt, args);
  printf("\n");
  va_end(args);
  fflush(stdout);
  return ret;
}

int log_file(const char *fmt, ...)
{
  va_list args;
  int ret;

  time_t timestamp;
  struct tm *t;
  char buffer[50];

  time(&timestamp);
  t = localtime(&timestamp);
  sprintf(buffer, "%02d-%02d-%d %02d:%02d:%02d", t->tm_mon+1, t->tm_mday, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec);

  va_start(args, fmt);
  fprintf(log, "[%s] ", buffer);
  ret = vfprintf(log, fmt, args);
  fprintf(log, "\n");
  va_end(args);
  fflush(log);
  return ret;
}

void daemonize()
{
  pid_t pid, sid, parent;

  // If we're already a daemon, return.
  if(getppid() == 1) return;

  signal(SIGCHLD, daemon_handler);
  signal(SIGALRM, daemon_handler);
  signal(SIGUSR1, daemon_handler);

  // Fork off the parent process.
  pid = fork();
  if(pid < 0)
    exit(1);
  else if(pid > 0) {
    // Wait for SIGUSR1 or exit with failure.
    alarm(2);
    pause();
    exit(1);
  }

  parent = getppid();
  
  // Cancel out signals
  signal(SIGCHLD, SIG_DFL);  // Child dies
  signal(SIGTERM, SIG_DFL);  // Default on SIGTERM
  signal(SIGHUP,  SIG_IGN);  // Ignore SIGHUP
  signal(SIGTSTP, SIG_IGN);  // TTY signals below:
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);

  // Change the file mode mask
  umask(0);

  // Create new session (sid)
  sid = setsid();
  if(sid < 0) exit(1);

  // Redirect standard files to /dev/null
  freopen("/dev/null", "r", stdin);
  freopen("/dev/null", "w", stdout);
  freopen("/dev/null", "w", stderr);
  
  kill(parent, SIGUSR1);
}

int main(int argc, char **argv)
{
  int port;
  if(argc < 2) {
    fprintf(stderr, "Usage: %s <port>\n", PROGNAME);
    return 1;
  }

  port = atoi(argv[1]);
  if(port <= 0) {
    fprintf(stderr, "%s: Invalid port number!\n", PROGNAME);
    return 2;
  }

#ifdef LOG_TO_FILE
  log = fopen(LOGNAME, "a");
  if(log == NULL) {
    fprintf(stderr, "%s: Cannot open logfile!\n", PROGNAME);
    return 2;
  }

  // Go daemon if logging to file
  daemonize();
#endif

  // Start tunneling service
  signal(SIGTERM, quit_handler);
  signal(SIGINT, quit_handler);
  if(!IPX_StartServer(port)) {
    fprintf(stderr, "%s: Could not start IPX server!\n", PROGNAME);
    return 1;
  }

  // Main loop
  LOG_MSG("MAIN: Server started on UDP port %d", port);
  while(1) {
    IPX_ServerLoop();
    usleep(TICK_TIME);
  }
  return 0;
}
