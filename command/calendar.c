# include "stdio.h"
# include "type.h"
# include "string.h"


int year, month, day;

int day_of_month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
int current_year, current_month, current_day;
//int fd_stdin = 0;
//int fd_stdout = 1;

int judgement(int y)
{
	if (y % 400 == 0 || (y % 100 != 0 && y % 4 == 0))
		return 1;
	else return 0;
}
int week_print(int year, int month, int day)
{

	int w, k;   
	int year_last = year % 100, c = year / 100, m = month;
	if (month == 1)
	{
		year_last -= 1;
		m = 13;
	}
	else if (month == 2)
	{
		year_last -= 1;
		m = 14;
	}
	w = (year_last + year_last / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + day - 1); // abs  ¾ø¶ÔÖµ
	if (w < 0)
	{
		k = (w % 7 + 7) % 7;
	}
	else k = w % 7;
	return k;
}


void year_show(int fd_stdin)
{
	int i, k, x, first_week;
	printf("please choose the year (eg:2020):");
	char rdbuf[128];
	int r = read(fd_stdin, rdbuf, 70);
	rdbuf[r] = 0;
	int A = rdbuf[0] - '0';
	int B = rdbuf[1] - '0';
	int C = rdbuf[2] - '0';
	int D = rdbuf[3] - '0';
	year = A * 1000 + B * 100 + C * 10 + D;


	printf("=================The Calendar of Year %d==================\n", year);
	printf("\n");
	if (judgement(year))
	{
		day_of_month[1] = 29;
	}
	else day_of_month[1] = 28;
	for (i = 1; i < 13; i++)
	{
		first_week = week_print(year, i, 1);
		printf("==========================%d=============================\n", i);
		printf("Sun Mon Tue Wed Thu Fri Sat\n");
		for (x = 1; x <= first_week; x++)
		{
			printf("    ");
			if (x % 7 == 0) printf("\n");
		}
		for (k = 1; k <= day_of_month[i - 1]; k++)
		{
			printf("%d  ", k);
			if (k < 10)printf(" ");
			if (x % 7 == 0) printf("\n");
			x++;
		}
		printf("\n");
		printf("\n");
		printf("\n");
	}

}


void month_show(int fd_stdin)
{
	int tag = 0;
	int k, x, first_week;

	do
	{
		printf("please choose the year (eg:2020):");
		char rdbuf2[128];
		int r2 = read(fd_stdin, rdbuf2, 70);
		rdbuf2[r2] = 0;
		int A = rdbuf2[0] - '0';
		int B = rdbuf2[1] - '0';
		int C = rdbuf2[2] - '0';
		int D = rdbuf2[3] - '0';
		year = A * 1000 + B * 100 + C * 10 + D;

		printf("please choose month (eg:12):");
		char rdbuf3[128];
		int r3 = read(fd_stdin, rdbuf3, 70);
		rdbuf3[r3] = 0;
		int E = rdbuf3[0] - '0';
		int F = rdbuf3[1] - '0';
		month = E * 10 + F;
		if (month < 1 || month>12)
		{
			printf("misinput,please choose again\n");

		}
	} while (1 > month || month > 12);
	printf("======================%d %d======================\n", year, month);
	if (judgement(year))
	{
		day_of_month[1] = 29;
	}
	else day_of_month[1] = 28;
	first_week = week_print(year, month, 1);
	printf("Sun Mon Tue Wed Thu Fri Sat\n");
	for (x = 1; x <= first_week; x++)
	{
		printf("    ");
		if (x % 7 == 0) printf("\n");
	}
	for (k = 1; k <= day_of_month[month - 1]; k++)
	{
		printf("%d  ", k);
		if (k < 10)printf(" ");
		if (x % 7 == 0) printf("\n");
		x++;
	}
	printf("\n");
}


int main(int argc, char * argv[])
{
	
	//int fd_stdin=2;
	//printf("fdin:%s\n",tty_name);
	//char *tty_name=;
	int fd_stdin=open("/dev_tty1",O_RDWR);
	//assert(fd_stdin==0);
	// int fd_stdout=open("/dev_tty1",O_RDWR);
	// assert(fd_stdin==1);
	int choice, flag = 1;
	year = 1;
	month = 1;
	day = 1;
	char c, k;
	for (; 1;)
	{
		printf("                                  MENU\n");
		printf("*****************************************************************************\n");
		printf("*                      OPTION:                                              *\n");
		printf("*                             1.search for a year                           *\n");
		printf("*                             2.search for a month                          *\n");
		printf("*                             0.exit                                        *\n");
		printf("*****************************************************************************\n");
		printf("choose please:   ");
		while (1)
		{           	
			while (1)
			{
				char at[70];
				char bt[70];
				if (flag)
				{
					char at[70];
					char bt[70];
					int r1 = read(fd_stdin, at, 70);
					at[r1] = 0;
					c = at[0];
					printf("\n");
				}



				choice = c - '0';
				if (choice >= 0 && choice < 3)
				{
					break;
				}
				else {
					printf("misinput,please choose a number between 0 and 2\n");
					printf("\n");
					printf("choose please: ");
				}

			}
			if (choice == 1)
			{
				year_show(fd_stdin);
				flag = 1;
			}
			else if (choice == 2)
			{
				month_show(fd_stdin);
				flag = 1;
			}
			else if (choice == 0)
				return 0;
			else
			{
				printf("misinput, please choose again\n");
			}
		}
	}

	return 0;
}