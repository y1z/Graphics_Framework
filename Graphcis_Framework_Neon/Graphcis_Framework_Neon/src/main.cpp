#include "appGraphics.h"
#include "util/enRangeIndexTracker.h"

int
main()
{

  std::unique_ptr<cBaseApp> app = std::make_unique<appGraphics>();
  if( app->init() )
  {
    int result = app->run();
    app->destroy();
    return result;
  }
  EN_LOG_DB("FAILED TO INITIALIZE THE APPLICATION")
  return -1;
}

