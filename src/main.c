/*
 * main.c
 *
 * Main function
 *
 * Created on: September 9, 2019
 *  Author: Victor Kronberg
 *
 */

#include "main.h"

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef	EnergyMode3
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM4;
#elif	EnergyMode2
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM3;
#elif	EnergyMode1
const SLEEP_EnergyMode_t blocked_sleep_mode = sleepEM2;
#endif

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


int main(void)
{

	__disable_irq();

	/* Event pointer for handling events */
	struct gecko_cmd_packet* evt;
	//int8_t rslt = BMP3_OK;

	uint16_t setting_sel;
	uint8_t sensor_comp;
	uint32_t msb,lsb,combined;
	//struct bmp3_dev bmp_device;
	//struct bmp3_data data;

	//bmp_device.dev_id = ICM_DEV_ID;
	//bmp_device.read = &spi_read;
	//bmp_device.write = &spi_write;
	//bmp_device.delay_ms = &delay_ms;
	//bmp_device.intf = BMP3_SPI_INTF;

	struct bme280_dev dev;
	struct bme280_data comp_data;
	struct imu_dev imu_dev;
	eIMU_ERRORS rslt = 0;
	uint8_t sensors;

	/* Sensor_0 interface over SPI with native chip select line */
	dev.dev_id = ICM_DEV_ID;
	dev.intf = BME280_SPI_INTF;
	dev.read = &spi_read;
	dev.write = &spi_write;
	dev.delay_ms = &delay_ms;

	imu_dev.dev_id = ICM_DEV_ID;
	imu_dev.read = &spi_read;
	imu_dev.write = &spi_write;
	imu_dev.delay_ms = &delay_ms;
	sensors = ACCELEROMETER | GYROSCOPE | THERMOMETER;






	uint8_t reg_data[RX_BUFFER_SIZE] = {0};

  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Initialize logging
  logInit();

  // Initialize event flag
  //my_state_struct.event_bitmask = 0;

  // Initialize GPIO
  //gpioInit();

  // Initialize timer
  init_letimer();

  initUSART1();

  //init_si7021();

  //i2c_init();

  //displayInit();

#ifdef BUILD_INCLUDES_BLE_SERVER
  my_state_struct.current_state = STATE0_WAIT_FOR_BLE;
  my_state_struct.next_state = STATE0_WAIT_FOR_BLE;
#endif

  // Set deepest sleep mode
  SLEEP_SleepBlockBegin(blocked_sleep_mode);

  //init_timer_interrupt();

  // Initialize BLE stack.
  // This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2
  //gecko_init(&config);

  //__enable_irq();

  uint8_t chip_id = 0;

  /* Infinite loop */

  //rslt = bmp3_init(&bmp_device);

  //LOG_INFO("Initilization result: %d", rslt);

  //bmp388_set_normal_mode(&bmp_device);

  //stream_sensor_data_normal_mode(&dev);
  //spi_read(ICM_DEV_ID,WHO_AM_I,reg_data,1);

  //spi_read(ICM_DEV_ID,PWR_MGMT_1,reg_data,1);

  // Enable Gyro and Accel
  //reg_data[0] = 0;
  //spi_write(ICM_DEV_ID,PWR_MGMT_2,reg_data,1);

  //reg_data[0] = WAKE_MODE|AUTO_CLK;

  // Wake the device
  //spi_write(ICM_DEV_ID,PWR_MGMT_1,reg_data,1);

  rslt = icm20948_init(&imu_dev);

  while (1)
  {
		// Check for external event
		//if(my_state_struct.event_bitmask != 0)
		//{
			// Call scheduler
		//	my_scheduler(&my_state_struct);
		//	SLEEP_Sleep();

		//}

	  //bmp388_get_sensor_data(&bmp_device);

	  //rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);
	  //print_sensor_data(&comp_data);

	  delay_ms(200);
	  rslt = icm20948_get_agmt(&imu_dev,sensors);
	  LOG_INFO("Accel data - X: %d  Y: %d  Z: %d",imu_dev.sensor_data.accelerometer_x,imu_dev.sensor_data.accelerometer_y,imu_dev.sensor_data.accelerometer_z);
	  LOG_INFO("Gyro data - X: %d  Y: %d  Z: %d",imu_dev.sensor_data.gyroscope_x,imu_dev.sensor_data.gyroscope_y,imu_dev.sensor_data.gyroscope_y);
	  LOG_INFO("Temperature: %d C",imu_dev.sensor_data.temperature);
	  //delay_ms(20);
	  //spi_read(ICM_DEV_ID,GYRO_DATA,reg_data,6);


	  //spi_read(ICM_DEV_ID, BMP3_CHIP_ID_ADDR, reg_data, 3);

	  //LOG_INFO("%d,%d,%d,%d",reg_data[0],reg_data[1],reg_data[2],reg_data[3]);

  }
}
