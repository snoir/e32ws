#include <stdio.h>
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_FREQ_HZ 100000
#define BME280_ADDRESS	0x77

esp_err_t
i2c_init(void)
{
	printf("Initializing I2C bus…\n");

	int i2c_master_port = 0;
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_MASTER_SDA_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ,
	};
	esp_err_t err = i2c_param_config(i2c_master_port, &conf);
	if (err != ESP_OK)
		return err;

	err = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

	return err;
}

esp_err_t
i2c_read(void)
{
	printf("Starting I2C communication…\n");

	i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
	esp_err_t err = i2c_master_start(cmd_handle);
	if (err != ESP_OK)
		return err;

	err = i2c_master_write_byte(cmd_handle,
		  (BME280_ADDRESS << 1) | I2C_MASTER_WRITE, true);

	uint8_t data[512];
	printf("Reading data…\n");
	err = i2c_master_read(cmd_handle, data, 1, true);
	for (int i=0; i<512; i++)
		printf("b: %d\n", data[i]);

	return err;
}

void
app_main(void)
{
	esp_err_t err = i2c_init();
	if (err != ESP_OK)
		fprintf(stderr, "I2C initialization failed!\n");
	ESP_ERROR_CHECK_WITHOUT_ABORT(err);

	err = i2c_read();
	ESP_ERROR_CHECK_WITHOUT_ABORT(err);
}
