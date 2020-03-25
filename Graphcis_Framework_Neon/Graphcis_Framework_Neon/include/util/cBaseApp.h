#pragma once

/**
* @brief : this is the base for all app that will be made
* @bugs : no know bugs
*/

class cBaseApp
{
public:
  cBaseApp() = default;
  virtual ~cBaseApp() = default;

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
private:

};

