#include <iostream>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
using namespace std;

void printf_onebyone(string str, int speed)
{
    for (int i = 0; i < str.length(); i++) // 修改循环条件
    {
        Sleep(speed);
        cout << str[i];
    }
}

 void welcome()
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        cout << " ___      _                    ___           _     __            __ " << endl;
        cout << "/ _ \____(_)  _____ _______ __/ _ | ___ ___ (_)__ / /____ ____  / /_" << endl;
        cout << "/ ___/ __/ / |/ / _ `/ __/ // / __ |(_-<(_-</ (_-</ __/ _ `/ _ \/ __/" << endl;
        cout << "/_/  /_/ /_/|___/\_,_/\__/\_, /_/ |_/___/___/_/___/\__/\_,_/_//_/\__/ " << endl;
        cout << "                        /___/                                         " << endl;
        printf_onebyone("\n\n                         PrivacyAssistant,文件伪装程序（将压缩包伪装为图片）\n", 35);
        Sleep(500);
    }

string Getfilesuffix_name(string path)
{
    string suffix_str = path.substr(path.find_last_of('.') + 1);
    return suffix_str; // 返回文件后缀名
}

int main()
{
    welcome();
    string filename, filename2, newfilename;
    cout << "请输入你想要伪装的文件" << endl;
    cin >> filename;
    cin >> filename2;

    // 检查文件是否存在
    ifstream file1(filename);
    ifstream file2(filename2);
    if (!file1 || !file2)
    {
        cout << "文件不存在" << endl;
        return 1;
    }

    string st = Getfilesuffix_name(filename);
    string st1 = Getfilesuffix_name(filename2);

    cout << "请输入你想生成的文件名字";
    cin >> newfilename;

    string command = "copy /b " + filename + "+" + filename2 + " " + newfilename;
    system(command.data());

    // 检查文件是否创建成功
    ifstream newfile(newfilename);
    if (!newfile)
    {
        cout << "生成文件失败" << endl;
        return 1;
    }

    cout << "生成完成！";
    return 0;
}