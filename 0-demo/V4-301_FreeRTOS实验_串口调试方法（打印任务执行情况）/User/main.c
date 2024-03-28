/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����ҪѧϰFreeRTOS�Ĵ��ڵ��Է�������ӡ����ִ�������
*              ʵ��Ŀ�ģ�
*                1. ѧϰFreeRTOS�Ĵ��ڵ��Է�������ӡ����ִ���������
*                2. Ϊ�˻�ȡFreeRTOS�����ִ���������Ҫִ��������������
*					a. ��FreeRTOSConfig.h�ļ���ʹ�����º�
*                	    #define configUSE_TRACE_FACILITY	                1
*                		#define configGENERATE_RUN_TIME_STATS               1
*                		#define configUSE_STATS_FORMATTING_FUNCTIONS        1
*                		#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    (ulHighFrequencyTimerTicks = 0ul)
*                		#define portGET_RUN_TIME_COUNTER_VALUE()            ulHighFrequencyTimerTicks
*					b. ����һ����ʱ���������ʱ����ʱ���׼����Ҫ����ϵͳʱ�ӽ��ģ������õ���������Ϣ��׼ȷ��
*                   c. ���ú���vTaskList��vTaskGetRunTimeStats���ɻ�ȡ���������ִ�������
*              ʵ�����ݣ�
*                1. ���°���K1����ͨ�����ڴ�ӡ����ִ�������������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                   =================================================
*                   ������      ����״̬ ���ȼ�   ʣ��ջ �������
*                   vTaskUserIF     R       1       318     1
*                	IDLE            R       0       118     5
*                	vTaskLED        B       2       490     2
*                	vTaskMsgPro     B       3       490     3
*               	vTaskStart      B       4       490     4
*
*                	������       ���м���         ʹ����
*                	vTaskUserIF     467             <1%
*                	IDLE            126495          99%
*                	vTaskMsgPro     1               <1%
*                	vTaskStart      639             <1%
*                	vTaskLED        0               <1%
*                  �����������ʹ��SecureCRT��V4���������д�������鿴��ӡ��Ϣ��
*                  ��������ʵ�ֵĹ������£�
*                   vTaskTaskUserIF ����: �ӿ���Ϣ����	
*                   vTaskLED        ����: LED��˸
*                   vTaskMsgPro     ����: ��Ϣ��������������LED��˸
*                   vTaskStart      ����: ��������Ҳ����������ȼ���������ʵ�ְ���ɨ��
*                2. ��������״̬�Ķ������£������洮�ڴ�ӡ��ĸB, R, D, S��Ӧ��
*                    #define tskBLOCKED_CHAR		( 'B' )  ����
*                    #define tskREADY_CHAR		    ( 'R' )  ����
*                    #define tskDELETED_CHAR		( 'D' )  ɾ��
*                    #define tskSUSPENDED_CHAR	    ( 'S' )  ����
*              ע�����
*                 1. ��ʵ���Ƽ�ʹ�ô������SecureCRT��Ҫ�����ڴ�ӡЧ�������롣�������
*                    V4��������������С�
*                 2. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��    ����         ����            ˵��
*       V1.0    2016-03-15   Eric2013    1. ST�̼��⵽V3.6.1�汾
*                                        2. BSP������V1.2
*                                        3. FreeRTOS�汾V8.2.3
*
*	Copyright (C), 2016-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"



/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
static void TaskKeyS(void *pvParameters);
/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static TaskHandle_t xHandleTaskKeyS = NULL; //���ݴ��ڷ����������ݾ�����Ӧ����Ϊ���߳�

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
	/* 
	  ����������ǰ��Ϊ�˷�ֹ��ʼ��STM32����ʱ���жϷ������ִ�У������ֹȫ���ж�(����NMI��HardFault)��
	  �������ĺô��ǣ�
	  1. ��ִֹ�е��жϷ����������FreeRTOS��API������
	  2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
	  3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
	  ����ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�ͨ��ָ��cpsie i������__set_PRIMASK(1)
	  ��cpsie i�ǵ�Ч�ġ�
     */
	//__set_PRIMASK(1);  
	
	/* Ӳ����ʼ�� */
	bsp_Init(); 
	
	/* 1. ��ʼ��һ����ʱ���жϣ����ȸ��ڵδ�ʱ���жϣ������ſ��Ի��׼ȷ��ϵͳ��Ϣ ��������Ŀ�ģ�ʵ����
		  Ŀ�в�Ҫʹ�ã���Ϊ������ܱȽ�Ӱ��ϵͳʵʱ�ԡ�
	   2. Ϊ����ȷ��ȡFreeRTOS�ĵ�����Ϣ�����Կ��ǽ�����Ĺر��ж�ָ��__set_PRIMASK(1); ע�͵��� 
	*/
	vSetupSysInfoTest();
	
	/* �������� */
	AppTaskCreate();
	
    /* �������ȣ���ʼִ������ */
    vTaskStartScheduler();

	/* 
	  ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
	  heap�ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ�FreeRTOSConfig.h�ļ��ж����heap��С��
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 17 * 1024 ) )
	*/
	while(1);
}

/*
*********************************************************************************************************
*	�� �� ��: vPrintTaskInfo
*	����˵��: ��ӡ������Ϣ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void vPrintTaskInfo()
{
	uint8_t pcWriteBuffer1[512];
	printf("=================================================\r\n");
	printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");
	vTaskList((char *)&pcWriteBuffer1);
	printf("%s\r\n", pcWriteBuffer1);

	printf("\r\n������\t ���м���\t ʹ����\r\n");
		
	TaskStatus_t xTaskStatusArray[10];
	UBaseType_t uxArraySize = sizeof(xTaskStatusArray) / sizeof(TaskStatus_t);
	uint32_t ulTotalRunTime = 0;
	
	UBaseType_t uxTaskCount = uxTaskGetSystemState(xTaskStatusArray,uxArraySize,&ulTotalRunTime);
	
	if(uxTaskCount == 0)
	{
		printf("uxTaskGetSystemState ʧ��\n");
		return ;
	}
	
	for(UBaseType_t xIndex=0;xIndex<uxTaskCount;xIndex++)
	{
		printf("%s \t ", xTaskStatusArray[xIndex].pcTaskName);  
		printf("%d \t ", xTaskStatusArray[xIndex].eCurrentState);  
		printf("%d \r\n", xTaskStatusArray[xIndex].uxCurrentPriority);  
	}
	
	//printf("Total Run Time: %lu\n", ulTotalRunTime);  
	printf("=================================================\r\n");

}

/*
*********************************************************************************************************
*	�� �� ��: vTaskTaskUserIF
*	����˵��: �ӿ���Ϣ����
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 1  (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters)
{
    while(1)
    {
		vPrintTaskInfo();
		vTaskDelay(2000);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskLED
*	����˵��: LED��˸	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    while(1)
    {
		bsp_LedToggle(2);
		printf("\r\n LED \r\n");
        vTaskDelay(200);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskMsgPro
*	����˵��: ��Ϣ��������������LED��˸	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    while(1)
    {
		bsp_LedToggle(3);
        vTaskDelay(300);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskStart
*	����˵��: ��������Ҳ����������ȼ�����������������ɨ�衣
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    while(1)
    {
		/* ����ɨ�� */
		bsp_KeyScan();
        vTaskDelay(10);
    }
}


/*
*********************************************************************************************************
*	�� �� ��: TaskKeyS
*	����˵��: ���ݴ��ڷ����������ݾ�����Ӧ����Ϊ���߳�
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 4  
*********************************************************************************************************
*/

static void TaskKeyS(void *pvParameters)
{
    while(1)
    {
		if(PRINTINFO == 1)
		{
			vPrintTaskInfo();
			PRINTINFO = 0;
		}
		
		if(DELTASK == 1)
		{
			printf("DELTASK \r\n");
			if(xHandleTaskLED != NULL)
			{
				vTaskDelete(xHandleTaskLED);
				xHandleTaskLED = NULL;
			}

			DELTASK = 0;
		}		
		
		if(CREATETASK == 1)
		{
			printf("CREATETASK \r\n");
			if(xHandleTaskLED != NULL)
			{
				printf("�Ѵ�����LED\r\n");
			}else
			{
				xTaskCreate( vTaskLED,    		/* ������  */
                 "vTaskLED",  		/* ������    */
                 512,         		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 2,           		/* �������ȼ�*/
                 &xHandleTaskLED ); /* ������  */
			
			}

			CREATETASK = 0;
		}		
		
		if(STASK == 1)
		{
			printf("�������� \r\n");
			vTaskSuspend(xHandleTaskLED);
			STASK = 0;
		}		
		
		if(RETASK == 1)
		{
			printf("�ָ����� \r\n");
			vTaskResume(xHandleTaskLED);
			RETASK = 0;
		}		
				
		vTaskDelay(10);
				
    }
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
//    xTaskCreate( vTaskTaskUserIF,   	/* ������  */
//                 "vTaskUserIF",     	/* ������    */
//                 512,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
//                 NULL,              	/* �������  */
//                 1,                 	/* �������ȼ�*/
//                 &xHandleTaskUserIF );  /* ������  */
	
	
//	xTaskCreate( vTaskLED,    		/* ������  */
//                 "vTaskLED",  		/* ������    */
//                 512,         		/* ����ջ��С����λword��Ҳ����4�ֽ� */
//                 NULL,        		/* �������  */
//                 2,           		/* �������ȼ�*/
//                 &xHandleTaskLED ); /* ������  */
	
	xTaskCreate( vTaskMsgPro,     		/* ������  */
                 "vTaskMsgPro",   		/* ������    */
                 512,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 3,               		/* �������ȼ�*/
                 &xHandleTaskMsgPro );  /* ������  */
	
	
	xTaskCreate( vTaskStart,     		/* ������  */
                 "vTaskStart",   		/* ������    */
                 512,            		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 1,              		/* �������ȼ�*/
                 &xHandleTaskStart );   /* ������  */
				 
	xTaskCreate( TaskKeyS,     		/* ������  */
                 "TaskKeyS",   		/* ������    */
                 512,            		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 4,              		/* �������ȼ�*/
                 &xHandleTaskKeyS );   /* ������  */
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
