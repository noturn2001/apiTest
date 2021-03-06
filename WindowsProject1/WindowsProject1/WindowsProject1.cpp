// WindowsProject1.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"//stdio같은 느낌의 api 헤더파일 인풋 아웃풋이 구현된 헤더파일
#include "WindowsProject1.h"//내 프로젝트 이름
#include <list>


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

WCHAR lpszText[MAX_LOADSTRING] = L"TEXT";
std::list<WCHAR> wCharLst;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);//LRESULT long*임
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 기본 메시지 루프입니다. 메세지를 계속 처리하고 다시도는것
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);//메세지를 넘겨주는 작업하는 곳
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
//내가 만들 윈도우의 속성을 지정하는 곳
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hCursor = LoadCursor(nullptr, IDC_IBEAM);
	//마우스 커서 바꾸는것
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH);
	//윈도우 창 색상
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,// WS_로 무수히 많은 속성을 넣거나 뺄 수 있다.
	   //szTitle 을 L"testApi"로 바꾸면 타이틀 명 바뀜  L을 붙여야함
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

BOOL isTogle = false;
BOOL isDown = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//winapi에서 제일 중요한 함수 모든 윈도우 메세지 처리//이걸 어떤식으로 처리하느냐가 api가 어떻게 돌아가느냐의 기본 구조 그래서 제일 중요!!
//api에서 제일 많이 쓰는 자료형은 DWORD = unsigned long
//								WORD = unsigned short
//                              BYTE = unsigned char
//								LPSTR = *char /LP로 시작하는 자료형은 기본적으로 포인터라고 생각하면 됨
//								//H가 붙으면 핸들이라고 생각하면됨
//조건문을 AND연산자와 OR연산자를 많이씀 비트연산으로 만드는 조건문 현재까지도 많이 통용되는 코딩패턴
//윈도우 메시지는 주로 큐스덱처럼 들어간 순서대로 나온다. fifo(선입선출법)


//범용성을 위해 DC Device Context를 개발했다. 이건 dx3d에서도 쓴다. 
//		현재 내 하드웨어 상태를 체크해 주는 것 주로 윈도우나 모바일에서 많이 쓴다.
//		이것을 통해 하드웨어정보를 얻어 디바이스를 처리한다.
//		해상도 오류는 주로 여기에서 에러가 난다. //모니터 종류 그래픽 카드의 종류 등등에 따라서
//		윈도우 기본 제공 출력 함수는 TextOut, DrawText 주로 DrawText을 많이 쓴다.


//이런식으로 api라는 껍대기 안에 c라는 로직으로 채워서 만드는게 프로그램이다.!프로그램의 기본!!
//이미 만들어놓은 함수를 잘 찾아서 쓰는것도 능력이다.!



//다음주에 공부를 할 것!!

//color에 대해서 bit 컬러 bit map키워드를 찾아보기
//한칸의 비트를 어떤식으로 메모리에 저장하는가를 알아보는게 숙제!!
{
    switch (message)
    {
	case WM_KEYDOWN:
	{
	//	//if (wParam == 0x41)//a키를 눌렀을때.. (msdn으로 찾고 찾아 들어가면 나옴 사전같은 개념이라 어떤식으로 찾는지가 중요하다.)
	//	//{
	//	//	HDC hdc = GetDC(hWnd);
	//	//	TextOut(hdc, 100, 100, L"text", 4);
	//	//}
	//	HDC hdc = GetDC(hWnd);
	//	RECT rt = { 100, 100, 400, 400 };
	//	DrawText(hdc, lpszText, 5, &rt, DT_CENTER);
	//	break;
		HDC hdc = GetDC(hWnd);

		if (wCharLst.size() >= 10)
		{
			wCharLst.pop_front();
		}

		WCHAR input = L'a';
		wCharLst.push_back(input);
		auto iter = wCharLst.begin();
		int idx = 0;
		for (iter; iter != wCharLst.end(); ++iter)
		{
			TextOut(hdc, 100 + idx*10, 100, &(*iter), 1);
			idx++;
		}
		ReleaseDC(hWnd, hdc);
	}

	case WM_LBUTTONUP:
	{
		isDown = true;
		
	}

	case WM_LBUTTONDOWN:
	{
		
	}
	 
	case WM_MOUSEMOVE:
	{
		WORD x = HIWORD(lParam);//클릭할때 커서위치를 표현해 주는 함수
		WORD y = LOWORD(lParam);//클릭할때 커서위치를 표현해 주는 함수
		isTogle = !isTogle;//클릭할때마다 다르게 넣어주는 불 함수
		if(isTogle)
		{
		HDC hdc = GetDC(hWnd);// hWnd = 현재 돌아가고있는 윈도우 핸들 (포인터이다.) 뒤에 붙은 DC가 중요하닷!!
		RECT rt = { 100, 100, 400, 400 };//100,100의 좌표에서DT_CENTER = 400,400 의 센터를 잡는데 썼다.
		DrawText(hdc, lpszText, 5, &rt, DT_CENTER);
		TextOut(hdc, y, x, lpszText, 10);
		ReleaseDC(hWnd, hdc);//한번 쓴 컨택스트는 필요 없을경우!! 릴리즈로 삭제해 줘야한다. 뒤에 붙은 DC가 중요하닷!!
		}
		else
		{
			
		}
		break;
	}
    case WM_COMMAND:
		//윈도우에서 어떤 명령이 들어온다.
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
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
		//창크기 변환시 불려지는 메세지
		//창을 조정하면 클릭시 만들어진 메시지가 사라진다.
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		//프로그램 종료시 들어오는 메세지
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
