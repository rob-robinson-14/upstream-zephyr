/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <nrf_sys_event_manual.h>
#include <nrf_sys_event_primatives.h>

#include <errno.h>
#include <stdint.h>

#if defined(__NRF_TFM__)
#include <cmsis.h>
#else
#include <zephyr/autoconf.h>
#endif

#if !defined(__NRF_TFM__) && IS_ENABLED(CONFIG_NRF_TFM_SYS_EVENT_SERVICE) && \
	IS_ENABLED(CONFIG_TRUSTED_EXECUTION_NONSECURE)
#include <tfm_ioctl_core_api.h>
#endif

#if !IS_ENABLED(CONFIG_TRUSTED_EXECUTION_NONSECURE) || defined(__NRF_TFM__)
#ifdef RRAMC_PRESENT
#include <hal/nrf_rramc.h>
#elif defined(MRAMC_PRESENT)
#include <hal/nrf_mramc.h>
#endif

static uint32_t event_ref_cnt;

#if !defined(__NRF_TFM__) && !defined(CONFIG_ZERO_LATENCY_IRQS)
static struct k_spinlock event_lock;
#endif

static void irq_low_latency_on(bool enable)
{
#ifdef RRAMC_POWER_LOWPOWERCONFIG_MODE_Standby
	nrf_rramc_lp_mode_set(NRF_RRAMC, enable ? NRF_RRAMC_LP_STANDBY : NRF_RRAMC_LP_POWER_OFF);
#elif defined(MRAMC_POWER_AUTOPOWERDOWN_ENABLE_Enable)
	nrf_mramc_power_autopowerdown_t cfg;

	nrf_mramc_power_autopowerdown_get(NRF_MRAMC, &cfg);
	/* Disable auto power down to enable reduced latency */
	cfg.enable = !enable;
	nrf_mramc_power_autopowerdown_set(NRF_MRAMC, &cfg);
#endif
}
#endif /* direct NVM low-latency path */

void nrf_sys_event_manual_register(bool from_zli)
{
#if !IS_ENABLED(CONFIG_TRUSTED_EXECUTION_NONSECURE) || defined(__NRF_TFM__)
	LOCKED(){
		if (event_ref_cnt == 0) {
			irq_low_latency_on(true);
		}
		event_ref_cnt++;
	}
#elif IS_ENABLED(CONFIG_NRF_TFM_SYS_EVENT_SERVICE)
	int32_t result;
	__ASSERT(!(from_zli), "Tried to make PSA call from zli context.");
	tfm_platform_sys_event_manual_register(&result);
#endif
}

void nrf_sys_event_manual_unregister(bool from_zli)
{
#if !IS_ENABLED(CONFIG_TRUSTED_EXECUTION_NONSECURE) || defined(__NRF_TFM__)
	LOCKED() {
		__ASSERT_NO_MSG(event_ref_cnt > 0);
		event_ref_cnt--;
		if (event_ref_cnt == 0) {
			irq_low_latency_on(false);
		}
	}
#elif IS_ENABLED(CONFIG_NRF_TFM_SYS_EVENT_SERVICE)
	int32_t result;
	__ASSERT(!(from_zli), "Tried to make PSA call from zli context.");
	tfm_platform_sys_event_manual_unregister(&result);
#endif
}
