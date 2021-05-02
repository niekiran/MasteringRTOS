

#include "main.h"

 void show_time_date_itm(void)
 {
	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

	memset(&rtc_date,0,sizeof(rtc_date));
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";


	printf("%02d:%02d:%02d [%s]",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	printf("\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);

 }

void show_time_date(void)
{
	static char showtime[40];
	static char showdate[40];

	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

	static char *time = showtime;
	static char *date = showdate;

	memset(&rtc_date,0,sizeof(rtc_date));
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	/* Display time Format : hh:mm:ss [AM/PM] */
	sprintf((char*)showtime,"%s:\t%02d:%02d:%02d [%s]","\nCurrent Time&Date",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	xQueueSend(q_print,&time,portMAX_DELAY);

	/* Display date Format : date-month-year */
	sprintf((char*)showdate,"\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);
	xQueueSend(q_print,&date,portMAX_DELAY);
}


void rtc_configure_time(RTC_TimeTypeDef *time)
{

	time->TimeFormat = RTC_HOURFORMAT12_AM;
	time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	time->StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&hrtc,time,RTC_FORMAT_BIN);
}


void rtc_configure_date(RTC_DateTypeDef *date)
{
	HAL_RTC_SetDate(&hrtc,date,RTC_FORMAT_BIN);
}


int validate_rtc_information(RTC_TimeTypeDef *time , RTC_DateTypeDef *date)
{
	if(time){
		if( (time->Hours > 12) || (time->Minutes > 59) || (time->Seconds > 59) )
			return 1;
	}

	if(date){
		if( (date->Date > 31) || (date->WeekDay > 7) || (date->Year > 99) || (date->Month > 12) )
			return 1;
	}

	return 0;
}
