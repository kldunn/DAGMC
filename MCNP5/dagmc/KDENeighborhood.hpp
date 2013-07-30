// MCNP5/dagmc/KDENeighborhood.hpp

#ifndef DAGMC_KDE_NEIGHBORHOOD_HPP
#define DAGMC_KDE_NEIGHBORHOOD_HPP

#include <set>

#include "TallyEvent.hpp"

// forward declarations
namespace moab {
  class AdaptiveKDTree;
  class CartVect;
  class Interface;
}

/**
 * \class KDENeighborhood
 * \brief Defines a neighborhood region for a KDE mesh tally event
 *
 * KDENeighborhood is a class that defines the neighborhood region for either
 * a collision or track-based event that is to be tallied as part of a Monte
 * Carlo particle transport simulation.  This neighborhood region is formally
 * defined as the region in space for which the kernel function produces a
 * non-trivial result for any mesh node that exists inside that region.  This
 * set of mesh nodes is also known as the set of calculation points for the
 * KDE mesh tally.
 *
 * Once a KDENeighborhood has been created, the calculation points for the
 * corresponding tally event can be obtained using the get_points() method.
 * For collision events this method will return a set of unique mesh nodes
 * that are all guaranteed to produce non-trivial tally contributions.  For
 * track-based events KDENeighborhood is only an approximation to the true
 * neighborhood region.  Therefore, for these events the get_points() method
 * may return some mesh nodes that produce trivial tally contributions.
 */
class KDENeighborhood
{
  public:
    /**
     * \brief Constructor
     * \param event the tally event for which the neighborhood is desired
     * \param bandwidth the bandwidth vector (hx, hy, hz)
     * \param kd_tree the kd-tree containing all mesh nodes in the input mesh
     * \param kd_tree_root the root of the kd-tree
     */
    KDENeighborhood(const TallyEvent& event,
                    const moab::CartVect& bandwidth,
                    moab::AdaptiveKDTree& kd_tree,
                    moab::EntityHandle& kd_tree_root);

    // >>> PUBLIC INTERFACE

    /**
     * \brief Gets the calculation points for this neighborhood region
     * \return vector of all calculation points in this neighborhood region
     *
     * The neighborhood region is currently assumed to be rectangular.  This
     * method will therefore return all of the calculation points that exist
     * within the boxed region defined by min_corner and max_corner.
     */
    std::set<moab::EntityHandle> get_points() const;

    /**
     * \brief checks if a point exists within this neighborhood region
     * \param coords the coordinates of the point to check
     * \return true if point does exist within this neighborhood region
     *
     * Currently only works for a rectangular neighborhood region.
     */
    bool point_in_region(const moab::CartVect& coords) const;

    /**
     * \brief Determines if point lies within radius of cylindrical region
     * \param point the calculation point to be tested
     * \return true if point is inside the region, false otherwise
     *
     * Note that this method is only valid for track-based events.  If the
     * event is not a track-based event, then it will always return false.
     */
    bool point_within_max_radius(const moab::CartVect& point) const;

  private:
    /// Minimum and maximum corner of a rectangular neighborhood region
    double min_corner[3];
    double max_corner[3];

    /// Radius of a cylindrical neighborhood region
    double radius;

    /// Tally event this KDENeighborhood was constructed from
    const TallyEvent& event;

    /// KD-Tree containing all mesh nodes in the input mesh
    moab::AdaptiveKDTree* kd_tree;
    moab::EntityHandle kd_tree_root;

    // >>> PRIVATE METHODS

    /**
     * \brief Sets the neighborhood region for a collision event
     * \param collision_point the location of the collision (x, y, z)
     * \param bandwidth the bandwidth vector (hx, hy, hz)
     */
    void set_neighborhood(const moab::CartVect& collision_point,
                          const moab::CartVect& bandwidth);

    /**
     * \brief Sets the neighborhood region for a track-based event
     * \param track_length the total length of the track segment
     * \param start_point the starting location of the particle (xo, yo, zo)
     * \param direction the direction the particle is traveling (uo, vo, wo)
     * \param bandwidth the bandwidth vector (hx, hy, hz)
     */
    void set_neighborhood(double track_length,
                          const moab::CartVect& start_point,
                          const moab::CartVect& direction,
                          const moab::CartVect& bandwidth);

    /**
     * \brief Finds the vertices that exist inside a rectangular region
     * \return the set of unique vertices
     *
     * Includes vertices that are within +/- 1e-12 of a box boundary.
     */
    std::set<moab::EntityHandle> points_in_box() const;
};

#endif // DAGMC_KDE_NEIGHBORHOOD_HPP

// end of MCNP5/dagmc/KDENeighborhood.hpp
