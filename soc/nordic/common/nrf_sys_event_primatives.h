#ifndef NRF_SYS_EVENT_PRIMATIVES_H_
#define NRF_SYS_EVENT_PRIMATIVES_H_

#if (defined(NRF_APPLICATION) && !defined(CONFIG_TRUSTED_EXECUTION_NONSECURE)) || \
	!defined(__ZEPHYR__)
#include <cmsis.h>
#elif !defined(CONFIG_ZERO_LATENCY_IRQS)
#include <zephyr/kernel.h>
#endif

#if ((defined(NRF_APPLICATION) && !defined(CONFIG_TRUSTED_EXECUTION_NONSECURE)) || \
	!defined(__ZEPHYR__)) || defined(CONFIG_ZERO_LATENCY_IRQS)
static inline uint32_t full_irq_lock(void)
{
	uint32_t mcu_critical_state;

	mcu_critical_state = __get_PRIMASK();
	__disable_irq();

	return mcu_critical_state;
}

static inline void full_irq_unlock(uint32_t mcu_critical_state)
{
	__set_PRIMASK(mcu_critical_state);
}

#define LOCKED() \
	for (uint32_t __tmp = 0, __key = full_irq_lock(); !__tmp; full_irq_unlock(__key), __tmp = 1)
#else
#define LOCKED() K_SPINLOCK(&event_lock)
#endif

#endif /* NRF_SYS_EVENT_PRIMATIVES_H_ */
