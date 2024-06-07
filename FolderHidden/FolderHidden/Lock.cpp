#include"stdafx.h"
using namespace std;
class Locker
{
private:

    string tempStorage, command, password, password_confirm;
    bool path_space_flag, r_flag, p_flag, pass_flag;

    void setConsoleFont()
    {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = 20;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        std::wcscpy(cfi.FaceName, L"Yunxi");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    }

    void SetColor(int color)
    {
        WORD wColor;
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;               

        if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
        {
   
            wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
            SetConsoleTextAttribute(hStdOut, wColor);
        }

    }

    void scrClr(float i = 0.0) 
    {
       
        Sleep(i);      
        system("cls"); 
    }

    void setCursorPos(int x, int y = 0) 
    {

     
        while (x > 0)
        {
            cout << endl;
            x--;
        }

     
        while (y != 0)
        {
            printf(" ");
            y--;
        }
    }

    bool dirExists(const char* path) 
    {
        struct stat info;

        if (stat(path, &info) != 0)
            return (false);
        else if (info.st_mode & S_IFDIR)
            return (true);
        else
            return (false);
    }

    bool fileExists(const string path)
    {
        bool status_flag = false;
        ifstream read(path.c_str(), ios::binary | ios::in);  
        if (!read.is_open())
        {
            status_flag = false;
        }
        else
        {
            status_flag = true;
        }
        read.close();
        return(status_flag);
    }

    bool checkSpInput(string& input)  
    {

        string list = "#%&{}*><$!:\'\"@+`|=";

        int j = 0, i = 0;
        bool flag = false;

        for (i = 0; i < list.length(); i++)
        {
            for (j = 0; j < input.length(); j++)
            {
                if (input[j] == ' ')
                {
                    path_space_flag = true;
                }

                if (input[1] == ':') 
                    continue;

                if (input[j] == list[i])
                {
                    flag = true;
                    break;
                }
                else if (input[j] == '/' || input[0] == '/' || input[0] == '\\') 
                {
                    flag = true;
                    break;
                }

            }
        }

        return(flag);
    }
    void writeDataToFile(string path, string write_data) 
    {
        ofstream write(path.c_str(), ios::app); 

        if (!write.is_open())
        {
            scrClr();
            setCursorPos(2, 30);
            SetColor(12);
            cout << "出现了一些严重的错误....";
            scrClr(1.5);
            SetColor(15);
            exit(1);
        }
        else
        {
            write << write_data << endl;
        }
        write.close();
    }
    void welcome()
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
        cout << " ___      _                    ___           _     __            __ " << endl;
        cout << "/ _ \____(_)  _____ _______ __/ _ | ___ ___ (_)__ / /____ ____  / /_" << endl;
        cout << "/ ___/ __/ / |/ / _ `/ __/ // / __ |(_-<(_-</ (_-</ __/ _ `/ _ \/ __/" << endl;
        cout << "/_/  /_/ /_/|___/\_,_/\__/\_, /_/ |_/___/___/_/___/\__/\_,_/_//_/\__/ " << endl;
        cout << "                        /___/                                         " << endl;
        printf_onebyone("\n\n                         PrivacyAssistant,文件夹加密隐藏程序\n", 35);
        Sleep(500);
    }
    void printf_onebyone(string str, int speed)
    {
        for (int i = 0; i <= str.length(); i++)
        {
            Sleep(speed);
            cout << str[i];
        }
    }
    string getPath() 
    {
    repeat:

        tempStorage.clear();
        command.clear();
        welcome();
        SetColor(15);
        setCursorPos(2, 2);
        SetColor(15);
        cout << "请输入你想上锁的完整路径: ";
        getline(cin, command);
        SetColor(15);

        if (checkSpInput(command))  
        {
            setCursorPos(2, 25);
            SetColor(12);
            cout << "输入的路径格式不正确！！";
            scrClr(1.5);
            SetColor(15);
            goto repeat;

        }
        else
        {
            if ((!dirExists(command.c_str())) || command[command.length() - 1] == '\\') 
            {
                setCursorPos(2, 35);
                SetColor(12);
                cout << "无法找到这个路径，请检查输入！";
                scrClr(1.5);
                SetColor(15);
                goto repeat;
            }
            else
            {
            repeat_opt:

                if (r_flag)
                {
                    tempStorage.clear();

                    scrClr();
                    setCursorPos(0, 48);
                    SetColor(10);
                    welcome();
                    SetColor(15);

                    setCursorPos(2, 2);
                    SetColor(15);
                    cout << "输入你想加密隐藏文件夹的完整路径 ";
                    SetColor(7);
                    cout << command;
                    SetColor(15);

                    r_flag = false;
                }

                setCursorPos(2, 47);
                SetColor(15);
                cout << "真的要加密吗? Y/n : ";
                SetColor(7);
                getline(cin, tempStorage);
                SetColor(15);

                if (tempStorage == "Y" || tempStorage == "y")
                {

                repeat_opt2:

                    tempStorage.clear();
                    scrClr();

                    if (p_flag)
                    {
                        tempStorage.clear();
                        scrClr();
                        setCursorPos(0, 48);
                        SetColor(10);
                        welcome();
                        SetColor(15);
                        setCursorPos(2, 2);
                        SetColor(15);
                        cout << "输入你想加密隐藏文件夹的完整路径: ";
                        SetColor(7);
                        cout << command;
                        SetColor(15);

                        p_flag = false;
                    }

                    setCursorPos(2, 30);
                    SetColor(15);
                    cout << "是否启用密码保护(Y/N) ";
                    fflush(stdin);
                    SetColor(7);
                    cin >> tempStorage;
                    SetColor(15);

                    if (tempStorage == "Y" || tempStorage == "y")
                    {
                        pass_flag = true;
                    }
                    else if (tempStorage == "N" || tempStorage == "n")
                    {
                        pass_flag = false;
                    }
                    else
                    {
                        setCursorPos(2, 47);
                        SetColor(12);
                        cout << "非法输入！";
                        scrClr(1.5);
                        SetColor(15);
                        p_flag = true;
                        goto repeat_opt2;
                    }

                    return(command);
                }
                else if (tempStorage == "N" || tempStorage == "n")
                {
                    scrClr(0.5);
                    exit(0);
                }
                else
                {
                    setCursorPos(2, 48);
                    SetColor(12);
                    cout << "非法输入！";
                    scrClr(1.5);
                    SetColor(15);
                    r_flag = true;
                    goto repeat_opt;
                }

            }
        }

    }

    string getpass(const char* prompt, bool show_asterisk = true) 
    {
        const char BACKSPACE = 8;
        const char RETURN = 13;
        SetColor(15);

        string pswd;
        unsigned char ch = 0;

        cout << prompt;

        DWORD con_mode;
        DWORD dwRead;

        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

        GetConsoleMode(hIn, &con_mode);
        SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

        while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN)
        {
            if (ch == BACKSPACE)
            {
                if (pswd.length() != 0)
                {
                    if (show_asterisk)
                        cout << "\b \b";
                    pswd.resize(pswd.length() - 1);
                }
            }
            else
            {
                pswd += ch;
                if (show_asterisk)
                    cout << '*';
            }
        }
        cout << endl;
        return pswd;
    }


public:

    void lock() 
    {
        string path = getPath();
        string temp_path;
        tempStorage = path;

        if (path_space_flag)
        {
            temp_path.clear();
            temp_path = "\"";
            temp_path += tempStorage + "\"";
            tempStorage = temp_path;
        }

        if (pass_flag) 
        {
        r_ask_psed:
            scrClr();
            setCursorPos(2, 35);
            SetColor(15);
            fflush(stdin);
            password = getpass("输入你的密码: ", true);
            SetColor(15);

            setCursorPos(2, 35);
            fflush(stdin);
            SetColor(15);
            password_confirm = getpass("确认你的密码:", true);
            SetColor(15);

            if (password_confirm != password) 
            {
                setCursorPos(2, 40);
                SetColor(12);
                cout << "密码太长辣！";
                scrClr(1.5);
                SetColor(15);
                goto r_ask_psed;
            }
            else
            {
                writeDataToFile(path + "\\password.txt", password);
            }

        }

        path = path + "\\locker.ini";


        if (fileExists(path.c_str())) 
        {
            setCursorPos(2, 45);
            SetColor(10);
            cout << " 文件夹已上锁成功！";
            scrClr(1.5);
            SetColor(15);
            exit(0);
        }

        writeDataToFile(path, "[.shellClassInfo]\n");  
        writeDataToFile(path, "CLSID = {145141919810}\n");

        command = "attrib +h +s " + tempStorage;
        WinExec(command.c_str(), 0);

        setCursorPos(2, 34);
        SetColor(10);
        cout << "  " << endl; 
        scrClr(1.5);
        SetColor(15);
        exit(0);
    }

    Locker()
    {
        path_space_flag = r_flag = p_flag = pass_flag = false;
        setConsoleFont();
    }
    ~Locker()
    {

    }

protected:

};
