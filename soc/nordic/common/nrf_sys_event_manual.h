/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Manual NVM low-latency mode for nrf_sys_event.
 *
 */

#ifndef SOC_NORDIC_COMMON_NRF_SYS_EVENT_MANUAL_H_
#define SOC_NORDIC_COMMON_NRF_SYS_EVENT_MANUAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief Enter manual NVM low-latency mode (refcounted).
 *
 * @param from_zli True if called from zero latency interrupt context, false otherwise.
 * 
 * @retval NRF_SYS_EVENT_MANUAL_HANDLE on success.
 * @retval -EAGAIN if the reference count would overflow.
 */
void nrf_sys_event_manual_register(bool from_zli);

/**
 * @brief Leave manual NVM low-latency mode (refcounted).
 *
 * @param from_zli True if called from ZLI context, false otherwise.
 * @retval 0 on success.
 * @retval -EINVAL if there is no active registration.
 */
void nrf_sys_event_manual_unregister(bool from_zli);

#ifdef __cplusplus
}
#endif

#endif /* SOC_NORDIC_COMMON_NRF_SYS_EVENT_MANUAL_H_ */
