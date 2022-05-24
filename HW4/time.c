#include <stdio.h>      
#include <time.h>       
#include <stdlib.h>

const int monthDays[]
    = {0,31,28,31,30,31,30,31,31,30,31,30,31};
int main ()
{
  int day, month, year;
  time_t now;
  struct tm* timeinfo;
  struct tm startDate;
  struct tm endDate;
  char buffer [80];
  char buffer2 [80];

  time (&now);
  timeinfo = localtime (&now);

  strftime (buffer,80,"Current date and time %D %A %r %Z.",timeinfo); //in-order to print out the current date
  puts (buffer);
  
  printf("Please enter a date time and year(mm/dd/yyyy):"); //read in the users input
  if(scanf("%d/%d/%d", &month, &day, &year) == 3)
  {
    if(month <= 0 || month > 12 || day <= 0 || day > 31 || year <= 2021) 
    {
      printf("There was an error\n");
    }
    else
    {
    printf("The date you enetered was: %d/%d/%d\n", month, day, year); 
    }
    
  }
  else
  {
   printf("There was an error\n"); 
  }
  
  long int n1 = 365 * timeinfo->tm_year + timeinfo -> tm_mday;

  for(int i = 0; i < timeinfo -> tm_mon - 1; i++)
    n1 += monthDays[i];

  long int n2 = year * 365 + day;
  for(int i = 0; i < month - 1; i++)
    n2 += monthDays[i];

  int curr1, curr2, curr3;
  startDate.tm_mday = curr1;
  startDate.tm_mon = curr2;
  startDate.tm_year = curr3;
  
  //return(n2-n1);

  unsigned long int diff = difftime(n2,n1);
  printf("Until the targetted date: %lu", diff);

  return 0;
}
