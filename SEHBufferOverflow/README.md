## DVCA SEH Buffer Overflow
Version: 1.0

### Compiling
```
cl /LD dvca_sehlib.c /link /DYNAMICBASE:NO /NXCOMPAT:NO /SAFESEH:NO
cl /Od /GS- dvca_seh.c dvca_sehlib.lib /link /BASE:"0x13370000" /SAFESEH:NO /NXCOMPAT:NO /defaultlib:ws2_32.lib
```
