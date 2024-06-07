#include "disk.h"
#include <iostream>
#include <windows.h>
using namespace std;
//获取调试权限
BOOL GetPrivileges()
{
    HANDLE hProcess;
    HANDLE hTokenHandle;
    TOKEN_PRIVILEGES tp;
    hProcess = GetCurrentProcess();
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle))
    {
        //	std::cerr <<("无法打开进程令牌" )<< std::endl;
        return false;
    }
    tp.PrivilegeCount = 1;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
    {
        //	std::cerr << "无法获取特权值" << std::endl;
        CloseHandle(hTokenHandle);
        CloseHandle(hProcess);
        return false;
    }
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hTokenHandle, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        //	std::cerr << "无法调整特权" << std::endl;
        CloseHandle(hTokenHandle);
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hTokenHandle);
    CloseHandle(hProcess);
    //std::cout << "获取特权成功" << std::endl;
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
    cout << endl << "现在，重新插上你待解锁的U盘" << endl;
    const int maxAttempts = 1000;
    const int timeout_ms = 10000;  // 超时时间为10秒
    int prevPhd = GetPHDnum();
    int elapsed_time = 0;
    while (elapsed_time < timeout_ms)
    {
        int currentPhd = GetPHDnum();
        if (currentPhd > prevPhd)
        {
            cout << "检测到新设备，序号:" << currentPhd - 1 << "  开始写入" << endl;
            unlock(currentPhd - 1);
            cout << "写入成功!即将退出!";
            Sleep(500);
            exit(0);
        }
        Sleep(10);
        prevPhd = currentPhd;
        elapsed_time += 10;
    }
    cout << "超时退出！" << endl;
    exit(0);
}


    void welcomeMessage() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        cout << " ___      _                    ___           _     __            __ " << endl;
        cout << "/ _ \\____(_)  _____ _______ __/ _ | ___ ___ (_)__ / /____ ____  / /_" << endl;
        cout << "/ ___/ __/ / |/ / _ `/ __/ // / __ |(_-<(_-</ (_-</ __/ _ `/ _ \\/ __/" << endl;
        cout << "/_/  /_/ /_/|___/\\_,_/\\__/\\_, /_/ |_/___/___/_/___/\\__/\\_,_/_//_/\\__/ " << endl;
        cout << "                        /___/                                         " << endl;
        typeWriterEffect("\n\n                         PrivacyAssistant,硬盘扇区加密锁程序\n", 35);
        Sleep(500);
    }

    void lockPhysicalDisk() {
        cout << "你是想本地磁盘上锁还是给移动硬盘上锁？" << endl
            << "①移动设备 ②本地磁盘 " << endl;
        char input;
        cin >> input;
        switch (input) {
        case '1': {
            cout << "请输入你的密码(10位): ";
            string password;
            cin >> password;
            if (password.length() != 10) {
                cout << "密码必须是10位，请重新操作。" << endl;
                return;   
            }
            cout << endl << "现在，重新插上你的U盘" << endl;
            int phds[1001] = { GetPHDnum() };
            phds[1] = GetPHDnum();//作为标准
            phds[2] = GetPHDnum();//作为标准
            for (int i = 2; i <= 1000; i++)
            {
                phds[i] = GetPHDnum();
                if (phds[i] > phds[i - 1])
                {
                    cout << "检测到新设备，序号:" << phds[i] - 1 << "  开始写入" << endl;
                    lockup(&password[0], GetPHDnum() - 1);
                    cout << "写入成功!即将退出!";
                    Sleep(500);
                    exit(0);
                }
                Sleep(10);//防止占用资源过多
            }
            cout << "超时退出！" << endl;
            exit(0);
            return;   // 终止函数执行
        }
        case '2': {
            cout << "输入你想要上锁的磁盘号（建议使用diskpart list disk命令查看）" << endl;
            int diskNumber;
            cin >> diskNumber;
            cout << "请输入你的密码(10位): ";
            string password;
            cin >> password;
            if (password.length() != 10) {
                cout << "密码必须是10位，请重新操作。" << endl;
                return;   // 终止函数执行
            }
            lockup(&password[0], diskNumber);
            cout << "任务完成!即将退出!";
            return;   // 终止函数执行
        }
        default:
            cout << "无效的选项。";
            return;   // 终止函数执行
        }
    }
};

int main() {
    GetPrivileges();
    DiskManager manager;
    manager.welcomeMessage();

    cout << "请选择你要进行的操作" << endl << "①给物理磁盘上锁" << endl << "②给物理磁盘解锁    (键入）" << endl;
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
        cout << "无效的选项。";
        break;
    }

    return 0;
}
