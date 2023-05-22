#include "MagI2C_driver.h"


#define MAGI2C_ADDR (0x21)<<1 //SA0=1 , SA1=0 ,0b0100001
#define POLL 		0x00
#define CMM_WR  	0x01
#define CMM_RD  	0x81
#define CCX_WR  	0x04
#define CCY_WR  	0x06
#define CCZ_WR  	0x08
#define CCX_RD  	0x84
#define CCY_RD  	0x86
#define CCZ_RD  	0x88
#define TMRC_WR 	0x0B
#define TMRC_RD 	0x8B
#define MX_RD   	0x24 //0xA4 ??
#define MY_RD   	0x27 //0xA7 ??
#define MZ_RD   	0x2A //0xAA ??
#define STATUS_RD 	0xB4
#define BIST_WR 	0x33
#define BIST_RD 	0xB3
#define HSHAKE_WR 	0x35
#define HSHAKE_RD 	0xB5
#define REVID_RD 	0xB6

#define ENABLE_I2C_MODE()  HAL_GPIO_WritePin(I2CEN_SPIEN_GPIO_Port,I2CEN_SPIEN_Pin,GPIO_PIN_SET)
#define CONFIG_SA0_BIT()   HAL_GPIO_WritePin(SA0_SSN_GPIO_Port,SA0_SSN_Pin,GPIO_PIN_SET)
#define CONFIG_SA1_BIT()   HAL_GPIO_WritePin(SA1_GPIO_Port,SA1_Pin,GPIO_PIN_RESET)
#define MAGI2C_STATUS()	   HAL_GPIO_ReadPin(DRDY_GPIO_Port, DRDY_Pin)

void  MagI2C_config_I2C(I2C_HandleTypeDef *hi2c){
	CMM_register CMM_config[1] = {[0].cmm_en=1,
									[0].drdm=0,
									 [0].cmx=1,
									 [0].cmy=1,
									 [0].cmz=1};
	ENABLE_I2C_MODE();
	CONFIG_SA0_BIT();
	CONFIG_SA1_BIT();
	//By default Cycle Count Registers are 0xC8 in the LSB and 0x00 in the MSB (200D)
	//By default TMRC = 0x96 = 37Hz
	HAL_I2C_Mem_Write(hi2c, MAGI2C_ADDR, CMM_WR,1, (uint8_t*)CMM_config, 1, 1000);
}

mag_field  MagI2C_get_meas_I2C(void)
{
	uint8_t meas_raw_data[9]={0};
	mag_field meas_field = {0};
	if(MAGI2C_STATUS()==1){
		HAL_I2C_Mem_Read(&hi2c1, MAGI2C_ADDR, MX_RD,1, meas_raw_data, 9, 1000);
		meas_field.mag_field_x = (meas_raw_data[2] | meas_raw_data[1]<<8 | ((int8_t)meas_raw_data[0])<<16);
		meas_field.mag_field_y = (meas_raw_data[5] | meas_raw_data[4]<<8 | ((int8_t)meas_raw_data[3])<<16);
		meas_field.mag_field_z = (meas_raw_data[8] | meas_raw_data[7]<<8 | ((int8_t)meas_raw_data[6])<<16);
	}
	return meas_field;
}
