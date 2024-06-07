#include "disk.h"
#include <iostream>
#include <windows.h>
using namespace std;
//��ȡ����Ȩ��
BOOL GetPrivileges()
{
    HANDLE hProcess;
    HANDLE hTokenHandle;
    TOKEN_PRIVILEGES tp;
    hProcess = GetCurrentProcess();
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle))
    {
        //	std::cerr <<("�޷��򿪽�������" )<< std::endl;
        return false;
    }
    tp.PrivilegeCount = 1;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
    {
        //	std::cerr << "�޷���ȡ��Ȩֵ" << std::endl;
        CloseHandle(hTokenHandle);
        CloseHandle(hProcess);
        return false;
    }
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hTokenHandle, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        //	std::cerr << "�޷�������Ȩ" << std::endl;
        CloseHandle(hTokenHandle);
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hTokenHandle);
    CloseHandle(hProcess);
    //std::cout << "��ȡ��Ȩ�ɹ�" << std::endl;
    return true;
}
class DiskManager {
public:
    void typeWriterEffect(const std::string& str, int speed) {
        for (char c : str) {
            Sleep(speed);
            std::cout << c;
        }
    }

void unlockDisk()
{
    cout << endl << "���ڣ����²������������U��" << endl;
    const int maxAttempts = 1000;
    const int timeout_ms = 10000;  // ��ʱʱ��Ϊ10��
    int prevPhd = GetPHDnum();
    int elapsed_time = 0;
    while (elapsed_time < timeout_ms)
    {
        int currentPhd = GetPHDnum();
        if (currentPhd > prevPhd)
        {
            cout << "��⵽���豸�����:" << currentPhd - 1 << "  ��ʼд��" << endl;
            unlock(currentPhd - 1);
            cout << "д��ɹ�!�����˳�!";
            Sleep(500);
            exit(0);
        }
        Sleep(10);
        prevPhd = currentPhd;
        elapsed_time += 10;
    }
    cout << "��ʱ�˳���" << endl;
    exit(0);
}


    void welcomeMessage() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        cout << " ___      _                    ___           _     __            __ " << endl;
        cout << "/ _ \\____(_)  _____ _______ __/ _ | ___ ___ (_)__ / /____ ____  / /_" << endl;
        cout << "/ ___/ __/ / |/ / _ `/ __/ // / __ |(_-<(_-</ (_-</ __/ _ `/ _ \\/ __/" << endl;
        cout << "/_/  /_/ /_/|___/\\_,_/\\__/\\_, /_/ |_/___/___/_/___/\\__/\\_,_/_//_/\\__/ " << endl;
        cout << "                        /___/                                         " << endl;
        typeWriterEffect("\n\n                         PrivacyAssistant,Ӳ����������������\n", 35);
        Sleep(500);
    }

    void lockPhysicalDisk() {
        cout << "�����뱾�ش����������Ǹ��ƶ�Ӳ��������" << endl
            << "���ƶ��豸 �ڱ��ش��� " << endl;
        char input;
        cin >> input;
        switch (input) {
        case '1': {
            cout << "�������������(10λ): ";
            string password;
            cin >> password;
            if (password.length() != 10) {
                cout << "���������10λ�������²�����" << endl;
                return;   
            }
            cout << endl << "���ڣ����²������U��" << endl;
            int phds[1001] = { GetPHDnum() };
            phds[1] = GetPHDnum();//��Ϊ��׼
            phds[2] = GetPHDnum();//��Ϊ��׼
            for (int i = 2; i <= 1000; i++)
            {
                phds[i] = GetPHDnum();
                if (phds[i] > phds[i - 1])
                {
                    cout << "��⵽���豸�����:" << phds[i] - 1 << "  ��ʼд��" << endl;
                    lockup(&password[0], GetPHDnum() - 1);
                    cout << "д��ɹ�!�����˳�!";
                    Sleep(500);
                    exit(0);
                }
                Sleep(10);//��ֹռ����Դ����
            }
            cout << "��ʱ�˳���" << endl;
            exit(0);
            return;   // ��ֹ����ִ��
        }
        case '2': {
            cout << "��������Ҫ�����Ĵ��̺ţ�����ʹ��diskpart list disk����鿴��" << endl;
            int diskNumber;
            cin >> diskNumber;
            cout << "�������������(10λ): ";
            string password;
            cin >> password;
            if (password.length() != 10) {
                cout << "���������10λ�������²�����" << endl;
                return;   // ��ֹ����ִ��
            }
            lockup(&password[0], diskNumber);
            cout << "�������!�����˳�!";
            return;   // ��ֹ����ִ��
        }
        default:
            cout << "��Ч��ѡ�";
            return;   // ��ֹ����ִ��
        }
    }
};

int main() {
    GetPrivileges();
    DiskManager manager;
    manager.welcomeMessage();

    cout << "��ѡ����Ҫ���еĲ���" << endl << "�ٸ������������" << endl << "�ڸ�������̽���    (���룩" << endl;
    char input;
    cin >> input;

    switch (input) {
    case '1':
        manager.lockPhysicalDisk();
        break;
    case '2':
        manager.unlockDisk();
        break;
    default:
        cout << "��Ч��ѡ�";
        break;
    }

    return 0;
}
