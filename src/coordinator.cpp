// File:           coordinator.cpp
// Description:     Code Description
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Thu Aug 02 2018 20:15
// Last Modified:  Thu Aug 02 2018 20:15

#include "config.hpp"
#include "coordinator.hpp"

void coordinator::load_configurations(int argc, const char** argv)
{


	// Overwrite settings if they were passed through the cli
	char **argv_buff = const_cast<char **>(argv); // Remove const from argv
	LoadSettings(argc, argv_buff);
}
