#pragma once

#include <Tempest/Painter>
#include <Tempest/Matrix4x4>

#include <zenload/zTypes.h>
#include <vector>
#include <functional>

#include "waypath.h"
#include "waypoint.h"

class World;
class DbgPainter;

class WayMatrix final {
  public:
    WayMatrix(World& owner,const ZenLoad::zCWayNetData& dat);

    const WayPoint* findWayPoint (const Tempest::Vec3& at, const std::function<bool(const WayPoint&)>& filter) const;
    const WayPoint* findFreePoint(const Tempest::Vec3& at, const char* name, const std::function<bool(const WayPoint&)>& filter) const;
    const WayPoint* findNextPoint(const Tempest::Vec3& at) const;

    void            addFreePoint (const Tempest::Vec3& pos, const Tempest::Vec3& dir, const char* name);
    void            addStartPoint(const Tempest::Vec3& pos, const Tempest::Vec3& dir, const char* name);

    const WayPoint& startPoint() const;
    void            buildIndex();

    const WayPoint* findPoint(const char* name, bool inexact) const;
    void            marchPoints(DbgPainter& p) const;

    WayPath         wayTo(const WayPoint &begin, const WayPoint& end) const;

  private:
    World&                 world;
    using Edge = std::pair<size_t,size_t>;
    std::vector<Edge>      edges;

    std::vector<WayPoint>  wayPoints;
    std::vector<WayPoint>  freePoints, startPoints;
    std::vector<WayPoint*> indexPoints;

    std::vector<WayPoint*> fpInd;

    struct FpIndex {
      std::string                  key;
      std::vector<const WayPoint*> index;
      };
    mutable std::vector<FpIndex>          fpIndex;

    mutable uint16_t                      pathGen=0;
    mutable std::vector<const WayPoint*>  stk[2];

    void                   adjustWaypoints(std::vector<WayPoint> &wp);

    const FpIndex&         findFpIndex(const char* name) const;
    const WayPoint*        findFreePoint(float x, float y, float z, const FpIndex &ind,
                                         const std::function<bool(const WayPoint&)>& filter) const;
  };
