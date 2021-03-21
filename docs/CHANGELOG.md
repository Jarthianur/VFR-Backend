# changelog

## 4.0.0

## 3.0.3

+ changed default compiler optimization level to 2

## 3.0.2

+ fixed client timeout handling
+ some internal interface changes for client

## 3.0.1

+ restructuring code
+ build with cmake
+ small changes in client

## 3.0.0

+ improved priority handling of feeds
+ take timestamps into account
+ a lot of internal code changes
+ several bugfixes
+ option to log into file
+ option for debug logging
+ split sensor feeds to wind and atmosphere
+ improved connection handling

## 2.2.0

+ new code documentation
+ new github pages
+ internal class representation changed (Feed, Parser)
+ integrated coveralls
+ more tests

### 2.1.0

+ support GPSD
+ common sensor interface (support [BME280 driver](https://github.com/Jarthianur/sensorics))
+ generic input feeds
+ ! configuration refactoring

### 2.0.1

+ latitude/longitude parsing fixed

### 2.0.0

+ integrate boost
+ major refactoring, packaging
+ changed logging format
+ changed config format
+ signal handling
+ disable filters via properties
+ systemd service
+ installation script
+ unit tests

### 1.3.0

+ ADS-B (SBS) input
+ APRS (OGN flavor) input
+ Wind input and output (WIMWV, WIMDA NMEA sentences)
+ height, distance filters
+ static GPS output
+ PFLAU / PFLAA NMEA output
+ all I/O's and parameters are configurable via config file
+ multi client support