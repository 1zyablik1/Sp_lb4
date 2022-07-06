#include <windows.h> 
#include <vector>
#include <string>
#include <Psapi.h>

using namespace std;

HINSTANCE hInst;   
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "Zyablitsev Kelchevskiy";

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;    
    wcex.lpfnWndProc = (WNDPROC)WndProc;     
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;      
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW - 1); 
    wcex.lpszMenuName = NULL;               
    wcex.lpszClassName = szWindowClass;    
    wcex.hIconSm = NULL;

    return RegisterClassEx(&wcex);  
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    hInst = hInstance; 
    hWnd = CreateWindow(szWindowClass,    
        szTitle,              
        WS_OVERLAPPEDWINDOW,        
        CW_USEDEFAULT,       
        CW_USEDEFAULT,           
        900,           
        350,       
        NULL,          
        NULL,              
        hInstance,         
        NULL);         

    if (!hWnd)  
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);    
    UpdateWindow(hWnd);        
    return TRUE;
}

vector <STARTUPINFO> si;
STARTUPINFO temp;
vector <PROCESS_INFORMATION> pi;
PROCESS_INFORMATION temp0;
static int i = 0;

void StartProces(HWND hWnd, char name[60])
{
    si.push_back(temp);
    pi.push_back(temp0);

    HDC hdc;

    ZeroMemory(&si[i], sizeof(si[i]));
    si[i].cb = sizeof(si[i]);
    ZeroMemory(&pi[i], sizeof(pi[i]));

    if (!CreateProcess(NULL,
        name, NULL, NULL, FALSE, NULL, NULL, NULL, &si[i], &pi[i]))
    {
        hdc = GetDC(hWnd);
        TextOut(hdc, 100, 300, "Process start failed!", 21);
        DeleteDC(hdc);
    }
    i++;
}

void DrawInfo(HWND hWnd, int IdProc)
{
    HDC hdc = GetDC(hWnd);
    IdProc = IdProc - 1;
    if (IdProc < i && IdProc >= 0)
    {
        char buffer[50];

        FILETIME ft[4];
        SYSTEMTIME tm[4], t;
        int temp[3];
        GetProcessTimes(pi[IdProc].hProcess, &ft[0], &ft[1], &ft[2], &ft[3]);
        GetLocalTime(&t);

        FileTimeToSystemTime(&ft[0], &tm[0]);

        temp[1] = (t.wMinute - tm[0].wMinute) * 60 + (t.wSecond - tm[0].wSecond);
        temp[0] = temp[1] / 60;
        temp[2] = temp[1] % 60;
        if (temp[2] % 60 == 0)
        {
            InvalidateRect(hWnd, NULL, 1);
        }

        GetProcessImageFileName(pi[IdProc].hProcess, buffer, 60);

        TextOut(hdc, 750, 100, buffer, wsprintf(buffer, "%d", GetProcessImageFileNameA(pi[IdProc].hProcess, buffer, 60)));
        TextOut(hdc, 750, 150, buffer, wsprintf(buffer, "%d", pi[IdProc].hProcess));
        TextOut(hdc, 750, 200, buffer, wsprintf(buffer, "%d", pi[IdProc].dwProcessId));
        TextOut(hdc, 750, 250, buffer, wsprintf(buffer, "%d:%d", temp[0], temp[2]));
    }
    else
    {
        HBRUSH hbr = GetSysColorBrush(COLOR_WINDOW - 3);

        SelectObject(hdc, hbr);
        FillRect(hdc, NULL, hbr);
        DeleteObject(hbr);

        TextOut(hdc, 910, 100, "Процесу під таким номером не знайдено", 38);
    }
    DeleteDC(hdc);
    BOOL WINAPI GetExitCodeProcess(
        HANDLE hProcess,    // дескриптор процесу
        LPDWORD lpExitCode    // код повідомлення
    );
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = GetDC(hWnd);
    RECT rt;
    static HWND twnd[12];
    static char buff[60] = { "th1.exe" };
    static char number[3] = { "1" };
    char buff2[40] = { "Переглянути інформацію про процес" };
    int temp = 0;


    switch (message)
    {
    case WM_CREATE:
        twnd[0] = CreateWindow("edit", buff, WS_CHILD | WS_VISIBLE, 10, 60, 200, 20, hWnd, (HMENU)0, hInst, NULL);
        twnd[1] = CreateWindow("button", "Розпочати процес", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 10, 100, 150, 40, hWnd, (HMENU)1, hInst, NULL);
        twnd[2] = CreateWindow("edit", number, WS_CHILD | WS_VISIBLE, 250, 60, 60, 20, hWnd, (HMENU)4, hInst, NULL);
        twnd[3] = CreateWindow("button", buff2, WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 250, 100, 270, 40, hWnd, (HMENU)2, hInst, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: StartProces(hWnd, buff);
            break;
        case 4:
            GetWindowText(twnd[2], number, 4);
            break;
        case 2:
            temp = stoi(number);
            TextOut(hdc, 540, 100, "Iм’я процесу:", 14);
            TextOut(hdc, 540, 150, "Значення покажчика:", 20);
            TextOut(hdc, 540, 200, "Ідентифікатор процесу:", 23);
            TextOut(hdc, 540, 250, "Час виконання процесу:", 23);

            DrawInfo(hWnd, temp);
            break;
        case 0:
            GetWindowText(twnd[0], buff, 60);
            break;
        }
        break;
    case WM_DESTROY:     
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    DeleteDC(hdc);
    return 0;
}
