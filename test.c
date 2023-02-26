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
#include "Uninstall_Font.h"
#include "hash.h"

#include "RPG_RT_2000.c"
#include "OTF.c"

void write_file(char *name, unsigned long len, unsigned char *rawData);
int system_install_font(char *str);
char *A2U(char *Ansi);

int main(void)
{
	char *WCHAT;
	char *FONT_PATH;
	int r;
	unsigned int RPG_RT_HASH, OTF_HASH;
	STARTUPINFOW StartInfo; 
	PROCESS_INFORMATION pinfo; 
	memset(&StartInfo,0,sizeof(STARTUPINFO)); 
	StartInfo.cb = sizeof(STARTUPINFO);
	
    printf("OTF Simple hash: %08x\n", OTF_HASH = hash(OTF_rawData, sizeof(OTF_rawData)));
    if(OTF_HASH != hash_file("game\\wqy12px+.otf"))
    {
    	printf("Font verification Failed!\n");
		write_file("game\\wqy12px+.otf", 5729328, OTF_rawData);
		printf("Write Font\n");

    }
    else
    {
    	printf("Font verification success!\n");
    }
    printf("RPG_RT Simple hash: %08x\n", RPG_RT_HASH = hash(RPG_RT_rawData, sizeof(RPG_RT_rawData)));
    if(RPG_RT_HASH != hash_file("game\\rpg_rt.exe"))
    {
    	printf("RPG_RT verification Failed!\n");
		write_file("game\\rpg_rt.exe", 762880, RPG_RT_rawData);
		printf("Write RPG_RT.exe\n");

    }
    else
    {
    	printf("RPG_RT verification success!\n");
    }
	
	char *Font_Path;
	Font_Path = find_FontReg();
	if(Font_Path)
	{
		printf("Font Path:%s\n",Font_Path);
		printf("WQY12PX+ exist\n");
	}
	else
	{
		//Uninstall_Font(Font_Path);
		//printf("Delete System Font\n");

		//AddFontResourceA(strcat(getcwd( NULL, 0 ), "\\game\\wqy12px+.otf"));
		FONT_PATH = malloc(strlen(getcwd( NULL, 0 )) + strlen("\\game\\wqy12px+.otf"));
		strcpy(FONT_PATH, getcwd( NULL, 0 ));
		strcat(FONT_PATH, "\\game\\wqy12px+.otf");
		if(AddFontResourceW(A2U(FONT_PATH)))
		{
			SendMessage((HWND)65535, 29, 0, 0);
			printf("AddFontResource Success\n");
		}
		else
			printf("AddFontResource Failed\n");
	}
	
	printf("START GAME\n");
	utf8_to_unicode(getfile("staff.txt"), &WCHAT);
	MessageBoxW(0, WCHAT, L"汉化名单", 64);
	//system("cd game");
	//system("rpg_rt.exe NormalPlay ShowTitle Window");
	char *RPG_RT_Path;
	RPG_RT_Path = malloc(strlen(getcwd( NULL, 0 )) + strlen("\\game\\rpg_rt.exe NormalPlay ShowTitle Window"));
	strcpy(RPG_RT_Path, getcwd( NULL, 0 ));
	strcat(RPG_RT_Path, "\\game\\rpg_rt.exe NormalPlay ShowTitle Window");
	CreateProcessW(NULL
		 ,A2U(RPG_RT_Path),NULL,NULL,FALSE,0,NULL,NULL,&StartInfo,&pinfo); 
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
	