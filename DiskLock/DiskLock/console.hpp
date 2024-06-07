#pragma once
#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
using std::ios_base;
using std::ostream;
//dwButtonState 鼠标按钮状态
#define L_BUTTON 0x1
#define R_BUTTON 0x2

// dwEventFlags 鼠标事件状态
#define MOUSE_CLICK 0x0
#define MOUSE_MOVED 0x1
#define DOUBLE_CLICK 0x2

//颜色
#define COLOR_default SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#define COLOR_Black_Cyan SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
#define COLOR_Yellow_Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xe9);

// 移动光标位置
void setcursor(short x = 0, short y = 0)
{
    COORD temp = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
void setcursor(const COORD& temp)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}

// 获得光标位置
void getcursor(COORD& other)
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    other = temp.dwCursorPosition;
}
COORD getcursor()
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    return temp.dwCursorPosition;
}
//输出光标坐标
ostream& operator<<(ostream& pout, const COORD& temp)
{
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}

//是否隐藏光标
void hidecursor(bool hide = true)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}

//button - click
//判断鼠标点击位置是否在指定的有效范围（本行，本列至5列之间）
bool operator-(const COORD& button, const COORD& click)
{
    if (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20)
        return true;
    else
        return false;
};

// 更改控制台设置，防止控制台属性导致的判定失败
void setmode()
{
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //移除快速编辑模式
    mode &= ~ENABLE_INSERT_MODE;     //移除插入模式
    // mode &= ~ENABLE_MOUSE_INPUT;     //移除鼠标输入
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

//等待鼠标事件，可排除鼠标移动
MOUSE_EVENT_RECORD waitmouse(bool move = true)
{
    INPUT_RECORD record; //输入事件
    DWORD reg;           //临时寄存
    while (1)
    {
        Sleep(10);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //将输入事件存入record
        if (record.EventType == MOUSE_EVENT && (move | record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)) //是鼠标事件 && 移动事件与模式对应
            return record.Event.MouseEvent;
    }
}
// 清屏函数
void clean()
{
    COORD temp = getcursor();
    setcursor(0, 0);
    for (int i = 0; i <= temp.Y; i++)
        std::cout << "                                                                                                    \n";
    setcursor(0, 0);
}
//暂停
void pause()
{
    std::cout << "请按任意键继续\n";
    std::cin.sync();
    std::cin.get();
}
#endif