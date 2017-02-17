
//��Ҫ���ñ���Ĳ���
struct	struct_save
{
		uint16	temp;
		
		
		uchar bCurrentRate1;			//��������1ϵ��(uint=1A)
		uchar bCurrentRate2;			//��������2ϵ��(uint=1A)
		uchar RemotName[4+2];
		
		uchar flag_sub;			//����ɸ������
		
		uchar flag;				//�����Ѿ���ʼ�����
};



extern struct	struct_save *gpParam;
#define Max_Param_Len		(12+2)
//�����Ѿ���ʼ�����
#define FlagParamInitnized	0xA6

extern uchar gbParamBuf[Max_Param_Len+2];
//������EEPROM���׵�ַ
#define EEPROM_BASE_ADR	0X10




void Default_ParamInit(void)
{	//gpParam�����ṹ��ָ��
uchar i;

gpParam=(struct	struct_save	*)&gbParamBuf[0];		//�����ṹ��ָ��


	//gpParam->bCurrentRate1=101;			//���1��
	//gpParam->bCurrentRate2=101;			//���1��
	//gpParam->bCurrentRate1=188;			//���1��
	//gpParam->bCurrentRate2=188;			//���1��
	gpParam->bCurrentRate1=165;			//���1��
	gpParam->bCurrentRate2=165;			//���2��	
for(i=0;i<4;i++)
		{
		gpParam->RemotName[i]='0';
		}
	gpParam->RemotName[4]=0x0;
	
	gpParam->flag_sub=0;		//����ɸ������
	gpParam->flag=FlagParamInitnized;		//�����Ѿ���ʼ�����
	
	
	for(i=0;i<Max_Param_Len;i++)
										{
										EEPROM_Write(EEPROM_BASE_ADR+i,gbParamBuf[i]);
										}
	
	
}