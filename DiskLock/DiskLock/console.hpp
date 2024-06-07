#pragma once
#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
using std::ios_base;
using std::ostream;
//dwButtonState ��갴ť״̬
#define L_BUTTON 0x1
#define R_BUTTON 0x2

// dwEventFlags ����¼�״̬
#define MOUSE_CLICK 0x0
#define MOUSE_MOVED 0x1
#define DOUBLE_CLICK 0x2

//��ɫ
#define COLOR_default SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#define COLOR_Black_Cyan SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
#define COLOR_Yellow_Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xe9);

// �ƶ����λ��
void setcursor(short x = 0, short y = 0)
{
    COORD temp = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
void setcursor(const COORD& temp)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}

// ��ù��λ��
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
//����������
ostream& operator<<(ostream& pout, const COORD& temp)
{
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}

//�Ƿ����ع��
void hidecursor(bool hide = true)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}

//button - click
//�ж������λ���Ƿ���ָ������Ч��Χ�����У�������5��֮�䣩
bool operator-(const COORD& button, const COORD& click)
{
    if (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20)
        return true;
    else
        return false;
};

// ���Ŀ���̨���ã���ֹ����̨���Ե��µ��ж�ʧ��
void setmode()
{
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //�Ƴ����ٱ༭ģʽ
    mode &= ~ENABLE_INSERT_MODE;     //�Ƴ�����ģʽ
    // mode &= ~ENABLE_MOUSE_INPUT;     //�Ƴ��������
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

//�ȴ�����¼������ų�����ƶ�
MOUSE_EVENT_RECORD waitmouse(bool move = true)
{
    INPUT_RECORD record; //�����¼�
    DWORD reg;           //��ʱ�Ĵ�
    while (1)
    {
        Sleep(10);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //�������¼�����record
        if (record.EventType == MOUSE_EVENT && (move | record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)) //������¼� && �ƶ��¼���ģʽ��Ӧ
            return record.Event.MouseEvent;
    }
}
// ��������
void clean()
{
    COORD temp = getcursor();
    setcursor(0, 0);
    for (int i = 0; i <= temp.Y; i++)
        std::cout << "                                                                                                    \n";
    setcursor(0, 0);
}
//��ͣ
void pause()
{
    std::cout << "�밴���������\n";
    std::cin.sync();
    std::cin.get();
}
#endif