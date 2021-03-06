/*

Copyright (c) 2013, Project OSRM, Dennis Luxen, others
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef EXTRACTION_WAY_H
#define EXTRACTION_WAY_H

#include "../DataStructures/HashTable.h"
#include "../DataStructures/TravelMode.h"
#include "../typedefs.h"

#include <string>
#include <vector>

struct ExtractionWay
{
    ExtractionWay() { Clear(); }

    inline void Clear()
    {
        id = SPECIAL_NODEID;
        nameID = INVALID_NAMEID;
        path.clear();
        keyVals.Clear();
        forward_speed = -1;
        backward_speed = -1;
        duration = -1;
        access = true;
        roundabout = false;
        isAccessRestricted = false;
        ignoreInGrid = false;
        forward_travel_mode = TRAVEL_MODE_DEFAULT;
        backward_travel_mode = TRAVEL_MODE_DEFAULT;
    }

    enum Directions
    { notSure = 0,
      oneway,
      bidirectional,
      opposite };
    
    // These accessor methods exists to support the depreciated "way.direction" access
    // in LUA. Since the direction attribute was removed from ExtractionWay, the 
    // accessors translate to/from the mode attributes.
    inline void set_direction(const Directions m)
    {
        if (Directions::oneway == m)
        {
            forward_travel_mode = TRAVEL_MODE_DEFAULT;
            backward_travel_mode = TRAVEL_MODE_INACCESSIBLE;
        }
        else if (Directions::opposite == m)
        {
          forward_travel_mode = TRAVEL_MODE_INACCESSIBLE;
          backward_travel_mode = TRAVEL_MODE_DEFAULT;
        }
        else if (Directions::bidirectional == m)
        {
          forward_travel_mode = TRAVEL_MODE_DEFAULT;
          backward_travel_mode = TRAVEL_MODE_DEFAULT;
        }
    }

    inline const Directions get_direction() const
    {
        if (TRAVEL_MODE_INACCESSIBLE != forward_travel_mode && TRAVEL_MODE_INACCESSIBLE != backward_travel_mode)
        {
            return Directions::bidirectional;
        }
        else if (TRAVEL_MODE_INACCESSIBLE != forward_travel_mode)
        {
            return Directions::oneway;
        }
        else if (TRAVEL_MODE_INACCESSIBLE != backward_travel_mode)
        {
            return Directions::opposite;
        }
        else
        {
            return Directions::notSure;
        }
    }

    // These accessors exists because it's not possible to take the address of a bitfield,
    // and LUA therefore cannot read/write the mode attributes directly.
    inline void set_forward_mode(const TravelMode m) { forward_travel_mode = m; }
    inline const TravelMode get_forward_mode() const { return forward_travel_mode; }
    inline void set_backward_mode(const TravelMode m) { backward_travel_mode = m; }
    inline const TravelMode get_backward_mode() const { return backward_travel_mode; }

    unsigned id;
    unsigned nameID;
    double forward_speed;
    double backward_speed;
    double duration;
    std::string name;
    bool access;
    bool roundabout;
    bool isAccessRestricted;
    bool ignoreInGrid;
    std::vector<NodeID> path;
    HashTable<std::string, std::string> keyVals;
    TravelMode forward_travel_mode : 4;
    TravelMode backward_travel_mode : 4;
};

#endif // EXTRACTION_WAY_H
