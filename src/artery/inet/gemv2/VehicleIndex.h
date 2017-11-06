/*
 * Artery V2X Simulation Framework
 * Copyright 2017 Thiago Vieira, Raphael Riebl
 * Licensed under GPLv2, see COPYING file for detailed license and warranty terms.
 */

#ifndef VEHICLEINDEX_H_V9YES7AR
#define VEHICLEINDEX_H_V9YES7AR

#include "artery/utility/Geometry.h"
#include "traci/Angle.h"
#include "traci/Boundary.h"
#include "traci/Position.h"
#include <boost/geometry/index/rtree.hpp>
#include <omnetpp/clistener.h>
#include <omnetpp/csimplemodule.h>
#include <set>
#include <vector>

// forward declaration
namespace traci {
    class LiteAPI;
}

namespace artery
{
namespace gemv2
{

class VehicleIndex : public omnetpp::cSimpleModule, public omnetpp::cListener
{
public:
    class Vehicle
    {
    public:
        Vehicle(traci::LiteAPI&, const std::string& id);
        void update(const traci::TraCIPosition& pos, traci::TraCIAngle heading);
        const std::vector<Position>& getOutline() const { return mWorldOutline; }
        const double getHeight() const { return mHeight; }
        const Position& getMidpoint() const { return mWorldMidpoint; }

    private:
        void createLocalOutline(double width, double length);
        void calculateWorldOutline();

        traci::TraCIBoundary mBoundary;
        double mHeight;
        Angle mHeading;
        Position mPosition;
        Position mLocalMidpoint;
        Position mWorldMidpoint;
        std::vector<Position> mLocalOutline;
        std::vector<Position> mWorldOutline;
    };

    // cSimpleModule
    void initialize() override;

    // cListener
    void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, unsigned long, omnetpp::cObject*) override;
    void receiveSignal(omnetpp::cComponent*, omnetpp::simsignal_t, const char*, omnetpp::cObject*) override;

    bool anyBlockage(const Position& a, const Position& b) const;

    /**
     * Get all vehicles obstructing the line of sight between given points
     * \param a position a, e.g. transmitter
     * \param b position b, e.g. receiver
     * \return pointer to vehicles obstructing line of sight
     */
    std::vector<const Vehicle*> getObstructingVehicles(const Position& a, const Position& b) const;

private:
    using VehicleMap = std::map<std::string, Vehicle>;
    using RtreeValue = std::pair<geometry::Box, VehicleMap::const_iterator>;
    using Rtree = boost::geometry::index::rtree<RtreeValue, boost::geometry::index::rstar<16>>;

    VehicleMap mVehicles;
    Rtree mVehicleRtree;
    bool mRtreeTainted = false;
};

} // namespace gemv2
} // namespace artery

#endif /* VEHICLEINDEX_H_V9YES7AR */

