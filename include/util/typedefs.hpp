/*

Copyright (c) 2016, Project OSRM contributors
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

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "util/strong_typedef.hpp"

#include <boost/assert.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>

// OpenStreetMap node ids are higher than 2^32
OSRM_STRONG_TYPEDEF(std::uint64_t, OSMNodeID)
OSRM_STRONG_TYPEDEF_HASHABLE(std::uint64_t, OSMNodeID)

OSRM_STRONG_TYPEDEF(std::uint32_t, OSMWayID)
OSRM_STRONG_TYPEDEF_HASHABLE(std::uint32_t, OSMWayID)

static const OSMNodeID SPECIAL_OSM_NODEID = OSMNodeID{std::numeric_limits<std::uint64_t>::max()};
static const OSMWayID SPECIAL_OSM_WAYID = OSMWayID{std::numeric_limits<std::uint32_t>::max()};

static const OSMNodeID MAX_OSM_NODEID = OSMNodeID{std::numeric_limits<std::uint64_t>::max()};
static const OSMNodeID MIN_OSM_NODEID = OSMNodeID{std::numeric_limits<std::uint64_t>::min()};
static const OSMWayID MAX_OSM_WAYID = OSMWayID{std::numeric_limits<std::uint32_t>::max()};
static const OSMWayID MIN_OSM_WAYID = OSMWayID{std::numeric_limits<std::uint32_t>::min()};

using OSMNodeID_weak = std::uint64_t;
using OSMEdgeID_weak = std::uint64_t;

using NodeID = std::uint32_t;
using EdgeID = std::uint32_t;
using NameID = std::uint32_t;
using EdgeWeight = std::int32_t;
using EdgeDuration = std::int32_t;
using SegmentWeight = std::uint16_t;
using SegmentDuration = std::uint16_t;
using TurnPenalty = std::int16_t; // turn penalty in 100ms units

static const std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

using LaneID = std::uint8_t;
static const LaneID INVALID_LANEID = std::numeric_limits<LaneID>::max();
using LaneDataID = std::uint16_t;
static const LaneDataID INVALID_LANE_DATAID = std::numeric_limits<LaneDataID>::max();
using LaneDescriptionID = std::uint16_t;
static const LaneDescriptionID INVALID_LANE_DESCRIPTIONID =
    std::numeric_limits<LaneDescriptionID>::max();

using BearingClassID = std::uint32_t;
static const BearingClassID INVALID_BEARING_CLASSID = std::numeric_limits<BearingClassID>::max();

using DiscreteBearing = std::uint16_t;

using EntryClassID = std::uint16_t;
static const EntryClassID INVALID_ENTRY_CLASSID = std::numeric_limits<EntryClassID>::max();

static const NodeID SPECIAL_NODEID = std::numeric_limits<NodeID>::max();
static const NodeID SPECIAL_SEGMENTID = std::numeric_limits<NodeID>::max() >> 1;
static const NodeID SPECIAL_GEOMETRYID = std::numeric_limits<NodeID>::max() >> 1;
static const EdgeID SPECIAL_EDGEID = std::numeric_limits<EdgeID>::max();
static const NameID INVALID_NAMEID = std::numeric_limits<NameID>::max();
static const NameID EMPTY_NAMEID = 0;
static const unsigned INVALID_COMPONENTID = 0;
static const SegmentWeight INVALID_SEGMENT_WEIGHT = std::numeric_limits<SegmentWeight>::max();
static const SegmentDuration INVALID_SEGMENT_DURATION = std::numeric_limits<SegmentDuration>::max();
static const EdgeWeight INVALID_EDGE_WEIGHT = std::numeric_limits<EdgeWeight>::max();
static const EdgeDuration MAXIMAL_EDGE_DURATION = std::numeric_limits<EdgeDuration>::max();
static const TurnPenalty INVALID_TURN_PENALTY = std::numeric_limits<TurnPenalty>::max();

// FIXME the bitfields we use require a reduced maximal duration, this should be kept consistent
// within the code base. For now we have to ensure that we don't case 30 bit to -1 and break any
// min() / operator< checks due to the invalid truncation. In addition, using signed and unsigned
// weights produces problems. As a result we can only store 1 << 29 since the MSB is still reserved
// for the sign bit. See https://github.com/Project-OSRM/osrm-backend/issues/3677
static const EdgeWeight MAXIMAL_EDGE_DURATION_INT_30 = (1 << 29) - 1;

using DatasourceID = std::uint8_t;

using BisectionID = std::uint32_t;
using LevelID = std::uint8_t;
using CellID = std::uint32_t;
using PartitionID = std::uint64_t;

static constexpr auto INVALID_LEVEL_ID = std::numeric_limits<LevelID>::max();
static constexpr auto INVALID_CELL_ID = std::numeric_limits<CellID>::max();

struct SegmentID
{
    SegmentID(const NodeID id_, const bool enabled_) : id{id_}, enabled{enabled_}
    {
        BOOST_ASSERT(!enabled || id != SPECIAL_SEGMENTID);
    }

    NodeID id : 31;
    std::uint32_t enabled : 1;
};

/* We need to bit pack here because the index for the via_node
 * is given to us without knowing whether the geometry should
 * be read forward or in reverse. The extra field `forward`
 * indicates that to the routing engine
 */
struct GeometryID
{
    GeometryID(const NodeID id_, const bool forward_) : id{id_}, forward{forward_} {}

    GeometryID() : id(std::numeric_limits<unsigned>::max() >> 1), forward(false) {}

    NodeID id : 31;
    std::uint32_t forward : 1;
};

static_assert(sizeof(SegmentID) == 4, "SegmentID needs to be 4 bytes big");

#endif /* TYPEDEFS_H */
