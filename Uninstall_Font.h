#include <windows.h>
#include <stdio.h>

int Remove_FontReg(char *name);
int Remove_FontReg_USER(char *name);
void Uninstall_Font(char *PATH);
char *find_FontReg(void);

int Remove_FontReg(char *name)
{
    HKEY hKey;
    LONG result;

    // 打开注册表的"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"子键
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        printf("RegOpenKeyExA failed, error code = %d\n", result);
        return -1;
    }

    // 删除"Arial"字体
    result = RegDeleteKeyExA(HKEY_LOCAL_MACHINE, name, KEY_WOW64_64KEY, 0);
    if (result != ERROR_SUCCESS)
    {
        printf("RegDeleteKeyExA HKEY_LOCAL_MACHINE 64 failed, error code = %d\n", result);
    }
	
    result = RegDeleteKeyExA(HKEY_LOCAL_MACHINE, name, KEY_WOW64_32KEY, 0);
    if (result != ERROR_SUCCESS)
    {
        printf("RegDeleteKeyExA HKEY_LOCAL_MACHINE 32 failed, error code = %d\n", result);
    }


    // 关闭注册表键
    RegCloseKey(hKey);

    return 0;
}

int Remove_FontReg_USER(char *name)
{
    HKEY hKey;
    LONG result;

    // 打开注册表的"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts"子键
    result = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        printf("RegOpenKeyExA failed, error code = %d\n", result);
        return -1;
    }

    // 删除"Arial"字体
    result = RegDeleteKeyExA(HKEY_CURRENT_USER, name, KEY_WOW64_64KEY, 0);
    if (result != ERROR_SUCCESS)
    {
        printf("RegDeleteKeyExA HKEY_CURRENT_USER 64 failed, error code = %d\n", result);
    }
	
    result = RegDeleteKeyExA(HKEY_CURRENT_USER, name, KEY_WOW64_32KEY, 0);
    if (result != ERROR_SUCCESS)
    {
        printf("RegDeleteKeyExA HKEY_CURRENT_USER 32 failed, error code = %d\n", result);
    }


    // 关闭注册表键
    RegCloseKey(hKey);

    return 0;
}


void Uninstall_Font(char *PATH)
{
	if(strchr(PATH,':'))
	{
		char str[65536] = "C:\\Windows\\Fonts\\";
		strcat(str,PATH);
		stpcpy(PATH,str);
	}
    if (!RemoveFontResourceA(PATH))
    {
        int nErr = GetLastError();
        printf("从系统字体库删除字体失败! 错误码：%d\n", nErr);
    }
 
    // 删除系统字体，操作需要程序具有管理员权限
    if (!DeleteFileA(PATH))
    {
        int nErr = GetLastError();
        printf("删除系统字体失败! 错误码：%d\n", nErr);
    }
    Remove_FontReg("WQY12PX+ (OpenType)");
	Remove_FontReg_USER("WQY12PX+ (OpenType)");
}

char *find_FontReg(void)
{
	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_QUERY_VALUE, &hKey);
	if (result != ERROR_SUCCESS)
	{
		printf("RegOpenKeyExA HKEY_LOCAL_MACHINE failed, error code = %d\n", result);
		return NULL;
	}

	char *szValue;
	szValue = malloc(65536);
	DWORD dwValueSize = 65536;//sizeof(szValue);
	result = RegQueryValueExA(hKey, "WQY12PX+ (OpenType)", NULL, NULL, (LPBYTE)szValue, &dwValueSize);
	switch(result){
	case ERROR_SUCCESS:
		printf("WQY12PX+ (OpenType) = %s\n", szValue);
		break;
	case ERROR_FILE_NOT_FOUND:
		printf("RegQueryValueExA HKEY_LOCAL_MACHINE failed, error code = ERROR_FILE_NOT_FOUND\n");
		result = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_QUERY_VALUE, &hKey);
		if (result != ERROR_SUCCESS)
		{
			printf("RegOpenKeyExA HKEY_CURRENT_USER failed, error code = %d\n", result);
			free(szValue);
			return NULL;
		}
		result = RegQueryValueExA(hKey, "WQY12PX+ (TrueType)", NULL, NULL, (LPBYTE)szValue, &dwValueSize);
		switch(result){
		case ERROR_SUCCESS:
			printf("WQY12PX+ (TrueType) = %s\n", szValue);
			break;
		case ERROR_FILE_NOT_FOUND:
			printf("RegQueryValueExA HKEY_CURRENT_USER failed, error code = ERROR_FILE_NOT_FOUND\n");
			free(szValue);
			return NULL;
		default:
			printf("RegQueryValueExA HKEY_CURRENT_USER failed, error code = %d\n", result);
			free(szValue);
			return NULL;
		}
		break;
	default:
		printf("RegQueryValueExA HKEY_LOCAL_MACHINE failed, error code = %d\n", result);
		free(szValue);
		return NULL;
	}
	// if (result != ERROR_SUCCESS)
	// {
		// printf("RegQueryValueExA HKEY_LOCAL_MACHINE failed, error code = %d\n", result);
		// free(szValue);
		// return NULL;
	// }
	// else
	// {
		// printf("WQY12PX+ (OpenType) = %s\n", szValue);
	// }

	RegCloseKey(hKey);
	return szValue;
}