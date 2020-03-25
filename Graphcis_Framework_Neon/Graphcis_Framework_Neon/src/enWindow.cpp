#include "..\include\enWindow.h"
#include "Resource.h"
#include "helperFucs.h"

#if UNICODE
constexpr static const wchar_t* defualName = L"nameless window";
constexpr static const wchar_t* defualClassName = L"entropy window";
#else 
constexpr static const char* defualName = "nameless window";
constexpr static const char* defualClassName = "entropy window";
#endif // UNICODE

enWindow::enWindow()
  : m_descriptor{ 0 },
  mptr_proc(nullptr),
  m_handle(0),
  m_width(1u),
  m_height(1u)
{
}

enWindow::~enWindow()
{

}

bool
enWindow::init(windProcType ptr_proc,
               const HMODULE Instance,
               const char* windowName,
               const char* className)
{
  if( windowName == nullptr )
  {
    m_name = defualName;
  }

  if( className == nullptr )
  {
    m_className = defualClassName;
  }

#if DIRECTX
  this->mptr_proc = ptr_proc;
  this->m_instance = Instance;
  // Register class W
  m_descriptor.cbSize = sizeof(WNDCLASSEX);
  m_descriptor.style = CS_HREDRAW | CS_VREDRAW;
  m_descriptor.lpfnWndProc = mptr_proc;
  m_descriptor.cbClsExtra = 0;
  m_descriptor.cbWndExtra = 0;
  m_descriptor.hInstance = m_instance;
  m_descriptor.hIcon = LoadIcon(m_instance, (LPCTSTR)IDI_TUTORIAL1);
  m_descriptor.hCursor = LoadCursor(NULL, IDC_ARROW);
  m_descriptor.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  m_descriptor.lpszMenuName = NULL;
  m_descriptor.lpszClassName = m_className.c_str();
  m_descriptor.hIconSm = LoadIcon(m_descriptor.hInstance, (LPCTSTR)IDI_TUTORIAL1);

  if( !RegisterClassEx(&m_descriptor) )
    return false;

  // Create window
  RECT rc = { 0, 0, 640, 480 };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

  m_width = rc.right - rc.left;
  m_height = rc.bottom - rc.top;

  m_handle = CreateWindow(m_className.c_str(),
                          m_name.c_str(),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          m_width,
                          m_height,
                          NULL,
                          NULL,
                          m_instance,
                          NULL);

  if( !m_handle )
    return false;

  ShowWindow(m_handle, SW_SHOW);
  UpdateWindow(m_handle);

  return true;
#elif OPENGL
  #ifdef UNICODE

  std::string convertedName = helper::convertWStringToString(m_name);

  m_handle = glfwCreateWindow(m_width, m_height, convertedName.c_str(), NULL, NULL);
  #else

  m_handle = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
  #endif // UNICODE
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
    /* Create a windowed mode window and its OpenGL context */
  if( !m_handle )
  {
    glfwTerminate();
    return false;
  }
  /* Make the window's context current */
  glfwMakeContextCurrent(m_handle);
  return true;
#endif
  return false;
}

#if DIRECTX
HWND
enWindow::getHandle() const
{
  return m_handle;
}
#elif OPENGL
GLFWwindow*
enWindow::getHandle() const
{
  return m_handle;
}
#endif // DIRECTX

HMODULE
enWindow::getInstance() const
{
  return m_instance;
}

uint32
enWindow::getWidth() const
{
  return m_width;
}

uint32
enWindow::getHeight() const
{
  return m_height;
}

void
enWindow::update()
{
#if DIRECTX
  RECT currentRect;
  GetClientRect(m_handle, &currentRect);
  m_width = currentRect.right - currentRect.left;
  m_height = currentRect.bottom - currentRect.top;
#elif OPENGL
#endif // DIRECTX
}