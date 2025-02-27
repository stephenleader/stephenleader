//注意看DS18B20温度转换和读取流程.png

#include "reg52.h"
#include "absacc.h"
#include "onewire.h"
#include "xmf_smg_ca.h"  //自己定义的头文件，c文件加载到该根目录下

//设置温度变量
unsigned int temp = 0;

//建整形数据
//567
//  567 / 100 = 5 = S
//  567 % 100 = 67 / 10 = 6 
//  567 % 10 = 7
void DisplaySMG_Temp()
{
	DisplaySMG_Bit(7,SMGNoDot_CA[temp % 10]);	 //个位
	DelaySMG(100);
	DisplaySMG_Bit(6,SMGDot_CA[(temp % 100) / 10]);	 //十位
	DelaySMG(100);
	DisplaySMG_Bit(5,SMGNoDot_CA[temp / 100]);	  //百位
	DelaySMG(100);
	
	
	DisplaySMG_Bit(4,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(3,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(2,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(1,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(0,0xff);	
	DelaySMG(100);
	
	Display_All(0xff);

}

//数码管动态演示的延时
void Delay(unsigned int t)
{
	while(t--)
	{
		DisplaySMG_Temp();
	}
}

//onewire。h声明的函数要调用
//读onwire.h
//查看根目录下的png
//注意转换的步骤
void Read_DS18B20_temp()
{
	unsigned char LSB,MSB;//low 8 bit ,high 8 bit
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	Delay(1000);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB;
	temp = (temp << 8) | LSB;  //16位温度数据
	
	if((temp & 0xf800) == 0x0000)  //0上工作
	{
		temp >>= 4;//小数移走
		temp = temp * 10;
		temp = temp + (LSB & 0x0f) * 0.625;//放大十倍，分辨率是0！�0625
	}
}

void main()
{
	XBYTE[8000] = 0x00;//灭灯
	while(1)
	{
		Read_DS18B20_temp();
		DisplaySMG_Temp();
	}
}