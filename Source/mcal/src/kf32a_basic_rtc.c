/**
  ******************************************************************************
  * �ļ���  kf32a_basic_rtc.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��ʵʱʱ��(RTC)��صĹ��ܺ�����������
  *          + ʵʱʱ��(RTC)���ܳ�ʼ������
  *          + ʵʱʱ��(RTC)���ƹ��ܺ���
  *          + ʵʱʱ��(RTC)�������ú���
  *          + ʵʱʱ��(RTC)����ʱ�����ú���
  *          + ʵʱʱ��(RTC)��ʱ�����ú���
  *          + ʵʱʱ��(RTC)�жϹ�����
  *          + ʵʱʱ��(RTC)˽�к���
  *
  *********************************************************************
  */

#include "kf32a_basic_osc.h"
#include "kf32a_basic_bkp.h"
#include "kf32a_basic_rtc.h"

/* ʵʱʱ��(RTC)˽�к궨�� --------------------------------------------------*/
/* RTC_TMR�Ĵ�����ʼ���������� */
#define RTC_TMR_INIT_MASK               (RTC_TMR_WEK \
                                       | RTC_TMR_AMPM \
                                       | RTC_TMR_HOUR \
                                       | RTC_TMR_MIN \
                                       | RTC_TMR_SEC)
/* RTC_DTR�Ĵ�����ʼ���������� */
#define RTC_DTR_INIT_MASK               (RTC_DTR_DAY \
                                       | RTC_DTR_MTH \
                                       | RTC_DTR_YEAR)
/* RTC_ALRA/B�Ĵ�����ʼ���������� */
#define RTC_ALR_INIT_MASK               (RTC_ALRA_ALRSA \
                                       | RTC_ALRA_ASENA \
                                       | RTC_ALRA_ALRMA \
                                       | RTC_ALRA_AMENA \
                                       | RTC_ALRA_ALRHA \
                                       | RTC_ALRA_AMPMA \
                                       | RTC_ALRA_AHENA \
                                       | RTC_ALRA_ALRWA \
                                       | RTC_ALRA_AWENA)


/* ʵʱʱ��(RTC)˽�к��� ----------------------------------------------------*/
static uint8_t RTC_Byte_To_Bcd (uint8_t Value);
static uint8_t RTC_Bcd_To_Byte (uint8_t Value);


/**
  *   ##### ʵʱʱ��(RTC)���ܳ�ʼ������ #####
  */
/**
  * ����  ��λRTCģ�顣
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Reset (void)
{
    /* ����RTCģ��ǰ��������ؼĴ��� */
    SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
    SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
    SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);

    /* ��λRTCģ�� */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
    /* �˳���λRTCģ�� */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
}

/**
  * ����  ����ʱ���ʼ�����ã��ú�������ʱ���˳�����ģʽ��
  * ����  TimeFormat: ����ʱ����ֵ��ʽ��ѡ���Ƿ�ΪBCD�룬ȡֵ��ΧΪ��
  *                     RTC_TIME_FORMAT_BCD: BCD���뷽ʽ��
  *                     RTC_TIME_FORMAT_BIN: ��ʹ��BCD���롣
  *       rtcInitStruct: ��ʼ����Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Configuration (uint32_t TimeFormat, RTC_InitTypeDef * rtcInitStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));
    CHECK_RESTRICTION(CHECK_RTC_SOURCE(rtcInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_RTC_HOURFORMAT(rtcInitStruct->m_HourFormat));
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12(rtcInitStruct->m_TimeStruct.m_Hours));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcInitStruct->m_TimeStruct.m_AMPM));
        }
        else
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR24(rtcInitStruct->m_TimeStruct.m_Hours));
            if(rtcInitStruct->m_TimeStruct.m_Hours>=12)
            {
                rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_PM;
            }
            else
            {
                rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
            }
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES(rtcInitStruct->m_TimeStruct.m_Minutes));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS(rtcInitStruct->m_TimeStruct.m_Seconds));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcInitStruct->m_DateStruct.m_WeekDay));
        CHECK_RESTRICTION(CHECK_RTC_DATE(rtcInitStruct->m_DateStruct.m_Day));
        CHECK_RESTRICTION(CHECK_RTC_MONTH_DEC(rtcInitStruct->m_DateStruct.m_Month));
        CHECK_RESTRICTION(CHECK_RTC_YEAR(rtcInitStruct->m_DateStruct.m_Year));
    }
    else
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Hours)));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcInitStruct->m_TimeStruct.m_AMPM));
        }
        else
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR24( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Hours)));
            if(rtcInitStruct->m_TimeStruct.m_Hours>=12)
            {
                rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_PM;
            }
            else
            {
                rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
            }
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Minutes)));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Seconds)));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcInitStruct->m_DateStruct.m_WeekDay));
        CHECK_RESTRICTION(CHECK_RTC_DATE( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_DateStruct.m_Day)));
        CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(rtcInitStruct->m_DateStruct.m_Month));
        CHECK_RESTRICTION(CHECK_RTC_YEAR( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_DateStruct.m_Year)));
    }

    /* ��������ģʽ */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);

    /*-------------------- ����RTC_TMR�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_TimeStruct.m_WeekDay������WEKλ�� */
    /* ���ݽṹ���Աm_TimeStruct.m_Hours������HOURλ�� */
    /* ���ݽṹ���Աm_TimeStruct.m_Minutes������MINλ�� */
    /* ���ݽṹ���Աm_TimeStruct.m_Seconds������SECλ�� */
    /* ���ݽṹ���Աm_TimeStruct.m_AMPM������AMPMλ�� */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_WeekDay \
                    << RTC_TMR_WEK0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Hours) \
                    << RTC_TMR_HOUR0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Minutes) \
                    << RTC_TMR_MIN0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Seconds) \
                    << RTC_TMR_SEC0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_AMPM << RTC_TMR_AMPM_POS);
    }
    else
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_WeekDay << RTC_TMR_WEK0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Hours << RTC_TMR_HOUR0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Minutes << RTC_TMR_MIN0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Seconds << RTC_TMR_SEC0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_AMPM << RTC_TMR_AMPM_POS);
    }
    RTC_TMR = SFR_Config (RTC_TMR,
                          ~RTC_TMR_INIT_MASK,
                          tmpreg);

    /*-------------------- ����RTC_DTR�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_DateStruct.m_Day������DAYλ�� */
    /* ���ݽṹ���Աm_DateStruct.m_Month������MTHλ�� */
    /* ���ݽṹ���Աm_DateStruct.m_Year������YEARλ�� */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Month) \
                    << RTC_DTR_MTH0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Day) \
                    << RTC_DTR_DAY0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Year) \
                    << RTC_DTR_YEAR0_POS);
    }
    else
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_Month << RTC_DTR_MTH0_POS) \
               | ((uint32_t)rtcInitStruct->m_DateStruct.m_Day << RTC_DTR_DAY0_POS) \
               | ((uint32_t)rtcInitStruct->m_DateStruct.m_Year << RTC_DTR_YEAR0_POS);
    }
    RTC_DTR = SFR_Config (RTC_DTR,
                          ~RTC_DTR_INIT_MASK,
                          tmpreg);

    /*-------------------- ����RTC_CR�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_HourFormat������HTλ�� */
    tmpreg = rtcInitStruct->m_HourFormat;
    RTC_CR = SFR_Config (RTC_CR,
                          ~RTC_CR_HT,
                          tmpreg);

    /*-------------------- ����BKP_CTL�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_ClockSource������RTCCLKSλ�� */
    BKP_CTL = SFR_Config (BKP_CTL,
                          ~BKP_CTL_RTCCLKS,
                          rtcInitStruct->m_ClockSource);

    /* �˳�����ģʽ */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
}

/**
  * ����  ��ʼ��ʱ����Ϣ�ṹ�塣
  * ����  rtcTimeInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Time_Struct_Init (RTC_TimeTypeDef* rtcTimeInitStruct)
{
    /* ʱ��: AM 00h:00m:00s */
    /* ����ʱ�� */
    rtcTimeInitStruct->m_Hours = 0;
    /* ���÷��� */
    rtcTimeInitStruct->m_Minutes = 0;
    /* �������� */
    rtcTimeInitStruct->m_Seconds = 0;
    /* ����AMPM */
    rtcTimeInitStruct->m_AMPM = RTC_TIME_AM;
}

/**
  * ����  ��ʼ��������Ϣ�ṹ�塣
  * ����  rtcDateInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Date_Struct_Init (RTC_DateTypeDef* rtcDateInitStruct)
{
    /* ����: xx00/1/1 ��һ*/
    /* ������ */
    rtcDateInitStruct->m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* ������ */
    rtcDateInitStruct->m_Year = 0;
    /* ������ */
    rtcDateInitStruct->m_Month = RTC_MONTH_JANUARY_BCD;
    /* ������ */
    rtcDateInitStruct->m_Day = 1;
}

/**
  * ����  ��ʼ��������Ϣ�ṹ�塣
  * ����  rtcInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Struct_Init (RTC_InitTypeDef * rtcInitStruct)
{
    /* ����RTCʱ��Դ */
    rtcInitStruct->m_ClockSource = BKP_RTC_NO_CLK;
    /* ����RTCʱ���ʽ */
    rtcInitStruct->m_HourFormat = RTC_HOUR_FORMAT_24;
    /* ����ʱ�� */
    rtcInitStruct->m_TimeStruct.m_Hours = 0;
    /* ���÷��� */
    rtcInitStruct->m_TimeStruct.m_Minutes = 0;
    /* �������� */
    rtcInitStruct->m_TimeStruct.m_Seconds = 0;
    /* ����AMPM */
    rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
    /* ������ */
    rtcInitStruct->m_DateStruct.m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* ������ */
    rtcInitStruct->m_DateStruct.m_Year = 0;
    /* ������ */
    rtcInitStruct->m_DateStruct.m_Month = RTC_MONTH_JANUARY_BCD;
    /* ������ */
    rtcInitStruct->m_DateStruct.m_Day = 1;
}

/**
  * ����  ��ȡʱ����Ϣ��
  * ����  TimeFormat: ����ʱ����ֵ��ʽ��ѡ���Ƿ�ΪBCD�룬ȡֵ��ΧΪ��
  *                     RTC_TIME_FORMAT_BCD: BCD���뷽ʽ��
  *                     RTC_TIME_FORMAT_BIN: ��ʹ��BCD���롣
  *       rtcTimeStruct: ʱ����Ϣ�洢ָ�롣
  * ����  �ޡ�
  */
void
RTC_Get_Time_Configuration (uint32_t TimeFormat,
                    RTC_TimeTypeDef* rtcTimeStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));

    /* ��ȡRTC_TMR�Ĵ��� */
    tmpreg = RTC_TMR;

    /* ��ȡʱ����Ϣ */
    rtcTimeStruct->m_Hours = (uint8_t)((tmpreg & RTC_TMR_HOUR) \
                            >> RTC_TMR_HOUR0_POS);
    rtcTimeStruct->m_Minutes = (uint8_t)((tmpreg & RTC_TMR_MIN) \
                              >> RTC_TMR_MIN0_POS);
    rtcTimeStruct->m_Seconds = (uint8_t)((tmpreg & RTC_TMR_SEC) \
                              >> RTC_TMR_SEC0_POS);
    rtcTimeStruct->m_AMPM = (uint8_t)((tmpreg & RTC_TMR_AMPM) \
                           >> RTC_TMR_AMPM_POS);

    /* BCD����תΪ��ֵ */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        rtcTimeStruct->m_Hours = RTC_Bcd_To_Byte(rtcTimeStruct->m_Hours);
        rtcTimeStruct->m_Minutes = RTC_Bcd_To_Byte(rtcTimeStruct->m_Minutes);
        rtcTimeStruct->m_Seconds = RTC_Bcd_To_Byte(rtcTimeStruct->m_Seconds);
    }
}

/**
  * ����  ��ȡ������Ϣ��
  * ����  TimeFormat: ����ʱ����ֵ��ʽ��ѡ���Ƿ�ΪBCD�룬ȡֵ��ΧΪ��
  *                     RTC_TIME_FORMAT_BCD: BCD���뷽ʽ��
  *                     RTC_TIME_FORMAT_BIN: ��ʹ��BCD���롣
  *       rtcDateStruct: ������Ϣ�洢ָ�롣
  * ����  �ޡ�
  */
void
RTC_Get_Date_Configuration (uint32_t TimeFormat,
                    RTC_DateTypeDef* rtcDateStruct)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));

    /* ��ȡRTC_TMR�Ĵ��� */
    tmpreg = RTC_TMR;

    /* ��ȡ����Ϣ */
    rtcDateStruct->m_WeekDay = (uint8_t)((tmpreg & RTC_TMR_WEK) \
                              >> RTC_TMR_WEK0_POS);

    /* ��ȡRTC_DTR�Ĵ��� */
    tmpreg = RTC_DTR;

    /* ��ȡʱ����Ϣ */
    rtcDateStruct->m_Year = (uint8_t)((tmpreg & RTC_DTR_YEAR) \
                           >> RTC_DTR_YEAR0_POS);
    rtcDateStruct->m_Month = (uint8_t)((tmpreg & RTC_DTR_MTH) \
                           >> RTC_DTR_MTH0_POS);
    rtcDateStruct->m_Day = (uint8_t)((tmpreg & RTC_DTR_DAY) \
                           >> RTC_DTR_DAY0_POS);

    /* BCD����תΪ��ֵ */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        rtcDateStruct->m_Year = RTC_Bcd_To_Byte(rtcDateStruct->m_Year);
        rtcDateStruct->m_Month = RTC_Bcd_To_Byte(rtcDateStruct->m_Month);
        rtcDateStruct->m_Day = RTC_Bcd_To_Byte(rtcDateStruct->m_Day);
    }
}

/**
  * ����  ���ӳ�ʼ�����ã����ô˺���ǰ������HTλ��
  *       ͨ��RTC_Hour_Format_Config����ʵ�֡�
  * ����  AlarmSelect: ����ѡ��ȡֵ��ΧΪ��
  *                      RTC_ALARM_A_ADDR_OFFSET: ����A��
  *                      RTC_ALARM_B_ADDR_OFFSET: ����B��
  *       TimeFormat: ����ʱ����ֵ��ʽ��ѡ���Ƿ�ΪBCD�룬ȡֵ��ΧΪ��
  *                     RTC_TIME_FORMAT_BCD: BCD���뷽ʽ��
  *                     RTC_TIME_FORMAT_BIN: ��ʹ��BCD���롣
  *       rtcAlarmInitStruct: ������Ϣ�ṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Alarm_Configuration (uint32_t AlarmSelect,
                    uint32_t TimeFormat,
                    RTC_AlarmTypeDef* rtcAlarmInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_ALARM(AlarmSelect));
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_WeekDayEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_HoursEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_MinutesEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_SecondsEnable));

    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12(rtcAlarmInitStruct->m_Hours));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcAlarmInitStruct->m_AMPM));
        }
        else
        {
            rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24(rtcAlarmInitStruct->m_Hours));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES(rtcAlarmInitStruct->m_Minutes));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS(rtcAlarmInitStruct->m_Seconds));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcAlarmInitStruct->m_WeekDay));
    }
    else
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Hours)));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcAlarmInitStruct->m_AMPM));
        }
        else
        {
            rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Hours)));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Minutes)));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Seconds)));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcAlarmInitStruct->m_WeekDay));
    }

    /* ��������ģʽ */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);

    /* ����AlarmSelect��ѡ���Ӧ�����ӿ��ƼĴ��� */
    tmpaddr = (uint32_t)&RTC_ALRA;
    tmpaddr += AlarmSelect;

    /*-------------------- ����RTC_ALRx�Ĵ��� --------------------*/
    /* ���ݽṹ���Աm_WeekDayEnable������AWENxλ�� */
    /* ���ݽṹ���Աm_HoursEnable������AHENxλ�� */
    /* ���ݽṹ���Աm_MinutesEnable������AMENxλ�� */
    /* ���ݽṹ���Աm_SecondsEnable������ASENλ�� */
    /* ���ݽṹ���Աm_AMPM������AMPMxλ�� */
    /* ���ݽṹ���Աm_WeekDay������ALRWxλ�� */
    /* ���ݽṹ���Աm_Hours������ALRHxλ�� */
    /* ���ݽṹ���Աm_Minutes������ALRMxλ�� */
    /* ���ݽṹ���Աm_Seconds������ALRSxλ�� */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = (rtcAlarmInitStruct->m_WeekDayEnable << RTC_ALRA_AWENA_POS) \
               | (rtcAlarmInitStruct->m_HoursEnable << RTC_ALRA_AHENA_POS) \
               | (rtcAlarmInitStruct->m_MinutesEnable << RTC_ALRA_AMENA_POS) \
               | (rtcAlarmInitStruct->m_SecondsEnable << RTC_ALRA_ASENA_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_AMPM << RTC_TMR_AMPM_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_WeekDay << RTC_ALRA_ALRWA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Hours) \
                    << RTC_ALRA_ALRHA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Minutes) \
                    << RTC_ALRA_ALRMA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Seconds) \
                    << RTC_ALRA_ALRSA0_POS);
    }
    else
    {
        tmpreg = (rtcAlarmInitStruct->m_WeekDayEnable << RTC_ALRA_AWENA_POS) \
               | (rtcAlarmInitStruct->m_HoursEnable << RTC_ALRA_AHENA_POS) \
               | (rtcAlarmInitStruct->m_MinutesEnable << RTC_ALRA_AMENA_POS) \
               | (rtcAlarmInitStruct->m_SecondsEnable << RTC_ALRA_ASENA_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_AMPM << RTC_TMR_AMPM_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_WeekDay << RTC_ALRA_ALRWA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Hours << RTC_ALRA_ALRHA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Minutes << RTC_ALRA_ALRMA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Seconds << RTC_ALRA_ALRSA0_POS);
    }

    *(volatile uint32_t *)tmpaddr =
                    SFR_Config (*(volatile uint32_t *)tmpaddr,
                                ~RTC_ALR_INIT_MASK,
                                tmpreg);

    /* �˳�����ģʽ */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
}

/**
  * ����  ��ʼ��������Ϣ�ṹ�塣
  * ����  rtcAlarmInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
RTC_Alarm_Struct_Init (RTC_AlarmTypeDef* rtcAlarmInitStruct)
{
    /* ����AM/PM */
    rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
    /* ������ */
    rtcAlarmInitStruct->m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* ʹ���� */
    rtcAlarmInitStruct->m_WeekDayEnable = FALSE;
    /* ����ʱ */
    rtcAlarmInitStruct->m_Hours = 0;
    /* ʹ��ʱ */
    rtcAlarmInitStruct->m_HoursEnable = FALSE;
    /* ���÷� */
    rtcAlarmInitStruct->m_Minutes = 0;
    /* ʹ�ܷ� */
    rtcAlarmInitStruct->m_MinutesEnable = FALSE;
    /* ������ */
    rtcAlarmInitStruct->m_Seconds = 0;
    /* ʹ���� */
    rtcAlarmInitStruct->m_SecondsEnable = FALSE;
}
/**
  *   ##### ʵʱʱ��(RTC)���ܳ�ʼ������������� #####
  */


/**
  *   ##### ʵʱʱ��(RTC)���ƹ��ܺ��� #####
  */
/**
  * ����  ����RTCʱ��У��ֵ��
  * ����  Calibration: RTCʱ��ƫ���ֵ��ȡֵΪ8λ�з�����ֵ��
  * ����  �ޡ�
  */
void
RTC_Clock_Calibration_Config (int8_t Calibration)
{
    uint32_t tmpreg = 0;

    /*---------------- ����RTC_CR�Ĵ���RTCCALλ ----------------*/
    tmpreg = (uint32_t)Calibration << RTC_CR_RTCCAL0_POS;
    RTC_CR = SFR_Config (RTC_CR, ~RTC_CR_RTCCAL, tmpreg);
}

/**
  * ����  ����RTC������ʱ����ģ�TT�����ʹ�ܡ�
  * ����  NewState: RTC������ʱ����ģ�TT�����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Tick_Output_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���TTOEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��TT��� */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TTOE_POS);
    }
    else
    {
        /* ��ֹTT��� */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TTOE_POS);
    }
}

/**
  * ����  ����ʱ���ͨ��ʹ�ܡ�
  * ����  TimeStampChannel: ʱ���ͨ����ȡֵΪ��
  *                    RTC_TIME_STAMP_CHANNEL0: RTC_TS����
  *                    RTC_TIME_STAMP_CHANNEL1: WKUP1����
  *                    RTC_TIME_STAMP_CHANNEL2: WKUP2����
  *                    RTC_TIME_STAMP_CHANNEL3: WKUP3����
  *                    RTC_TIME_STAMP_CHANNEL4: WKUP4����
  *                    RTC_TIME_STAMP_CHANNEL5: WKUP5����
  *                    NewState: RTC��ʱ�������ʹ�����ã�ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Stamp_Channel_Enable (uint32_t TimeStampChannel,FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_STAMP_CHANNEL(TimeStampChannel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���RTCTSCHλ ----------------*/
    if (NewState != FALSE)
    {
    	RTC_CR |= TimeStampChannel;
    }
    else
    {

    	RTC_CR &= ~TimeStampChannel;
    }
}

/**
  * ����  ����ʱ��������ء�
  * ����  TimeStamp: ʱ��������أ�ȡֵΪ��
  *                    RTC_TIME_STAMP_RISE: �����ش���
  *                    RTC_TIME_STAMP_FALL: �½��ش���
  * ����  �ޡ�
  */
void
RTC_Time_Stamp_Edge_Config (uint32_t TimeStamp)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_STAMP_EDGE(TimeStamp));

    /*---------------- ����RTC_CR�Ĵ���TSEDGEλ ----------------*/
    if (TimeStamp != RTC_TIME_STAMP_RISE)
    {
        /* �½��ش��� */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TSEDGE_POS);
    }
    else
    {
        /* �����ش��� */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TSEDGE_POS);
    }
}

/**
  * ����  ����ʱ���������ʹ�ܡ�
  * ����  NewState: ʱ���������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Stamp_Edge_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���TSENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��ʱ������� */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TSEN_POS);
    }
    else
    {
        /* δʹ��ʱ������� */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TSEN_POS);
    }
}

/**
  * ����  ����RTCʱ������1Сʱʹ�ܡ�
  * ����  NewState: RTCʱ������1Сʱʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Add_One_Hour_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���ADD1Hλ ----------------*/
    if (NewState != FALSE)
    {
        /* RTCʱ������1Сʱ */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_ADD1H_POS);
    }
    else
    {
        /* RTC������ʱ */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_ADD1H_POS);
    }
}

/**
  * ����  ����RTCʱ�����1Сʱʹ�ܡ�
  * ����  NewState: RTCʱ�����1Сʱʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Sub_One_Hour_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���SUB1Hλ ----------------*/
    if (NewState != FALSE)
    {
        /* RTCʱ�����1Сʱ */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_SUB1H_POS);
    }
    else
    {
        /* RTC������ʱ */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_SUB1H_POS);
    }
}

/**
  * ����  ����ʱ����ġ�
  * ����  TimeTick: RTCʱ����ģ�ȡֵΪ:
  *                   RTC_TIME_TICK_DIV_1: ʱ�����Ϊ1��
  *                   RTC_TIME_TICK_DIV_2: ʱ�����Ϊ1/2��
  *                   RTC_TIME_TICK_DIV_4: ʱ�����Ϊ1/4��
  *                   RTC_TIME_TICK_DIV_8: ʱ�����Ϊ1/8��
  *                   RTC_TIME_TICK_DIV_16: ʱ�����Ϊ1/16��
  *                   RTC_TIME_TICK_DIV_32: ʱ�����Ϊ1/32��
  *                   RTC_TIME_TICK_DIV_64: ʱ�����Ϊ1/64��
  *                   RTC_TIME_TICK_DIV_128: ʱ�����Ϊ1/128��
  * ����  �ޡ�
  */
void
RTC_Time_Tick_Config (uint32_t Calibration)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_TICK(Calibration));

    /*---------------- ����RTC_CR�Ĵ���RTCTTλ ----------------*/
    RTC_CR = SFR_Config (RTC_CR, ~RTC_CR_RTCTT, Calibration);
}


/**
  * ����  ��ʼ��ʵʱʱ��ģ�顣
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Reset_Config (void)
{
    /*---------------- ����RTC_CR�Ĵ���RESETλ ----------------*/
    /* ��λRTCģ�� */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
    /* �˳���λRTCģ�� */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
}

/**
  * ����  ��ȡ����ָʾ��־��
  * ����  �ޡ�
  * ����  1:��ǰ���Ϊ���ꣻ0:��ǰ���Ϊƽ�ꡣ
  */
FlagStatus
RTC_Get_Leap_Year_Flag (void)
{
    /*---------------- ��ȡRTC_CR�Ĵ���LIFλ ----------------*/
    if (RTC_CR & RTC_CR_LIF)
    {
        /* ��ǰ���Ϊ���� */
        return SET;
    }
    else
    {
        /* ��ǰ���Ϊƽ�� */
        return RESET;
    }
}

/**
  * ����  ����Сʱ��ʾ�����͡�
  * ����  HourFormat: RTCСʱ��ʾ�����ͣ�ȡֵΪ:
  *                     RTC_HOUR_FORMAT_24: 24Сʱ�ƣ�
  *                     RTC_HOUR_FORMAT_12: 12Сʱ�ơ�
  * ����  �ޡ�
  */
void
RTC_Hour_Format_Config (uint32_t HourFormat)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_HOURFORMAT(HourFormat));

    /*---------------- ����RTC_CR�Ĵ���HTλ ----------------*/
    if (HourFormat != RTC_HOUR_FORMAT_24)
    {
        /* 12Сʱ�� */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_HT_POS);
    }
    else
    {
        /* 24Сʱ�� */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_HT_POS);
    }
}

/**
  * ����  ��������ģʽʹ�ܡ�
  * ����  ConfigMode: ����ģʽ״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Config_Mode_Enable (FunctionalState ConfigMode)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ConfigMode));

    /*---------------- ����RTC_CR�Ĵ���CNFλ ----------------*/
    if (ConfigMode != FALSE)
    {
        /* ��������ģʽ */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
    }
    else
    {
        /* �˳�����ģʽ */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
    }
}

/**
  * ����  ��ȡRTC�����ر�ָʾ��־��
  * ����  �ޡ�
  * ����  1:��һ�ζ�RTC�Ĵ�����д�����Ѿ���ɣ�
  *       0:��һ�ζ�RTC�Ĵ�����д�������ڽ��С�
  */
FlagStatus
RTC_Get_Operation_Off_Flag (void)
{
    /*---------------- ��ȡRTC_CR�Ĵ���RTOFFλ ----------------*/
    if (RTC_CR & RTC_CR_RTOFF)
    {
        /* ��һ�ζ�RTC�Ĵ�����д�����Ѿ���� */
        return SET;
    }
    else
    {
        /* ��һ�ζ�RTC�Ĵ�����д�������ڽ��� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC�״̬ ��
  * ����  �ޡ�
  * ����  1:RTC����������Ч״̬��
  *       0:RTC���ڸ�λ״̬��
  */
FlagStatus
RTC_Get_Action_State_Flag (void)
{
    /*---------------- ��ȡRTC_CR�Ĵ���RTCLDλ ----------------*/
    if (RTC_CR & RTC_CR_RTCLD)
    {
        /* RTC����������Ч״̬ */
        return SET;
    }
    else
    {
        /* RTC���ڸ�λ״̬ */
        return RESET;
    }
}

/**
  * ����  ����RTCʹ�ܡ�
  * ����  NewState: RTCʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_CR�Ĵ���RTCENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��RTC */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RTCEN_POS);
    }
    else
    {
        /* ��ֹRTC */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RTCEN_POS);
    }
}
/**
  *   ##### ʵʱʱ��(RTC)���ƹ��ܺ���������� #####
  */


/**
  *   ##### ʵʱʱ��(RTC)�������ú��� #####
  */
/**
  * ����  ��������Aʹ�ܡ�
  * ����  NewState: ����Aʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRA�Ĵ���ALRENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������A���� */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_ALRENA_POS);
    }
    else
    {
        /* ��ֹ����A���� */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_ALRENA_POS);
    }
}

/**
  * ����  ��������A��ʹ�ܡ�
  * ����  NewState: ����A��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Weekday_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRA�Ĵ���AWENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������A��λ */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AWENA_POS);
    }
    else
    {
        /* ��ֹ����A��λ */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AWENA_POS);
    }
}

/**
  * ����  ���������ж�����ʱ�䡣
  * ����  Weekday: �����ж�����ʱ�䣬ȡֵΪ:
  *                  RTC_WEEKDAY_MONDAY: ����һ
  *                  RTC_WEEKDAY_TUESDAY: ���ڶ�
  *                  RTC_WEEKDAY_WEDNESDAY: ������
  *                  RTC_WEEKDAY_THURSDAY: ������
  *                  RTC_WEEKDAY_FRIDAY: ������
  *                  RTC_WEEKDAY_SATURDAY: ������
  *                  RTC_WEEKDAY_SUNDAY: ������
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- ����RTC_ALRA�Ĵ���ALRWAλ ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_ALRA_ALRWA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRWA, tmpreg);
}

/**
  * ����  ��������Aʱ��ʹ�ܡ�
  * ����  NewState: ����Aʱ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Hours_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRA�Ĵ���AHENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������Aʱ��λ */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AHENA_POS);
    }
    else
    {
        /* ��ֹ����Aʱ��λ */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AHENA_POS);
    }
}

/**
  * ����  ��������A��������ѡ��
  * ����  NewSelect: ����A��������ѡ��ȡֵΪ:
  *                    RTC_TIME_AM: ѡ������ʱ�䣬
  *                    RTC_TIME_PM: ѡ������ʱ�䡣
  * ����  �ޡ�
  */
void
RTC_Alarm_A_AMPM_Config (uint32_t NewSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- ����RTC_ALRA�Ĵ���AMPMAλ ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* ѡ������ʱ�� */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AMPMA_POS);
    }
    else
    {
        /* ѡ������ʱ�� */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AMPMA_POS);
    }
}

/**
  * ����  ���������ж�Сʱʱ�䣬�û�����ƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  Hour: �����ж�Сʱʱ�䣬ȡֵƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- ����RTC_ALRA�Ĵ���ALRHAλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_ALRA_ALRHA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRHA, tmpreg);
}

/**
  * ����  ��������A����ʹ�ܡ�
  * ����  NewState: ����A����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Minutes_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRA�Ĵ���AMENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������A����λ */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AMENA_POS);
    }
    else
    {
        /* ��ֹ����A����λ */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AMENA_POS);
    }
}

/**
  * ����  ���������жϷ���ʱ�䡣
  * ����  Minutes: �����жϷ���ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- ����RTC_ALRA�Ĵ���ALRMAλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_ALRA_ALRMA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRMA, tmpreg);
}

/**
  * ����  ��������A��ʹ�ܡ�
  * ����  NewState: ����A��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Seconds_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRA�Ĵ���ASENAλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������A����λ */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_ASENA_POS);
    }
    else
    {
        /* ��ֹ����A����λ */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_ASENA_POS);
    }
}

/**
  * ����  ���������ж���ʱ�䡣
  * ����  Seconds: �����ж���ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- ����RTC_ALRA�Ĵ���ALRSAλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_ALRA_ALRSA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRSA, tmpreg);
}

/**
  * ����  ��������Bʹ�ܡ�
  * ����  NewState: ����Bʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRB�Ĵ���ALRENBλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������B���� */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_ALRENB_POS);
    }
    else
    {
        /* ��ֹ����B���� */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_ALRENB_POS);
    }
}

/**
  * ����  ��������B��ʹ�ܡ�
  * ����  NewState: ����B��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Weekday_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRB�Ĵ���AWENBλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������B��λ */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AWENB_POS);
    }
    else
    {
        /* ��ֹ����B��λ */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AWENB_POS);
    }
}

/**
  * ����  ���������ж�����ʱ�䡣
  * ����  Weekday: �����ж�����ʱ�䣬ȡֵΪ:
  *                  RTC_WEEKDAY_MONDAY: ����һ
  *                  RTC_WEEKDAY_TUESDAY: ���ڶ�
  *                  RTC_WEEKDAY_WEDNESDAY: ������
  *                  RTC_WEEKDAY_THURSDAY: ������
  *                  RTC_WEEKDAY_FRIDAY: ������
  *                  RTC_WEEKDAY_SATURDAY: ������
  *                  RTC_WEEKDAY_SUNDAY: ������
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- ����RTC_ALRB�Ĵ���ALRWBλ ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_ALRB_ALRWB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRWB, tmpreg);
}

/**
  * ����  ��������Bʱ��ʹ�ܡ�
  * ����  NewState: ����Bʱ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Hours_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRB�Ĵ���AHENBλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������Bʱ��λ */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AHENB_POS);
    }
    else
    {
        /* ��ֹ����Bʱ��λ */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AHENB_POS);
    }
}

/**
  * ����  ��������B��������ѡ��
  * ����  NewSelect: ����B��������ѡ��ȡֵΪ:
  *                    RTC_TIME_AM: ѡ������ʱ�䣬
  *                    RTC_TIME_PM: ѡ������ʱ�䡣
  * ����  �ޡ�
  */
void
RTC_Alarm_B_AMPM_Config (uint32_t NewSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- ����RTC_ALRB�Ĵ���AMPMBλ ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* ѡ������ʱ�� */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AMPMB_POS);
    }
    else
    {
        /* ѡ������ʱ�� */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AMPMB_POS);
    }
}

/**
  * ����  ���������ж�Сʱʱ�䣬�û�����ƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  Hour: �����ж�Сʱʱ�䣬ȡֵƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- ����RTC_ALRB�Ĵ���ALRHBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_ALRB_ALRHB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRHB, tmpreg);
}

/**
  * ����  ��������B����ʹ�ܡ�
  * ����  NewState: ����B����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Minutes_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRB�Ĵ���AMENBλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������B����λ */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AMENB_POS);
    }
    else
    {
        /* ��ֹ����B����λ */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AMENB_POS);
    }
}

/**
  * ����  ���������жϷ���ʱ�䡣
  * ����  Minutes: �����жϷ���ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- ����RTC_ALRB�Ĵ���ALRMBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_ALRB_ALRMB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRMB, tmpreg);
}

/**
  * ����  ��������B��ʹ�ܡ�
  * ����  NewState: ����B��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Seconds_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_ALRB�Ĵ���ASENBλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������B����λ */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_ASENB_POS);
    }
    else
    {
        /* ��ֹ����B����λ */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_ASENB_POS);
    }
}

/**
  * ����  ���������ж���ʱ�䡣
  * ����  Seconds: �����ж���ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- ����RTC_ALRB�Ĵ���ALRSBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_ALRB_ALRSB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRSB, tmpreg);
}
/**
  *   ##### ʵʱʱ��(RTC)�������ú���������� #####
  */


/**
  *   ##### ʵʱʱ��(RTC)����ʱ�����ú��� #####
  */
/**
  * ����  ����RTCʱ�����ڡ�
  * ����  Weekday: RTCʱ�����ڣ�ȡֵΪ:
  *                  RTC_WEEKDAY_MONDAY: ����һ
  *                  RTC_WEEKDAY_TUESDAY: ���ڶ�
  *                  RTC_WEEKDAY_WEDNESDAY: ������
  *                  RTC_WEEKDAY_THURSDAY: ������
  *                  RTC_WEEKDAY_FRIDAY: ������
  *                  RTC_WEEKDAY_SATURDAY: ������
  *                  RTC_WEEKDAY_SUNDAY: ������
  * ����  �ޡ�
  */
void
RTC_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- ����RTC_TMR�Ĵ���WEKλ ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_TMR_WEK0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_WEK, tmpreg);
}

/**
  * ����  ����RTCʱ����������ѡ��
  * ����  NewSelect: RTCʱ����������ѡ��ȡֵΪ:
  *                    RTC_TIME_AM: ѡ������ʱ�䣬
  *                    RTC_TIME_PM: ѡ������ʱ�䡣
  * ����  �ޡ�
  */
void
RTC_AMPM_Config (uint32_t NewSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- ����RTC_TMR�Ĵ���AMPMλ ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* ѡ������ʱ�� */
        SFR_SET_BIT_ASM(RTC_TMR, RTC_TMR_AMPM_POS);
    }
    else
    {
        /* ѡ������ʱ�� */
        SFR_CLR_BIT_ASM(RTC_TMR, RTC_TMR_AMPM_POS);
    }
}

/**
  * ����  ����RTCʱ��ʱ��ʱ�䡣
  * ����  Hour: RTCʱ��ʱ��ʱ�䣬ȡֵƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  �ޡ�
  */
void
RTC_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- ����RTC_TMR�Ĵ���HOURλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_TMR_HOUR0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_HOUR, tmpreg);
}

/**
  * ����  ����RTCʱ�ӷ���ʱ�䡣
  * ����  Minutes: RTCʱ�ӷ���ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- ����RTC_TMR�Ĵ���MINλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_TMR_MIN0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_MIN, tmpreg);
}

/**
  * ����  ����RTCʱ������ʱ�䡣
  * ����  Seconds: RTCʱ������ʱ�䣬ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- ����RTC_TMR�Ĵ���SECλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_TMR_SEC0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_SEC, tmpreg);
}

/**
  * ����  ����RTCʱ����ݡ�
  * ����  Year: RTCʱ����ݣ�ȡֵΪ0-99��
  * ����  �ޡ�
  */
void
RTC_Year_Config (uint32_t Year)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_YEAR(Year));

    /*---------------- ����RTC_DTR�Ĵ���YEARλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Year);
    tmpreg <<= RTC_DTR_YEAR0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_YEAR, tmpreg);
}

/**
  * ����  ����RTCʱ���·ݡ�
  * ����  Month: RTCʱ���·ݣ�ȡֵΪ:
  *                RTC_MONTH_JANUARY: 1��
  *                RTC_MONTH_FEBRUARY: 2��
  *                RTC_MONTH_MARCH: 3��
  *                RTC_MONTH_APRIL: 4��
  *                RTC_MONTH_MAY: 5��
  *                RTC_MONTH_JUNE: 6��
  *                RTC_MONTH_JULY: 7��
  *                RTC_MONTH_AUGUST: 8��
  *                RTC_MONTH_SEPTEMBER: 9��
  *                RTC_MONTH_OCTOBER: 10��
  *                RTC_MONTH_NOVEMBER: 11��
  *                RTC_MONTH_DECEMBER: 12��
  * ����  �ޡ�
  */
void
RTC_Month_Config (uint32_t Month)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(Month));

    /*---------------- ����RTC_DTR�Ĵ���MTHλ ----------------*/
    tmpreg = Month << RTC_DTR_MTH0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_MTH, tmpreg);
}

/**
  * ����  ����RTCʱ�����ڡ�
  * ����  Day: RTCʱ�����ڣ�ȡֵΪ1-31��
  * ����  �ޡ�
  */
void
RTC_Day_Config (uint32_t Day)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_DATE(Day));

    /*---------------- ����RTC_DTR�Ĵ���DAYλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Day);
    tmpreg <<= RTC_DTR_DAY0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_DAY, tmpreg);
}

/**
  * ����  ����RTCʱ�����ڱ��ݡ�
  * ����  Weekday: RTCʱ�����ڱ��ݣ�ȡֵΪ:
  *                  RTC_WEEKDAY_MONDAY: ����һ
  *                  RTC_WEEKDAY_TUESDAY: ���ڶ�
  *                  RTC_WEEKDAY_WEDNESDAY: ������
  *                  RTC_WEEKDAY_THURSDAY: ������
  *                  RTC_WEEKDAY_FRIDAY: ������
  *                  RTC_WEEKDAY_SATURDAY: ������
  *                  RTC_WEEKDAY_SUNDAY: ������
  * ����  �ޡ�
  */
void
RTC_Weekday_Backup_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- ����RTC_TMBR�Ĵ���WEKBλ ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_TMBR_WEKB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_WEKB, tmpreg);
}

/**
  * ����  ����RTCʱ����������ѡ�񱸷ݡ�
  * ����  NewSelect: RTCʱ����������ѡ�񱸷ݣ�ȡֵΪ:
  *                    RTC_TIME_AM: ѡ������ʱ�䣬
  *                    RTC_TIME_PM: ѡ������ʱ�䡣
  * ����  �ޡ�
  */
void
RTC_AMPM_Backup_Config (uint32_t NewSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- ����RTC_TMBR�Ĵ���AMPMBλ ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* ѡ������ʱ�� */
        SFR_SET_BIT_ASM(RTC_TMBR, RTC_TMBR_AMPMB_POS);
    }
    else
    {
        /* ѡ������ʱ�� */
        SFR_CLR_BIT_ASM(RTC_TMBR, RTC_TMBR_AMPMB_POS);
    }
}

/**
  * ����  ����RTCʱ��ʱ��ʱ�䱸�ݡ�
  * ����  Hour: RTCʱ��ʱ��ʱ�䱸�ݣ�ȡֵƥ��12Сʱ�ƻ�24Сʱ�ơ�
  * ����  �ޡ�
  */
void
RTC_Hours_Backup_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- ����RTC_TMBR�Ĵ���HOURBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_TMBR_HOURB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_HOURB, tmpreg);
}

/**
  * ����  ����RTCʱ�ӷ���ʱ�䱸�ݡ�
  * ����  Minutes: RTCʱ�ӷ���ʱ�䱸�ݣ�ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Minutes_Backup_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- ����RTC_TMBR�Ĵ���MINBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_TMBR_MINB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_MINB, tmpreg);
}

/**
  * ����  ����RTCʱ������ʱ�䱸�ݡ�
  * ����  Seconds: RTCʱ������ʱ�䱸�ݣ�ȡֵΪ0-59��
  * ����  �ޡ�
  */
void
RTC_Seconds_Backup_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- ����RTC_TMBR�Ĵ���SECBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_TMBR_SECB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_SECB, tmpreg);
}

/**
  * ����  ����RTCʱ����ݱ��ݡ�
  * ����  Year: RTCʱ����ݱ��ݣ�ȡֵΪ0-99��
  * ����  �ޡ�
  */
void
RTC_Year_Backup_Config (uint32_t Year)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_YEAR(Year));

    /*---------------- ����RTC_DTBR�Ĵ���YEARBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Year);
    tmpreg <<= RTC_DTBR_YEARB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_YEARB, tmpreg);
}

/**
  * ����  ����RTCʱ���·ݱ��ݡ�
  * ����  Month: RTCʱ���·ݱ��ݣ�ȡֵΪ:
  *                RTC_MONTH_JANUARY: 1��
  *                RTC_MONTH_FEBRUARY: 2��
  *                RTC_MONTH_MARCH: 3��
  *                RTC_MONTH_APRIL: 4��
  *                RTC_MONTH_MAY: 5��
  *                RTC_MONTH_JUNE: 6��
  *                RTC_MONTH_JULY: 7��
  *                RTC_MONTH_AUGUST: 8��
  *                RTC_MONTH_SEPTEMBER: 9��
  *                RTC_MONTH_OCTOBER: 10��
  *                RTC_MONTH_NOVEMBER: 11��
  *                RTC_MONTH_DECEMBER: 12��
  * ����  �ޡ�
  */
void
RTC_Month_Backup_Config (uint32_t Month)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(Month));

    /*---------------- ����RTC_DTBR�Ĵ���MTHBλ ----------------*/
    tmpreg = Month << RTC_DTBR_MTHB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_MTHB, tmpreg);
}

/**
  * ����  ����RTCʱ�����ڱ��ݡ�
  * ����  Day: RTCʱ�����ڱ��ݣ�ȡֵΪ1-31��
  * ����  �ޡ�
  */
void
RTC_Day_Backup_Config (uint32_t Day)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_DATE(Day));

    /*---------------- ����RTC_DTBR�Ĵ���DAYBλ ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Day);
    tmpreg <<= RTC_DTBR_DAYB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_DAYB, tmpreg);
}
/**
  *   ##### ʵʱʱ��(RTC)����ʱ�����ú���������� #####
  */


/**
  *   ##### ʵʱʱ��(RTC)��ʱ�����ú��� #####
  */
/**
  * ����  ����RTC��ʱ��1����ֵ��
  * ����  Counter: RTC��ʱ��1����ֵ��ȡֵΪ16λ��ֵ��
  * ����  �ޡ�
  */
void
RTC_Timer1_Config (uint16_t Counter)
{
    uint32_t tmpreg = 0;

    /*---------------- ����RTC_TMER�Ĵ���TMR1λ ----------------*/
    tmpreg = (uint32_t)Counter << RTC_TMER_TMR1_0_POS;
    RTC_TMER = SFR_Config (RTC_TMER, ~RTC_TMER_TMR1, tmpreg);
}

/**
  * ����  ����RTC��ʱ��0����ֵ��
  * ����  Counter: RTC��ʱ��0����ֵ��ȡֵΪ16λ��ֵ��
  * ����  �ޡ�
  */
void
RTC_Timer0_Config (uint16_t Counter)
{
    uint32_t tmpreg = 0;

    /*---------------- ����RTC_TMER�Ĵ���TMR0λ ----------------*/
    tmpreg = (uint32_t)Counter << RTC_TMER_TMR0_0_POS;
    RTC_TMER = SFR_Config (RTC_TMER, ~RTC_TMER_TMR0, tmpreg);
}

/**
  * ����  ����RTC��ʱ��1ʹ�ܡ�
  * ����  TimerEnable: RTC��ʱ��1ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Timer1_Enable (FunctionalState TimerEnable)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(TimerEnable));

    /*---------------- ����RTC_TCR�Ĵ���TMR1ENλ ----------------*/
    if (TimerEnable != FALSE)
    {
        /* ʹ�ܶ�ʱ��1 */
        SFR_SET_BIT_ASM(RTC_TCR, RTC_TCR_TMR1EN_POS);
    }
    else
    {
        /* ��ֹ��ʱ��1 */
        SFR_CLR_BIT_ASM(RTC_TCR, RTC_TCR_TMR1EN_POS);
    }
}

/**
  * ����  ����RTC��ʱ��0ʹ�ܡ�
  * ����  TimerEnable: RTC��ʱ��0ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Timer0_Enable (FunctionalState TimerEnable)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(TimerEnable));

    /*---------------- ����RTC_TCR�Ĵ���TMR0ENλ ----------------*/
    if (TimerEnable != FALSE)
    {
        /* ʹ�ܶ�ʱ��0 */
        SFR_SET_BIT_ASM(RTC_TCR, RTC_TCR_TMR0EN_POS);
    }
    else
    {
        /* ��ֹ��ʱ��0 */
        SFR_CLR_BIT_ASM(RTC_TCR, RTC_TCR_TMR0EN_POS);
    }
}

/**
  * ����  ���ö�ʱ��1ʱ��Դѡ��
  * ����  ClockSource: ��ʱ��1ʱ��Դѡ��ȡֵΪ:
  *                      RTC_TIMER_CLOCK_RTC_DIV_128: RTCʱ��Դ/128 Լ1/256s
  *                      RTC_TIMER_CLOCK_RTC_DIV_512: RTCʱ��Դ/512 Լ1/64s
  *                      RTC_TIMER_CLOCK_RTC_DIV_1024: RTCʱ��Դ/1024 Լ1/32s
  *                      RTC_TIMER_CLOCK_RTC_DIV_2048: RTCʱ��Դ/2048 Լ1/16s
  *                      RTC_TIMER_CLOCK_RTC_DIV_8192: RTCʱ��Դ/8192 Լ1/4s
  *                      RTC_TIMER_CLOCK_ONE_SECOND: 1s
  *                      RTC_TIMER_CLOCK_ONE_MINUTE: 1min
  *                      RTC_TIMER_CLOCK_ONE_HOUR: 1hour
  *                      RTC_TIMER_CLOCK_CONNECTION: ������TMR1��TMR0����
  * ����  �ޡ�
  */
void
RTC_Timer1_Source_Config (uint16_t ClockSource)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIMER1_CLOCK(ClockSource));

    /*---------------- ����RTC_TCR�Ĵ���TMR1Cλ ----------------*/
    tmpreg = (uint32_t)ClockSource << RTC_TCR_TMR1C0_POS;
    RTC_TCR = SFR_Config (RTC_TCR, ~RTC_TCR_TMR1C, tmpreg);
}

/**
  * ����  ���ö�ʱ��0ʱ��Դѡ��
  * ����  ClockSource: ��ʱ��0ʱ��Դѡ��ȡֵΪ:
  *                      RTC_TIMER_CLOCK_RTC_DIV_128: RTCʱ��Դ/128 Լ1/256s
  *                      RTC_TIMER_CLOCK_RTC_DIV_512: RTCʱ��Դ/512 Լ1/64s
  *                      RTC_TIMER_CLOCK_RTC_DIV_1024: RTCʱ��Դ/1024 Լ1/32s
  *                      RTC_TIMER_CLOCK_RTC_DIV_2048: RTCʱ��Դ/2048 Լ1/16s
  *                      RTC_TIMER_CLOCK_RTC_DIV_8192: RTCʱ��Դ/8192 Լ1/4s
  *                      RTC_TIMER_CLOCK_ONE_SECOND: 1s
  *                      RTC_TIMER_CLOCK_ONE_MINUTE: 1min
  *                      RTC_TIMER_CLOCK_ONE_HOUR: 1hour
  * ����  �ޡ�
  */
void
RTC_Timer0_Source_Config (uint16_t ClockSource)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_RTC_TIMER0_CLOCK(ClockSource));

    /*---------------- ����RTC_TCR�Ĵ���TMR0Cλ ----------------*/
    tmpreg = (uint32_t)ClockSource << RTC_TCR_TMR0C0_POS;
    RTC_TCR = SFR_Config (RTC_TCR, ~RTC_TCR_TMR0C, tmpreg);
}

/**
  *   ##### ʵʱʱ��(RTC)��ʱ�����ú���������� #####
  */


/**
  *   ##### ʵʱʱ��(RTC)�жϹ����� #####
  */
/**
  * ����  ����RTCʱ����ж�ʹ�ܡ�
  * ����  NewState: RTCʱ����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Stamp_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���TSIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��ʱ����ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TSIE_POS);
    }
    else
    {
        /* ��ֹʱ����ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TSIE_POS);
    }
}

/**
  * ����  ����RTCʱ�������ж�ʹ�ܡ�
  * ����  NewState: RTCʱ�������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Stamp_Overflow_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���TSOVFIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��ʱ�������ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TSOVFIE_POS);
    }
    else
    {
        /* ��ֹʱ�������ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TSOVFIE_POS);
    }
}

/**
  * ����  ����RTC��ʱ��1�ж�ʹ�ܡ�
  * ����  NewState: RTC��ʱ��1�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Timer1_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���RTCT1IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ�ʱ��1�ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_RTCT1IE_POS);
    }
    else
    {
        /* ��ֹ��ʱ��1�ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_RTCT1IE_POS);
    }
}

/**
  * ����  ����RTC��ʱ��0�ж�ʹ�ܡ�
  * ����  NewState: RTC��ʱ��0�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Timer0_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���RTCT0IEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ�ʱ��0�ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_RTCT0IE_POS);
    }
    else
    {
        /* ��ֹ��ʱ��0�ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_RTCT0IE_POS);
    }
}

/**
  * ����  ����RTCʱ������ж�ʹ�ܡ�
  * ����  NewState: RTCʱ������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Time_Tick_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���TTIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��ʱ������ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TTIE_POS);
    }
    else
    {
        /* ��ֹʱ������ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TTIE_POS);
    }
}

/**
  * ����  ����RTC����B�ж�ʹ�ܡ�
  * ����  NewState: RTC����B�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_B_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���ALRBIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������B�ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_ALRBIE_POS);
    }
    else
    {
        /* ��ֹ����B�ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_ALRBIE_POS);
    }
}

/**
  * ����  ����RTC����A�ж�ʹ�ܡ�
  * ����  NewState: RTC����A�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Alarm_A_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���ALRAIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ������A�ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_ALRAIE_POS);
    }
    else
    {
        /* ��ֹ����A�ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_ALRAIE_POS);
    }
}

/**
  * ����  ����RTC�ս����ж�ʹ�ܡ�
  * ����  NewState: RTC�ս����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Days_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���DAYIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���ս����ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_DAYIE_POS);
    }
    else
    {
        /* ��ֹ�ս����ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_DAYIE_POS);
    }
}

/**
  * ����  ����RTCСʱ�����ж�ʹ�ܡ�
  * ����  NewState: RTCСʱ�����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Hours_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���HOURIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Сʱ�����ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_HOURIE_POS);
    }
    else
    {
        /* ��ֹСʱ�����ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_HOURIE_POS);
    }
}

/**
  * ����  ����RTC�ֽ����ж�ʹ�ܡ�
  * ����  NewState: RTC�ֽ����ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Minutes_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���MINIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ֽܷ����ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_MINIE_POS);
    }
    else
    {
        /* ��ֹ�ֽ����ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_MINIE_POS);
    }
}

/**
  * ����  ����RTC������ж�ʹ�ܡ�
  * ����  NewState: RTC������ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
RTC_Seconds_INT_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����RTC_IER�Ĵ���SECIEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��������ж� */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_SECIE_POS);
    }
    else
    {
        /* ��ֹ������ж� */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_SECIE_POS);
    }
}

/**
  * ����  ��ȡʱ����жϱ�־��
  * ����  �ޡ�
  * ����  1:������ʱ����жϣ�0:δ����ʱ����жϡ�
  */
FlagStatus
RTC_Get_Time_Stamp_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���TSIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_TSIF)
    {
        /* ������ʱ����ж� */
        return SET;
    }
    else
    {
        /* δ����ʱ����ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡʱ�������жϱ�־��
  * ����  �ޡ�
  * ����  1:������ʱ�������жϣ�0:δ����ʱ�������жϡ�
  */
FlagStatus
RTC_Get_Time_Stamp_Overflow_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���TSOVFIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_TSOVFIF)
    {
        /* ������ʱ�������ж� */
        return SET;
    }
    else
    {
        /* δ����ʱ�������ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC��ʱ��1�жϱ�־��
  * ����  �ޡ�
  * ����  1:������RTC��ʱ��1�жϣ�0:δ����RTC��ʱ��1�жϡ�
  */
FlagStatus
RTC_Get_Timer1_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���RTCT1IFλ ----------------*/
    if (RTC_IFR & RTC_IFR_RTCT1IF)
    {
        /* ������RTC��ʱ��1�ж� */
        return SET;
    }
    else
    {
        /* δ����RTC��ʱ��1�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC��ʱ��0�жϱ�־��
  * ����  �ޡ�
  * ����  1:������RTC��ʱ��0�жϣ�0:δ����RTC��ʱ��0�жϡ�
  */
FlagStatus
RTC_Get_Timer0_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���RTCT0IFλ ----------------*/
    if (RTC_IFR & RTC_IFR_RTCT0IF)
    {
        /* ������RTC��ʱ��0�ж� */
        return SET;
    }
    else
    {
        /* δ����RTC��ʱ��0�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTCʱ������жϱ�־��
  * ����  �ޡ�
  * ����  1:������ʱ������жϣ�0:δ����ʱ������жϡ�
  */
FlagStatus
RTC_Get_Time_Tick_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���TTIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_TTIF)
    {
        /* ������ʱ������ж� */
        return SET;
    }
    else
    {
        /* δ����ʱ������ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC����B�жϱ�־��
  * ����  �ޡ�
  * ����  1:����������B�жϣ�0:δ��������B�жϡ�
  */
FlagStatus
RTC_Get_Alarm_B_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���ALRBIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_ALRBIF)
    {
        /* ����������B�ж� */
        return SET;
    }
    else
    {
        /* δ��������B�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC����A�жϱ�־��
  * ����  �ޡ�
  * ����  1:����������A�жϣ�0:δ��������B�жϡ�
  */
FlagStatus
RTC_Get_Alarm_A_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���ALRAIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_ALRAIF)
    {
        /* ����������A�ж� */
        return SET;
    }
    else
    {
        /* δ��������A�ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC�ս����жϱ�־��
  * ����  �ޡ�
  * ����  1:�������ս����жϣ�0:δ�����ս����жϡ�
  */
FlagStatus
RTC_Get_Days_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���DAYIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_DAYIF)
    {
        /* �������ս����ж� */
        return SET;
    }
    else
    {
        /* δ�����ս����ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTCСʱ�����жϱ�־��
  * ����  �ޡ�
  * ����  1:������Сʱ�����жϣ�0:δ����Сʱ�����жϡ�
  */
FlagStatus
RTC_Get_Hours_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���HOURIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_HOURIF)
    {
        /* ������Сʱ�����ж� */
        return SET;
    }
    else
    {
        /* δ����Сʱ�����ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC�ֽ����жϱ�־��
  * ����  �ޡ�
  * ����  1:�����˷ֽ����жϣ�0:δ�����ֽ����жϡ�
  */
FlagStatus
RTC_Get_Minutes_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���MINIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_MINIF)
    {
        /* �����˷ֽ����ж� */
        return SET;
    }
    else
    {
        /* δ�����ֽ����ж� */
        return RESET;
    }
}

/**
  * ����  ��ȡRTC������жϱ�־��
  * ����  �ޡ�
  * ����  1:������������жϣ�0:δ����������жϡ�
  */
FlagStatus
RTC_Get_Seconds_INT_Flag (void)
{
    /*---------------- ��ȡRTC_IFR�Ĵ���SECIFλ ----------------*/
    if (RTC_IFR & RTC_IFR_SECIF)
    {
        /* ������������ж� */
        return SET;
    }
    else
    {
        /* δ����������ж� */
        return RESET;
    }
}

/**
  * ����  ����RTCʱ����жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Time_Stamp_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���TSICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TSIC_POS);
    while(RTC_IFR & RTC_IFR_TSIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TSIC_POS);
}

/**
  * ����  ����RTCʱ�������жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Time_Stamp_Overflow_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���TSOVFICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TSOVFIC_POS);
    while(RTC_IFR & RTC_IFR_TSOVFIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TSOVFIC_POS);
}

/**
  * ����  ����RTC��ʱ��1�жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Timer1_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���RTCT1ICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_RTCT1IC_POS);
    while(RTC_IFR & RTC_IFR_RTCT1IF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_RTCT1IC_POS);
}

/**
  * ����  ����RTC��ʱ��0�жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Timer0_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���RTCT0ICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_RTCT0IC_POS);
    while(RTC_IFR & RTC_IFR_RTCT0IF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_RTCT0IC_POS);
}

/**
  * ����  ����ʱ������жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Time_Tick_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���TTICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TTIC_POS);
    while(RTC_IFR & RTC_IFR_TTIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TTIC_POS);
}

/**
  * ����  ��������B�жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Alarm_B_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���ALRBICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_ALRBIC_POS);
    while(RTC_IFR & RTC_IFR_ALRBIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_ALRBIC_POS);
}

/**
  * ����  ��������A�жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Alarm_A_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���ALRAICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_ALRAIC_POS);
    while(RTC_IFR & RTC_IFR_ALRAIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_ALRAIC_POS);
}

/**
  * ����  ����RTC�ս����жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Days_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���DAYICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_DAYIC_POS);
    while(RTC_IFR & RTC_IFR_DAYIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_DAYIC_POS);
}

/**
  * ����  ����RTCСʱ�����жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Hours_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���HOURICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_HOURIC_POS);
    while(RTC_IFR & RTC_IFR_HOURIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_HOURIC_POS);
}

/**
  * ����  ����RTC�ֽ����жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Minutes_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���MINICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_MINIC_POS);
    while(RTC_IFR & RTC_IFR_MINIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_MINIC_POS);
}

/**
  * ����  ����RTC������жϱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
RTC_Clear_Seconds_INT_Flag (void)
{
    /*---------------- ����RTC_IFR�Ĵ���SECICλ ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_SECIC_POS);
    while(RTC_IFR & RTC_IFR_SECIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_SECIC_POS);
}
/**
  *   ##### ʵʱʱ��(RTC)�жϹ������������ #####
  */


/**
  *   ##### ʵʱʱ��(RTC)˽�к��� #####
  */
/**
  * ����  ʮ������ֵת��ΪBCD�롣
  * ����  Value: ��ת����ʮ������λ����8λ��Ч��ֵ��
  * ����  2�ֽ�BCD�롣
  */
static uint8_t
RTC_Byte_To_Bcd (uint8_t Value)
{
    uint8_t bcdhigh = 0;
    uint8_t bcdlow = 0;

    bcdhigh = Value / ((uint8_t)10);
    bcdlow = Value - bcdhigh * ((uint8_t)10);

    return (uint8_t)((bcdhigh << (uint8_t)4) | bcdlow);
}

/**
  * ����  BCD��ת��Ϊʮ������λ����
  * ����  Value: ��ת����BCD�룬8λ��Ч��ֵ��
  * ����  ʮ������λ����
  */
static uint8_t
RTC_Bcd_To_Byte (uint8_t Value)
{
    uint8_t tmp = 0;

    tmp = (Value >> (uint8_t)4) * ((uint8_t)10);

    return (tmp + (Value & (uint8_t)0xF));
}
/**
  *   ##### ʵʱʱ��(RTC)˽�к���������� #####
  */

