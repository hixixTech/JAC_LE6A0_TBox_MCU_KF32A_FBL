/**
  ******************************************************************************
  * �ļ���  kf32a_basic_rtc.h
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ʵʱʱ��(RTC)��ع��ܺ�����������غ궨�塣
  *********************************************************************
  */

#ifndef _KF32A_BASIC_RTC_H_
#define _KF32A_BASIC_RTC_H_

#include "KF32A_BASIC.h"


/**
  * ����  ʵʱʱ��(RTC)ʱ����Ϣ�ṹ��
  */
typedef struct
{
    uint8_t m_Hours;              /* RTCʱ�ӣ�
                                   ȡֵ��Χ24Сʱ��ʱΪ0-23��12Сʱ��ʱΪ1-12��*/
    uint8_t m_Minutes;            /* RTC���ӣ�
                                   ȡֵ��ΧΪ0-59��*/
    uint8_t m_Seconds;            /* RTC���ӣ�
                                   ȡֵ��ΧΪ0-59��*/
    uint8_t m_AMPM;               /* RTC AM/PMѡ��
                                   ȡֵΪ�ꡰAM/PMѡ���е�һ����*/
}RTC_TimeTypeDef;

/**
  * ����  ʵʱʱ��(RTC)������Ϣ�ṹ��
  */
typedef struct
{
    uint8_t m_WeekDay;            /* RTC ���ڣ�
                                   ȡֵΪ�ꡰRTC ���ڡ��е�һ����*/
    uint8_t m_Day;                /* RTC �գ�
                                   ȡֵ��ΧΪ1-31��*/
    uint8_t m_Month;              /* RTC �£�
                                   ȡֵΪ�ꡰRTC �·ݡ��е�һ����*/
    uint8_t m_Year;               /* RTC �꣬
                                   ȡֵ��ΧΪ0-99��*/
}RTC_DateTypeDef;

/**
  * ����  ʵʱʱ��(RTC)��ʼ����Ϣ�ṹ��
  */
typedef struct
{
    uint32_t m_ClockSource;         /* RTCʱ��Դ��
                                   ȡֵΪ�ꡰRTCʱ��Դ���е�һ����*/
    uint32_t m_HourFormat;          /* ʱ���ʽ��
                                   ȡֵΪ�ꡰʱ���ʽ���е�һ����*/
    RTC_TimeTypeDef m_TimeStruct;  /* ʱ����Ϣ�ṹ�� */
    RTC_DateTypeDef m_DateStruct;  /* ������Ϣ�ṹ�� */
}RTC_InitTypeDef;

/**
  * ����  ʵʱʱ��(RTC)������Ϣ�ṹ��
  */
typedef struct
{
    uint8_t m_WeekDay;            /* RTC ���ڣ�
                                   ȡֵΪ�ꡰRTC ���ڡ��е�һ����*/
    uint8_t m_Hours;              /* RTCʱ�ӣ�
                                   ȡֵ��Χ24Сʱ��ʱΪ0-23��12Сʱ��ʱΪ1-12��*/
    uint8_t m_Minutes;            /* RTC���ӣ�
                                   ȡֵ��ΧΪ0-59��*/
    uint8_t m_Seconds;            /* RTC���ӣ�
                                   ȡֵ��ΧΪ0-59��*/
    uint8_t m_AMPM;               /* RTC AM/PMѡ��
                                   ȡֵΪ�ꡰAM/PMѡ���е�һ����*/
    uint32_t m_WeekDayEnable;     /* RTC ������ʹ�ܣ�
                                   ȡֵΪTRUE��FALSE��*/
    uint32_t m_HoursEnable;       /* RTC ����ʱʹ�ܣ�
                                   ȡֵΪTRUE��FALSE��*/
    uint32_t m_MinutesEnable;     /* RTC ���ӷ�ʹ�ܣ�
                                   ȡֵΪTRUE��FALSE��*/
    uint32_t m_SecondsEnable;     /* RTC ������ʹ�ܣ�
                                   ȡֵΪTRUE��FALSE��*/
}RTC_AlarmTypeDef;

/**
  * RTCʱ���ͨ��
  */
#define RTC_TIME_STAMP_CHANNEL0               ((uint32_t)1<<RTC_CR_RTCTSCH0_POS)
#define RTC_TIME_STAMP_CHANNEL1               ((uint32_t)1<<RTC_CR_RTCTSCH1_POS)
#define RTC_TIME_STAMP_CHANNEL2               ((uint32_t)1<<RTC_CR_RTCTSCH2_POS)
#define RTC_TIME_STAMP_CHANNEL3       		  ((uint32_t)1<<RTC_CR_RTCTSCH3_POS)
#define RTC_TIME_STAMP_CHANNEL4       		  ((uint32_t)1<<RTC_CR_RTCTSCH4_POS)
#define RTC_TIME_STAMP_CHANNEL5       		  ((uint32_t)1<<RTC_CR_RTCTSCH5_POS)
#define CHECK_RTC_TIME_STAMP_CHANNEL(SEL)         (((SEL) == RTC_TIME_STAMP_CHANNEL0) \
												|| ((SEL) == RTC_TIME_STAMP_CHANNEL1) \
												|| ((SEL) == RTC_TIME_STAMP_CHANNEL2) \
												|| ((SEL) == RTC_TIME_STAMP_CHANNEL3) \
												|| ((SEL) == RTC_TIME_STAMP_CHANNEL4) \
												|| ((SEL) == RTC_TIME_STAMP_CHANNEL5))

/**
  * RTCʱ��Դ
  */
#define RTC_SOURCE_NO_CLK               ((uint32_t)0<<BKP_CTL_RTCCLKS0_POS)
#define RTC_SOURCE_EXTLF                ((uint32_t)1<<BKP_CTL_RTCCLKS0_POS)
#define RTC_SOURCE_INTLF                ((uint32_t)2<<BKP_CTL_RTCCLKS0_POS)
#define RTC_SOURCE_EXTHF_DIV_128        ((uint32_t)3<<BKP_CTL_RTCCLKS0_POS)
#define CHECK_RTC_SOURCE(SEL)           (((SEL) == RTC_SOURCE_NO_CLK) \
                                      || ((SEL) == RTC_SOURCE_EXTLF) \
                                      || ((SEL) == RTC_SOURCE_INTLF) \
                                      || ((SEL) == RTC_SOURCE_EXTHF_DIV_128))

/**
  * ����ʱ����ֵ��ʽ
  */
#define RTC_TIME_FORMAT_BCD             ((uint32_t)0)
#define RTC_TIME_FORMAT_BIN             ((uint32_t)1)
#define CHECK_RTC_TIME_FORMAT(FORMAT)   (((uint32_t)(FORMAT) == RTC_TIME_FORMAT_BCD) \
                                      || ((uint32_t)(FORMAT) == RTC_TIME_FORMAT_BIN))

/**
  * ����ѡ��
  */
#define RTC_ALARM_A_ADDR_OFFSET         ((uint32_t)0)
#define RTC_ALARM_B_ADDR_OFFSET         ((uint32_t)0xC)
#define CHECK_RTC_ALARM(ALARM)          (((uint32_t)(ALARM) == RTC_ALARM_A_ADDR_OFFSET) \
                                      || ((uint32_t)(ALARM) == RTC_ALARM_B_ADDR_OFFSET))

/**
  * ʱ���ʽ
  */
#define RTC_HOUR_FORMAT_24              ((uint32_t)0<<RTC_CR_HT_POS)
#define RTC_HOUR_FORMAT_12              ((uint32_t)1<<RTC_CR_HT_POS)
#define CHECK_RTC_HOURFORMAT(FORMAT)    (((uint32_t)(FORMAT) == RTC_HOUR_FORMAT_24) \
                                      || ((uint32_t)(FORMAT) == RTC_HOUR_FORMAT_12))

/**
  * ʱ������ֵ���
  */
#define CHECK_RTC_HOUR12(HOUR)          (((HOUR) > 0) && ((HOUR) <= 12))
#define CHECK_RTC_HOUR24(HOUR)          (((HOUR) <= 23))
#define CHECK_RTC_MINUTES(MINUTES)      (((MINUTES) <= 59))
#define CHECK_RTC_SECONDS(SECONDS)      (((SECONDS) <= 59))

/**
  * AM/PMѡ��
  */
#define RTC_TIME_AM                     ((uint32_t)0)
#define RTC_TIME_PM                     ((uint32_t)1)
#define CHECK_RTC_TIME_AMPM(TIME)       (((uint32_t)(TIME) == RTC_TIME_AM) \
                                      || ((uint32_t)(TIME) == RTC_TIME_PM))

/**
  * RTC ����
  */
#define RTC_WEEKDAY_MONDAY_BCD          ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY_BCD         ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY_BCD       ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY_BCD        ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY_BCD          ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY_BCD        ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY_BCD          ((uint8_t)0x00)

#define RTC_WEEKDAY_MONDAY_DEC          ((uint8_t)0x01)
#define RTC_WEEKDAY_TUESDAY_DEC         ((uint8_t)0x02)
#define RTC_WEEKDAY_WEDNESDAY_DEC       ((uint8_t)0x03)
#define RTC_WEEKDAY_THURSDAY_DEC        ((uint8_t)0x04)
#define RTC_WEEKDAY_FRIDAY_DEC          ((uint8_t)0x05)
#define RTC_WEEKDAY_SATURDAY_DEC        ((uint8_t)0x06)
#define RTC_WEEKDAY_SUNDAY_DEC          ((uint8_t)0x00)
#define CHECK_RTC_WEEKDAY(DAY)          (((DAY) == RTC_WEEKDAY_MONDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_TUESDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_WEDNESDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_THURSDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_FRIDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_SATURDAY_BCD) \
                                      || ((DAY) == RTC_WEEKDAY_SUNDAY_BCD))

/**
  * RTC �·�
  */
#define RTC_MONTH_JANUARY_BCD           ((uint8_t)0x01)
#define RTC_MONTH_FEBRUARY_BCD          ((uint8_t)0x02)
#define RTC_MONTH_MARCH_BCD             ((uint8_t)0x03)
#define RTC_MONTH_APRIL_BCD             ((uint8_t)0x04)
#define RTC_MONTH_MAY_BCD               ((uint8_t)0x05)
#define RTC_MONTH_JUNE_BCD              ((uint8_t)0x06)
#define RTC_MONTH_JULY_BCD              ((uint8_t)0x07)
#define RTC_MONTH_AUGUST_BCD            ((uint8_t)0x08)
#define RTC_MONTH_SEPTEMBER_BCD         ((uint8_t)0x09)
#define RTC_MONTH_OCTOBER_BCD           ((uint8_t)0x10)
#define RTC_MONTH_NOVEMBER_BCD          ((uint8_t)0x11)
#define RTC_MONTH_DECEMBER_BCD          ((uint8_t)0x12)
#define CHECK_RTC_MONTH_BCD(MONTH)      (((MONTH) == RTC_MONTH_JANUARY_BCD) \
                                        || ((MONTH) == RTC_MONTH_FEBRUARY_BCD) \
                                        || ((MONTH) == RTC_MONTH_MARCH_BCD) \
                                        || ((MONTH) == RTC_MONTH_APRIL_BCD) \
                                        || ((MONTH) == RTC_MONTH_MAY_BCD) \
                                        || ((MONTH) == RTC_MONTH_JUNE_BCD) \
                                        || ((MONTH) == RTC_MONTH_JULY_BCD) \
                                        || ((MONTH) == RTC_MONTH_AUGUST_BCD) \
                                        || ((MONTH) == RTC_MONTH_SEPTEMBER_BCD) \
                                        || ((MONTH) == RTC_MONTH_OCTOBER_BCD) \
                                        || ((MONTH) == RTC_MONTH_NOVEMBER_BCD) \
                                        || ((MONTH) == RTC_MONTH_DECEMBER_BCD))
#define RTC_MONTH_JANUARY_DEC           ((uint8_t)0x01)
#define RTC_MONTH_FEBRUARY_DEC          ((uint8_t)0x02)
#define RTC_MONTH_MARCH_DEC             ((uint8_t)0x03)
#define RTC_MONTH_APRIL_DEC             ((uint8_t)0x04)
#define RTC_MONTH_MAY_DEC               ((uint8_t)0x05)
#define RTC_MONTH_JUNE_DEC              ((uint8_t)0x06)
#define RTC_MONTH_JULY_DEC              ((uint8_t)0x07)
#define RTC_MONTH_AUGUST_DEC            ((uint8_t)0x08)
#define RTC_MONTH_SEPTEMBER_DEC         ((uint8_t)0x09)
#define RTC_MONTH_OCTOBER_DEC           ((uint8_t)0x0A)
#define RTC_MONTH_NOVEMBER_DEC          ((uint8_t)0x0B)
#define RTC_MONTH_DECEMBER_DEC          ((uint8_t)0x0C)
#define CHECK_RTC_MONTH_DEC(MONTH)      (((MONTH) >= 1) && ((MONTH) <= 12))

/**
  * RTC ��
  */
#define CHECK_RTC_DATE(DATE)            (((DATE) >= 1) && ((DATE) <= 31))

/**
  * RTC ��
  */
#define CHECK_RTC_YEAR(YEAR)            (((YEAR) >= 0) && ((YEAR) <= 99))

/**
  * ʱ���������
  */
#define RTC_TIME_STAMP_RISE             ((uint32_t)0 << RTC_CR_TSEDGE_POS)
#define RTC_TIME_STAMP_FALL             ((uint32_t)1 << RTC_CR_TSEDGE_POS)
#define CHECK_RTC_TIME_STAMP_EDGE(EDGE) (((EDGE) == RTC_TIME_STAMP_RISE) \
                                        || ((EDGE) == RTC_TIME_STAMP_FALL))

/**
  * RTCʱ�����
  */
#define RTC_TIME_TICK_DIV_1             ((uint32_t)0 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_2             ((uint32_t)1 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_4             ((uint32_t)2 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_8             ((uint32_t)3 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_16            ((uint32_t)4 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_32            ((uint32_t)5 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_64            ((uint32_t)6 << RTC_CR_RTCTT0_POS)
#define RTC_TIME_TICK_DIV_128           ((uint32_t)7 << RTC_CR_RTCTT0_POS)
#define CHECK_RTC_TIME_TICK(TICK)       (((TICK) == RTC_TIME_TICK_DIV_1) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_2) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_4) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_8) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_16) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_32) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_64) \
                                      || ((TICK) == RTC_TIME_TICK_DIV_128))

/**
  * RTC��ʱ��ʱ��Դ
  */
#define RTC_TIMER_CLOCK_RTC_DIV_128     ((uint32_t)0)
#define RTC_TIMER_CLOCK_RTC_DIV_512     ((uint32_t)1)
#define RTC_TIMER_CLOCK_RTC_DIV_1024    ((uint32_t)2)
#define RTC_TIMER_CLOCK_RTC_DIV_2048    ((uint32_t)3)
#define RTC_TIMER_CLOCK_RTC_DIV_8192    ((uint32_t)4)
#define RTC_TIMER_CLOCK_ONE_SECOND      ((uint32_t)5)
#define RTC_TIMER_CLOCK_ONE_MINUTE      ((uint32_t)6)
#define RTC_TIMER_CLOCK_ONE_HOUR        ((uint32_t)7)
#define RTC_TIMER_CLOCK_CONNECTION      ((uint32_t)8)
#define CHECK_RTC_TIMER1_CLOCK(CFG)     (((CFG) == RTC_TIMER_CLOCK_RTC_DIV_128) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_512) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_1024) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_2048) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_8192) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_SECOND) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_MINUTE) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_HOUR) \
                                      || ((CFG) == RTC_TIMER_CLOCK_CONNECTION))
#define CHECK_RTC_TIMER0_CLOCK(CFG)     (((CFG) == RTC_TIMER_CLOCK_RTC_DIV_128) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_512) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_1024) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_2048) \
                                      || ((CFG) == RTC_TIMER_CLOCK_RTC_DIV_8192) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_SECOND) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_MINUTE) \
                                      || ((CFG) == RTC_TIMER_CLOCK_ONE_HOUR))


/* ʵʱʱ��(RTC)���ܳ�ʼ����������******************************************/
void RTC_Reset (void);
void RTC_Configuration (uint32_t TimeFormat,
                    RTC_InitTypeDef * rtcInitStruct);
void RTC_Time_Struct_Init (RTC_TimeTypeDef* rtcTimeInitStruct);
void RTC_Date_Struct_Init (RTC_DateTypeDef* rtcDateInitStruct);
void RTC_Struct_Init (RTC_InitTypeDef * rtcInitStruct);
void RTC_Get_Time_Configuration (uint32_t TimeFormat,
                    RTC_TimeTypeDef* rtcTimeStruct);
void RTC_Get_Date_Configuration (uint32_t TimeFormat,
                    RTC_DateTypeDef* rtcDateStruct);
void RTC_Alarm_Configuration (uint32_t AlarmSelect,
                    uint32_t TimeFormat,
                    RTC_AlarmTypeDef* rtcAlarmInitStruct);
void RTC_Alarm_Struct_Init (RTC_AlarmTypeDef* rtcAlarmInitStruct);
/* ʵʱʱ��(RTC)���ƹ��ܺ�������********************************************/
void RTC_Clock_Calibration_Config (int8_t Calibration);
void RTC_Time_Tick_Output_Enable (FunctionalState NewState);
void RTC_Time_Stamp_Channel_Enable (uint32_t TimeStampChannel,FunctionalState NewState);
void RTC_Time_Stamp_Edge_Config (uint32_t TimeStamp);
void RTC_Time_Stamp_Edge_Enable (FunctionalState NewState);
void RTC_Add_One_Hour_Enable (FunctionalState NewState);
void RTC_Sub_One_Hour_Enable (FunctionalState NewState);
void RTC_Time_Tick_Config (uint32_t Calibration);
void RTC_Reset_Config (void);
FlagStatus RTC_Get_Leap_Year_Flag (void);
void RTC_Hour_Format_Config (uint32_t HourFormat);
void RTC_Config_Mode_Enable (uint32_t ConfigMode);
FlagStatus RTC_Get_Operation_Off_Flag (void);
FlagStatus RTC_Get_Action_State_Flag (void);
void RTC_Enable (FunctionalState NewState);
/* ʵʱʱ��(RTC)�������ú�������********************************************/
void RTC_Alarm_A_Enable (FunctionalState NewState);
void RTC_Alarm_A_Weekday_Enable (FunctionalState NewState);
void RTC_Alarm_A_Weekday_Config (uint8_t Weekday);
void RTC_Alarm_A_Hours_Enable (FunctionalState NewState);
void RTC_Alarm_A_AMPM_Config (uint32_t NewSelect);
void RTC_Alarm_A_Hours_Config (uint32_t Hour);
void RTC_Alarm_A_Minutes_Enable (FunctionalState NewState);
void RTC_Alarm_A_Minutes_Config (uint32_t Minutes);
void RTC_Alarm_A_Seconds_Enable (FunctionalState NewState);
void RTC_Alarm_A_Seconds_Config (uint32_t Seconds);
void RTC_Alarm_B_Enable (FunctionalState NewState);
void RTC_Alarm_B_Weekday_Enable (FunctionalState NewState);
void RTC_Alarm_B_Weekday_Config (uint8_t Weekday);
void RTC_Alarm_B_Hours_Enable (FunctionalState NewState);
void RTC_Alarm_B_AMPM_Config (uint32_t NewSelect);
void RTC_Alarm_B_Hours_Config (uint32_t Hour);
void RTC_Alarm_B_Minutes_Enable (FunctionalState NewState);
void RTC_Alarm_B_Minutes_Config (uint32_t Minutes);
void RTC_Alarm_B_Seconds_Enable (FunctionalState NewState);
void RTC_Alarm_B_Seconds_Config (uint32_t Seconds);
/* ʵʱʱ��(RTC)����ʱ�����ú�������****************************************/
void RTC_Weekday_Config (uint8_t Weekday);
void RTC_AMPM_Config (uint32_t NewSelect);
void RTC_Hours_Config (uint32_t Hour);
void RTC_Minutes_Config (uint32_t Minutes);
void RTC_Seconds_Config (uint32_t Seconds);
void RTC_Year_Config (uint32_t Year);
void RTC_Month_Config (uint32_t Month);
void RTC_Day_Config (uint32_t Day);
void RTC_Weekday_Backup_Config (uint8_t Weekday);
void RTC_AMPM_Backup_Config (uint32_t NewSelect);
void RTC_Hours_Backup_Config (uint32_t Hour);
void RTC_Minutes_Backup_Config (uint32_t Minutes);
void RTC_Seconds_Backup_Config (uint32_t Seconds);
void RTC_Year_Backup_Config (uint32_t Year);
void RTC_Month_Backup_Config (uint32_t Month);
void RTC_Day_Backup_Config (uint32_t Day);
/* ʵʱʱ��(RTC)��ʱ�����ú�������******************************************/
void RTC_Timer1_Config (uint16_t Counter);
void RTC_Timer0_Config (uint16_t Counter);
void RTC_Timer1_Enable (uint32_t TimerEnable);
void RTC_Timer0_Enable (uint32_t TimerEnable);
void RTC_Timer1_Source_Config (uint16_t Counter);
void RTC_Timer0_Source_Config (uint16_t Counter);
/* ʵʱʱ��(RTC)�жϹ���������********************************************/
void RTC_Time_Stamp_INT_Enable (FunctionalState NewState);
void RTC_Time_Stamp_Overflow_INT_Enable (FunctionalState NewState);
void RTC_Timer1_INT_Enable (FunctionalState NewState);
void RTC_Timer0_INT_Enable (FunctionalState NewState);
void RTC_Time_Tick_INT_Enable (FunctionalState NewState);
void RTC_Alarm_B_INT_Enable (FunctionalState NewState);
void RTC_Alarm_A_INT_Enable (FunctionalState NewState);
void RTC_Days_INT_Enable (FunctionalState NewState);
void RTC_Hours_INT_Enable (FunctionalState NewState);
void RTC_Minutes_INT_Enable (FunctionalState NewState);
void RTC_Seconds_INT_Enable (FunctionalState NewState);
FlagStatus RTC_Get_Time_Stamp_INT_Flag (void);
FlagStatus RTC_Get_Time_Stamp_Overflow_INT_Flag (void);
FlagStatus RTC_Get_Timer1_INT_Flag (void);
FlagStatus RTC_Get_Timer0_INT_Flag (void);
FlagStatus RTC_Get_Time_Tick_INT_Flag (void);
FlagStatus RTC_Get_Alarm_B_INT_Flag (void);
FlagStatus RTC_Get_Alarm_A_INT_Flag (void);
FlagStatus RTC_Get_Days_INT_Flag (void);
FlagStatus RTC_Get_Hours_INT_Flag (void);
FlagStatus RTC_Get_Minutes_INT_Flag (void);
FlagStatus RTC_Get_Seconds_INT_Flag (void);
void RTC_Clear_Time_Stamp_INT_Flag (void);
void RTC_Clear_Time_Stamp_Overflow_INT_Flag (void);
void RTC_Clear_Timer1_INT_Flag (void);
void RTC_Clear_Timer0_INT_Flag (void);
void RTC_Clear_Time_Tick_INT_Flag (void);
void RTC_Clear_Alarm_B_INT_Flag (void);
void RTC_Clear_Alarm_A_INT_Flag (void);
void RTC_Clear_Days_INT_Flag (void);
void RTC_Clear_Hours_INT_Flag (void);
void RTC_Clear_Minutes_INT_Flag (void);
void RTC_Clear_Seconds_INT_Flag (void);

#endif /* _KF32A_BASIC_RTC_H */
