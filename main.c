#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <fileapi.h>
#include <processthreadsapi.h>

#include "getfile.h"
#include "utf8_to_unicode.h"
//#include "Uninstall_Font.h"
#include "hash.h"

#include "RPG_RT_2000_NT_20250921.c"
#include "OTF.c"
#include "kernel33.c"
#include "user33.c"
#include "ini.c"

void write_file(char *name, unsigned long len, unsigned char *rawData);
int system_install_font(char *str);
char *A2U(char *Ansi);

int main(void)
{
	char *WCHAT;
	char *FONT_PATH;
	int r;
	unsigned int HASH;
	STARTUPINFOW StartInfo; 
	PROCESS_INFORMATION pinfo; 
	memset(&StartInfo,0,sizeof(STARTUPINFO)); 
	StartInfo.cb = sizeof(STARTUPINFO);
	
    printf("OTF Simple hash: %08x\n", HASH = hash(OTF_rawData, sizeof(OTF_rawData)));
    if(HASH != hash_file("game\\wqy12px+.otf"))
    {
    	printf("Font verification Failed!\n");
		write_file("game\\wqy12px+.otf", 5729328, OTF_rawData);
		printf("Write Font\n");

    }
    else
    {
    	printf("Font verification success!\n");
    }
    printf("RPG_RT Simple hash: %08x\n", HASH = hash(RPG_RT_rawData, sizeof(RPG_RT_rawData)));
    if(HASH != hash_file("game\\rpg_rt.exe"))
    {
    	printf("RPG_RT verification Failed!\n");
		write_file("game\\rpg_rt.exe", 762880, RPG_RT_rawData);
		printf("Write RPG_RT.exe\n");

    }
    else
    {
    	printf("RPG_RT verification success!\n");
    }
    printf("kernel33 Simple hash: %08x\n", HASH = hash(kernel33_rawData, sizeof(kernel33_rawData)));
    if(HASH != hash_file("game\\kernel33.dll"))
    {
    	printf("kernel33 verification Failed!\n");
		write_file("game\\kernel33.dll", sizeof(kernel33_rawData), kernel33_rawData);
		printf("Write kernel33.dll\n");

    }
    else
    {
    	printf("kernel33 verification success!\n");
    }
    printf("user33 Simple hash: %08x\n", HASH = hash(user33_rawData, sizeof(user33_rawData)));
    if(HASH != hash_file("game\\user33.dll"))
    {
    	printf("user33 verification Failed!\n");
		write_file("game\\user33.dll", sizeof(user33_rawData), user33_rawData);
		printf("Write user33.dll\n");

    }
    else
    {
    	printf("user33 verification success!\n");
    }
    printf("ini Simple hash: %08x\n", HASH = hash(ini_rawData, sizeof(ini_rawData)));
    if(HASH != hash_file("game\\game.ini"))
    {
    	printf("ini verification Failed!\n");
		write_file("game\\game.ini", sizeof(ini_rawData), ini_rawData);
		printf("Write game.ini\n");

    }
    else
    {
    	printf("ini verification success!\n");
    }
	
	printf("START GAME\n");
	utf8_to_unicode(getfile("staff.txt"), &WCHAT);
	MessageBoxW(0, WCHAT, L"汉化名单", 64);
	//system("cd game");
	//system("rpg_rt.exe NormalPlay ShowTitle Window");
	char *RPG_RT_Path;
	RPG_RT_Path = malloc(strlen(getcwd( NULL, 0 )) + strlen("\\game\\rpg_rt.exe NormalPlay ShowTitle Window") + 1);
	strcpy(RPG_RT_Path, getcwd( NULL, 0 ));
	strcat(RPG_RT_Path, "\\game\\rpg_rt.exe NormalPlay ShowTitle Window\0");
	char *GF_Path;
	GF_Path = malloc(strlen(getcwd( NULL, 0 )) + strlen("\\game") + 1);
	strcpy(GF_Path, getcwd( NULL, 0 ));
	strcat(GF_Path, "\\game\0");
	CreateProcessW(NULL
		 ,A2U(RPG_RT_Path),NULL,NULL,FALSE,0,NULL,A2U(GF_Path),&StartInfo,&pinfo); 
	return 0;
}

void write_file(char *name, unsigned long len, unsigned char *rawData)
{
	FILE *uni = fopen(name, "wb");
	fwrite(rawData, len, 1, uni);
	fclose(uni);
}

char *A2U(char *Ansi)
{
	int len = MultiByteToWideChar(0, 0, Ansi, -1, 0, 0);
	char *uni = malloc(len * 2);
	MultiByteToWideChar (936, 0, Ansi, -1, uni, len);
	return uni;
}
	