
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "core/graphics_api.hpp"
#include "core/config.hpp"
#include "widgets/widgets.hpp"
#include "../citos/module.h"
#include "../þifreleme/md5.h"
#include <stdio.h>
#include "../þifreleme/xor.h"
#include <synchapi.h>
#include "../misckoruma/protect.h"
#include "../misckoruma/protect2.h"
#include "misc3.h"
#include "../citos/font.h"
#include "../selfcode/selfcode.h"
#include "../driver/globals.hpp"

using namespace std;



int inticitos();

bool g_open = true;
int g_menuKeyBind = VK_HOME;


bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return true;
}
void renk(WORD renk) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), renk);
}



void stylemenu()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF(Pixel_compressed_data, Pixel_compressed_size, 12.f, NULL, io.Fonts->GetGlyphRangesJapanese());
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);

    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.TabRounding = 4;
}

static int sayfa = 1;


void RenderUI() {
    drawingg();
	POINT mPos;
	GetCursorPos(&mPos);
	if (g_open) {
	ImGui::SetNextWindowSize({ 290.f,350.f });
	ImGui::Begin(_xor_("AppollonCheats | Apex Hack").c_str()); {
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();
			
			bool isHovering = mPos.x >= pos.x && mPos.y >= pos.y && mPos.x <= (pos.x + size.x) && mPos.y <= (pos.y + size.y);

			if ( isHovering) {
				GAPI::EnableInput();
			}

			else if (!isHovering) {
				GAPI::DisableInput();
			}
            if (ImGui::Button(_xor_("Visual").c_str(), ImVec2(130, 20))) {
                sayfa = 1;

            }
            ImGui::SameLine();
            if (ImGui::Button(_xor_("Weapon").c_str(), ImVec2(130, 20))) {
                sayfa = 2;
            }
            if (sayfa == 1) {
                ImGui::Checkbox(_xor_("Glow").c_str(), &glow_a);
                ImGui::Checkbox(_xor_("ESP").c_str(), &espa_a);
                if (espa_a)
                    if (!espsimple_a)
                        ImGui::Checkbox(_xor_("Corner Box").c_str(), &espcorner_a);
                if (espa_a)
                    if (!espcorner_a)
                        ImGui::Checkbox(_xor_("Simple Box").c_str(), &espsimple_a);
                if (espa_a)
                    ImGui::Checkbox(_xor_("Filled Box").c_str(), &filledesp_a);
                if (espa_a)
                    ImGui::SliderInt(_xor_("ESP Distance").c_str(), &espmetre, 1000, 7000);
            }
            if (sayfa == 2) {
                ImGui::Checkbox(_xor_("Aimbot ").c_str(), &aimbot_a);
                if (aimbot_a) {
                    ImGui::SameLine();
                    ImGui::Checkbox(_xor_("Draw FOV").c_str(), &drawfov);
                }
                if (aimbot_a)
                    ImGui::Hotkey(_xor_("Aimbot Hotkey").c_str(), &aimkey, ImVec2(140, 25));
                if (aimbot_a)
                    ImGui::SliderInt(_xor_("Aimbot Distance").c_str(), &aimmetre, 1000, 7000);
                if (aimbot_a)
                    ImGui::SliderFloat(_xor_("Aimbot Smooth").c_str(), &aSmoothAmount, 1.f, 10.f);
                if (drawfov)
                    ImGui::SliderInt(_xor_("Aimbot Fov").c_str(), &FOV, 50, 250);
                ImGui::Checkbox(_xor_("Recoil").c_str(), &recoil_a);

            }
		}
	}
	ImGui::End();
}
HANDLE window = NULL;

void selam() {
	inticitos();
}

std::string random_string_s(const int len)
{
    const std::string alpha_numeric(_xor_("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890").c_str());

    std::default_random_engine generator{ std::random_device{}() };
    const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

    std::string str(len, 0);
    for (auto& it : str)
        it = alpha_numeric[distribution(generator)];

    return str;
}

int inticitos()
{
    do {
        system(_xor_("cls").c_str());
        std::printf(_xor_("Waiting for Apex Legends\n").c_str());
        Sleep(1500);
    } while (!FindWindowA(_xor_("Respawn001").c_str(), 0));
    system(_xor_("cls").c_str());
    rfo::memory = std::make_unique<memory_rfo>(random_string_s(25), (_xor_("r5apex.exe").c_str()));
    exebase = rfo::memory->base_address();
    GAPI::MakeWindow(random_string_s(25), random_string_s(25), WndProc, stylemenu, RenderUI, &g_open);
    return EXIT_SUCCESS;
}



int main() {
    auto protect = selfcode_protect();
    if (protect) {
        inticitos();
    }
    else {
        return EXIT_SUCCESS;
    }
}
