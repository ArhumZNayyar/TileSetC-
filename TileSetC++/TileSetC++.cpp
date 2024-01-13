// TileSetC++.cpp : Defines the entry point for the application.
//

#include "TileSetC++.h"

#include "framework.h"
#include <windows.h>
#include <ShObjIdl.h>

#define MAX_LOADSTRING 100
#define ID_BROWSE_BUTTON 1
#define ID_GENERATE_BUTTON 2
#define ID_TEXTBOX 3
#define ID_TILE_WIDTH_TEXTBOX 4
#define ID_TILE_HEIGHT_TEXTBOX 5

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hBrowseButton, hGenerateButton, hTextBox, hTileWidthTextbox, hTileHeightTextbox;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TILESETC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TILESETC));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HINSTANCE hPrevInstance = NULL;
    LPWSTR lpCmdLine = GetCommandLineW();
    int nCmdShow = SW_SHOWDEFAULT;

    return wWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TILESETC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TILESETC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    hBrowseButton = CreateWindow(
        L"BUTTON", L"Browse Folder",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 120, 30,
        hWnd, (HMENU)ID_BROWSE_BUTTON, hInstance, NULL);

    hGenerateButton = CreateWindow(
        L"BUTTON", L"Generate Tileset",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        140, 10, 120, 30,
        hWnd, (HMENU)ID_GENERATE_BUTTON, hInstance, NULL);

    hTextBox = CreateWindow(
        L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        270, 10, 200, 30,
        hWnd, (HMENU)ID_TEXTBOX, hInstance, NULL);

    hTileWidthTextbox = CreateWindow(
        L"EDIT", L"16",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER,
        10, 50, 50, 20,
        hWnd, (HMENU)ID_TILE_WIDTH_TEXTBOX, hInstance, NULL);

    hTileHeightTextbox = CreateWindow(
        L"EDIT", L"16",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER,
        70, 50, 50, 20,
        hWnd, (HMENU)ID_TILE_HEIGHT_TEXTBOX, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_BROWSE_BUTTON:
            {
                // Initialize COM Library.
                CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

                // Create File Open Dialog object.
                IFileDialog* pfd = NULL;
                HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pfd));

                if (SUCCEEDED(hr))
                {
                    // Set options to select folders.
                    DWORD dwOptions;
                    if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
                    {
                        pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
                    }

                    // Show the dialog.
                    hr = pfd->Show(NULL);

                    if (SUCCEEDED(hr))
                    {
                        // Obtain the result.
                        IShellItem* psiResult;
                        hr = pfd->GetResult(&psiResult);

                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath = NULL;
                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            if (SUCCEEDED(hr))
                            {
                                // Set the folder path to the textbox
                                SetWindowText(hTextBox, pszFilePath);
                                CoTaskMemFree(pszFilePath);
                            }
                            psiResult->Release();
                        }
                    }
                    pfd->Release();
                }

                CoUninitialize();
                break;
            }
            case ID_GENERATE_BUTTON:
            {
                wchar_t folderPath[260];
                wchar_t tileWidthStr[10];
                wchar_t tileHeightStr[10];

                GetWindowText(hTextBox, folderPath, 260);
                GetWindowText(hTileWidthTextbox, tileWidthStr, 10);
                GetWindowText(hTileHeightTextbox, tileHeightStr, 10);

                std::wstring wsFolderPath(folderPath);
                std::string strFolderPath(wsFolderPath.begin(), wsFolderPath.end());

                // Convert tile dimensions to integers.
                int tileWidth = _wtoi(tileWidthStr);
                int tileHeight = _wtoi(tileHeightStr);

                Tileset tileset;
                tileset.CreateTileset(strFolderPath, tileWidth, tileHeight);
            }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
