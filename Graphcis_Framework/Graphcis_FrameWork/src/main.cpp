#include "appGraphics.h"
//namespace dx = DirectX;

int
main()
{
  cBaseApp * ptr_app = new appGraphcis();
  ptr_app->init(); 
  int result = ptr_app->run();
  ptr_app->destroy();
  return result;
}

