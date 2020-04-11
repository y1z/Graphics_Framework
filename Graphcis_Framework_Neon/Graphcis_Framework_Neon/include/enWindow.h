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
  \param[in] ptr_proc  a pointer to the wndProc function
  \param[in] instance  a variable used to get an HWND
  \param[in] windowName  give the window a name
  \param[in] className  this is just an identifier for the window */
  bool
  init(windProcType ptr_proc,
       const HMODULE Instance,
       int32 const windowWidth = 720,
       int32 const windowHeight = 1280,
       const char* windowName = nullptr,
       const char* className = nullptr);

#if DIRECTX

  /*! returns  the window handle */
  HWND
  getHandle()const;
#elif OPENGL 

  /**
  * @returns : a handle to the window.
  * @bug : no known bugs.
  */
  GLFWwindow*
  getHandle()const;
#else

  void*
  getHandle();
#endif // DIRECTX

   /*! returns the current HMODULE of the window */
  HMODULE 
  getInstance() const;

  /**
  * @returns : the width of the window.
  * @bug :no known bugs.
  */
  uint32
  getWidth() const;

  /**
  * @returns : the height of the window.
  * @bug :no known bugs.
  */
  uint32
  getHeight() const;

  /**
  * @brief : finds out the current height and width of the window.
  * @bug : no known bugs.
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

  /**
  * @brief : the width of the window.
  */
  uint32 m_width;

  /**
  * @brief : the height of the window.
  */
  uint32 m_height;
};
