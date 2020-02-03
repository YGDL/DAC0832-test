#include<stc15f2k60s2.h>

/*
ILE 输入锁存允许信号,高电平有效
CS 片选信号,低电平有效
WR1 写信号1,低电平有效

当ILE、CS、WR1同时有效时,LE1=1,输入寄存器的输出随输入而变化
ILE=0,将输入数据锁存到输入寄存器

XFER 转移控制信号，低电平有效
WR2 写信号2,低电平有效

当XFER、WR2同时有效时,LE2=1,DAC寄存器输出随输入而变化
WR2=1,将输入数据锁存到DAC寄存器
*/

sbit WR1=P1^0;										//写脉冲引脚1
sbit CS1=P1^1;										//低位片选引脚
sbit ILE=P1^2;										//一直拉高
sbit WR2=P1^3;										//写脉冲引脚2
sbit XFER=P1^4;										//DAC更新引脚，锁存引脚
sfr DAC_bus=0x80;									//DAC数据总线



void delay_us(int us)					//约微秒延时(us)
{
	int i;
	for(;us--;us>0)
		for(i=12;i--;i>0);
}

void write_DAC_input_register(long int data_)		//DAC输入寄存器写数据，并锁存
{
	//ILE不用理会，一直为高电平即可
	//在这个函数中，需要完成两个DAC的数据写入
	DAC_bus=data_;							//低8位数据写
	CS1=0;											//选中低位DAC
	WR1=0;											//低位DAC写脉冲
	WR1=1;											//输入寄存器锁存
	CS1=1;											//取消低位片选

}

void DAC_analog_output()			//DAC锁存器数据更新
{
	XFER=0;											//预更新信号
	WR2=0;											//更新写脉冲
	WR2=1;											//锁存数据
	XFER=1;											//全部复位
}

void main()
{
	int i;
	WR1=WR2=CS1=XFER=0;					//处于直通状态
	while(1)
	{
		for(i=255;i--;i>=0)
		{
			DAC_bus=i;							//直接赋值
		}
	}
}