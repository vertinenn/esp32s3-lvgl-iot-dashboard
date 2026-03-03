#include "pw.h"
char PwStr[20];//字符串数组

void remove_last_char(char *str)
{
    if(str==NULL) return;
    size_t len=strlen(str);
    if(len>0)
    {
        str[len-1]='\0';//把最后一个字符串替换为结束符
    }
}

void InputPW(uint32_t id,const char *str)
{
    if(id==3)//x
    {
        remove_last_char(PwStr);
    }
    else if(id==11)//y
    {
        if(strcmp(PwCorrect,PwStr)==0)
        { 
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_del, setup_scr_screen_1, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        }
    }
    else{
        strcat(PwStr,str);//把矩阵按键的每一个文本追加到Pwstr末尾
    }

    lv_label_set_text(guider_ui.screen_PassWord_label,PwStr);
}