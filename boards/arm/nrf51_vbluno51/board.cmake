set(FLASH_SCRIPT pyocd.sh)
set(DEBUG_SCRIPT pyocd.sh)

set_property(GLOBAL APPEND PROPERTY FLASH_SCRIPT_ENV_VARS
  PYOCD_TARGET=nrf51
  )
