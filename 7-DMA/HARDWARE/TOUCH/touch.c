#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"	 
  
Pen_Holder Pen_Point;//定义笔实体
//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

//SPI写数据
//向7843写入1byte数据   
void ADS_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0;
		TCLK=1;//上升沿有效
	} 			    
} 		 
//SPI读数据 
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据	   
u16 ADS_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;//先拉低时钟 	 
	TCS=0; //选中触摸屏IC
	ADS_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	TCLK=1;//给1个时钟，清除BUSY    
	TCLK=0; 	 
	for(count=0;count<15;count++)  
	{ 				  
		Num<<=1; 	 
		TCLK=1;			    
		TCLK=0; //下降沿有效  	    	
		if(DOUT)Num++; 		 
	}  	
	Num>>=3;   //只有高12位有效.
	TCS=1;	   //释放片选	 
	return(Num); 
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 1 //读取次数
#define LOST_VAL 0	 //丢弃值
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}	
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	// u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);
    *x=x1;
    *y=y1;
    if(flag==0)return(0);
    return 1;
    /*
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
    */
} 
//读取一次坐标值	
//仅仅读取一次,直到PEN松开才返回!				   
u8 Read_TP_Once(void)
{
	u8 t=0;	    
	Pen_Int_Set(0);//关闭中断
	Pen_Point.Key_Sta=Key_Up;
	Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		//delay_ms(10);
		delay_ms(1);
	};
	Pen_Int_Set(1);//开启中断		 	 
	if(t>=250)return 0;//按下2.5s 认为无效
	else return 1;	
}

//////////////////////////////////////////////////
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
void Drow_Touch_Point(u8 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);//横线
	LCD_DrawLine(x,y-12,x,y+13);//竖线
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);//画中心圈
}	  
//画一个大点(2*2的点)		   
//x,y:坐标	   
void Draw_Big_Point(u8 x,u16 y)
{	    
	LCD_DrawPoint(x,y);//中心点 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}
//////////////////////////////////////////////////

//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
void Convert_Pos(void)
{		 	  
	if(Read_ADS2(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}	   
//中断,检测到PEN脚的一个下降沿.
//置位Pen_Point.Key_Sta为按下状态
//中断线上的中断检测
void EXTI15_10_IRQHandler(void)
{ 		   			 
	Pen_Point.Key_Sta=Key_Down;//按键按下  		 			 
	EXTI->PR=1<<11;  //清除LINE11上的中断标志位 
} 
//PEN中断设置	 
void Pen_Int_Set(u8 en)
{
	if(en)EXTI->IMR|=1<<11;   //开启line11上的中断	  		  	
	else EXTI->IMR&=~(1<<11); //关闭line11上的中断	   
}	  
//显示校准值
void ADJ_INFO_SHOW(char *str)
{
	LCD_ShowString(40,40,"x1:       y1:       ");
	LCD_ShowString(40,60,"x2:       y2:       ");
	LCD_ShowString(40,80,"x3:       y3:       ");
	LCD_ShowString(40,100,"x4:       y4:       ");
 	LCD_ShowString(40,100,"x4:       y4:       ");
 	LCD_ShowString(40,120,str);					   
}
	 
//触摸屏校准代码
//得到四个校准参数
void Touch_Adjust(void)
{								 
	signed short pos_temp[4][2];//坐标缓存值
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 	   
	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	//LCD_Clear(WHITE);//清屏   
	POINT_COLOR=RED;//红色 
	LCD_Clear(WHITE);//清屏 
	Drow_Touch_Point(20,20);//画点1 
	Pen_Point.Key_Sta=Key_Up;//消除触发信号 
	Pen_Point.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 	 
	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)//按键按下了
		{
			if(Read_TP_Once())//得到单次按键值
			{  								   
				pos_temp[cnt][0]=Pen_Point.X;
				pos_temp[cnt][1]=Pen_Point.Y;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(220,20);//画点2
					break;
				case 2:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(20,300);//画点3
					break;
				case 3:
					LCD_Clear(WHITE);//清屏 
					Drow_Touch_Point(220,300);//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.92||fac>1.07||d1==0||d2==0)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("ver fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3,16);//显示数值,该数值必须在95~105范围之内.
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.92||fac>1.07)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("hor fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3,16);//显示数值,该数值必须在95~105范围之内.
 						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.92||fac>1.07)//不合格
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("dia fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//显示数值
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//显示数值
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//显示数值
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//显示数值
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//显示数值
						//扩大100倍显示
						LCD_ShowNum(40,140,fac*100,3,16);//显示数值,该数值必须在95~105范围之内.
						continue;
					}//正确了
					//计算结果
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  

					if(abs(Pen_Point.xfac)>2||abs(Pen_Point.yfac)>2)//触屏和预设的相反了.
					{
						cnt=0;
						LCD_Clear(WHITE);//清屏 
						Drow_Touch_Point(20,20);
						LCD_ShowString(35,110,"TP Need readjust!");
						Pen_Point.touchtype=!Pen_Point.touchtype;//修改触屏类型.
						if(Pen_Point.touchtype)//X,Y方向与屏幕相反
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y方向与屏幕相同
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}
						//delay_ms(500);
						delay_ms(5);
						continue;
					}
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//清屏
					LCD_ShowString(35,110,"Touch Screen Adjust OK!");//校正完成
					//delay_ms(500);
					delay_ms(5);
					LCD_Clear(WHITE);//清屏   
					return;//校正完成				 
			}
		}
	} 
}		    
//外部中断初始化函数
void Touch_Init(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;  //中断
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	EXTI_InitTypeDef EXTI_InitStructure;	//外部中断线		    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_5|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	   
 	Pen_Point.X = 3736;
 	Pen_Point.Y = 3851;
 	Read_ADS(&Pen_Point.X,&Pen_Point.Y);//第一次读取初始化	
			 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); 
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;	//外部线路EXIT1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //外部中断触发沿选择:设置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	Pen_Point.xfac = 0.058617;
    Pen_Point.yfac = 0.077864;
    Pen_Point.xoff = 0;
    Pen_Point.yoff = 0; 
 
}

