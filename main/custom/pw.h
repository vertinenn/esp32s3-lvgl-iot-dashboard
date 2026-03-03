#include <string.h>
#include "gui_guider.h"

#define PwCorrect "123456"//定义正确密码
extern char PwStr[20];//字符串数组
void remove_last_char(char *str);//删除字符串最后一个字符函数声明
void InputPW(uint32_t id,const char *str);//输入密码函数声明
