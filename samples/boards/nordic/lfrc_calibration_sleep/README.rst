.. zephyr:code-sample:: lfrc_calibration_sleep
   :name: nRF LFRC calibration followed by sleep

   Force an LFRC calibration and immediately put the CPU to sleep.

Overview
********

This sample performs 100 iterations matching the force-calibration ztest:
wait for the previous calibration, force a new calibration, hold the HF clock,
and sleep for 40 milliseconds. It prints before and after every sleep.

Building
********

Build the sample for the nRF7120 DK:

.. zephyr-app-commands::
   :zephyr-app: samples/boards/nordic/lfrc_calibration_sleep
   :board: nrf7120dk/nrf7120/cpuapp
   :goals: build flash
   :west-args: --sysbuild
   :compact:

On affected nRF7120 engineering silicon, an iteration may print its
``forcing calibration and sleeping`` message without the corresponding
``awake`` message. The UICR image produced by sysbuild is part of the tested
configuration and must be flashed with the application.
