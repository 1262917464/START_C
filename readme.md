## START_C

This is a game launcher for RPG MAKER 2000/2003, written in the C language, with a code license of GPL3 and higher versions.
The author is: 1262917464.

#### Build Require

- Windows;
- msys2;
- UPX; (It is not required, just for compressing the program.)

#### Start Build

1. Build .rc

	run `windres -o start_NoReg_C.rc UAC.o` in msys2.
	
	`It is not required, but if skip this step the program will not have an icon and will not run as administrator.`
	
2. Build start.exe

	run `gcc UAC.o main.c -lgdi32 -o start.exe` in msys2
	
#### Compress The Executable File (Optional)

UPX Homepage: <https://upx.github.io/>

Run the following code 

`upx.exe --best --lzma start.exe`
