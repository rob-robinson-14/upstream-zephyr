/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <hal/nrf_clock.h>
#include <zephyr/device.h>
#include <zephyr/drivers/clock_control/nrf_clock_control.h>
#include <zephyr/kernel.h>

#define CALIBRATION_SLEEP_TIME K_MSEC(150)
#define CALIBRATION_ATTEMPTS   100

int main(void)
{
	// const struct device *const hf_clock = DEVICE_DT_GET_ONE(nordic_nrf_clock_xo);

	// if (!device_is_ready(hf_clock)) {
	// 	printk("HF clock device is not ready\n");
	// 	return 0;
	// }

	for (int attempt = 1; attempt <= CALIBRATION_ATTEMPTS; attempt++) {
		// struct onoff_client hf_client;
		int calibration_count;
		// int err;
		int result;

		while (z_nrf_clock_calibration_is_in_progress()) {
			k_sleep(K_MSEC(1));
		}

		calibration_count = z_nrf_clock_calibration_count();
		printk("Attempt %d: forcing calibration and sleeping\n", attempt);
		z_nrf_clock_calibration_force_start();

		/* Match test_force_calibration(): hold HF on during the sleep. */
		// sys_notify_init_spinwait(&hf_client.notify);
		// err = nrf_clock_control_request(hf_clock, NULL, &hf_client);
		// if (err < 0) {
		// 	printk("HF clock request failed: %d\n", err);
		// 	return 0;
		// }

		// while (sys_notify_fetch_result(&hf_client.notify, &result) != 0) {
		// }
		// if (result < 0) {
		// 	printk("HF clock startup failed: %d\n", result);
		// 	return 0;
		// }

		k_sleep(CALIBRATION_SLEEP_TIME);
		// k_busy_wait(150*1000);

		printk("Attempt %d awake; calibrations: %d\n", attempt,
		       z_nrf_clock_calibration_count() - calibration_count);
		// (void)nrf_clock_control_release(hf_clock, NULL);
	}

	printk("All calibration attempts completed\n");
	return 0;
}
