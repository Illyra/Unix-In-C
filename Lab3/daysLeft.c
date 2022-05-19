#include <stdio.h>      
#include <time.h>       
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <apue.h>

#define START     0
#define from_ctrl_C     1
#define from_alarm      2


void      INThandler(int sig_num);
void      ALARMhandler(int s);

void ALARMhandler(int s){
  alarm(3);
  signal(SIGALRM, ALARMhandler);
}

void INThandler(int sig_num)
{
  double seconds3, days2, hours2, minutes2, seconds4;
  time_t now;
  struct tm* timeinfo;
  time_t currentDate;
  struct tm *july4th;
  struct tm july, thatDay;
  char buffer [80];
  
  char c;

    time (&now);
    timeinfo = localtime(&now);
    time (&currentDate);
    thatDay = *localtime(&currentDate);

    july4th = localtime(&currentDate);
    july4th -> tm_mon = 6; /*0 == january 6 == july*/
    july4th -> tm_mday = 4;
    if (thatDay.tm_mday > 4 && thatDay.tm_mon == 4)
      july4th -> tm_year = thatDay.tm_year + 1;

    july.tm_hour = 23;
    july.tm_min = 59;
    july.tm_sec = 60;

    seconds3 = difftime(mktime(july4th), currentDate);
    hours2 = (july.tm_hour) - (timeinfo -> tm_hour); /*subtracting the hours (12:00) by the current time*/
    days2 = seconds3/86400; /*calculating the number of days until christmas*/
    minutes2 = (july.tm_min) - (timeinfo -> tm_min); /*calculating the number of minutes left (00) by the current time*/
    seconds4 = (july.tm_sec) - (timeinfo -> tm_sec); /*calculating the number of seconds left (0-60) by the current time to return number of secs left*/

    signal(SIGALRM, SIG_IGN);
    signal(SIGINT, SIG_IGN);
  
    printf("Quit  (y/n)?: ");
    c = getchar();
    if (c == 'Y' || c == 'y')
      exit(0);
    else
    {
        printf("By SIGINT:\n");
        strftime (buffer,80,"  Current date and time %D %A %r %Z.",timeinfo); /*in-order to print out the current date*/
        puts (buffer);
        printf("  Until 2022 July 4th: %.f Days, ", days2);
        printf("%.f Hours, ", hours2);
        printf("%g Minutes, ", minutes2);
        printf("%.f Seconds \n", seconds4);  
        printf("  %.f Total number of seconds until July 4th\n", seconds3);
    }
  
  signal(SIGINT, INThandler);
  fflush(stdout);
}

int main()
{
    double seconds, days, hours, minutes, seconds2;
    time_t now;
    struct tm* timeinfo;
    time_t currentDate;
    struct tm *christmas, today;
    struct tm xmas;
    char buffer [80];

    time (&now);
    timeinfo = localtime(&now);
    time (&currentDate);
    today = *localtime(&currentDate);
  
    christmas = localtime(&currentDate);
    christmas -> tm_mon = 11; /*0 == January, 11 == December*/
    christmas -> tm_mday = 25; /*number of days*/
    if (today.tm_mday > 25 && today.tm_mon == 11)
      christmas -> tm_year = today.tm_year + 1;
  
    xmas.tm_hour = 23; /*0-23 to represent 24 hours*/
    xmas.tm_min = 59; /*0-59 to represent 60 minutes*/
    xmas.tm_sec  = 60; /*0-60 represents a full minute but in seconds*/

    seconds = difftime(mktime(christmas),currentDate);
    hours = (xmas.tm_hour) - (timeinfo -> tm_hour); /*subtracting the hours (12:00) by the current time*/
    days = seconds/86400; /*calculating the number of days until christmas*/
    minutes = (xmas.tm_min) - (timeinfo -> tm_min); /*calculating the number of minutes left (00) by the current time*/
    seconds2 = (xmas.tm_sec) - (timeinfo -> tm_sec); /*calculating the number of seconds left (0-60) by the current time to return number of secs left*/


  signal(SIGINT, SIG_DFL); /*reset to default case*/

  signal(SIGALRM, ALARMhandler);
  signal (SIGINT, INThandler);
  alarm(3); /*delay the start by 3 seconds*/
  int i;
  while(1){
    for(i = 0;;i ++)
      {
      if(i % 2 == 1) /*for every 3rd signal*/
      {
        printf("%d By SIGALARM:\n", i);
        strftime (buffer,80,"  Current date and time %D %A %r %Z.",timeinfo); /*in-order to print out the current date*/
        puts (buffer);
        printf("  Until 2019 Christmas Day: %.f Days, ", days);
        printf("%.f Hours, ", hours);
        printf("%g Minutes, ", minutes);
        printf("%.f Seconds \n", seconds2);  
        printf("  %.f Total number of seconds until Christmas\n", seconds);
        alarm(8); /*delay by 8 seconds*/
        }
      else /*else if its not every 3rd signal just continue*/
      {
        printf("%d By SIGALARM:\n", i);
        strftime (buffer,80,"  Current date and time %D %A %r %Z.",timeinfo); /*in-order to print out the current date*/
        puts (buffer);
        printf("  Until 2019 Christmas Day: %.f Days, ", days);
        printf("%.f Hours, ", hours);
        printf("%g Minutes, ", minutes);
        printf("%.f Seconds \n", seconds2);  
        printf("  %.f Total number of seconds until Christmas\n", seconds);
        alarm(3); /*delay by 3 seconds*/
        }
      pause(); 
      }
    }
    return 0;
}
