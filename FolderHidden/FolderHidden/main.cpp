#include"Lock.cpp"
#include"unLock.cpp"
using namespace std;
void printf_onebyone(string str, int speed)
{
    for (int i = 0; i <= str.length(); i++)
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
    printf_onebyone("\n\n                         PrivacyAssistant,�ļ������ؼ��ܳ���", 35);
    Sleep(500);
}
int main()
{
    Locker Folder;
    Unlocker Folder_u;
    welcome();
    system("cls");
    cout << "\n��ѡ����Ҫ���еĲ���" << endl << "�ٸ��ļ�������" << endl << "�ڸ��ļ��н���    (���룩" << endl;
    while (true)
    {

        if (_getch() == '1')
        {
            Folder.lock();
        }
        if (_getch() == '2')
        {
            Folder_u.unlock();
        }

    }
}