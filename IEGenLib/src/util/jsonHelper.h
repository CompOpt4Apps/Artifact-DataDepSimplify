/*!
 * \file util.h
 *
 * \brief Utilities for the IEGen project
 *
 * \date Started: 9/13/2010, but has been completely rewritten
 * # $Revision:: 622                $: last committed revision
 * # $Date:: 2013-01-18 13:11:32 -0#$: date of last committed revision
 * # $Author:: cathie               $: author of last committed revision
 *
 * \authors
 *
 * Copyright (c) 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#ifndef JSONHELPER_H_
#define JSONHELPER_H_


#include <iostream>
#include <iegenlib.h>
#include <parser/jsoncons/json.hpp>

using jsoncons::json;
using namespace iegenlib;

// Reads a list of UFCs from a json structure and stores them in the environment  
void addUFCs(json &ufcs);

// Reads a list of universially quantified constraints from a json structure
// and stores them in the environment
void addUniQuantRules(json &uqCons);

// Reads iterators that we should not project from a json sructure
void notProjectIters(Relation* rel, std::set<int> &parallelTvs, json &np);

#endif
