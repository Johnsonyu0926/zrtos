.. zephyr:board:: acp_6_0_adsp

Overview
********

ACP 6.0 Audio DSP board is based on AMD ACP 6.0 Xtensa Architecture DSP,
Zephyr OS is ported to run on the HiFi5 DSP.

Hardware
********

- Board features:

  - RAM & Storage: 1.75 MB HP SRAM / 512 KB IRAM/DRAM
  - Audio Interfaces:

      - 1 x SP (I2S, PCM),
      - 1 x BT (I2S, PCM),
      - 1 x HS(I2S, PCM),
      - DMIC

System requirements
*******************

Xtensa Toolchain
----------------

The Zephyr SDK provides GCC-based toolchains necessary to build Zephyr for
the AMD ACP boards. For users looking for higher optimization levels, building with
the proprietary Xtensa toolchain from Cadence might be preferable.

The following instructions assume you have purchased and
installed the toolchain(s) and core(s) for your board following their
instructions.

If you choose to build with the Xtensa toolchain instead of the Zephyr SDK, set
the following environment variables specific to the board in addition to the
Xtensa toolchain environment variable listed below.

First, make sure to set the following environment variables:

.. code-block:: shell

   export XTENSA_TOOLCHAIN_PATH="tools installed path"
   export XTENSA_BUILDS_DIR="user build directory path"
   export ZEPHYR_TOOLCHAIN_VARIANT=xcc
   export TOOLCHAIN_VER=RI-2019.1-linux
   export XTENSA_CORE=LX7_HiFi5_PROD

The bottom three variables are specific to acp_6_0.

Programming and Debugging
*************************

Building
--------

Build as usual.

For debugging purposes, use xt-gdb. For xcc debugging,
please contact the AMD developers’ debug tools team or utilize Cadence debugging tools.

Supported Features
==================

The following hardware features are supported:

+-----------+------------+-------------------------------------+
| Interface | Controller | Driver/Component                    |
+===========+============+=====================================+
| I2S       | on-chip    | interrupt controller                |
+-----------+------------+-------------------------------------+

System Clock
------------

This board configuration uses a system clock frequency of @ 200 - 800MHz.
