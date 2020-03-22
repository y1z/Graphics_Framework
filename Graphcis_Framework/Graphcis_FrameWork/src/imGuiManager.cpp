#include "..\include\imGuiManager.h"
#include "enDevice.h"
#include "enWindow.h"
#include "enShaderResourceView.h"

#include <cstdint>
#include <string_view>
/*******************************/
#include"imgui/imgui.h"
#if DIRECTX
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#elif OPENGL
#include "imgui/imgui_impl_glfw.h" 
#include "imgui/imgui_impl_opengl3.h"
#endif // USING_DIRECTX
/*****************************/

namespace ig = ImGui;
using namespace std::string_literals;

static constexpr uint32_t s_fpsSamplesCount = 360;
static constexpr uint32_t s_buttonCount = 0;
//! this variable is for ImGui_ImplOpenGL3_Init
static constexpr const char *s_GlslVersion = "#version 430 core";
//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------


imGuiManager::imGuiManager()
  :m_childCount(0),
  //mptr_FileFunc(nullptr),
  is_initialized(false),
  m_windowCount(0)
{}

/*!  Highly  Important to free the memory*/
imGuiManager::~imGuiManager()
{
  if (is_initialized)
  {
  #if DIRECTX
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
  #elif OPENGL
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
  #endif // USING_DIRECTX
    ImGui::DestroyContext();
  }
}

bool
imGuiManager::Init(enWindow& window)
{
  bool isSuccesful = false;
  is_initialized = IMGUI_CHECKVERSION();
  enDevice& device = enDevice::getInstance();
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  if (is_initialized)
  {
    ImGui::CreateContext();
    //this can be used to enable or disable opciones
    ImGuiIO& GuiIo = ImGui::GetIO();
    // check for possible error 
  #if DIRECTX
    isSuccesful = ImGui_ImplWin32_Init(window.getHandle());
    isSuccesful = ImGui_ImplDX11_Init(device.getInterface(), deviceContext.m_interface);
  #elif OPENGL
    // TODO: REMOVE COMMIT WHEN READY 
    isSuccesful = ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    isSuccesful = ImGui_ImplOpenGL3_Init(s_GlslVersion);
  #endif // USING_DIRECTX
  }

  if (is_initialized)
  {
    ImGui::StyleColorsDark();
  }
  return is_initialized;
}

//void
//imGuiManager::setOpenFileFunction(ptr_FileOpenFunc openFileFunc)
//{
//  mptr_FileFunc = openFileFunc;
//}


void
imGuiManager::beginChildWithFpsCount(float DeltaTime)
{
  m_childCount++;
  float averageFps = this->calculateAverageFPS(DeltaTime);
  static const std::string fpsMassage("Average FPS %f");
   //fpsMassage += std::to_string(averageFps);

  ImGui::BeginChild("FPS", ImVec2(420, 130));
  ImGui::Text(fpsMassage.c_str(), averageFps);

}

float
imGuiManager::calculateAverageFPS(float deltaTime)
{
  // gets the fps 
  float const fps = 1.0f / deltaTime;
  // contains the results for later to average them out 
  static float fpsTimes[s_fpsSamplesCount] = { 0 };
  static uint32_t  currenFpsIndex = 0;

  if (currenFpsIndex > s_fpsSamplesCount - 1)
  { currenFpsIndex = 0; }

  fpsTimes[currenFpsIndex] = fps;
  currenFpsIndex++;

  float averageFps = 0.0f;
  for (size_t i = 0; i < s_fpsSamplesCount; ++i)
  {
    averageFps += fpsTimes[i];
  }
  return averageFps = averageFps / s_fpsSamplesCount;
}

//void
//imGuiManager::MenuForOpenFile(enWindow & window, std::string & PathOfFile, bool & used)
//{
//  if (ImGui::Button("Load model from file "))
//  {
//    PathOfFile = mptr_FileFunc(window);
//    used = true;
//  }
//
//}

//std::string
//imGuiManager::OpenFileFunc(enWindow & window)
//{
//  return this->mptr_FileFunc(window);
//}

void
imGuiManager::beginFrame(const char * windowName)
{
#if DIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#elif OPENGL 
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif// DIRECTX
  ImGui::NewFrame();
  ImGui::Begin(windowName);
}

void
imGuiManager::beginExtraWindow(const char * windowName)
{
  ig::Begin(windowName);
  m_windowCount++;
}

void
imGuiManager::addItemCount(std::string_view itemName, uint32 itemCount)
{

  static std::string messageStr = "Item Name : ";
  // get the data in the string view 
  messageStr += itemName.data();
  ig::Text(messageStr.c_str());

  messageStr = "Item Count =  %d";

  ig::TextColored(ImVec4(0.98f, 0.88f, 0.20f, 1.0f), messageStr.c_str(), itemCount);
  // reset the static string 
  messageStr = "Item Name : ";

}

void
imGuiManager::addButton(std::string_view buttonUse, bool & buttonCondtion)
{
  if (ig::Button(buttonUse.data()))
  {
    if (buttonCondtion == false)
    {
      buttonCondtion = true;
    }
    else
    {
      buttonCondtion = false;
    }
  }
}

void
imGuiManager::addText(std::string_view message, sColorf TextColor)
{
  ig::TextColored(ImVec4(TextColor.red, TextColor.green, TextColor.blue, TextColor.alpha), message.data());
}

void
imGuiManager::addMultipleText(const std::vector<std::string>& strings)
{
  for (const std::string &singleString : strings)
  {
    ig::Text("\n===================================================\n");
    ig::Text(singleString.c_str());
    ig::Text("\n===================================================\n");
  }

}

void imGuiManager::addImage(enShaderResourceView& ResourceView, uint32 SizeInX, uint32 SizeInY)
{
#if DIRECTX
  ig::Image(( void* )ResourceView.m_interface, ImVec2(SizeInX, SizeInY));
#elif OPENGL
  ig::Image((void*)ResourceView.m_interface, ImVec2(SizeInX, SizeInY));
#endif // DIRECTX
}



void 
imGuiManager::addSliderFloat(std::string_view NameOfValue, float & Value, float lowerRange, float upperRange)
{
  ig::SliderFloat(NameOfValue.data(), &Value, lowerRange, upperRange);
}

void
imGuiManager::addCounter(int32_t& CounterValue, std::string_view nameOfValue)
{
  // making room to add the strings "increase" and later "decrease" 
  std::string Message(nameOfValue.length() + 11, '\0');

  Message = ("Increase "s += nameOfValue);
  if (ig::Button(Message.c_str()))
  {
    CounterValue++;
  }

  ig::Text("%d", CounterValue);
  // replace "increase" with "decrease"
  Message.replace(0, 2, "De");
  if (ig::Button(Message.c_str()))
  {
    CounterValue--;
  }

}

void 
imGuiManager::addCounter(uint32_t & CounterValue, std::string_view nameOfValue)
{
  // making room to add the strings "increase" and later "decrease" 
  std::string Message(nameOfValue.length() + 11, '\0');

  Message = ("Increase "s += nameOfValue);
  if (ig::Button(Message.c_str()))
  {
    CounterValue++;
  }

  ig::Text("%u", CounterValue);
  // replace "increase" with "decrease"
  Message.replace(0, 2, "De");
  if (ig::Button(Message.c_str()))
  {
    CounterValue--;
  }

}

void 
imGuiManager::addCounter(int64_t & CounterValue, std::string_view nameOfValue)
{
  // making room to add the strings "increase" and later "decrease" 
  std::string Message(nameOfValue.length() + 11, '\0');

  Message = ("Increase "s += nameOfValue);
  if (ig::Button(Message.c_str()))
  {
    CounterValue++;
  }

  ig::Text("%d", CounterValue);
  // replace "increase" with "decrease"
  Message.replace(0, 2, "De");
  if (ig::Button(Message.c_str()))
  {
    CounterValue--;
  }
}

void imGuiManager::addCounter(uint64_t & CounterValue, std::string_view nameOfValue)
{
  // making room to add the strings "increase" and later "decrease" 
  std::string Message(nameOfValue.length() + 11, '\0');

  Message = ("Increase "s += nameOfValue);
  if (ig::Button(Message.c_str()))
  {
    CounterValue++;
  }

  ig::Text("%u", CounterValue);
  // replace "increase" with "decrease"
  Message.replace(0, 2, "De");
  if (ig::Button(Message.c_str()))
  {
    CounterValue--;
  }
}

void
imGuiManager::beginChild(const char * childID)
{
  m_childCount++;
  ig::BeginChild(childID);
}

void
imGuiManager::endAllChildren()
{
  for (uint32 i = 0; i < m_childCount; ++i)
  {
    ig::EndChild();
  }

  m_childCount = 0;
}

void
imGuiManager::endAllExtraWindows()
{
  for (uint32 i = 0; i < m_windowCount; ++i)
  {
    ig::End();
  }
  m_windowCount = 0;
}

void
imGuiManager::endExtraWindow()
{
  ig::End();
  --m_windowCount;
}

void
imGuiManager::endFrame()
{
  ImGui::End();
  ImGui::Render();

#if DIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif OPENGL
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX
}

