#ifndef VORONOI_HPP
#define VORONOI_HPP

#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include <limits>
#include <unordered_set>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

#include <boost/variant.hpp>

// typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel                  K;
typedef CGAL::Delaunay_triangulation_2<K>                                    DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT,AT,AP>                                    VD;

typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;

typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             Vertex_handle;
typedef VD::Face_handle               Face_handle;
typedef VD::Halfedge_handle           Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;

class GeometryUtils {
    private:

        inline void print_endpoint(Halfedge_handle e, bool is_src);
    public:
        std::map<Point_2, std::vector<Point_2>> voronoi_face_vertex_map;
        std::vector<Point_2> voronoi_points;
        void UpdateVoronoiFaces(const std::vector<Point_2>& points, std::map<Point_2, std::vector<Point_2>>& face_vertex_map);

};

#endif // VORONOI_HPP
