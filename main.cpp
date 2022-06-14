//=======================================================================================
//
//	�L�[�t���[���A�j���[�V����
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS

// �C���N���[�h�t�@�C��
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define RAD				3.14159265f/180.0f

#define JINTAI_PARTS 10			// �l�̃p�[�c��
#define KEYFRAME_MAX 26			// �L�[�t���[���ő吔

//----- �\���̒�` -----
typedef struct {
	float	m[3][3];
} MATRIX;

typedef struct {
	int		parent;				// �e�p�[�c�ԍ�
	int		type;				// �`����
	float	length;				// ����
	float	offx, offy;			// �I�t�Z�b�g�l
	float	angle;				// �p�x
	MATRIX	matrix;				// ��]�}�g���b�N�X
} PARTS;

typedef struct {
	int		status;					// �X�e�[�^�X
	int		timer;					// �^�C�}�[
	int		anim_index;				// �A�j���C���f�b�N�X
	PARTS	parts[JINTAI_PARTS];	// �p�[�c
} JINTAI;

typedef struct {
	int		parent;				// �e�p�[�c�ԍ�
	int		type;				// �`���ʁi�O�F�����A�P�F�~�j
	float	length;				// ����
	float	offx, offy;			// �I�t�Z�b�g�l
} PARTS_DATA;

typedef struct {
	int		time;					// ����
	float	angle[JINTAI_PARTS];	// �p�x
} ANIM_DATA;

//----- �ϐ� -----
JINTAI	jintai;				// �l�̃��f��
bool	g_bOnTimer;			// �A�j�����s���t���O
HPEN	g_hpenBlue;			// �J�X�^���y��

PARTS_DATA jintai_data[JINTAI_PARTS] = {				// �p�[�c�����f�[�^
	// parent type length offx offy
	{      -1,   0,    80,   0,   0,},	// �p�[�c�O�@����
	{       0,   1,    80,   0, -80,},	// �p�[�c�P�@��
	{       0,   0,    50,   0, -80,},	// �p�[�c�Q�@�E��r
	{       2,   0,    50,   0, -50,},	// �p�[�c�R�@�E���r
	{       0,   0,    50,   0, -80,},	// �p�[�c�S�@����r
	{       4,   0,    50,   0, -50,},	// �p�[�c�T�@�����r
	{       0,   0,    70,   0,   0,},	// �p�[�c�U�@�E���
	{       6,   0,    70,   0, -70,},	// �p�[�c�V�@�E����
	{       0,   0,    70,   0,   0,},	// �p�[�c�W�@�����
	{       8,   0,    70,   0, -70,},	// �p�[�c�X�@������
};

ANIM_DATA anim_data[KEYFRAME_MAX] = {					// �A�j���[�V�����f�[�^�i�p�x�̂݁j
	// time angle
	{     0,    0, 0, 135, 45, -135, -45,  160, 20, -160, -20,},		// �t���[���O�i�����|�[�Y�j

	{    30,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���P
	{    60,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���Q
	{    70,   45, 0, 135, 90,    0,  20,   90, 30, -180, -30,},		// �t���[���R
	{    80,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���S
	{    90,   45, 0, 135, 90,    0,  20,   90, 30, -180, -30,},		// �t���[���T
	{   100,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���U
	{   110,  -45, 0,   0,-20, -135, -90,  180, 30,  -90, -30,},		// �t���[���V
	{   120,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���W
	{   130,  -45, 0,   0,-20, -135, -90,  180, 30,  -90, -30,},		// �t���[���X
	{   140,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���P�O

	{   150,   45, 0, 135, 90,    0,  20,   90, 30, -180, -30,},		// �t���[���P�P
	{   160,    0, 0,  90,  0,  -90,   0,  140, 30, -140, -30,},		// �t���[���P�Q
	{   170,   45, 0, 135, 90,    0,  20,   90, 30, -180, -30,},		// �t���[���P�R
	{   200,   45, 0, 135, 90,    0,  20,   90, 30, -180, -30,},		// �t���[���P�S
	{   201,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���P�T
	{   240,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���P�U

	{   242,   50, 0, 125, 80,    0,  20,   85, 30, -185, -25,},		// �t���[���P�V
	{   244,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���P�W
	{   246,   50, 0, 125, 80,    0,  20,   85, 30, -185, -25,},		// �t���[���P�X
	{   260,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���Q�O
	{   262,   50, 0, 125, 80,    0,  20,   85, 30, -185, -25,},		// �t���[���Q�P
	{   264,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���Q�Q
	{   266,   50, 0, 125, 80,    0,  20,   85, 30, -185, -25,},		// �t���[���Q�R
	{   280,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���Q�S
	{   350,   50, 0, 125, 80,    0,  20,   85, 30, -185, -30,},		// �t���[���Q�T
};

static TCHAR szClassName[] = _T("KeyFrameAnim");

// �v���g�^�C�v
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CalcMatrix();
void SetAnimData(int index);
void ExecJintai();
void DrawModel(HDC hdc);

//---------------------------------------------------------------------------------------
//	���C��
//---------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	HWND	hWnd;
	MSG		msg;
	int		frx, fry, cpy;

	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= szClassName;
	wc.hIconSm			= NULL;
	if (RegisterClassEx(&wc) == 0) return false;	// �E�B���h�E�N���X��o�^

	// �E�B���h�E���쐬
	frx = GetSystemMetrics(SM_CXFIXEDFRAME);
	fry = GetSystemMetrics(SM_CYFIXEDFRAME);
	cpy = GetSystemMetrics(SM_CYCAPTION);
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, szClassName, _T("�L�[�t���[���A�j���[�V����"),
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		SCREEN_WIDTH + frx * 2, SCREEN_HEIGHT + fry * 2 + cpy,
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return false;
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	g_bOnTimer    = false;
	jintai.status = 0;
	// ���b�Z�[�W���[�v
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	return msg.wParam;
}

//---------------------------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//---------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT	ps;
	TCHAR		buf[256];
	int			len;

	switch (uMsg) {
	case WM_CREATE:
		g_hpenBlue = CreatePen(PS_SOLID, 4,RGB(0,0,255));	// �J�X�^���y�����쐬
		SetTimer(hWnd, 1, 20, NULL);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		DeleteObject(g_hpenBlue);		// �y�����폜
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		ExecJintai();
		InvalidateRect(hWnd, NULL, true);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:			// [ESC]
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			return 0;
		case VK_SPACE:			// [SPACE]
			if (g_bOnTimer) {
				jintai.status = 0;		// �t���[���m�F���[�h��
				g_bOnTimer = false;
			} else {
				jintai.status = 2;		// �A�j�����s���[�h��
				g_bOnTimer = true;
			}
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawModel(hdc);		// ���f����`��
		// �e�����`��
		if (g_bOnTimer) {
			len = _stprintf(buf, _T("�A�j�����s���[�h"));					TextOut(hdc, 0,  0, buf, len);
			len = _stprintf(buf, _T("�t���[����%d"), jintai.anim_index);	TextOut(hdc, 0, 20, buf, len);
			len = _stprintf(buf, _T("�^�C�}�[��%d"), jintai.timer);			TextOut(hdc, 0, 40, buf, len);
		} else {
			len = _stprintf(buf, _T("�t���[���m�F���[�h"));					TextOut(hdc, 0,  0, buf, len);
			len = _stprintf(buf, _T("�t���[����%d"), jintai.anim_index);	TextOut(hdc, 0, 20, buf, len);
		}
		EndPaint(hWnd, &ps);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//---------------------------------------------------------------------------------------
//	�p�x����}�g���b�N�X���v�Z�i�S�p�[�c���j
//---------------------------------------------------------------------------------------
void CalcMatrix()
{
	for (int i = 0; i < JINTAI_PARTS; i++) {
		jintai.parts[i].matrix.m[0][0] =  cosf(jintai.parts[i].angle * RAD);
		jintai.parts[i].matrix.m[0][1] = -sinf(jintai.parts[i].angle * RAD);
		jintai.parts[i].matrix.m[0][2] = 0.0f;
		jintai.parts[i].matrix.m[1][0] =  sinf(jintai.parts[i].angle * RAD);
		jintai.parts[i].matrix.m[1][1] =  cosf(jintai.parts[i].angle * RAD);
		jintai.parts[i].matrix.m[1][2] = 0.0f;
		jintai.parts[i].matrix.m[2][0] = jintai.parts[i].offx;
		jintai.parts[i].matrix.m[2][1] = jintai.parts[i].offy;
		jintai.parts[i].matrix.m[2][2] = 1.0f;
	}
}

//---------------------------------------------------------------------------------------
//	�A�j���[�V�����f�[�^��l�̃��f���ɃZ�b�g
//---------------------------------------------------------------------------------------
void SetAnimData(int index)
{
	for (int i = 0; i < JINTAI_PARTS; i++) {
		jintai.parts[i].angle = anim_data[index].angle[i];
	}
	CalcMatrix();
	jintai.anim_index = index;
}

//---------------------------------------------------------------------------------------
//	�l�̃��f���@���C������
//---------------------------------------------------------------------------------------
void ExecJintai()
{
	static	bool	key_on;
	float			dt, dg;		// ���Ԃ̍����A�p�x�̍���
	int				i;

	switch (jintai.status) {
	case 0:			// ������
		for (i = 0; i < JINTAI_PARTS; i++) {
			jintai.parts[i].parent = jintai_data[i].parent;
			jintai.parts[i].type   = jintai_data[i].type;
			jintai.parts[i].length = jintai_data[i].length;
			jintai.parts[i].offx   = jintai_data[i].offx;
			jintai.parts[i].offy   = jintai_data[i].offy;
			jintai.parts[i].angle  = anim_data[0].angle[i];
		}
		CalcMatrix();
		jintai.timer      = 0;
		jintai.anim_index = 0;
		key_on            = false;
		jintai.status     = 1;
		// THRU
	case 1:			// �L�[���͂Ńt���[���f�[�^���m�F
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {				// �J�[�\���i���j
			if (key_on == false) {
				if (jintai.anim_index > 0) SetAnimData(--jintai.anim_index);
				key_on = true;
			}
		} else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {		// �J�[�\���i�E�j
			if (key_on == false) {
				if (jintai.anim_index < KEYFRAME_MAX - 1) SetAnimData(++jintai.anim_index);
				key_on = true;
			}
		} else {
			key_on = false;
		}
		break;
	case 2:			// �A�j�����s����
		jintai.timer      = 0;
		jintai.anim_index = 0;
		jintai.status     = 3;
		// THRU
	case 3:			// �A�j�����s��

		// ���Ԃ���p�x����`��Ԃ���
		dt = (float)(jintai.timer - anim_data[jintai.anim_index].time) /
					(anim_data[jintai.anim_index + 1].time - anim_data[jintai.anim_index].time);
		for (i = 0; i < JINTAI_PARTS; i++) {
			dg = (float)(anim_data[jintai.anim_index + 1].angle[i] - anim_data[jintai.anim_index].angle[i]);
			jintai.parts[i].angle = anim_data[jintai.anim_index].??? + (int)(??? * ???);
		}

		CalcMatrix();

		// ���Ԃƃt���[����i�߂�
		jintai.timer++;
		if (jintai.timer >= anim_data[jintai.anim_index + 1].time) {
			// ���̃t���[���ֈڍs
			jintai.anim_index++;
			if (jintai.anim_index >= KEYFRAME_MAX - 1) {
				jintai.status = 0;		// �Ō�܂Ŋ��������̂Ńt���[���m�F���[�h��
				g_bOnTimer = false;
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------------------
//	�l�̃��f���@�`�揈��
//---------------------------------------------------------------------------------------
void DrawModel(HDC hdc)
{
	int		i, pr;
	float	x0, y0, x1, y1, xx, yy;
	MATRIX	m0, m1;
	HPEN	hpenOld;

	if (jintai.status == 0) return;		// �܂�����������ĂȂ������瑦�I��

	hpenOld = (HPEN)SelectObject(hdc, g_hpenBlue);	// �y����I��
	for (i = 0; i < JINTAI_PARTS; i++) {
		m0.m[0][0] = jintai.parts[i].matrix.m[0][0];
		m0.m[0][1] = jintai.parts[i].matrix.m[0][1];
		m0.m[0][2] = jintai.parts[i].matrix.m[0][2];
		m0.m[1][0] = jintai.parts[i].matrix.m[1][0];
		m0.m[1][1] = jintai.parts[i].matrix.m[1][1];
		m0.m[1][2] = jintai.parts[i].matrix.m[1][2];
		m0.m[2][0] = jintai.parts[i].matrix.m[2][0];
		m0.m[2][1] = jintai.parts[i].matrix.m[2][1];
		m0.m[2][2] = jintai.parts[i].matrix.m[2][2];
		pr = i;
		while (jintai.parts[pr].parent != -1) {		// ���[���h�܂ł��ǂ�
			// �e�p�[�c�̃}�g���b�N�X����Z����
			pr = jintai.parts[pr].parent;
			m1.m[0][0] =  m0.m[0][0] * jintai.parts[pr].matrix.m[0][0]
						+ m0.m[0][1] * jintai.parts[pr].matrix.m[1][0]
						+ m0.m[0][2] * jintai.parts[pr].matrix.m[2][0];
			m1.m[0][1] =  m0.m[0][0] * jintai.parts[pr].matrix.m[0][1]
						+ m0.m[0][1] * jintai.parts[pr].matrix.m[1][1]
						+ m0.m[0][2] * jintai.parts[pr].matrix.m[2][1];
			m1.m[0][2] =  m0.m[0][0] * jintai.parts[pr].matrix.m[0][2]
						+ m0.m[0][1] * jintai.parts[pr].matrix.m[1][2]
						+ m0.m[0][2] * jintai.parts[pr].matrix.m[2][2];
			m1.m[1][0] =  m0.m[1][0] * jintai.parts[pr].matrix.m[0][0]
						+ m0.m[1][1] * jintai.parts[pr].matrix.m[1][0]
						+ m0.m[1][2] * jintai.parts[pr].matrix.m[2][0];
			m1.m[1][1] =  m0.m[1][0] * jintai.parts[pr].matrix.m[0][1]
						+ m0.m[1][1] * jintai.parts[pr].matrix.m[1][1]
						+ m0.m[1][2] * jintai.parts[pr].matrix.m[2][1];
			m1.m[1][2] =  m0.m[1][0] * jintai.parts[pr].matrix.m[0][2]
						+ m0.m[1][1] * jintai.parts[pr].matrix.m[1][2]
						+ m0.m[1][2] * jintai.parts[pr].matrix.m[2][2];
			m1.m[2][0] =  m0.m[2][0] * jintai.parts[pr].matrix.m[0][0]
						+ m0.m[2][1] * jintai.parts[pr].matrix.m[1][0]
						+ m0.m[2][2] * jintai.parts[pr].matrix.m[2][0];
			m1.m[2][1] =  m0.m[2][0] * jintai.parts[pr].matrix.m[0][1]
						+ m0.m[2][1] * jintai.parts[pr].matrix.m[1][1]
						+ m0.m[2][2] * jintai.parts[pr].matrix.m[2][1];
			m1.m[2][2] =  m0.m[2][0] * jintai.parts[pr].matrix.m[0][2]
						+ m0.m[2][1] * jintai.parts[pr].matrix.m[1][2]
						+ m0.m[2][2] * jintai.parts[pr].matrix.m[2][2];
			m0 = m1;
		}
		// �p�[�c�̕`��p�[�_�����߂�
		xx = 0;		yy = 0;
		x0 = xx * m0.m[0][0] + yy * m0.m[1][0] + m0.m[2][0];
		y0 = xx * m0.m[0][1] + yy * m0.m[1][1] + m0.m[2][1];
		xx = 0;		yy = 0 - jintai.parts[i].length;
		x1 = xx * m0.m[0][0] + yy * m0.m[1][0] + m0.m[2][0];
		y1 = xx * m0.m[0][1] + yy * m0.m[1][1] + m0.m[2][1];
		// �^�C�v�ɂ���ĕ`���ς���
		switch (jintai.parts[i].type) {
		case 0:			// ����
			MoveToEx(hdc, (int)x0 + SCREEN_WIDTH/2, (int)y0 + SCREEN_HEIGHT/2, NULL);
			LineTo  (hdc, (int)x1 + SCREEN_WIDTH/2, (int)y1 + SCREEN_HEIGHT/2);
			break;
		case 1:			// �~
			Ellipse(hdc, (int)(x0 - jintai.parts[i].length/2) + SCREEN_WIDTH /2,
						 (int)(y0 - jintai.parts[i].length  ) + SCREEN_HEIGHT/2,
						 (int)(x0 + jintai.parts[i].length/2) + SCREEN_WIDTH /2,
						 (int)(y0 + SCREEN_HEIGHT/2));
			break;
		}
	}
	SelectObject(hdc, hpenOld);
}

//=======================================================================================
//	end
//=======================================================================================
