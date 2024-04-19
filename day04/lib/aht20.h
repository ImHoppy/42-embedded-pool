#include <i2c.h>

typedef struct
{
	uint64_t humidity;
	uint64_t temp;
} aht20_data;

void aht20_init(void);
aht20_data aht20_mesure(void);

void print_temp(uint64_t temp);
void print_humi(uint64_t humi);
