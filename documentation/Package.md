
Intoduction
---------------

the purpose of this document is to define a directory structure for a project

The "requirements" are defines as following

- the project shall be able to be build on the 3 major Operating System (Windows, Linux, MacOS X)
- the project shall be able to be executed on the  3 major Operating System and on embedded System (bare-metal, RTOS)
- each Software Component shall be compile as a library (Shared, Static)
- the project shall delivered the binary, the binary image

For embedded System Software Only

- Software Component shall be compatible with the ARM Keil Pack
- the <Software Component>_config shall use the [CMSIS Configuration Wizard] (https://sourceforge.net/projects/cmsisconfig/)
- a keil package shall at least contain the sources code (*.h, *c)
- the project shall delivered the binary, the binary images, configurations & Templates, System Resources.


Table of Contents
---------------

[TOC]

The structure of the Software Component is as follows:

```
<Software Component>
\- CMakeList.txt
|- configuration
|  \- <Software Component>_config.h (this file is used to configure features, option of the Software Components )
|- interfaces
|  \- <Software Component>_API.h (Software Component API)
|- documentations
|  \- (Software Component documentation in markdown)
|- sources
|  \- (Software Component sources)
|- test
|  \- (Unit-Tests sources)
```




The structure of the XXXX project is as follows:

```
.
|- scripts
|  \- JNodeServerLauncher (For test prupose)
|  \- builder (script to make packages)

|- server (Use to manage the configuration ? Package ? Test Configuration ?)
|  \- Backend
|  \- Frontend

|- build
|  \- pack.json							
|  |- Configuration					
|  |  \- Parameters.txt.enc			
|  |- binaries							
|  |  \- Bootloader.bin.enc				
|  |  \- Upgrader.bin.enc				
|  |  \- Application.bin.enc			
|  |- System						
|  |  \- Audio
|  |  \- Pictures
|  |  \- Icons	
|  |- keys								
|  |  \- Device-<UUID>-key.enc	

|- Delivery
|  \- Debug
|  |   \- UPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- CPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- PPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- CPKG-<PLATFORM>-<CUID>-<DATE>-<TIME>.zip (For test prupose)
|  \- Release
|  |   \- DPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- UPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- CPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- PPKG-<PLATFORM>-<DATE>-<TIME>.zip
|  |   \- CPKG-<PLATFORM>-<CUID>-<DATE>-<TIME>.zip (For test prupose)

|- Pre-build
|  \- pack.json							
|  |- Configuration					
|  |  \- Parameters.txt			
|  |- binaries							
|  |  \- Bootloader.bin				
|  |  \- Upgrader.bin				
|  |  \- Application.bin			
|  |- System						
|  |  \- Audio
|  |  \- Pictures
|  |  \- Icons	
|  |- keys								
|  |  \- Device-<UUID>-key.enc

|- keys 					
|  \- Manufactor
|  |  \- Manufactor-private-key.pem
|  |  \- Manufactor-public-key.pem	
|  \- Devices								
|  |  \- Device-<UUID>-public-key.pem
|  \- Operators
|  |  \- Operators-private-key.pem			
|  |  \- Operators-public-key.pem

|- Devices
|  \-Device-ID<UUID>
|  |  \- Configuration.sql
|  |  \- logs.sql

\- TestDataSets
|  \- Configurarion-<CUID> 
|  |  \- (Test Datasets )

```