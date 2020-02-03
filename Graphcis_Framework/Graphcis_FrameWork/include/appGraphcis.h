#pragma once
#include "util/cBaseApp.h"
class appGraphcis :
  public cBaseApp
{


public:
  /**
  * @brief : should be used for resource gathering
  */
  virtual bool
  init() = 0;

 /**
  * @brief : this is where the main starts
  */
  virtual int
  run() = 0;

  /**
  * @brief : used for freeing resources
  * @bugs : no know bugs
  */
  virtual void
  destroy() = 0;


};

