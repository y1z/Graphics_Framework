#pragma once
#include "util/Header.h"
#include <string>


class enWindow
{
public:
  using windProcType = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
public:

  enWindow();
  ~enWindow();

 public:
  /*! this starts the window
  \param ptr_proc [in] a pointer to the wndProc function
  \param instance [in] a variable used to get an HWND
  \param windowName [in] give the window a name
  \param className [in] this is just an identifier for the window */
  bool
  init(windProcType ptr_proc,
       const HMODULE Instance,
       const char* windowName = nullptr,
       const char* className = nullptr);
#if DIRECTX
  /*! returns  the window handle */
  HWND
  getHandle()const;
#elif OPENGL 
  GLFWwindow*
  getHandle()const;
#else
  void*
  getHandle();
#endif // DIRECTX

   /*! returns the current HMODULE of the window */
  HMODULE 
  getInstance() const;

  uint32
  getWidth() const;

  uint32
  getHeight() const;

  /**
  * @brief : finds out the current height and width of the window.
  * @bug : no known bugs
  */
  void
  update();

private:


#if UNICODE 
  std::wstring m_name;
  std::wstring m_className;
#else
  std::string m_name;
  std::string m_className;
#endif // UNICODE 
  /*! a pointer for the wndProc function */
  WNDCLASSEX m_descriptor;
  windProcType mptr_proc = nullptr;

#if !OPENGL 
  HWND m_handle;
#elif OPENGL
  GLFWwindow* m_handle;

#endif // DIRECTX
  HINSTANCE m_instance;

  uint32 m_width;
  uint32 m_height;
};
