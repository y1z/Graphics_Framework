#include "appGraphics.h"
#include "util/enRangeIndexTarcker.h "

int
main()
{

  enRangeIndexTracker<long, 3> instance;

  auto testIndex0 = instance.getFirstUnusedIndex(true);
  auto testIndex1 = instance.getFirstUnusedIndex(true);
  auto testIndex2 = instance.getFirstUnusedIndex(false);
  bool isSuccessful = instance.registerIndexAsUsed(testIndex2);

  assert(isSuccessful == true);
  isSuccessful = instance.getFirstUnusedIndex(false) == -1;
  assert(isSuccessful == true);

  isSuccessful = instance.registerIndexAsAvailable(testIndex0);
  assert(isSuccessful == true);
  isSuccessful = instance.registerIndexAsAvailable(testIndex1);
  assert(isSuccessful == true);

  testIndex0 = instance.getFirstUnusedIndex(false);
  isSuccessful = instance.registerIndexAsUsed(testIndex0);
  assert(isSuccessful == true);
  testIndex1 = instance.getFirstUnusedIndex(false);
  isSuccessful = instance.registerIndexAsUsed(testIndex1);
  assert(isSuccessful == true);

  isSuccessful = instance.registerIndexAsUsed(-1);
  assert(isSuccessful == false );

  isSuccessful = instance.registerIndexAsUsed(instance.getSizeOfRange());
  assert(isSuccessful == true);
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

