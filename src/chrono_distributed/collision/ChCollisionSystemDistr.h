// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2016 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Nic Olsen
// =============================================================================

#ifndef CHRONO_DISTRIBUTED_COLLISION_CHCOLLISIONSYSTEMDISTR_H_
#define CHRONO_DISTRIBUTED_COLLISION_CHCOLLISIONSYSTEMDISTR_H_

#include <memory>

#include "chrono/collision/ChCCollisionSystem.h"

#include "chrono_parallel/ChParallelDefines.h"
#include "chrono_parallel/ChDataManager.h"

#include "chrono_distributed/ChApiDistributed.h"
#include "chrono_distributed/physics/ChSystemDistr.h"

namespace chrono {

namespace collision {

class CH_DISTR_API ChCollisionSystemDistr : public ChCollisionSystem {
public:

	ChCollisionSystemDistr(ChParallelDataManager* dc);
    virtual ~ChCollisionSystemDistr() {}

    /// Clears all data instanced by this algorithm
    /// if any (like persistent contact manifolds)
    virtual void Clear(void) {}

    /// Adds a collision model to the collision
    /// engine (custom data may be allocated).
    virtual void Add(ChCollisionModel* model);

    /// Removes a collision model from the collision
    /// engine (custom data may be deallocated).
    virtual void Remove(ChCollisionModel* model);

    /// Removes all collision models from the collision
    /// engine (custom data may be deallocated).
    // virtual void RemoveAll();

    /// Run the algorithm and finds all the contacts.
    /// (Contacts will be managed by the Bullet persistent contact cache).
    virtual void Run();

    /// After the Run() has completed, you can call this function to
    /// fill a 'contact container', that is an object inherited from class
    /// ChContactContainerBase. For instance ChSystem, after each Run()
    /// collision detection, calls this method multiple times for all contact containers in the system,
    /// The basic behavior of the implementation is the following: collision system
    /// will call in sequence the functions BeginAddContact(), AddContact() (x n times),
    /// EndAddContact() of the contact container. But if a special container (say, GPU enabled)
    /// is passed, a more rapid buffer copy might be performed)
    virtual void ReportContacts(ChContactContainerBase* mcontactcontainer) {}

    /// After the Run() has completed, you can call this function to
    /// fill a 'proximity container' (container of narrow phase pairs), that is
    /// an object inherited from class ChProximityContainerBase. For instance ChSystem, after each Run()
    /// collision detection, calls this method multiple times for all proximity containers in the system,
    /// The basic behavior of the implementation is  the following: collision system
    /// will call in sequence the functions BeginAddProximities(), AddProximity() (x n times),
    /// EndAddProximities() of the proximity container. But if a special container (say, GPU enabled)
    /// is passed, a more rapid buffer copy might be performed)
    virtual void ReportProximities(ChProximityContainerBase* mproximitycontainer) {}

    std::vector<vec2> GetOverlappingPairs();

    void GetOverlappingAABB(custom_vector<char>& active_id, real3 Amin, real3 Amax);

    void SetAABB(real3 aabbmin, real3 aabbmax) {
        data_manager->settings.collision.aabb_min = aabbmin;
        data_manager->settings.collision.aabb_max = aabbmax;
        data_manager->settings.collision.use_aabb_active = true;
    }

    bool GetAABB(real3& aabbmin, real3& aabbmax) {
        aabbmin = data_manager->settings.collision.aabb_min;
        aabbmax = data_manager->settings.collision.aabb_max;

        return data_manager->settings.collision.use_aabb_active;
    }
private:
  ChParallelDataManager* data_manager;
  custom_vector<char> body_active;
  friend class chrono::ChSystemDistr;
};


} /* namespace collision */
} /* namespace chrono */

#endif /* CHRONO_DISTRIBUTED_COLLISION_CHCOLLISIONSYSTEMDISTR_H_ */
