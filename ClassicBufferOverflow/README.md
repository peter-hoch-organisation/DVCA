## DVCA Classic Buffer Overflow
Version: 1.0

### Compiling
```
gcc.exe -c dvca_boflib.c
gcc.exe -shared -o dvca_boflib.dll -Wl,--out-implib=dvca_boflib.a -Wl,--image-base=0x13370000 dvca_boflib.o
gcc.exe dvca_bof.c -o dvca_bof.exe -lws2_32 ./dvca_boflib.a
```
