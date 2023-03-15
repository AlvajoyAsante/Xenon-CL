/*
 *--------------------------------------
 * Program Name: Xenon CL
 * Author: Alvajoy 'Alvajoy123' Asante
 * License: AGPL-3.0
 * Description: XENON COMMAND LINE INTERFACE USING OXYGEN AND ETC.
 *--------------------------------------
*/

#include "core/core.h"
#include "core/oxygen/oxygen.h"

#include <tice.h>
#include <graphx.h>
#include <debug.h>

int main(void)
{
	/*
	* Beginning of CE C Graphical Setup.
	*/
	gfx_Begin();
	
	/*
	* Loads All Oxygen Files in and setups.
	*/
	oxy_Begin(); 

	/*
	* Sets up rendering and loads any data needed.
	*/
	core_Setup();

	/*
	* Renders the terminal.
	*/
	core_RenderHome();
	
	/*
	* Saves any data after running the shell.
	*/
	core_Save();
	
	/*
	* Saves all Oxygen's systems.
	*/
	oxy_End();
	
	/*
	* End of CE C Graphical Setup.
	*/
	gfx_End();
	
	return 0;
} 