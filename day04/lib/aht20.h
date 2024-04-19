#include <i2c.h>

typedef struct
{
	uint16_t humidity;
	uint8_t humidity_temp;
	uint16_t temp;
} aht20_data;

void aht20_init(void);
aht20_data aht20_mesure(void);
