/**
  ******************************************************************************
  * �ļ���  kf32a_basic_usb.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.61
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ��USBģ��(USB)��صĹ��ܺ�����������
  *          + USBģ��(USB)��ʼ������
  *          + USBģ��(USB)�������ú���
  *          + USBģ��(USB)����������
  *          + USBģ��(USB)�жϹ�����
  *
  *********************************************************************
  */

#include "kf32a_basic_usb.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"
#include "string.h"


#ifdef KF32A_Periph_usb
/* ��ʱ��˽�ж��� ----------------------------------------------------*/
/* USB_UCON�Ĵ�����ʼ������ */
#define USB_UCON_INIT_MASK              (USB_UCON_SPEEDEN \
                                       | USB_UCON_UPUEN \
                                       | USB_UCON_DB)

/* USB_UEP�Ĵ�����ʼ������ */
#define USB_UEP_INIT_MASK               (USB_UEP_EPHSHK \
                                       | USB_UEP_EPCONDIS \
                                       | USB_UEP_EPOUTEN \
                                       | USB_UEP_EPINEN)

/* USB_UEP�Ĵ�����ʼ������ */
#define USB_BDSTAT_INIT_MASK            (USB_BDSTAT_BADDR \
                                       | USB_BDSTAT_BC \
                                       | USB_BDSTAT_UOWN \
                                       | USB_BDSTAT_DTS \
                                       | USB_BDSTAT_DTSEN \
                                       | USB_BDSTAT_BSTALL)

/**
  *   ##### USBģ��(USB)��ʼ������ #####
  */
/**
  * ����  ��λUSB���裬ʹ������ʱ�ӡ�
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
USB_Reset(void)
{
    /* ��λUSB���� */
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USBRST, TRUE);
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USBRST, FALSE);

    /* ʹ��USB����ʱ�� */
    PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USBCLKEN, TRUE);
}

/**
  * ����  USBģ��(USB)��ʼ����
  * ����  usbInitStruct: USB������Ϣ�ṹ�塣
  * ����  �ޡ�
  */
void
USB_Configuration(USB_InitTypeDef* usbInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t i = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_SPEED_SELECT(usbInitStruct->m_Speed));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Pullup));
    CHECK_RESTRICTION(CHECK_USB_DOUBLE_BUFFER(usbInitStruct->m_DoubleBufferCfg));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_DoubleBufferRst));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_MASK(usbInitStruct->m_EndPointMask));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Handshake));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Bidirectional));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Output));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Input));

    /*---------------- ����USB_UCON�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_Speed������SPEEDENλ�� */
    /* ���ݽṹ���Աm_Pullup������UPUENλ�� */
    /* ���ݽṹ���Աm_DoubleBufferCfg������DBλ�� */
    tmpreg = usbInitStruct->m_Speed \
           | ((uint32_t)usbInitStruct->m_Pullup << USB_UCON_UPUEN_POS) \
           | usbInitStruct->m_DoubleBufferCfg
           | ((uint32_t)usbInitStruct->m_DoubleBufferRst << USB_UCON_DBRST_POS);
    USB_UCON = SFR_Config (USB_UCON, ~USB_UCON_INIT_MASK, tmpreg);

    if (usbInitStruct->m_DoubleBufferRst != FALSE)
    {
        /* ˫��������λ���� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
    else
    {
        ;
    }

    /*---------------- ����USB_UEPn�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_EndPointMask��ѡ��USB_UEPn�Ĵ��� */
    /* ���ݽṹ���Աm_Handshake������EPHSHKλ�� */
    /* ���ݽṹ���Աm_Bidirectional������EPCONDISλ�� */
    /* ���ݽṹ���Աm_Output������EPOUTENλ�� */
    /* ���ݽṹ���Աm_Input������EPINENλ�� */
    tmpreg = ((uint32_t)usbInitStruct->m_Handshake << USB_UEP_EPHSHK_POS) \
           | ((uint32_t)usbInitStruct->m_Bidirectional << USB_UEP_EPCONDIS_POS) \
           | ((uint32_t)usbInitStruct->m_Output << USB_UEP_EPOUTEN_POS) \
           | ((uint32_t)usbInitStruct->m_Input << USB_UEP_EPINEN_POS);
    tmpreg1 = usbInitStruct->m_EndPointMask;
    for (i = 0; i <= USB_ENDPOINT_7; i++)
    {
        if (tmpreg1 & 0x1)
        {
            USB_SFR->UEP[i] = SFR_Config (USB_SFR->UEP[i],
                                    ~USB_UEP_INIT_MASK,
                                    tmpreg);
        }
        else
        {
            ;
        }
        tmpreg1 >>= 1;
    }
}

/**
  * ����  USBģ��(USB)��������ʼ����
  * ����  usbInitStruct: USB��������Ϣ�ṹ�塣
  * ����  �ޡ�
  */
void
USB_Buffer_Configuration(USB_BufferTypeDef* usbInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(usbInitStruct->m_EndPoint));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_DIRECT(usbInitStruct->m_Direction));
    CHECK_RESTRICTION(CHECK_USB_ODD_EVEN(usbInitStruct->m_BDPointer));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(usbInitStruct->m_ByteCount));
    CHECK_RESTRICTION(CHECK_USB_OWN_BD(usbInitStruct->m_Own));
    CHECK_RESTRICTION(CHECK_USB_DATA_TOGGLE_SYNC(usbInitStruct->m_DataToggleSync));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_DataToggleSyncEn));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_BufferStall));

    /* ��ȡ���������õ�ַ���ڴ� */
    tmpaddr = USB_Get_Endpoint_Double_Buffer_Addr (usbInitStruct->m_EndPoint, \
                                usbInitStruct->m_Direction,
                                usbInitStruct->m_BDPointer);
    tmpreg = *(volatile uint32_t *) tmpaddr;

    /*---------------- ����USB_BDnSTAT�Ĵ��� ----------------*/
    /* ���ݽṹ���Աm_BufferAddr������BADDRλ�� */
    /* ���ݽṹ���Աm_ByteCount������BCλ�� */
    /* ���ݽṹ���Աm_Own������UOWNλ�� */
    /* ���ݽṹ���Աm_DataToggleSync������DTSλ�� */
    /* ���ݽṹ���Աm_DataToggleSyncEn������DTSENλ�� */
    /* ���ݽṹ���Աm_BufferStall������BSTALLλ�� */

    /* ��ȡBCλ�ĸ���λ */
    tmpreg &= USB_BDSTAT_BCH;
    tmpreg >>= USB_BDSTAT_BC8_POS;
    /* ��ȡBCλ�ĵ���λ */
    tmpreg1 &= USB_BDSTAT_BCL;
    tmpreg1 >>= USB_BDSTAT_BC0_POS;
    /* ���BC��BITλ�� */
    tmpreg = (tmpreg << (USB_BDSTAT_BC7_POS + 1)) | (tmpreg1);

    /* USB_BDnSTAT�Ĵ���������Ϣ */
    tmpreg = ((uint32_t)usbInitStruct->m_BufferAddr << USB_BDSTAT_BADDR0_POS) \
           | tmpreg \
           | (usbInitStruct->m_Own << USB_BDSTAT_UOWN_POS) \
           | usbInitStruct->m_DataToggleSync \
           | ((uint32_t)usbInitStruct->m_DataToggleSyncEn << USB_BDSTAT_DTSEN_POS) \
           | ((uint32_t)usbInitStruct->m_BufferStall << USB_BDSTAT_BSTALL_POS);

    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_INIT_MASK,
                            tmpreg);
}

/**
  * ����  ��ʼ��USB������Ϣ�ṹ�塣
  * ����  usbInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
USB_Struct_Init (USB_InitTypeDef* usbInitStruct)
{
    /* ��ʼ�� USB�ٶ� */
    usbInitStruct->m_Speed = USB_LOW_SPEED;

    /* ��ʼ�� USBƬ������ʹ�� */
    usbInitStruct->m_Pullup = FALSE;

    /* ��ʼ�� USB˫���������� */
    usbInitStruct->m_DoubleBufferCfg = USB_DOUBLE_BUFFER_FORBID;

    /* ��ʼ�� USB�˵������� */
    usbInitStruct->m_EndPointMask = USB_ENDPOINT_MASK_NONE;

    /* ��ʼ�� USB�˵�����ʹ�� */
    usbInitStruct->m_Handshake = FALSE;

    /* ��ʼ�� ˫��˵����ʹ�� */
    usbInitStruct->m_Bidirectional = FALSE;

    /* ��ʼ�� USB�˵����ʹ�� */
    usbInitStruct->m_Output = FALSE;

    /* ��ʼ�� USB�˵�����ʹ�� */
    usbInitStruct->m_Input = FALSE;
}

/**
  * ����  ��ʼ��USB��������Ϣ�ṹ�塣
  * ����  usbInitStruct: ָ�����ʼ���Ľṹ��ָ�롣
  * ����  �ޡ�
  */
void
USB_Buffer_Struct_Init (USB_BufferTypeDef* usbInitStruct)
{
    /* ��ʼ�� USB�˵��� */
    usbInitStruct->m_EndPoint = USB_ENDPOINT_0;

    /* ��ʼ�� USB�˵㷽�� */
    usbInitStruct->m_Direction = USB_ENDPOINT_OUTPUT;

    /* ��ʼ�� USB�˵���żBD�洢�� */
    usbInitStruct->m_BDPointer = USB_EVEN_BUFFER_DESCRIPOR;

    /* ��ʼ�� RAM�����ַ */
    usbInitStruct->m_BufferAddr = 0;

    /* ��ʼ�� RAM�ֽڼ��� */
    usbInitStruct->m_ByteCount = 0;

    /* ��ʼ�� USB ӵ��λ */
    usbInitStruct->m_Own = USB_KERNEL_OWN_BD;

    /* ��ʼ�� USB���ݷ�תͬ�� */
    usbInitStruct->m_DataToggleSync = USB_DATA_0_TOGGLE_SYNC;

    /* ��ʼ�� USB���ݷ�תͬ��ʹ�� */
    usbInitStruct->m_DataToggleSyncEn = FALSE;

    /* ��ʼ�� USB������ֹͣʹ�� */
    usbInitStruct->m_BufferStall = FALSE;
}
/**
  *   ##### USBģ��(USB)��ʼ������������� #####
  */


/**
  *   ##### USBģ��(USB)�������ú��� #####
  */
/**
  * ����  ����USB˫��������λʹ�ܡ�
  * ����  NewState: USB˫��������λʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Double_Buffer_Pointer_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���DBRSTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ������˫������ָ�븴λΪż��������������BD�洢�� */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
    else
    {
        /* ����λ˫������ָ�� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
}

/**
  * ����  ��ȡ��Ч����0��ƽ��־��
  * ����  �ޡ�
  * ����  1:USB�����ϵĵ���0��ƽ��Ч��0:δ��⵽����0��ƽ��
  */
FlagStatus
USB_Get_Single_Ended_0_Flag (void)
{
    /*---------------- ��ȡUSB_UCON�Ĵ���SE0λ ----------------*/
    if (USB_UCON_SE0 & USB_UCON)
    {
        /* USB�����ϵĵ���0��ƽ��Ч */
        return SET;
    }
    else
    {
        /* δ��⵽����0��ƽ */
        return RESET;
    }
}

/**
  * ����  ������Ч����0��ƽ��־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
USB_Clear_Single_Ended_0_Flag (void)
{
    /*---------------- ����USB_UCON�Ĵ���SE0λ ----------------*/
	while(USB_UCON_SE0 & USB_UCON)
	{
		SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SE0_POS);
	}
}

/**
  * ����  ��ȡ���ݰ����ͱ�־��
  * ����  �ޡ�
  * ����  1: �ѽ�ֹSIE���ƺ����ݰ������յ�SETUP���ƣ�
  *       0: ����SIE���ƺ����ݰ�����
  */
FlagStatus
USB_Get_Package_Transmit_Flag (void)
{
    /*---------------- ��ȡUSB_UCON�Ĵ���PKTDISλ ----------------*/
    if (USB_UCON_PKTDIS & USB_UCON)
    {
        /* �ѽ�ֹSIE���ƺ����ݰ������յ�SETUP���� */
        return SET;
    }
    else
    {
        /* ����SIE���ƺ����ݰ����� */
        return RESET;
    }
}

/**
  * ����  �������ݰ����ͱ�־��
  * ����  �ޡ�
  * ����  �ޡ�
  */
void
USB_Clear_Package_Transmit_Flag (void)
{
    /*---------------- ����USB_UCON�Ĵ���PKTDISλ ----------------*/
	while(USB_UCON_PKTDIS & USB_UCON)
	{
		SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_PKTDIS_POS);
	}
}

/**
  * ����  ����USB��ͣ(����)ʹ�ܡ�
  * ����  NewState: USB��ͣ(����)ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Suspend_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���SUSPNDλ ----------------*/
    if (NewState != FALSE)
    {
        /* USBģ���֧�ֵ�·��������ģʽ��SIEʱ�Ӳ����� */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_SUSPND_POS);
    }
    else
    {
        /* USBģ���֧�ֵ�·������������״̬��SIEʱ�������õ��������� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SUSPND_POS);
    }
}

/**
  * ����  ����USB�ָ��źŴ���ʹ�ܡ�
  * ����  NewState: USB�ָ��źŴ���ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Resume_Signal_Transmit_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���RESUMEλ ----------------*/
    if (NewState != FALSE)
    {
        /* ����ָ��źŴ��� */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_RESUME_POS);
    }
    else
    {
        /* ��ֹ�ָ��źŴ��� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_RESUME_POS);
    }
}

/**
  * ����  ����USBģ��ʹ�ܡ�
  * ����  NewState: USBģ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Cmd (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���USBENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��USBģ���֧�ֵ�·�������豸�� */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_USBEN_POS);
    }
    else
    {
        /* ��ֹUSBģ���֧�ֵ�·���Ͽ��豸�� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_USBEN_POS);
    }
}

/**
  * ����  ����USB��ͼ����ʹ�ܡ�
  * ����  NewState: USB��ͼ����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Eye_Test_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���EYETESTλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���Զ���ͼ���� */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_EYETEST_POS);
    }
    else
    {
        /* ��ֹ�Զ���ͼ���� */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_EYETEST_POS);
    }
}

/**
  * ����  ����USB�ٶ�ѡ��
  * ����  SpeedSelect: USB�ٶ�ѡ��ȡֵΪ:
  *                      USB_LOW_SPEED: �����豸
  *                      USB_FULL_SPEED: ȫ���豸
  * ����  �ޡ�
  */
void
USB_Speed_Config (uint32_t SpeedSelect)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_SPEED_SELECT(SpeedSelect));

    /*---------------- ����USB_UCON�Ĵ���SPEEDENλ ----------------*/
    if (SpeedSelect != USB_LOW_SPEED)
    {
        /* ȫ���豸 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_SPEEDEN_POS);
    }
    else
    {
        /* �����豸 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SPEEDEN_POS);
    }
}

/**
  * ����  ����USBƬ������ʹ�ܡ�
  * ����  NewState: USBƬ������ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_On_Chip_Pull_Up_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UCON�Ĵ���UPUENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Ƭ������ */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_UPUEN_POS);
    }
    else
    {
        /* ��ֹƬ������ */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_UPUEN_POS);
    }
}

/**
  * ����  ����USB˫������ʹ�ܡ�
  * ����  DoubleBuffer: USB˫������ʹ��״̬��ȡֵΪ:
  *                       USB_DOUBLE_BUFFER_FORBID: ��ֹż��/����˫������
  *                       USB_OUT_ENDPOINT_0: ʹ��OUT�˵�0��ż��/����˫������
  *                       USB_ALL_ENDPOINTS: ʹ�����ж˵��ż��/����˫������
  *                       USB_ENDPOINT_1_TO_7: ʹ�ܶ˵�1��7��ż��/����˫������
  * ����  �ޡ�
  */
void
USB_Double_Buffer_Config (uint32_t DoubleBuffer)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_DOUBLE_BUFFER(DoubleBuffer));

    /*---------------- ����USB_UCON�Ĵ���DBλ ----------------*/
    USB_UCON = SFR_Config (USB_UCON, ~USB_UCON_DB, DoubleBuffer);
}

/**
  * ����  ��ȡUSB״̬��Ϣ��
  * ����  usbStateStruct: USB״̬��Ϣ�ṹ�壬��¼SIE�е�����״̬��
  * ����  �ޡ�
  */
void
USB_Get_Arrangement_State (USB_StateTypeDef* usbStateStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* ����У�� */
    CHECK_RESTRICTION(usbStateStruct != (USB_StateTypeDef*)0);

    /*---------------- ��ȡUSB_USTAT�Ĵ��� ----------------*/
    tmpreg = USB_USTAT;

    /*---------------- ��ȡUSB_USTAT�Ĵ���PEPλ ----------------*/
    /* ��¼m_EndPoint��m_EndPointDirection */
    tmpreg1 = tmpreg & USB_USTAT_PEP;
    tmpreg1 >>= USB_USTAT_PEP0_POS;
    usbStateStruct->m_EndPoint = tmpreg1;

    /*---------------- ��ȡUSB_USTAT�Ĵ���PDIRλ ----------------*/
    tmpreg1 = tmpreg & USB_USTAT_PDIR;
    tmpreg1 >>= USB_USTAT_PDIR_POS;
    usbStateStruct->m_BDDirection = tmpreg1;

    /*---------------- ��ȡUSB_USTAT�Ĵ���DBIλ ----------------*/
    tmpreg1 = tmpreg & USB_USTAT_DBI;
    tmpreg1 >>= USB_USTAT_DBI_POS;
    usbStateStruct->m_BDPointer = tmpreg1;
}

/**
  * ����  ����USB��ַ��
  * ����  Address: USB��ַ��ȡֵΪ0~0x7F��
  * ����  �ޡ�
  */
void
USB_Address_Config (uint32_t Address)
{
	uint32_t tmpreg = 0;
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ADDRESS(Address));

    /*---------------- ����USB_UADDR�Ĵ���UADDRλ ----------------*/
    tmpreg = Address << USB_UADDR_UADDR0_POS;
    USB_UADDR = SFR_Config (USB_UADDR, ~USB_UADDR_UADDR, tmpreg);
}

/**
  * ����  ��ȡUSB��ַ��
  * ����  �ޡ�
  * ����  USB��ַ��ȡֵΪ0~0x7F��
  */
uint32_t
USB_Get_Address (void)
{
    uint32_t tmpreg = 0;

    /*------------- ��ȡUSB_UADDR�Ĵ���UADDRλ -------------*/
    tmpreg = USB_UADDR;
    tmpreg &= USB_UADDR_UADDR;
    tmpreg >>= USB_UADDR_UADDR0_POS;

    return tmpreg;
}

/**
  * ����  ��ȡUSB֡��š�
  * ����  �ޡ�
  * ����  USB֡��ţ�ȡֵΪ0~0x7FF��
  */
uint32_t
USB_Get_Frame_Num (void)
{
    uint32_t tmpreg = 0;

    /*------------- ��ȡUSB_UFRM�Ĵ���UFRMλ -------------*/
    tmpreg = USB_UFRM;
    tmpreg &= USB_UFRM_UFRM;
    tmpreg >>= USB_UFRM_UFRM0_POS;

    return tmpreg;
}

/**
  * ����  ����USB�˵�����ʹ�ܡ�
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  *       NewState: USB�˵�����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Endpoint_Handshake_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UEP�Ĵ���EPHSHKλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ˵����� */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPHSHK_POS);
    }
    else
    {
        /* ��ֹ�˵����� */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPHSHK_POS);
    }
}

/**
  * ����  ����USB˫��˵����ʹ�ܡ�
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  *       NewState: USB˫��˵����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Bidirectional_Endpoint_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UEP�Ĵ���EPCONDISλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ˵���ƣ�SETUP�����ͣ�ͬʱ����IN ��OUT ���� */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPCONDIS_POS);
    }
    else
    {
        /* ��ֹ�˵���ƴ��ͣ�������IN ��OUT ���� */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPCONDIS_POS);
    }
}

/**
  * ����  ����USB�˵����ʹ�ܡ�
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  *       NewState: USB�˵����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Endpoint_Output_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UEP�Ĵ���EPOUTENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ˵���� */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPOUTEN_POS);
    }
    else
    {
        /* ��ֹ�˵���� */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPOUTEN_POS);
    }
}

/**
  * ����  ����USB�˵�����ʹ�ܡ�
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  *       NewState: USB�˵�����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Endpoint_Input_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UEP�Ĵ���EPINENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ�ܶ˵����� */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPINEN_POS);
    }
    else
    {
        /* ��ֹ�˵����� */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPINEN_POS);
    }
}

/**
  * ����  ��ȡUSB�˵�STALLָʾ��־��
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  * ����  USB�˵�STALLָʾ��־��1���˵��Ϸ���STALL���ְ���2���˵���δ����
  *       STALL���ְ���
  */
FlagStatus
USB_Get_Endpoint_STALL_Flag (uint32_t EndPoint)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));

    /*---------------- ��ȡUSB_UEP�Ĵ���EPSTLλ ----------------*/
    if (USB_SFR->UEP[EndPoint] & USB_UEP_EPSTL)
    {
        /* �˵��Ϸ���STALL���ְ� */
        return SET;
    }
    else
    {
        /* �˵���δ����STALL���ְ� */
        return RESET;
    }
}

/**
  * ����  ����USB�˵�STALLָʾ��־��
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  * ����  �ޡ�
  */
void
USB_Clear_Endpoint_STALL_Flag (uint32_t EndPoint)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));

    /*---------------- ����USB_UEP�Ĵ���EPSTLλ ----------------*/
    while(USB_SFR->UEP[EndPoint] & USB_UEP_EPSTL)
    {
    	SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPSTL_POS);
    }
}

/**
  * ����  ����USB Debug����ʹ�ܡ�
  * ����  NewState: USB Debug����ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Debug_Signal_Output_Enable (FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UDB�Ĵ���DEBUGENλ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ��Debug�ź���� */
        SFR_SET_BIT_ASM(USB_UDB, USB_UDB_DEBUGEN_POS);
    }
    else
    {
        /* ��ֹDebug�ź���� */
        SFR_CLR_BIT_ASM(USB_UDB, USB_UDB_DEBUGEN_POS);
    }
}

/**
  * ����  ����USB Debug���ѡ��
  * ����  DebugCtl: USB Debug���ѡ��ȡֵΪ:
  *                   USB_DBG_DATA_OUTPUT_AND_CLOCK: ����������ˡ�����������ˡ������ڲ�ʱ��
  *                   USB_DBG_RECEIVE_AND_CLOCK: ����ģ�鴦���������ݡ�ģ��ģ����յ������ݡ�����ģ��ͬ��ʱ��
  *                   USB_DBG_DATA_INPUT_AND_CLOCK: �����������ˡ��������븺�ˡ�����ʹ��
  *                   USB_DBG_RAM_DATA: RAM����
  *                   USB_DBG_USB_STATE_MACHINE: USB״̬��
  *                   USB_DBG_RAM_STATE_MACHINE: USB RAM״̬��
  *                   USB_DBG_RAM_ADDRESS: RAM��ַ
  *                   USB_DBG_OPERATIONAL_CONTROL: CRC5���п��ơ�CRC16���п��ơ�USB���տ���
  * ����  �ޡ�
  */
void
USB_Debug_Output_Config (uint32_t DebugCtl)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_DBG_CONFIG(DebugCtl));

    /*---------------- ����USB_UDB�Ĵ���DEBUGλ ----------------*/
    USB_UDB = SFR_Config (USB_UDB, ~USB_UDB_DEBUG, DebugCtl);
}
/**
  *   ##### USBģ��(USB)�������ú���������� #####
  */


/**
  *   ##### USBģ��(USB)���������� #####
  */
/**
  * ����  ��ȡUSB�˵㻺����������RAM��ַ������USB_UCON�Ĵ�����DBλѡ����Ӧ�Ĳ���
  *       �����Ӧ�Ļ�������������ż�������������BufferDescripor��Ч��
  * ����  EndPoint: USB�˵��ţ�ȡֵΪUSB_ENDPOINT_0~USB_ENDPOINT_8��
  *       Direction: USB�˵㷽��ȡֵΪ��
  *                    USB_ENDPOINT_OUTPUT: �˵����
  *                    USB_ENDPOINT_INPUT: �˵�����
  *       BufferDescripor: USB��żBD�洢����ȡֵΪ��
  *                          USB_ODD_BUFFER_DESCRIPOR: ����������
  *                          USB_EVEN_BUFFER_DESCRIPOR: ż��������
  * ����  USB�˵㻺����������32λRAM��ַ���������򷵻�0��
  */
uint32_t
USB_Get_Endpoint_Double_Buffer_Addr (uint32_t EndPoint,
                    uint32_t Direction, uint32_t BufferDescripor)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_DIRECT(Direction));
    CHECK_RESTRICTION(CHECK_USB_ODD_EVEN(BufferDescripor));

    /*------------- ��ȡUSB_UCON�Ĵ���DBλ -------------*/
    tmpreg = USB_UCON;
    tmpreg &= USB_UCON_DB;

    /* �˵㻺�����Ĺ���ģʽƥ�� */
    switch (tmpreg)
    {
        /* ��ֹż��/����˫������ */
        case USB_DOUBLE_BUFFER_FORBID:
        {
            /* �˵�0��7��˫������������ */
            tmpaddr = USB_BUFFER_START_ADDR;
            tmpaddr += EndPoint * 8;
            tmpaddr += Direction * 4;
        }
            break;
        /* ʹ��OUT�˵�0��ż��/����˫������ */
        case USB_OUT_ENDPOINT_0:
        {
            if (EndPoint != USB_ENDPOINT_0)
            {
                /* �˵�1��7��˫������������ */
                tmpaddr = USB_BUFFER_START_ADDR + (4 * 3);
                tmpaddr += (EndPoint - 1) * 8;
                tmpaddr += Direction * 4;
            }
            else
            {
                /* �˵�0��˫������������ */
                if (Direction != USB_ENDPOINT_INPUT)
                {
                    /* �˵�0�����˫������������ */
                    tmpaddr = USB_BUFFER_START_ADDR;
                    tmpaddr += (uint32_t)(!BufferDescripor) * 4;
                }
                else
                {
                    /* �˵�0������˫������������ */
                    tmpaddr = USB_BUFFER_START_ADDR + (4 * 2);
                }
            }
        }
            break;
        /* ʹ�����ж˵��ż��/����˫������ */
        case USB_ALL_ENDPOINTS:
        {
            tmpaddr = USB_BUFFER_START_ADDR;
            tmpaddr += EndPoint * 4 * 4;
            tmpaddr += Direction * 4 * 2;
            tmpaddr += (uint32_t)(!BufferDescripor) * 4;
        }
            break;
        /* ʹ�ܶ˵�1��7��ż��/����˫������ */
        case USB_ENDPOINT_1_TO_7:
        {
            if (EndPoint != USB_ENDPOINT_0)
            {
                /* �˵�1��7��˫������������ */
                tmpaddr = USB_BUFFER_START_ADDR + 4 * 2;
                tmpaddr += (EndPoint - 1) * 4 * 4;
                tmpaddr += Direction * 4 * 2;
                tmpaddr += (uint32_t)(!BufferDescripor) * 4;
            }
            else
            {
                /* �˵�0��˫������������ */
                tmpaddr = USB_BUFFER_START_ADDR;
                tmpaddr += Direction * 4;
            }
        }
            break;
        default:
            CHECK_RESTRICTION(0);
            break;
    }

    return tmpaddr;
}

/**
  * ����  ����USB RAM��������ַ��Ӳ��10λ��Ч�����ֶ��롣
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       BufferAddr: USB RAM��������ַ��ȡֵΪ0~0xFFFF��
  * ����  �ޡ�
  */
void
USB_Buffer_Address_Config (uint32_t SfrAddr, uint16_t BufferAddr)
{
    uint32_t tmpaddr = 0;
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ����USB_BDnSTAT�Ĵ���BADDRλ ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = ((uint32_t)BufferAddr) << USB_BDSTAT_BADDR0_POS;
    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_BADDR,
                            tmpreg);
}

/**
  * ����  ��ȡUSB RAM��������ַ��Ӳ��10λ��Ч�����ֶ��롣
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  * ����  USB RAM��������ַ���ֶ��룬10λ��Ч��ֵ��
  */
uint32_t
USB_Get_Buffer_Address (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ��ȡUSB_BDnSTAT�Ĵ���BADDRλ ----------------*/
    tmpreg = *(volatile uint32_t *) SfrAddr;
    tmpreg &= USB_BDSTAT_BADDR;
    tmpreg >>= USB_BDSTAT_BADDR0_POS;

    return tmpreg;
}

/**
  * ����  ����USB RAM�����������ֽڳ��ȡ�
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       ByteLength: USB RAM�����������ֽڳ��ȣ�ȡֵΪ0~0x3FF��
  * ����  �ޡ�
  */
void
USB_Buffer_Data_Length_Config (uint32_t SfrAddr, uint32_t ByteLength)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(ByteLength));

    /*---------------- ����USB_BDnSTAT�Ĵ���BCλ ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = ((ByteLength >> 8) & USB_BDSTAT_BCH) << USB_BDSTAT_BC8_POS;
    tmpreg |= (ByteLength & 0xFF) << USB_BDSTAT_BC0_POS;
    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_BC,
                            tmpreg);
}

/**
  * ����  ��ȡUSB�������ֽ�����
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  * ����  USB�������ֽ�����10λ��Ч��ֵ��
  */
uint32_t
USB_Get_Buffer_Data_Length (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ��ȡUSB_BDnSTAT�Ĵ���BADDRλ ----------------*/
    tmpaddr = SfrAddr;
    tmpreg =  *(volatile uint32_t *) tmpaddr;
    tmpreg1 = *(volatile uint32_t *) tmpaddr;
    /* ��ȡBCλ�ĸ���λ */
    tmpreg &= USB_BDSTAT_BCH;
    tmpreg >>= USB_BDSTAT_BC8_POS;
    /* ��ȡBCλ�ĵ���λ */
    tmpreg1 &= USB_BDSTAT_BCL;
    tmpreg1 >>= USB_BDSTAT_BC0_POS;

    tmpreg = (tmpreg << (USB_BDSTAT_BC7_POS + 1)) | (tmpreg1);

    return tmpreg;
}

/**
  * ����  ����USB ӵ��λ��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       OwnSelect: USB ӵ��λѡ��ȡֵΪ:
  *                    USB_KERNEL_OWN_BD: ��Ƭ���ں�ӵ��BD ������Ӧ�Ļ�����
  *                    USB_SIE_OWN_BD: SIE ӵ��BD ������Ӧ�Ļ�����
  * ����  �ޡ�
  */
void
USB_Own_Config (uint32_t SfrAddr, uint32_t OwnSelect)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_OWN_BD(OwnSelect));

    /*---------------- ����USB_BDnSTAT�Ĵ���UOWNλ ----------------*/
    tmpaddr = SfrAddr;
    if (OwnSelect != USB_KERNEL_OWN_BD)
    {
        /* SIE ӵ��BD ������Ӧ�Ļ����� */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_UOWN_POS);
    }
    else
    {
        /* ��Ƭ���ں�ӵ��BD ������Ӧ�Ļ����� */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_UOWN_POS);
    }
}

/**
  * ����  ��ȡUSB ӵ��λ��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  * ����  1: SIE ӵ��BD ������Ӧ�Ļ�������
  *       0: ��Ƭ���ں�ӵ��BD ������Ӧ�Ļ�������
  */
FlagStatus
USB_Get_Own_Status (uint32_t SfrAddr)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ��ȡUSB_BDnSTAT�Ĵ���UOWNλ ----------------*/
    tmpaddr = SfrAddr;
    if (USB_BDSTAT_UOWN & (*(volatile uint32_t *) tmpaddr))
    {
        /* SIE ӵ��BD ������Ӧ�Ļ����� */
        return SET;
    }
    else
    {
        /* ��Ƭ���ں�ӵ��BD ������Ӧ�Ļ����� */
        return RESET;
    }
}

/**
  * ����  ����USB���ݷ�תͬ��λ��ֻ�е�Ƭ���ں�ӵ��BD������Ӧ�Ļ�������
  *       ��ʹ�����ݷ�תͬ��ʱ����λ��Ч��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       DataPackage: USB ���ݷ�תͬ��λѡ��ȡֵΪ:
  *                      USB_DATA_0_TOGGLE_SYNC: ����0 ���ݰ�
  *                      USB_DATA_1_TOGGLE_SYNC: ����1 ���ݰ�
  * ����  �ޡ�
  */
void
USB_Data_Toggle_Sync_Config (uint32_t SfrAddr, uint32_t DataPackage)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_TOGGLE_SYNC(DataPackage));

    /*---------------- ����USB_BDnSTAT�Ĵ���DTSλ ----------------*/
    tmpaddr = SfrAddr;
    if (DataPackage != USB_DATA_0_TOGGLE_SYNC)
    {
        /* ����1 ���ݰ� */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTS_POS);
    }
    else
    {
        /* ����0 ���ݰ� */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTS_POS);
    }
}

/**
  * ����  ��ȡUSB���ݷ�תͬ��λ��ֻ�е�Ƭ���ں�ӵ��BD������Ӧ�Ļ�������
  *       ��ʹ�����ݷ�תͬ��ʱ����λ��Ч��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  * ����  1: ����1 ���ݰ���
  *       0: ����0 ���ݰ���
  */
FlagStatus
USB_Get_Data_Toggle_Sync_Status (uint32_t SfrAddr)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ��ȡUSB_BDnSTAT�Ĵ���DTSλ ----------------*/
    tmpaddr = SfrAddr;
    if (USB_BDSTAT_DTS & (*(volatile uint32_t *) tmpaddr))
    {
        /* ����1 ���ݰ� */
        return SET;
    }
    else
    {
        /* ����0 ���ݰ� */
        return RESET;
    }
}

/**
  * ����  ����USB���ݷ�תͬ��ʹ�ܣ�ֻ�е�Ƭ���ں�ӵ��BD������Ӧ�Ļ�����ʱ��
  *       �ù�����Ч��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       NewState: USB���ݷ�תͬ��ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Data_Toggle_Sync_Enable (uint32_t SfrAddr, FunctionalState NewState)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_BDnSTAT�Ĵ���DTSENλ ----------------*/
    tmpaddr = SfrAddr;
    if (NewState != FALSE)
    {
        /* ʹ�����ݷ�תͬ�� */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTSEN_POS);
    }
    else
    {
        /* ��ִ�����ݷ�תͬ�� */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTSEN_POS);
    }
}

/**
  * ����  ����USB������ֹͣʹ�ܣ�ֻ�е�Ƭ���ں�ӵ��BD������Ӧ�Ļ�����ʱ��
  *       �ù�����Ч��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  *       NewState: USB������ֹͣʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Buffer_Stall_Enable (uint32_t SfrAddr, FunctionalState NewState)
{
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));


    /*---------------- ����USB_BDnSTAT�Ĵ���BSTALLλ ----------------*/
    tmpaddr = SfrAddr;
    if (NewState != FALSE)
    {
        /* ʹ�ܻ�����ֹͣ */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_BSTALL_POS);
    }
    else
    {
        /* ��ֹ������ֹͣ */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_BSTALL_POS);
    }
}

/**
  * ����  ��ȡUSB���ݰ���ʶ����ֻ��SIEӵ��BD������Ӧ�Ļ�����ʱ���ù�����Ч��
  * ����  SfrAddr: USB ������������״̬�Ĵ�����ַ��ȡֵΪָ����32λRAM��ַ��
  *                ���ɺ���USB_Get_Endpoint_Double_Buffer_Addr��ȡ��
  * ����  ��һ�δ��ͽ��յ�������PIDֵ��
  */
uint32_t
USB_Get_Last_Receive_PID (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- ��ȡUSB_BDnSTAT�Ĵ���PIDλ ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = *(volatile uint32_t *) tmpaddr;
    tmpreg &= USB_BDSTAT_PID;
    tmpreg >>= USB_BDSTAT_PID0_POS;

    return tmpreg;
}

/**
  * ����  д��һ��USB���������ݣ�д�볤�Ȳ����������֤��WriteAddrΪUSB��������
  *       ַ��WriteAddr��WriteDataAddr���ڴ��ڳ���Length�в����ص���
  * ����  WriteAddr: ��д���RAM��ַ��ȡֵΪָ����32λRAM��ַ��
  *       WriteDataAddr: ��д������ݵ�ַ��ȡֵΪָ����32λRAM��ַ��
  *       Length: ��д������ݳ��ȣ�ȡֵ10λ��Ч��ֵ��
  * ����  �ޡ�
  */
void
USB_Write_Buffer_Data (uint32_t WriteAddr,
                    uint32_t WriteDataAddr, uint32_t Length)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(WriteAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(Length));

    memcpy ((void *)WriteAddr, (void *)WriteDataAddr, Length);
}
/**
  *   ##### USBģ��(USB)����������������� #####
  */


/**
  *   ##### USBģ��(USB)�жϹ����� #####
  */
/**
  * ����  ��ȡUSB�жϱ�־��
  * ����  InterruptType: USB�ж����ͣ�ȡֵ��ΧΪ�����ṩ��һ��:
  *                        USB_INT_ERROR: USB ���������ж�
  *                        USB_INT_RESET: USB ��λ�ж�
  *                        USB_INT_TRANSACTION_COMPLETED: ��������ж�
  *                        USB_INT_BUS_ACTIVE: ���߻����ж�
  *                        USB_INT_IDLE: ���м���ж�
  *                        USB_INT_STALL_HANDSHAKE: STALL �����ж�
  *                        USB_INT_SOF_TAKEN: ֡��ʼ�����ж�
  *                        USB_INT_DATE_FIELD_NOT_BYTE: �����ֶδ�С�����ж�
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 ���������ж�
  *                        USB_INT_CRC16_FAILURE: CRC16 ʧ���ж�
  *                        USB_INT_PID_CHECK_FAILURE: PID ���ʧ���ж�
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: ������ת��ʱ�����ж�
  *                        USB_INT_BIT_STUFF_ERROR: λ�������ж�
  * ����  1:�����жϣ�0:δ�����жϡ�
  */
FlagStatus
USB_Get_INT_Flag (uint32_t InterruptType)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_INT_ONE_EVENT(InterruptType));

    /*---------------- ��ȡUSB_UIR�Ĵ����жϱ�־λ ----------------*/
    if (USB_UIR & InterruptType)
    {
        /* �����ж� */
        return SET;
    }
    else
    {
        /* δ�����ж� */
        return RESET;
    }
}

/**
  * ����  ����USB�жϱ�־��
  * ����  InterruptType: USB�ж����ͣ�ȡֵ��ΧΪ�����ṩ��һ���������:
  *                        USB_INT_ERROR: USB ���������ж�
  *                        USB_INT_RESET: USB ��λ�ж�
  *                        USB_INT_TRANSACTION_COMPLETED: ��������ж�
  *                        USB_INT_BUS_ACTIVE: ���߻����ж�
  *                        USB_INT_IDLE: ���м���ж�
  *                        USB_INT_STALL_HANDSHAKE: STALL �����ж�
  *                        USB_INT_SOF_TAKEN: ֡��ʼ�����ж�
  *                        USB_INT_DATE_FIELD_NOT_BYTE: �����ֶδ�С�����ж�
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 ���������ж�
  *                        USB_INT_CRC16_FAILURE: CRC16 ʧ���ж�
  *                        USB_INT_PID_CHECK_FAILURE: PID ���ʧ���ж�
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: ������ת��ʱ�����ж�
  *                        USB_INT_BIT_STUFF_ERROR: λ�������ж�
  * ����  �ޡ�
  */
void
USB_Clear_INT_Flag (uint32_t InterruptType)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_INT_EVENT(InterruptType));

    /*---------------- ����USB_UIR�Ĵ����жϱ�־λ ----------------*/
    while(USB_UIR & InterruptType)
    {
    	USB_UIR &= ~InterruptType;
    }
}

/**
  * ����  ����USB�ж�ʹ�ܡ�
  * ����  InterruptType: USB�ж����ͣ�ȡֵ��ΧΪ�����ṩ��һ���������:
  *                        USB_INT_ERROR: USB ���������ж�
  *                        USB_INT_RESET: USB ��λ�ж�
  *                        USB_INT_TRANSACTION_COMPLETED: ��������ж�
  *                        USB_INT_BUS_ACTIVE: ���߻����ж�
  *                        USB_INT_IDLE: ���м���ж�
  *                        USB_INT_STALL_HANDSHAKE: STALL �����ж�
  *                        USB_INT_SOF_TAKEN: ֡��ʼ�����ж�
  *                        USB_INT_DATE_FIELD_NOT_BYTE: �����ֶδ�С�����ж�
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 ���������ж�
  *                        USB_INT_CRC16_FAILURE: CRC16 ʧ���ж�
  *                        USB_INT_PID_CHECK_FAILURE: PID ���ʧ���ж�
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: ������ת��ʱ�����ж�
  *                        USB_INT_BIT_STUFF_ERROR: λ�������ж�
  *       NewState: USB�ж�ʹ��״̬��ȡֵΪTRUE �� FALSE��
  * ����  �ޡ�
  */
void
USB_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- ����USB_UIE�Ĵ����ж�ʹ��λ ----------------*/
    if (NewState != FALSE)
    {
        /* ʹ���ж� */
        USB_UIE |= InterruptType;
    }
    else
    {
        /* ��ֹ�ж� */
        USB_UIE &= ~InterruptType;
    }
}

/**
  * ����  ��ȡUSB�ж�ʹ��״̬��
  * ����  InterruptType: USB�ж����ͣ�ȡֵ��ΧΪ�����ṩ��һ��:
  *                        USB_INT_ERROR: USB ���������ж�
  *                        USB_INT_RESET: USB ��λ�ж�
  *                        USB_INT_TRANSACTION_COMPLETED: ��������ж�
  *                        USB_INT_BUS_ACTIVE: ���߻����ж�
  *                        USB_INT_IDLE: ���м���ж�
  *                        USB_INT_STALL_HANDSHAKE: STALL �����ж�
  *                        USB_INT_SOF_TAKEN: ֡��ʼ�����ж�
  *                        USB_INT_DATE_FIELD_NOT_BYTE: �����ֶδ�С�����ж�
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 ���������ж�
  *                        USB_INT_CRC16_FAILURE: CRC16 ʧ���ж�
  *                        USB_INT_PID_CHECK_FAILURE: PID ���ʧ���ж�
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: ������ת��ʱ�����ж�
  *                        USB_INT_BIT_STUFF_ERROR: λ�������ж�
  * ����  1:�ж���ʹ�ܣ�0:�ж�δʹ�ܡ�
  */
FlagStatus
USB_Get_INT_Enable (uint32_t InterruptType)
{
    /* ����У�� */
    CHECK_RESTRICTION(CHECK_USB_INT_ONE_EVENT(InterruptType));

    /*---------------- ��ȡUSB_UIE�Ĵ����жϱ�־λ ----------------*/
    if (USB_UIE & InterruptType)
    {
        /* �ж���ʹ�� */
        return SET;
    }
    else
    {
        /* �ж�δʹ�� */
        return RESET;
    }
}
/**
  *   ##### USBģ��(USB)�жϹ������������ #####
  */
#endif  //KF32A_Periph_usb

