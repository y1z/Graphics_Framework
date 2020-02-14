#include "appGraphics.h"

int
main()
{
  cBaseApp * ptr_app = new appGraphcis();
  if( ptr_app->init() )
  {
    int result = ptr_app->run();
    ptr_app->destroy();
    return result;
  }
  EN_LOG_DB("FAILED TO INITIALIZE THE APPLICATION ")
  return -1;
}

