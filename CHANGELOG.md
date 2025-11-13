# Changelog

All notable changes to this project will be documented in this file.

## [1.0.5] - 2025-11-13
- Added InitList function (limit size is 16)

## [1.0.4] - 2025-10-03
- Renamed GPIO pins

## [1.0.3] - 2025-08-22
- Added NULL check of CS functions

## [1.0.2] - 2025-08-05
- Fixed bug to calculate 0 and 100 % in MAX6967_WritePortPWMReg function

## [1.0.1] - 2025-07-25
- Modified init function (added set global current, write conf reg and init port value)
- Fixed bug in WritePortPWMReg function concerning polarity of pwm value
- Deleted unused DisableAllPorts function

## [1.0.0] - 2025-04-23
- Added doxygen documentation for plib_max6967_spi.h and plib_max6967.h
- Fixed bug in functions using id instead of port number
- Added disable port functions