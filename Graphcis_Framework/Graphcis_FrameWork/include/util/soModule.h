/********************************************************************/
/**
* @file   soModule.h
* @author Samuel Prince (samuel.prince.quezada@gmail.com)
* @date   2015/02/22
* @brief  Represents one engine module
*
* Represents one engine module. Essentially it is a specialized type
* of singleton. Module must be manually started up and shut down
* before and after use.
*
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Includes                                                                     										*/
/************************************************************************************************************************/
#include <iostream>

/************************************************************************************************************************/
/**
* @brief	Represents one engine module. Essentially it is a specialized type of singleton. Module must be manually
*			started up and shut down before and after use.
*/
/************************************************************************************************************************/

template <class T>
class soModule
{
public:
	/************************************************************************************************************************/
	/**
	* @brief	Returns a reference to the module instance. Module has to have been started up first otherwise an exception
	*			will be thrown.
	*/
	/************************************************************************************************************************/
	static T& getInstance()
	{
		if (IsShutDown())
		{
			std::cout<<"soModule", "getInstance()", "Trying to access a module but it hasn't been started up yet.";
		}

		if (IsDestroyed())
		{
			///console.log("soModule", "Instance()", "Trying to access a destroyed module.");
			std::cerr << "soModule\n getInstance()\n Trying to access a destroyed module.";
		}

		return *_Instance();
	}

	/************************************************************************************************************************/
	/**
	* @brief	Returns a pointer to the module instance. Module has to have been started up first otherwise an exception
	*			will be thrown.
	*/
	/************************************************************************************************************************/
	static T* getInstancePtr()
	{
		if (IsShutDown())
		{
			//console.log("soModule", "getInstancePtr()", "Trying to access a module but it hasn't been started up yet.");
			std::cerr << "soModule\n getInstancePtr() \n Trying to access a module but it hasn't been started up yet.";
			return NULL;
		}

		if (IsDestroyed())
		{
			//console.log("soModule", "getInstancePtr()", "Trying to access a destroyed module.");
			std::cerr << "soModule\n getInstancePtr() \n Trying to access a destroyed module.";
			return NULL;
		}

		return _Instance();
	}

	/************************************************************************************************************************/
	/**
	* @brief	Constructs and starts the module using the specified parameters.
	*/
	/************************************************************************************************************************/
	static int StartUp(void* _Desc)
	{
		if (!IsShutDown())
		{
			std::cerr<< "soModule\n" <<"StartUp()\n" <<"Trying to start an already started module.\n";
			return -1;
		}

		_Instance() = new T;
		IsShutDown() = false;

		return ((soModule*)_Instance())->OnStartUp(_Desc);
	}

	/************************************************************************************************************************/
	/**
	* @brief	Shuts down this module and frees any resources it is using.
	*/
	/************************************************************************************************************************/
	static void ShutDown()
	{
		if (IsShutDown())
		{
      std::cerr<< "soModule\n" <<"ShutDown()\n" <<"Trying to shut down an already shut down module.\n";
			return;
		}

		((soModule*)_Instance())->OnShutDown();

		delete(_Instance());
		IsShutDown() = true;
	}

	/************************************************************************************************************************/
	/**
	* @brief	Query if the module has been started.
	*/
	/************************************************************************************************************************/
	static bool IsStarted()
	{
		return !IsShutDown() && !IsDestroyed();
	}

protected:
	soModule()
	{
	}

	virtual ~soModule()
	{
		_Instance() = nullptr;
		IsDestroyed() = true;
	}

	soModule(const soModule&)
	{

	}

	soModule& operator=(const soModule&)
	{
		return *this;
	}

protected:
	/************************************************************************************************************************/
	/**
	* @brief	Override if you want your module to be notified just before it is deleted.
	*
	* @note		Useful when your module is polymorphic and you might want to perform some kind of clean up perhaps overriding
	*			that of a base class.
	*/
	/************************************************************************************************************************/
	virtual void OnShutDown()
	{

	}


	virtual int OnStartUp(void* _Desc)
	{

		return -1;
	}

	/************************************************************************************************************************/
	/**
	* @brief	Returns a singleton instance of this module. Throws an exception if module is not yet initialized.
	*/
	/************************************************************************************************************************/
	static T*& _Instance()
	{
		static T* inst = nullptr;
		return inst;
	}

	/************************************************************************************************************************/
	/**
	* @brief	Checks has the Module been shut down.
	*
	* @note		If module was never even started, this will return false.
	*/
	/************************************************************************************************************************/
	static bool& IsDestroyed()
	{
		static bool inst = false;
		return inst;
	}

	/************************************************************************************************************************/
	/**
	* @brief	Checks has the Module been started up.
	*/
	/************************************************************************************************************************/
	static bool& IsShutDown()
	{
		static bool inst = true;
		return inst;
	}
};