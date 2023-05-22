#ifndef INC_MAGI2C_DRIVER_H_
#define INC_MAGI2C_DRIVER_H_
#include "i2c.h"
#include "spi.h"
#include "main.h"

typedef struct{
	int mag_field_x;
	int mag_field_y;
	int mag_field_z;
}mag_field;

typedef struct{
	uint8_t cmm_en:1,
			  res1:1,
			  drdm:1,
			  res3:1,
			   cmx:1,
			   cmy:1,
			   cmz:1,
			  res7:1;
}CMM_register;

typedef struct{
	uint8_t tmrc0:1,
			tmrc1:1,
			tmrc2:1,
			tmrc3:1;
}TMRC_register;

typedef struct{
    uint16_t x;
	uint16_t y;
	uint16_t z;
}CC_register;

void  MagI2C_config_I2C(I2C_HandleTypeDef *hi2c);
mag_field  MagI2C_get_meas_I2C(void);
#endif /* INC_MAGI2C_DRIVER_H_ */
