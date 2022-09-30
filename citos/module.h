#pragma once
#include <thread>
#include "offsets.h"
#include "math.h"
#include "../draw/cizim.h"
#include "../driver/globals.hpp"

uintptr_t localPlayer;
uintptr_t entityList;
uintptr_t nameList;
uintptr_t currentAimTarget;
uintptr_t viewRenderer;
#define ARGB(a, r, g, b) D3DCOLOR_ARGB(a, r, g, b)
float color_red = 1.;
float color_green = 0;
float color_blue = 0;
float color_random = 0.0;
float color_speed = -10.0;
static bool rgb_speed = false;
bool glow_a = false;
bool aimbot_a = false;
bool espa_a = false;
bool espcorner_a = false;
bool espsimple_a = false;
bool corneresp_a = false;
bool filledesp_a = false;
bool healthesp_a = false;
bool drawfov = false;
bool recoil_a = false;
int aimkey = 0;
bool marks = true;
DWORD rgbb;

class HitBoxManager
{
private:

public:

	Vector2 ScreenHeadBone{};
	Vector2 ScreenNeckBone{};
	Vector2 ScreenChestBone{};
	Vector2 ScreenWaistBone{};
	Vector2 ScreenBotmBone{};

	Vector2 ScreenLeftshoulderBone{};
	Vector2 ScreenLeftelbowBone{};
	Vector2 ScreenLeftHandBone{};
	Vector2 ScreenRightshoulderBone{};
	Vector2 ScreenRightelbowBone{};
	Vector2 ScreenRightHandBone{};

	Vector2 ScreenLeftThighsBone{};
	Vector2 ScreenLeftkneesBone{};
	Vector2 ScreenLeftlegBone{};
	Vector2 ScreenRightThighsBone{};
	Vector2 ScreenRightkneesBone{};
	Vector2 ScreenRightlegBone{};
};



RGBA fillbox = { 30, 30, 30, 130 };


D3DCOLOR __fastcall FLOAT4TOD3DCOLOR(float Col[])
{
	ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)r, (int)g, (int)b);
}

namespace Color {
	static float CrosshairColors[] = { 255.f, 55.f, 55.f, 255.f };
	static float cornercolor[] = { 255.f, 000.f, 000.f, 255.f };
	static float simplecolor[] = { 255.f, 000.f, 000.f, 255.f };
	static float kemikcolor[] = { 255.f, 000.f, 000.f, 255.f };
}





class LocalEntity {
public:

	uintptr_t Entity;

	explicit LocalEntity()
	{
		Entity = rfo::memory->read<uintptr_t>(exebase + OFFSET_LOCAL_ENT);
	}

	uintptr_t LocalEntityss()
	{
		return rfo::memory->read<uintptr_t>(Entity + OFFSET_LOCAL_ENT);
	}

	Vector3 Position() {
		return rfo::memory->read<Vector3>(Entity + OFFSET_ORIGIN);
	}

	Vector3 BonePosition(int boneIndex)
	{
		Bone bone = {};
		Vector3 vec_bone = Vector3();
		Vector3 pos = Position();

		ULONG64 bone_array = rfo::memory->read<ULONG64>(Entity + OFFSET_BONES);
		ULONG64 bone_location = (boneIndex * 0x30);

		bone = rfo::memory->read<Bone>(bone_array + bone_location);
		vec_bone.x = bone.x + pos.x;
		vec_bone.y = bone.y + pos.y;
		vec_bone.z = bone.z + pos.z;
		return vec_bone;
	}

	Vector3 getViewAngles() {
		return rfo::memory->read<Vector3>(Entity + OFFSET_VIEWANGLES);
	}

	void setViewAngles(Vector2 angles) {
		rfo::memory->write<Vector2>(Entity + OFFSET_VIEWANGLES, { angles.x, angles.y });
	}

	Vector3 getAimPunch() {
		return rfo::memory->read<Vector3>(Entity + OFFSET_AIMPUNCH);
	}

	Vector3 getBreathAngles() {
		return rfo::memory->read<Vector3>(Entity + OFFSET_BREATH_ANGLES);
	}

	Vector3 getRecoil() {
		return *(Vector3*)(Entity + OFFSET_AIMPUNCH);
	}

	Vector3 getCamPosition() {
		return *(Vector3*)(Entity + OFFSET_CAMERAPOS);
	}

	int iHealth() {
		return rfo::memory->read<int>(Entity + OFFSET_HEALTH);
	}

	int iShield() {
		return rfo::memory->read<int>(Entity + OFFSET_SHIELD);
	}

	int iTeam() {
		return rfo::memory->read<int>(Entity + OFFSET_TEAM);
	}
};

class BaseEntity {
private:
	DWORD64 EntityList;
	DWORD64 BaseEnt;
	DWORD64 EntityHandle;

	std::string Identifier;

	DWORD64 atIndex(int index)
	{
		return rfo::memory->read<DWORD64>(EntityList + (index << 5)); 
	}

public:

	DWORD64 Entity;
	LPCSTR IdentifierC = Identifier.c_str();

	explicit BaseEntity(int index)
	{
		EntityList = exebase + OFFSET_ENTITYLIST;
		DWORD64 BaseEnt = rfo::memory->read<DWORD64>(EntityList);
		Entity = atIndex(index);
		if (Entity != 0)
			EntityHandle = rfo::memory->read<DWORD64>(Entity + 0x589);
		Identifier = rfo::memory->read<std::string>(EntityHandle);
	}

	bool isPlayer()
	{
		return (bool)strcmp(IdentifierC, "player");
	}

	Vector3 Position() {
		return rfo::memory->read<Vector3>(Entity + OFFSET_ORIGIN);
	}

	Vector3 BonePosition(int boneIndex)
	{
		Bone bone = {};
		Vector3 vec_bone = Vector3();
		Vector3 pos = Position();

		ULONG64 bone_array = rfo::memory->read<ULONG64>(Entity + OFFSET_BONES);
		ULONG64 bone_location = (boneIndex * 0x30);

		bone = rfo::memory->read<Bone>(bone_array + bone_location);
		vec_bone.x = bone.x + pos.x;
		vec_bone.y = bone.y + pos.y;
		vec_bone.z = bone.z + pos.z;
		return vec_bone;
	}

	int iHealth() {
		return rfo::memory->read<int>(Entity + OFFSET_HEALTH);
	}

	int iShield() {
		return rfo::memory->read<int>(Entity + OFFSET_SHIELD);
	}

	int iTeam() {
		return rfo::memory->read<int>(Entity + OFFSET_TEAM);
	}

	int isKnocked() {
		return rfo::memory->read<int>(Entity + OFFSET_BLEED_OUT_STATE);
	}

	float distanceToLocal(LocalEntity pLocal) {
		return Position().distance(pLocal.Position());
	}


	void PredictPosition(LocalEntity pLocal, Vector3* bonePos)
	{
		uintptr_t actWeaponID = rfo::memory->read<uintptr_t>(pLocal.Entity + OFFSET_WEAPON) & 0xFFFF;
		uintptr_t currentWeapon = rfo::memory->read<uintptr_t>(EntityList + (actWeaponID << 5));

		if (currentWeapon != 0)
		{
			float bulletSpeed = rfo::memory->read<float>(currentWeapon + OFFSET_BULLET_SPEED);
			float bulletGravity = rfo::memory->read<float>(currentWeapon + OFFSET_BULLET_SCALE);

			if (bulletSpeed > 1.f)
			{
				Vector3 muzzle = rfo::memory->read<Vector3>(pLocal.Entity + OFFSET_CAMERAPOS);
				float time = bonePos->DistTo(muzzle) / bulletSpeed;
				bonePos->z += (700.f * bulletGravity * 0.5f) * (time * time);
				Vector3 velDelta = (rfo::memory->read<Vector3>(Entity + OFFSET_ORIGIN - 0xC) * time);
				bonePos->x += velDelta.x;
				bonePos->y += velDelta.y;
				bonePos->z += velDelta.z;
			}
		}
	}


};
ID3DXFont* pESPFont;
void DrawShadowString(const char* String, int x, int y, int offset, D3DCOLOR color, D3DCOLOR shadowColor, ID3DXFont* font, DWORD format)
{
	RECT FontPos;
	RECT shadowPos;
	FontPos.left = x;
	FontPos.top = y;
	shadowPos.left = x - offset;
	shadowPos.top = y + offset;
	font->DrawTextA(0, String, strlen(String), &shadowPos, format, shadowColor);
	font->DrawTextA(0, String, strlen(String), &FontPos, format, color);
}

Vector3 CalcAngles(Vector3 src, Vector3 dst)
{
	Vector3 origin = dst - src;
	float dist = sqrt(origin.x * origin.x + origin.y * origin.y + origin.z * origin.z);
	Vector3 angles = { 0.f, 0.f, 0.f };
	angles.x = -asinf(origin.z / dist) * (180 / M_PI);
	angles.y = atan2f(origin.y, origin.x) * (180 / M_PI);
	return angles;
}

void Aimbot(BaseEntity pEntity, LocalEntity pLocal, Matrix m, AimContext* Ctx, int index) {

	Vector3 entAimPos = pEntity.BonePosition(7);

	pEntity.PredictPosition(pLocal, &entAimPos);

	Vector3 w2sEntAimPos = entAimPos.ScreenPosition(m); if (w2sEntAimPos.z <= 0.f) return;

	int entX = w2sEntAimPos.x;
	int entY = w2sEntAimPos.y;

	Ctx->entNewVisTime = rfo::memory->read<float>(pEntity.Entity + OFFSET_VISIBLE_TIME);
	int entKnockedState = pEntity.isKnocked();
	int playerTeamID = pLocal.iTeam();
	int entTeamID = pEntity.iTeam();

	if (entTeamID != playerTeamID) {

		if (Ctx->entNewVisTime != Ctx->entOldVisTime[index])
		{
			Ctx->visCooldownTime[index] = 24;

			if (entKnockedState == 0)
			{

				if (abs(Ctx->crosshairX - entX) < abs(Ctx->crosshairX - Ctx->closestX) && abs(Ctx->crosshairX - entX) < FOV && abs(Ctx->crosshairY - entY) < abs(Ctx->crosshairY - Ctx->closestY) && abs(Ctx->crosshairY - entY) < FOV)
				{
					Ctx->closestX = entX;
					Ctx->closestY = entY;
				}
			}
			Ctx->entOldVisTime[index] = Ctx->entNewVisTime;
		}
		if (Ctx->visCooldownTime[index] >= 0) Ctx->visCooldownTime[index] -= 1;
	}
}
bool colors = true;

void ColorChange()
{
	static float Color[3];
	static DWORD Tickcount = 0;
	static DWORD Tickcheck = 0;
	ImGui::ColorConvertRGBtoHSV(color_red, color_green, color_blue, Color[0], Color[1], Color[2]);
	if (GetTickCount() - Tickcount >= 1)
	{
		if (Tickcheck != Tickcount)
		{
			Color[0] += 0.001f * color_speed;
			Tickcheck = Tickcount;
		}
		Tickcount = GetTickCount();
	}
	if (Color[0] < 0.0f) Color[0] += 1.0f;
	ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], color_red, color_green, color_blue);
}


static void DrawCorneredBox(int X, int Y, int W, int H, float thickness) {
	auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });
	float lineW = (W / 3);
	float lineH = (H / 3);
	ImDrawList* Renderer = ImGui::GetOverlayDrawList();
	Renderer->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), RGB, thickness);
	
	Renderer->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), RGB, thickness);

	Renderer->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), RGB, thickness);

	Renderer->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), RGB, thickness);

	Renderer->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), RGB, thickness);

	Renderer->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), RGB, thickness);

	Renderer->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), RGB, thickness);

	Renderer->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), RGB, thickness);

}
static void __fastcall flatss(int x, int y, int w, int h, RGBA color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0)), 0, 0);
}

DWORD64 GetEntityById(int Ent, DWORD64 Base)
{
	DWORD64 EntityList = Base + OFFSET_ENTITYLIST;
	DWORD64 BaseEntity = rfo::memory->read<DWORD64>(EntityList);
	if (!BaseEntity)
		return NULL;
	return  rfo::memory->read<DWORD64>(EntityList + (Ent << 5));
}

void glow() {
	for (int i = 0; i < 100; i++)
	{
		DWORD64 Entity = GetEntityById(i, exebase);
		if (Entity == 0)
			continue;
		DWORD64 EntityHandle = rfo::memory->read<DWORD64>(Entity + 0x589);
		std::string Identifier = rfo::memory->read<std::string>(EntityHandle);
		LPCSTR IdentifierC = Identifier.c_str();
		if (strcmp(IdentifierC, "player"))
		{
			rfo::memory->write<int>(Entity + 0x3C8, 1);
			rfo::memory->write<int>(Entity + 0x3D0, 2);
			rfo::memory->write<GlowMode>(Entity + 0x2C4, { 101,101,46,90 });
			rfo::memory->write<float>(Entity + 0x1D0, 61.f);
			rfo::memory->write<float>(Entity + 0x1D4, 2.f);
			rfo::memory->write<float>(Entity + 0x1D8, 2.f);
		}
	}
}


void DrawBorderBox(int x, int y, int x2, int y2, int thickness, RGBA color)
{
	ExternDrawFilledRectImGui(x, y, x2, thickness, color); // top
	ExternDrawFilledRectImGui(x, y + y2, x2, thickness, color); // bottom
	ExternDrawFilledRectImGui(x, y, thickness, y2, color); // left
	ExternDrawFilledRectImGui(x + x2, y, thickness, y2 + thickness, color); // right
}



RGBA dismet1 = { 255, 0, 255, 0 };
RGBA dismet2 = { 120, 255, 0, 0 };
RGBA distmet3 = { 255, 0, 239, 252 };
RGBA distmet4 = { 0, 0, 0, 0 };
RGBA distmet5 = { 255, 0, 0, 0 };
RGBA dismet6 = { 255, 0, 0, 0 };


void DrawHealthbars(float x, float y, float width, float height, int thickness, int health)
{
	ExternDrawFilledRectImGui(x - (thickness + 5), y, thickness, height, dismet1);
	ExternDrawFilledRectImGui(x - (thickness + 5), y, thickness, height - ((height / 100) * health), dismet2);
	ExternDrawFilledRectImGui(x - (thickness * 2 + 12), y, thickness, height, distmet3);
	//ExternDrawFilledRectImGui(x - (thickness * 2 + 12), y, thickness, height - ((height / 125) * shieldHealth), distmet4);
}




void DrawBox(float X, float Y, float W, float H)
{
	auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });

	ImGui::GetOverlayDrawList()->AddRects(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::GetColorU32(RGB));
	ImGui::GetOverlayDrawList()->AddRects(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(RGB));
}


uintptr_t GetMss()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount64();
	}
}



float height;
float width;
float middle;
float boxThickness = 2;
float snaplinesThickness = 2;
int boxColor;
Vector3 w2sHead;
float distances;


void ESP(BaseEntity pEntity, LocalEntity pLocal, Matrix m)
{
	Vector3 entHead = pEntity.BonePosition(8); // get head position
	 w2sHead = entHead.ScreenPosition(m); if (w2sHead.z <= 0.f) return;

	Vector3 entPos = pEntity.Position();
	int health = pEntity.iHealth();
	Vector3 w2sPos = w2sPos.ScreenPosition(m); if (w2sPos.z <= 0.f) return;

	 height = abs(abs(w2sHead.y) - abs(w2sPos.y));
	 width = height / 2.f;
	 middle = w2sPos.x - (width / 2.f);
	 float distance = pEntity.distanceToLocal(pLocal);

	if (distance <= espmetre) {

		if (espcorner_a) {
			DrawCorneredBox(middle, w2sHead.y, width, height, boxThickness);
		}

		if (espsimple_a) {
			DrawBox(middle, w2sHead.y, width, height);
		}

		if (filledesp_a) {
			flatss(middle, w2sHead.y, width, height, fillbox);
		}

	}


}


void AimbotMove(AimContext* Ctx) {
	CURSORINFO ci = { sizeof(CURSORINFO) };
	if (Ctx->closestX != 50000 && Ctx->closestY != 50000)
	{
		if (GetAsyncKeyState(aimkey))
		{
			Ctx->aX = (Ctx->closestX - Ctx->crosshairX) / aSmoothAmount;
			Ctx->aY = (Ctx->closestY - Ctx->crosshairY) / aSmoothAmount;

			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0) 
					mouse_event(MOUSEEVENTF_MOVE, Ctx->aX, Ctx->aY, 0, 0);
			}
		}
	}
}

Vector3 oldPunch = { 0.f, 0.f, 0.f };



void NoRecoil(LocalEntity pLocal) {
		Vector3 viewAngles = pLocal.getViewAngles();
		Vector3 punchAngle = pLocal.getAimPunch();

		Vector3 newAngle = viewAngles + (oldPunch - punchAngle);

		newAngle.Normalize();
		pLocal.setViewAngles(Vector2({ newAngle.x, newAngle.y }));

		oldPunch = punchAngle;
}





void Rendesr() {
	LocalEntity pLocal;
	uint64_t viewRenderer = rfo::memory->read<uint64_t>(exebase + OFFSET_RENDER);
	uint64_t viewMatrix = rfo::memory->read<uint64_t>(viewRenderer + OFFSET_MATRIX);
	Matrix m = rfo::memory->read<Matrix>(viewMatrix);
	if (recoil_a) {
		NoRecoil(pLocal);
	}
	AimContext aimCtx; aimCtx.closestX = 50000; aimCtx.closestY = 50000; FOV; aSmoothAmount;
	for (int i = 0; i < 100; i++)
	{
		BaseEntity pEntity(i);
		if (pEntity.isPlayer() && pEntity.iHealth() != 0 && pEntity.Entity != pLocal.Entity)
		{
			float distance = pEntity.distanceToLocal(pLocal);


			if (aimbot_a) {
				if (distance <= aimmetre)
					Aimbot(pEntity, pLocal, m, &aimCtx, i);
			}
			if (espa_a)
			{
				ESP(pEntity, pLocal, m);
			}
		}
	}
	if (aimbot_a) {
		AimbotMove(&aimCtx);
	}
}




void drawingg()
{
	ColorChange();
	ImGui::PushFont(main_font);

	if (marks) {
		DrawShadowTextImGui(5, 5, { 255, 255, 255, 255 }, ("AppollonCheats")), 5000.f;
	}
	if (drawfov)
	{
		DrawCircleImGui(GAPI::GetWindowSize().x / 2, GAPI::GetWindowSize().y / 2, FOV, { 255, 255, 255, 255 }, 50000.f);
	}
	if (glow_a) {
		glow();
	}
		Rendesr();
}


