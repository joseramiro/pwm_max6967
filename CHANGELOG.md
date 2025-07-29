# Changelog

All notable changes to this project will be documented in this file.

## [1.0.1] - 2025-07-25
### Changed
- Modified init function (added set global current, write conf reg and init port value)
- Fixed bug in WritePortPWMReg function concerning polarity of pwm value
- Deleted unused DisableAllPorts function

## [1.0.0] - 2025-04-23
### Changed
- Added doxygen documentation for plib_max6967_spi.h and plib_max6967.h
- Fixed bug in functions using id instead of port number
- Added disable port functions