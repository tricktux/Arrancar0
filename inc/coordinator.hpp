// File:           coordinator.hpp
// Description:    Wrapper around sc2::Coordinator
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 19:25
// Last Modified:  Fri Jun 01 2018 19:25

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

class coordinator : public sc2::Coordinator
{
	coordinator() : sc2::Coordinator() {}
public:

	static coordinator& get_coordinator(void)
	{
		static coordinator rc;
		return rc;
	}

	coordinator(coordinator const&) = delete;
	void operator= (coordinator const&) = delete;
};

#endif
