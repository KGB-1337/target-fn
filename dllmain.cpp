#include "main.h"
#include "internal_utils.h"
#include "imgui_internal.h"
#include <signal.h>
#include "Mapper/linker.h"
#define OFFSET_UWORLD 0x7F5C978
#define location_Offs 0x7F35748
#define teamIndex 0xE60
#define FLT_MAX          3.402823466e+38F        // max value

// ITEMS RELATED:
#define ITEMS_ACTORID 867287
#define LLAMA_ACTORID 20644444

// debugging here:
int botActorID = 0;
Vector3 entityHeadOut;
bool isFortniteFocus;
bool botsESP = true;
//bool showDateTime = true;

bool ShowMenu = false;
bool visuals = true;
bool pSilent = false;
bool skeleton = true;
bool aimbot = true;
bool itemsEsp = true;
bool itemsDisEsp = true;
bool trapsEsp = true;
bool trapsDisEsp = true;
bool weakSpotsAim = true;
bool isRecFov = false;
bool box = false;
bool lineEsp = false;
bool drawfov = false;
bool playeresp = true;
bool dist = true;
bool aimClosest = false;
bool filled = true;
bool fovchanger = false;
bool debugging = true;
bool enemyDistance = true;
bool drawCrosshair = true;
float snaplinePower = 1.0f;
float AimSmoothPepe = 0.0f;
bool drawSelf = false;
bool isFilled = false;
bool isCornerBox = false;
int aimKeyNuclear = 0;
int crosshairNuclear = 1;
int crosshairScale = 8;
int debuggingVar = 0;
int itemEspDisAmount = 100;
float cornerBoxScale = 1.0f;
unsigned __int64 pickAxeID = 0;
static int camfov = 50;
static int test1337 = 0;
static float test13372 = 1.19f;
bool isPlayerAiming = false;
bool isBodyAim = false;

static int VisDist = 350;
static int AimDist = 350;
int AimSmooth = 3;

static int modepos = 0;
static int hitboxpos = 1;
static int aimkeypos = 0;
static int linemodepos = 0;
static int boxmodepos = 0;
static int hitbox;
static int aimkey;

char* configFile = _strdup(_xor_("C://Program Files//7ds7//doodoo").c_str());

void replaceAll(string& s, const string& search, const string& replace) {
	for (size_t pos = 0; ; pos += replace.length()) {
		// Locate the substring to replace
		pos = s.find(search, pos);
		if (pos == string::npos) break;
		// Replace by erasing and inserting
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void DrawNewText(int x, int y, RGBA* color, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	//ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::PushFontShadow(0xFF000000);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
	ImGui::PopFontShadow();
}

void DrawCircle(int x, int y, int radius, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void DrawLineF(int x1, int y1, int x2, int y2, int thickness, float r, float g, float b, float a)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a)), thickness);
}

void DrawBox(int x1, int y1, int x2, int y2, float r, float g, float b, float a, bool isFilled)
{
	if (isFilled)
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x1, y1), ImVec2(x1 + x2, y1 + y2), ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a)));
	else
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x1 + x2, y1 + y2), ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a)));
}

void writerotator(unsigned long long int Address, FRotator stuff)
{
	info_t Input_Output_Data;

	Input_Output_Data.pid = processID;

	Input_Output_Data.address = (void*)Address;

	Input_Output_Data.value = &stuff;

	Input_Output_Data.size = sizeof(float);

	unsigned long int Readed_Bytes_Amount;

	DeviceIoControl(DriverHandle, ctl_write, &Input_Output_Data, sizeof Input_Output_Data, &Input_Output_Data, sizeof Input_Output_Data, &Readed_Bytes_Amount, nullptr);
}

void writefloat(unsigned long long int Address, float stuff)
{
	info_t Input_Output_Data;

	Input_Output_Data.pid = processID;

	Input_Output_Data.address = (void*)Address;

	Input_Output_Data.value = &stuff;

	Input_Output_Data.size = sizeof(float);

	unsigned long int Readed_Bytes_Amount;

	DeviceIoControl(DriverHandle, ctl_write, &Input_Output_Data, sizeof Input_Output_Data, &Input_Output_Data, sizeof Input_Output_Data, &Readed_Bytes_Amount, nullptr);
}

DWORD loopThread(LPVOID in)
{
	SetupWindow();
	DirectXInit(MyWnd);
	while (1)
	{
		MainLoop();
		Sleep(5);
	}
//	return true;
}

DWORD Menukey(LPVOID in)
{
	const auto KEY_DOWN = 0x8000;
	bool wasDown = false;
	while (1)
	{
		if (isFortniteFocus) {
			if (GetAsyncKeyState(VK_INSERT) & 1) {
				wasDown = true;
			}
			if ((GetAsyncKeyState(VK_INSERT) & KEY_DOWN) == 0) {
				if (wasDown) {
					ShowMenu = !ShowMenu;
					wasDown = false;
				}
			}
		}
		Sleep(5);
	}
}

Vector3 Clamp(Vector3 r)
{
	if (r.y > 180.f)
		r.y -= 360.f;
	else if (r.y < -180.f)
		r.y += 360.f;

	if (r.y > 180.f)
		r.y -= 360.f;
	else if (r.y < -180.f)
		r.y += 360.f;

	if (r.x < -89.f)
		r.x = -89.f;
	else if (r.x > 89.f)
		r.x = 89.f;

	r.z = 0.f;

	return r;
}

double RAD2DEG(int x)
{
	double degrees;

	degrees = x * 180 / M_PI;
	return degrees;
}

Vector3 Vec2Rot(Vector3 vec)
{
	Vector3 rot;

	rot.y = RAD2DEG(std::atan2f(vec.y, vec.x));
	rot.x = RAD2DEG(std::atan2f(vec.z, std::sqrtf(vec.x * vec.x + vec.y * vec.y)));
	rot.z = 0.f;

	return rot;
}


DWORD init(LPVOID)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, _xor_("conin$").c_str(), _xor_("r+t").c_str(), stdin);
	freopen_s(&f, _xor_("conout$").c_str(), _xor_("w+t").c_str(), stdout);
	freopen_s(&f, _xor_("conout$").c_str(), _xor_("w+t").c_str(), stderr);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 100, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);

	DriverHandle = CreateFileW(_xor_(L"\\\\.\\targetudoodoo").c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (DriverHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << _xor_("\n\n   [!] Failed to load!");
		printf(_xor_("\n\n   [!] Quitting..").c_str());
		Sleep(3000);
		exit(3);
	}

	cout << _xor_("\n\n   TargetFN ").c_str();
	cout << _xor_(" - External").c_str();
	
	Sleep(800);

	cout << _xor_("\n\n   Looking for Fortnite..");

	info_t Input_Output_Data1;
	unsigned long int Readed_Bytes_Amount1;
	DeviceIoControl(DriverHandle, ctl_clear, &Input_Output_Data1, sizeof Input_Output_Data1, &Input_Output_Data1, sizeof Input_Output_Data1, &Readed_Bytes_Amount1, nullptr);
	
	while (hwnd == NULL)
	{
		hwnd = FindWindowA(0, _xor_("Fortnite  ").c_str());
		Sleep(1000);
	}

	std::cout << _xor_("\n\n   Game was found!");

	Sleep(1000);
	std::cout << _xor_("\n\n   Loading..");
	GetWindowThreadProcessId(hwnd, &processID);

	info_t Input_Output_Data;
	Input_Output_Data.pid = processID;
	unsigned long int Readed_Bytes_Amount;

	DeviceIoControl(DriverHandle, ctl_base, &Input_Output_Data, sizeof Input_Output_Data, &Input_Output_Data, sizeof Input_Output_Data, &Readed_Bytes_Amount, nullptr);

	base_address = (unsigned long long int)Input_Output_Data.data;

	std::cout << _xor_("\n\n   TargetFN Loaded!");
	Sleep(1000);
	::ShowWindow(consoleWindow, SW_HIDE);

	CreateThread(NULL, NULL, loopThread, NULL, NULL, NULL);
	CreateThread(NULL, NULL, Menukey, NULL, NULL, NULL);
}

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = ReadFNMemory<DWORD_PTR>(mesh + 0x420);
	if (bonearray == NULL)
		return FTransform();

	return ReadFNMemory<FTransform>(bonearray + (index * 0x30));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = ReadFNMemory<FTransform>(mesh + 0x1C0);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation, Vector3 camrot)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Rotation = camrot; // FRotator

	D3DMATRIX tempMatrix = Matrix(Rotation);

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 camloc = ReadFNMemory<Vector3>(base_address + location_Offs);

	Vector3 vDelta = WorldLocation - camloc;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	uint64_t zoomSubBase = ReadFNMemory<uint64_t>(Localplayer + 0xA8);
	uint64_t zoomBase = ReadFNMemory<uint64_t>(zoomSubBase + 8);
	float zoom = ReadFNMemory<float>(zoomBase + 0x500);

	if (fovchanger)
	{
		if (camfov == 50)
		{
			test1337 = 30;
			test13372 = 1.19f;
		}
		else if (camfov == 60)
		{
			test1337 = 23;
			test13372 = 1.277f;
		}
		else if (camfov == 70)
		{
			test1337 = 20;
			test13372 = 1.277f;
		}
		else if (camfov == 80)
		{
			test1337 = 17;
			test13372 = 1.277f;
		}
		else if (camfov == 90)
		{
			test1337 = 14;
			test13372 = 1.277f;
		}
		else if (camfov == 100)
		{
			test1337 = 11;
			test13372 = 1.277f;
		}
		else if (camfov == 110)
		{
			test1337 = 9;
			test13372 = 1.19f;
		}
		else if (camfov = 120)
		{
			test1337 = 6;
			test13372 = 1.277f;
		}
	}
	else
	{
		camfov = 80;
		test1337 = 0;
		test13372 = 1.19f;
	}
	float FovAngle = camfov + test1337 / (zoom / test13372);

	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

Vector3 Camera(unsigned __int64 RootComponent)
{
	Vector3 Camera;

	auto pitchE = ReadFNMemory<uintptr_t>(Localplayer + 0xA8);
	auto pitch = ReadFNMemory<uintptr_t>(pitchE + 8);
	Camera.x = ReadFNMemory<float>(RootComponent + 0x12C);
	Camera.y = ReadFNMemory<float>(pitch + 0x678);

	float test = asin(Camera.y);
	float degrees = test * (180.0 / M_PI);

	Camera.y = degrees;

	if (Camera.x < 0)
		Camera.x = 360 + Camera.x;

	return Camera;
}

bool GetAimKey()
{
	return (GetAsyncKeyState(aimkey));
}

void WriteAngles(float TargetX, float TargetY)
{
	float theNum = floor(TargetX / AimSmooth);
	float result = theNum / 6.666666666666667f;

	float theNum1 = floor(TargetY / AimSmooth);
	float resulte = theNum1 / 6.666666666666667f;
	float result1 = -(resulte);

	writefloat(PlayerController + 0x418 + 0x0, result1);
	writefloat(PlayerController + 0x418 + 0x4, result);
}

void AimbotNow(float x, float y)
{
	
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);
	int   AimSpeed = AimSmooth;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}
	// mem aim only.
	
	float theNum = floor(TargetX / AimSpeed);
	float result = theNum / 6.666666666666667f;

	float theNum1 = floor(TargetY / AimSpeed);
	float resulte = theNum1 / 6.666666666666667f;
	float result1 = -(resulte);

	writefloat(PlayerController + 0x418 + 0x0, result1);
	writefloat(PlayerController + 0x418 + 0x4, result);

	return;
}

double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

bool isLocalPlayer(Vector3 loclplayer, Vector3 player) {

	if (loclplayer.x == player.x && loclplayer.y == player.y && loclplayer.z == player.z)
		return true;

	return false;
}

std::vector<DWORD_PTR> candidates;
std::vector<Vector3> candidatesPositions;
DWORD_PTR GetClosestPlayerToLocalPlayer(float maxDistance)
{
	Vector3 localPos = ReadFNMemory<Vector3>(Rootcomp + 0x11C);

	DWORD_PTR closestPlayer = (DWORD_PTR)nullptr;

	float distance = FLT_MAX;
	float closestDistance = FLT_MAX;

	for (auto candidate : candidates)
	{
		Vector3 playerLoc;

		auto currentactormesh = ReadFNMemory<uint64_t>(candidate + 0x278);
		playerLoc = GetBoneWithRotation(currentactormesh, 66);

		float curDist = localPos.Distance(playerLoc) / 100.f;

		if (curDist < distance && curDist < maxDistance)
		{
			distance = curDist;
			closestPlayer = candidate;
		}
	}

	return closestPlayer;
}

bool GetClosestPlayerToCrossHair(Vector3 Pos, float& max, float aimfov, DWORD_PTR entity, float maxDistance)
{
	if (!GetAimKey() || !isaimbotting)
	{
		if (entity)
		{
			float Dist = GetCrossDistance(Pos.x, Pos.y, Width / 2, Height / 2);

			if (Dist < max)
			{
				Vector3 localPos = ReadFNMemory<Vector3>(Rootcomp + 0x11C);

				DWORD_PTR closestPlayer = (DWORD_PTR)nullptr;

				float distance = FLT_MAX;
				float closestDistance = FLT_MAX;
				Vector3 playerLoc;

				auto currentactormesh = ReadFNMemory<uint64_t>(entity + 0x278);
				playerLoc = GetBoneWithRotation(currentactormesh, 66);

				float curDist = localPos.Distance(playerLoc) / 100.f;

				if (curDist < distance && curDist < maxDistance)
				{
					distance = curDist;
					closestPlayer = entity;
					max = Dist;
					aimfov = aimfov;
					entityx = entity;
				}
			}
		}
	}
	return false;
}

void newAim(DWORD_PTR enemy, uint64_t currentactormesh, Vector3 camrot) {
	Vector3 relativelocation = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 screenlocation = ProjectWorldToScreen(relativelocation, camrot);

	float headX = screenlocation.x - Width;
	float headY = screenlocation.y - Height;
	Vector3 Localcam = Camera(Rootcomp);

	auto rootHead = GetBoneWithRotation(currentactormesh, hitbox); // change to hitbox later
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));

	auto pos = rootHeadOut;
	//auto delta = (pos - camloc);
	auto delta = pos;

	Vector3 ideal = Clamp(rootHeadOut);
	auto angDelta = Clamp(ideal - camrot);

	string tabak = _xor_("AIMBOT: \nClamp X: ");

	std::stringstream ss;
	ss << Clamp(angDelta).x;
	string s;
	ss >> s;
	tabak += s;

	tabak += "\nClamp Y: ";
	std::stringstream ss1;
	ss1 << Clamp(angDelta).y;
	string s1;
	ss1 >> s1;
	tabak += s1;

	tabak += "\n AngDelta X: ";
	std::stringstream ss2;
	ss2 << angDelta.x;
	string s2;
	ss2 >> s2;
	tabak += s2;

	tabak += "\n AngDelta Y: ";
	std::stringstream ss3;
	ss3 << angDelta.y;
	string s3;
	ss3 >> s3;
	tabak += s3;

	tabak += "\n Ideal X: ";
	std::stringstream ss4;
	ss4 << ideal.x;
	string s4;
	ss4 >> s4;
	tabak += s4;

	tabak += "\n Ideal Y: ";
	std::stringstream ss5;
	ss5 << ideal.y;
	string s5;
	ss5 >> s5;
	tabak += s5;

	DrawNewText(105, 384, &Col.white_, tabak.c_str());

	DrawNewText(90, 384, &Col.white_, "HIHI:");

}

void AimAt(DWORD_PTR entity, Vector3 Localcam)
{
	if (aimClosest) {
		// we'll ignore the entity variable for now.
		float j = (float)AimDist;
		float MaxAimbotDistance = j;
		DWORD_PTR dentity = GetClosestPlayerToLocalPlayer(MaxAimbotDistance);

		uint64_t currentactormesh = ReadFNMemory<uint64_t>(dentity + 0x278);
		auto rootHead = GetBoneWithRotation(currentactormesh, hitbox); // change to hitbox later
		Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));

		if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
		{
			if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, ScreenCenterX, ScreenCenterY) <= AimFOV * 8))
			{
				AimbotNow(rootHeadOut.x, rootHeadOut.y);
			}
		}
	}
	else {
		uint64_t currentactormesh = ReadFNMemory<uint64_t>(entity + 0x278);
		auto rootHead = GetBoneWithRotation(currentactormesh, hitbox); // change to hitbox later
		Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));

		if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
		{
			if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, ScreenCenterX, ScreenCenterY) <= AimFOV * 8))
			{
				AimbotNow(rootHeadOut.x, rootHeadOut.y);
			}
		}
	}

}

void Aimbot(Vector3 Localcam)
{
	// NO AIM WHEN NOT IN FORTNITE
	if (isFortniteFocus && entityx != 0)
	{
		if (GetAimKey())
		{
			//auto rootHead = GetBoneWithRotation(currentactormesh, hitbox);
			//Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));
			AimAt(entityx, Localcam);
			//writefloat(entityx + 0x492, (5 / 100));
		}
		else
		{
			isaimbotting = false;
		}
	}
}

bool IsInLobby()
{
	ULONG Identifier = 10;

	Identifier = ReadFNMemory<float>((ULONGLONG)base_address + 0x7D393B0);

	if (Identifier == 1)
		return true;

	else return false;
}

void AIms(DWORD_PTR entity, Vector3 Localcam)
{
	float max = 100.f;
	//float max = AimFOV;

	uint64_t currentactormesh = ReadFNMemory<uint64_t>(entity + 0x278);

	Vector3 rootHead = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));
	//Vector3 Pos, float& max, float aimfov, DWORD_PTR entity, float maxDistance
	if (GetClosestPlayerToCrossHair(rootHeadOut, max, AimFOV, entity, 350)) {
		entityx = entity;
		candidates.push_back(entity);
	}
}

int colorOrder = 1;
float playersSavedColors[100][6]{};
int playerID = 0;
int enemyID = 0;
bool isFirstTime = true;
int ScreenX = GetSystemMetrics(SM_CXSCREEN) / 2;
int ScreenY = GetSystemMetrics(SM_CYSCREEN);
int ScreenXx = GetSystemMetrics(SM_CXSCREEN) / 2;
int ScreenYy = GetSystemMetrics(SM_CYSCREEN) / 20000;
int ScreenYyy = GetSystemMetrics(SM_CYSCREEN) / 2;

DWORD DrawESP(){
	HWND test = FindWindowA(0, _xor_("Fortnite  ").c_str());

	if (test == NULL)
	{
		ExitProcess(0);
	}

	if (hitboxpos == 0)
	{
		hitbox = BONE_HEAD;//head
	}
	else if (hitboxpos == 1)
	{
		hitbox = BONE_NECK;//neck
	}
	else if (hitboxpos == 2)
	{
		hitbox = BONE_TORSO;//body
	}
	else if (hitboxpos == 3)
	{
		hitbox = 0;//root
	}
	else if (hitboxpos == 4)
	{
		hitbox = BONE_PELVIS_1;//pelvis
	}

	if (aimkeypos == 0)
	{
		aimkey = 0x02;//right mouse button
	}
	else if (aimkeypos == 1)
	{
		aimkey = 0x01;//left mouse button
	}
	else if (aimkeypos == 2)
	{
		aimkey = 0x04;//middle mouse button
	}
	else if (aimkeypos == 3)
	{
		aimkey = 0x05;//x1 mouse button
	}
	else if (aimkeypos == 4)
	{
		aimkey = 0x06;//x2 mouse button
	}
	else if (aimkeypos == 5)
	{
		aimkey = 0x03;//control break processing
	}
	else if (aimkeypos == 6)
	{
		aimkey = 0x08;//backspace
	}
	else if (aimkeypos == 7)
	{
		aimkey = 0x09;//tab
	}
	else if (aimkeypos == 8)
	{
		aimkey = 0x0c;//clear
	}
	else if (aimkeypos == 9)
	{
		aimkey == 0x0D;//enter
	}
	else if (aimkeypos == 10)
	{
		aimkey = 0x10;//shift
	}
	else if (aimkeypos == 11)
	{
		aimkey = 0x11;//ctrl
	}
	else if (aimkeypos == 12)
	{
		aimkey == 0x12;//alt
	}
	else if (aimkeypos == 13)
	{
		aimkey == 0x14;//caps lock
	}
	else if (aimkeypos == 14)
	{
		aimkey == 0x1B;//esc
	}
	else if (aimkeypos == 15)
	{
		aimkey == 0x20;//space
	}
	else if (aimkeypos == 16)
	{
		aimkey == 0x30;//0
	}
	else if (aimkeypos == 17)
	{
		aimkey == 0x31;//1
	}
	else if (aimkeypos == 18)
	{
		aimkey == 0x32;//2
	}
	else if (aimkeypos == 19)
	{
		aimkey == 0x33;//3
	}
	else if (aimkeypos == 20)
	{
		aimkey == 0x34;//4
	}
	else if (aimkeypos == 21)
	{
		aimkey == 0x35;//5
	}
	else if (aimkeypos == 22)
	{
		aimkey == 0x36;//6
	}
	else if (aimkeypos == 23)
	{
		aimkey == 0x37;//7
	}
	else if (aimkeypos == 24)
	{
		aimkey == 0x38;//8
	}
	else if (aimkeypos == 25)
	{
		aimkey == 0x39;//9
	}
	else if (aimkeypos == 26)
	{
		aimkey == 0x41;//a
	}
	else if (aimkeypos == 27)
	{
		aimkey == 0x42;//b
	}
	else if (aimkeypos == 28)
	{
		aimkey == 0x43;//c
	}
	else if (aimkeypos == 29)
	{
		aimkey == 0x44;//d
	}
	else if (aimkeypos == 30)
	{
		aimkey == 0x45;//e
	}
	else if (aimkeypos == 31)
	{
		aimkey == 0x46;//f
	}
	else if (aimkeypos == 32)
	{
		aimkey == 0x47;//g
	}
	else if (aimkeypos == 33)
	{
		aimkey == 0x48;//h
	}
	else if (aimkeypos == 34)
	{
		aimkey == 0x49;//i
	}
	else if (aimkeypos == 35)
	{
		aimkey == 0x4A;//j
	}
	else if (aimkeypos == 36)
	{
		aimkey == 0x4B;//k
	}
	else if (aimkeypos == 37)
	{
		aimkey == 0x4C;//L
	}
	else if (aimkeypos == 38)
	{
		aimkey == 0x4D;//m
	}
	else if (aimkeypos == 39)
	{
		aimkey == 0x4E;//n
	}
	else if (aimkeypos == 40)
	{
		aimkey == 0x4F;//o
	}
	else if (aimkeypos == 41)
	{
		aimkey == 0x50;//p
	}
	else if (aimkeypos == 42)
	{
		aimkey == 0x51;//q
	}
	else if (aimkeypos == 43)
	{
		aimkey == 0x52;//r
	}
	else if (aimkeypos == 44)
	{
		aimkey == 0x53;//s
	}
	else if (aimkeypos == 45)
	{
		aimkey == 0x54;//t
	}
	else if (aimkeypos == 46)
	{
		aimkey == 0x55;//u
	}
	else if (aimkeypos == 47)
	{
		aimkey == 0x56;//v
	}
	else if (aimkeypos == 48)
	{
		aimkey == 0x57;//w
	}
	else if (aimkeypos == 49)
	{
		aimkey == 0x58;//x
	}
	else if (aimkeypos == 50)
	{
		aimkey == 0x59;//y
	}
	else if (aimkeypos == 51)
	{
		aimkey == 0x5A;//z
	}
	else if (aimkeypos == 52)
	{
		aimkey == 0x60;//numpad 0
	}
	else if (aimkeypos == 53)
	{
		aimkey == 0x61;//numpad 1
	}
	else if (aimkeypos == 54)
	{
		aimkey == 0x62;//numpad 2
	}
	else if (aimkeypos == 55)
	{
		aimkey == 0x63;//numpad 3
	}
	else if (aimkeypos == 56)
	{
		aimkey == 0x64;//numpad 4
	}
	else if (aimkeypos == 57)
	{
		aimkey == 0x65;//numpad 5
	}
	else if (aimkeypos == 58)
	{
		aimkey == 0x66;//numpad 6
	}
	else if (aimkeypos == 59)
	{
		aimkey == 0x67;//numpad 7
	}
	else if (aimkeypos == 60)
	{
		aimkey == 0x68;//numpad 8
	}
	else if (aimkeypos == 61)
	{
		aimkey == 0x69;//numpad 9
	}
	else if (aimkeypos == 62)
	{
		aimkey == 0x6A;//multiply
	}
	float radiusx = AimFOV * (ScreenCenterX / 100);
	float radiusy = AimFOV * (ScreenCenterY / 100);

	float calcradius = (radiusx + radiusy) / 2;
	if (drawfov)
	{
		//ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), calcradius, 0xFFFFFFFF, 25);
		if (isRecFov) {
			DrawLine((float)(Width / 2) - AimFOV, (float)(Height / 2) - AimFOV, (float)(Width / 2) + AimFOV, (float)(Height / 2) - AimFOV, &Col.white, 1);
			DrawLine((float)(Width / 2) - AimFOV, (float)(Height / 2) + AimFOV, (float)(Width / 2) + AimFOV, (float)(Height / 2) + AimFOV, &Col.white, 1);
			DrawLine((float)(Width / 2) + AimFOV, (float)(Height / 2) - AimFOV, (float)(Width / 2) + AimFOV, (float)(Height / 2) + AimFOV, &Col.white, 1);
			DrawLine((float)(Width / 2) - AimFOV, (float)(Height / 2) - AimFOV, (float)(Width / 2) - AimFOV, (float)(Height / 2) + AimFOV, &Col.white, 1);
		}
		else
			ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), AimFOV, 0xFFFFFFFF, 25);
	}
	
	if (drawCrosshair) {
		DrawLine((float)(Width / 2), (float)(Height / 2) - crosshairScale, (float)(Width / 2), (float)(Height / 2) + crosshairScale, &Col.white, 1);
		DrawLine((float)(Width / 2) - crosshairScale, (float)(Height / 2), (float)(Width / 2) + crosshairScale, (float)(Height / 2), &Col.white, 1);
	}

	Uworld = ReadFNMemory<DWORD_PTR>(base_address + OFFSET_UWORLD);
	DWORD_PTR Gameinstance = ReadFNMemory<DWORD_PTR>(Uworld + 0x1C8);

	if (Gameinstance == (DWORD_PTR)nullptr)
		return 0;

	DWORD_PTR LocalPlayers = ReadFNMemory<DWORD_PTR>(Gameinstance + 0x38);

	if (LocalPlayers == (DWORD_PTR)nullptr)
		return 0;

	Localplayer = ReadFNMemory<DWORD_PTR>(LocalPlayers);

	if (Localplayer == (DWORD_PTR)nullptr)
		return 0;

	/*
	if (fovchanger)
	{
		if (camfov)
		{
			writefloat(Localplayer + 0x3F8, camfov);
		}
	}
	*/

	PlayerController = ReadFNMemory<DWORD_PTR>(Localplayer + 0x30);

	if (PlayerController == (DWORD_PTR)nullptr)
		return 0;

	LocalPawn = ReadFNMemory<DWORD_PTR>(PlayerController + 0x298);

	if (LocalPawn == (DWORD_PTR)nullptr)
		return 0;

	Rootcomp = ReadFNMemory<DWORD_PTR>(LocalPawn + 0x130);

	if (Rootcomp == (DWORD_PTR)nullptr)
		return 0;

	if (LocalPawn != 0) {
		localplayerID = ReadFNMemory<int>(LocalPawn + 0x18);
	}

	Ulevel = ReadFNMemory<DWORD_PTR>(Uworld + 0x30);

	if (Ulevel == (DWORD_PTR)nullptr)
		return 0;

	DWORD ActorCount = ReadFNMemory<DWORD>(Ulevel + 0xA0);

	DWORD_PTR AActors = ReadFNMemory<DWORD_PTR>(Ulevel + 0x98);

	DWORD64 PlayerState = ReadFNMemory<DWORD64>(LocalPawn + 0x238);

	Vector3 Localpos = ReadFNMemory<Vector3>(Rootcomp + 0x11C);

	if (AActors == (DWORD_PTR)nullptr)
		return 0;

	Vector3 Localcam = Camera(Rootcomp);

	candidates.clear();
	candidatesPositions.clear();

	int revise = 0;
	int radius = 0;
	float LowestDistance = FLT_MAX;
	DWORD64 toApuntar;
	float closestDistance = 0.f;

	for (int i = 0; i < ActorCount; i++)
	{
		if (ActorCount > 1)
		{
			uint64_t CurrentActor = ReadFNMemory<uint64_t>(AActors + i * 0x8);

			if (CurrentActor == (uint64_t)nullptr || CurrentActor == -1 || CurrentActor == NULL)
				continue;

			int curactorid = ReadFNMemory<int>(CurrentActor + 0x18);

			if (curactorid == localplayerID || (botsESP && isFirstTime && curactorid > 20500000 && curactorid < 20900000) || (botsESP && botActorID != 0 && botActorID == curactorid))
			{
				uint64_t CurrentActorRootComponent = ReadFNMemory<uint64_t>(CurrentActor + 0x130);

				if (CurrentActorRootComponent == (uint64_t)nullptr || CurrentActorRootComponent == -1 || CurrentActorRootComponent == NULL)
					continue;

				uint64_t currentactormesh = ReadFNMemory<uint64_t>(CurrentActor + 0x278);

				if (currentactormesh == (uint64_t)nullptr || currentactormesh == -1 || currentactormesh == NULL)
					continue;

				Vector3 BoxHeadPos = GetBoneWithRotation(currentactormesh, 66);
				Vector3 Headpos = GetBoneWithRotation(currentactormesh, 66);
				Vector3 rootOut = GetBoneWithRotation(currentactormesh, 0);
				Vector3 actorpos = ReadFNMemory<Vector3>(CurrentActorRootComponent + 0x11C);

				if (!drawSelf && isLocalPlayer(Localpos, actorpos))
					continue;

				int MyTeamId = ReadFNMemory<int>(PlayerState + teamIndex);
				DWORD64 otherPlayerState = ReadFNMemory<uint64_t>(CurrentActor + 0x238);
				int ActorTeamId = ReadFNMemory<int>(otherPlayerState + teamIndex);

				Vector3 vHeadBone = GetBoneWithRotation(currentactormesh, 96);
				Vector3 vHip = GetBoneWithRotation(currentactormesh, 2);
				Vector3 vNeck = GetBoneWithRotation(currentactormesh, 65);
				Vector3 vUpperArmLeft = GetBoneWithRotation(currentactormesh, 34);
				Vector3 vUpperArmRight = GetBoneWithRotation(currentactormesh, 91);
				Vector3 vLeftHand = GetBoneWithRotation(currentactormesh, 35);
				Vector3 vRightHand = GetBoneWithRotation(currentactormesh, 63);
				Vector3 vLeftHand1 = GetBoneWithRotation(currentactormesh, 33);
				Vector3 vRightHand1 = GetBoneWithRotation(currentactormesh, 60);
				Vector3 vRightThigh = GetBoneWithRotation(currentactormesh, 74);
				Vector3 vLeftThigh = GetBoneWithRotation(currentactormesh, 67);
				Vector3 vRightCalf = GetBoneWithRotation(currentactormesh, 75);
				Vector3 vLeftCalf = GetBoneWithRotation(currentactormesh, 68);
				Vector3 vLeftFoot = GetBoneWithRotation(currentactormesh, 69);
				Vector3 vRightFoot = GetBoneWithRotation(currentactormesh, 76);

				Vector3 vHeadBoneOut = ProjectWorldToScreen(vHeadBone, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vHipOut = ProjectWorldToScreen(vHip, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vNeckOut = ProjectWorldToScreen(vNeck, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vUpperArmLeftOut = ProjectWorldToScreen(vUpperArmLeft, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vUpperArmRightOut = ProjectWorldToScreen(vUpperArmRight, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftHandOut = ProjectWorldToScreen(vLeftHand, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightHandOut = ProjectWorldToScreen(vRightHand, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftHandOut1 = ProjectWorldToScreen(vLeftHand1, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightHandOut1 = ProjectWorldToScreen(vRightHand1, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightThighOut = ProjectWorldToScreen(vRightThigh, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftThighOut = ProjectWorldToScreen(vLeftThigh, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightCalfOut = ProjectWorldToScreen(vRightCalf, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftCalfOut = ProjectWorldToScreen(vLeftCalf, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vLeftFootOut = ProjectWorldToScreen(vLeftFoot, Vector3(Localcam.y, Localcam.x, Localcam.z));
				Vector3 vRightFootOut = ProjectWorldToScreen(vRightFoot, Vector3(Localcam.y, Localcam.x, Localcam.z));

				Vector3 bone0 = GetBoneWithRotation(currentactormesh, 0);
				Vector3 bottom = ProjectWorldToScreen(bone0, Vector3(Localcam.y, Localcam.x, Localcam.z));

				float distance = Localpos.Distance(Headpos) / 100.f;

				//if (distance < 1.5f)
				//	continue;

				//auto pawn = ReadFNMemory<DWORD_PTR>(CurrentActor);
				bool bIsDying = ReadFNMemory<bool>(CurrentActor + 0x520);

				if (ActorTeamId != MyTeamId && !(bIsDying & 1))
					//if (1 != 2)
				{
					Vector3 HeadposW2s = ProjectWorldToScreen(Headpos, Vector3(Localcam.y, Localcam.x, Localcam.z));
					Vector3 Out = ProjectWorldToScreen(rootOut, Vector3(Localcam.y, Localcam.x, Localcam.z));
					Vector3 BoxHead = ProjectWorldToScreen(BoxHeadPos, Vector3(Localcam.y, Localcam.x, Localcam.z));

					if (lineEsp)
					{
						if (visuals)
						{
							if (distance < VisDist)
							{
								if (linemodepos == 0)
								{
									DrawLine(ScreenX, ScreenY, bottom.x, bottom.y, &Col.orange, snaplinePower);//bottom
								}
								else if (linemodepos == 1)
								{
									DrawLine(ScreenXx, ScreenYy, HeadposW2s.x, HeadposW2s.y, &Col.orange, snaplinePower);//top
								}
								else if (linemodepos == 2)
								{
									DrawLine(ScreenX, ScreenYyy, vHipOut.x, vHipOut.y, &Col.orange, snaplinePower);//center
								}
							}
						}
					}

					if (playeresp)
					{
						playerID += 1;
						if (visuals)
						{
							if (distance < VisDist)
							{
								float Height1 = abs(BoxHead.y - bottom.y);
								float Width1 = Height1 * 0.65;


								char cName[55];
								if (curactorid == localplayerID) {
									sprintf_s(cName, _xor_("Player #%d").c_str(), playerID);
								}
								else {
									sprintf_s(cName, _xor_("Bot #%d").c_str(), playerID);
									if (isFirstTime) {
										isFirstTime = false;
										botActorID = curactorid;
									}
								}
								revise = strlen(cName) * 7 + 28;
								int textX = (int)(Out.x - (revise / 2) + (Rect.w / 2));

								//DrawNewText(10, 10, &Col.green, ("MY STATE: " + str1).c_str());

								if (enemyDistance)
								{
									DrawNewText(BoxHead.x + 40 + -130 + 28 + 42 + -6, BoxHead.y - 25 + -14 + 2, &Col.white_, cName);
									CHAR dist[50];
									sprintf_s(dist, _xor_("%s").c_str(), Util::DistanceToString(distance));
									DrawNewText(BoxHead.x + 40 - 130 + 28 - 2, BoxHead.y - 25 + -14 + 2, &Col.white_, dist);
								}
								else {
									if (curactorid == localplayerID) {
										DrawNewText(BoxHead.x + 40 + -130 + 28 + 42 + -6 - 1, BoxHead.y - 25 + -14 + 2, &Col.white_, cName);
									}
									else {
										DrawNewText(BoxHead.x + 40 + -130 + 28 + 42 + -6 + 4, BoxHead.y - 25 + -14 + 2, &Col.white_, cName);
									}
								}
							}
						}
					}
					if (skeleton)
					{
						if (visuals)
						{
							if (distance < VisDist)
							{
								if (distance > 0)
									radius = 50 / distance;

								RGBA ESPcolor = Col.orange;
								DrawLine(vHipOut.x, vHipOut.y, vNeckOut.x, vNeckOut.y, &ESPcolor, 2);

								DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &ESPcolor, 2);
								DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &ESPcolor, 2);

								DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &ESPcolor, 2);
								DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &ESPcolor, 2);

								DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &ESPcolor, 2);
								DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &ESPcolor, 2);

								DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &ESPcolor, 2);
								DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &ESPcolor, 2);

								DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &ESPcolor, 2);
								DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &ESPcolor, 2);

								DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &ESPcolor, 2);
								DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &ESPcolor, 2);

							}
						}
					}
					if (aimbot)
					{
						if (distance <= AimDist)
						{
							AIms(CurrentActor, Localcam);
						}
					}
				}
			}
			else if (itemsEsp && (curactorid == LLAMA_ACTORID || curactorid == ITEMS_ACTORID)) {
				uint64_t CurrentActorRootComponent = ReadFNMemory<uint64_t>(CurrentActor + 0x130);
				if (CurrentActorRootComponent == (uint64_t)nullptr) continue;

				Vector3 localactorpos = ReadFNMemory<Vector3>(Rootcomp + 0x11C);
				Vector3 actorpos = ReadFNMemory<Vector3>(CurrentActorRootComponent + 0x11C);
				Vector3 actorposW2s = ProjectWorldToScreen(actorpos, Vector3(Localcam.y, Localcam.x, Localcam.z));

				float distance = localactorpos.Distance(actorpos) / 100.f;

				if (distance >= 100) // distance of 100 max
					continue;

				if (distance <= 1) // distance of 100 max
					continue;

				char cName[64];
				if (itemsDisEsp) {
					if (curactorid == LLAMA_ACTORID)
						sprintf_s(cName, _xor_("[ Llama! %s ]").c_str(), Util::DistanceToString(distance));
					else
						sprintf_s(cName, _xor_("[ Item %s ]").c_str(), Util::DistanceToString(distance));
				}
				else {
					if (curactorid == LLAMA_ACTORID)
						sprintf_s(cName, _xor_("[ Llama! ]").c_str());
					else
						sprintf_s(cName, _xor_("[ Item ]").c_str());
				}

				if (curactorid == LLAMA_ACTORID)
					DrawNewText(actorposW2s.x, actorposW2s.y, &Col.pink_, cName);
				else
					DrawNewText(actorposW2s.x, actorposW2s.y, &Col.white_, cName);
			}
		}
	}

	playerID = 0;
	enemyID = 0;
	Aimbot(Localcam);

	return 0;
}

HRESULT DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	
	ImGui::StyleColorsLight();

	io.Fonts->AddFontFromFileTTF(_xor_("C:\\Windows\\Fonts\\Arial.ttf").c_str(), 15);

	p_Object->Release();
	return S_OK;
}

void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetOverlayDrawList();

	float height = ImGui::GetFrameHeight() - 10;
	float width = height * 1.45f;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.05f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim); 
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.185f, 0.62f, 1.075f, 1.00f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.185f, 0.62f, 1.075f, 1.00f), t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f + 1.0f, IM_COL32(0, 0, 0, 255));
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

void SetupWindow()
{
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, nullptr, 0, nullptr);

	WNDCLASSEX wClass =
	{
		sizeof(WNDCLASSEX),
		0,
		WinProc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		M_Name,
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wClass)) {
		exit(1);
	}

	if (hwnd)
	{
		GetClientRect(hwnd, &GameRect);
		POINT xy;
		ClientToScreen(hwnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else
		exit(2);

	MyWnd = CreateWindowEx(NULL, M_Name, M_Name, WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, Width, Height, NULL, NULL, 0, NULL);

	DwmExtendFrameIntoClientArea(MyWnd, &Margin);

	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}


void render() {

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiStyle* style = &ImGui::GetStyle();

	if (ShowMenu)
	{
		// put ur menu here
	}
	ImGui::End();

	DrawESP();

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == hwnd) {
			isFortniteFocus = true;
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			isFortniteFocus = false;
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;
		io.IniFilename = NULL;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();
		//creater
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}

int isTopwin()
{
	HWND hWnd = GetForegroundWindow();

	if (hWnd == hwnd)
		return TopWindowGame;

	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}

void SetWindowToTarget()
{
	while (true)
	{
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
		else
		{
			MessageBox(0, _xor_("XZR").c_str(), _xor_("ZMD").c_str(), MB_OK | MB_ICONERROR);
			exit(0);
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, init, nullptr, 0, nullptr);
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
}