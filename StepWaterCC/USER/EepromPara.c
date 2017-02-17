
//需要设置保存的参数
struct	struct_save
{
		uint16	temp;
		
		
		uchar bCurrentRate1;			//工作电流1系数(uint=1A)
		uchar bCurrentRate2;			//工作电流2系数(uint=1A)
		uchar RemotName[4+2];
		
		uchar flag_sub;			//对码成副机标记
		
		uchar flag;				//参数已经初始化标记
};



extern struct	struct_save *gpParam;
#define Max_Param_Len		(12+2)
//参数已经初始化标记
#define FlagParamInitnized	0xA6

extern uchar gbParamBuf[Max_Param_Len+2];
//参数的EEPROM区首地址
#define EEPROM_BASE_ADR	0X10




void Default_ParamInit(void)
{	//gpParam参数结构的指针
uchar i;

gpParam=(struct	struct_save	*)&gbParamBuf[0];		//参数结构的指针


	//gpParam->bCurrentRate1=101;			//电机1的
	//gpParam->bCurrentRate2=101;			//电机1的
	//gpParam->bCurrentRate1=188;			//电机1的
	//gpParam->bCurrentRate2=188;			//电机1的
	gpParam->bCurrentRate1=165;			//电机1的
	gpParam->bCurrentRate2=165;			//电机2的	
for(i=0;i<4;i++)
		{
		gpParam->RemotName[i]='0';
		}
	gpParam->RemotName[4]=0x0;
	
	gpParam->flag_sub=0;		//对码成副机标记
	gpParam->flag=FlagParamInitnized;		//参数已经初始化标记
	
	
	for(i=0;i<Max_Param_Len;i++)
										{
										EEPROM_Write(EEPROM_BASE_ADR+i,gbParamBuf[i]);
										}
	
	
}