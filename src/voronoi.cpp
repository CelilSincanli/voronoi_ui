#include "voronoi.hpp"

#include <unordered_set>
#include <algorithm>


void GeometryUtils::UpdateVoronoiFaces(const std::vector<Point_2>& points, std::map<Point_2, std::vector<Point_2>>& face_vertex_map) {
    VD vd;

    // Insert points into the Voronoi diagram
    for (const auto& p : points) {
        vd.insert(Site_2(p));
    }

    // Ensure the Voronoi diagram is valid
    assert(vd.is_valid());

    // Process each point and update the face_vertex_map
    for (const auto& p : points) {
        Locate_result lr = vd.locate(p);

        if (const Vertex_handle* v = boost::get<Vertex_handle>(&lr)) {
            // Handle vertex case (optional, not stored in face_vertex_map)
        } else if (const Halfedge_handle* e = boost::get<Halfedge_handle>(&lr)) {
            // Handle edge case (optional, not stored in face_vertex_map)
        } else if (const Face_handle* f = boost::get<Face_handle>(&lr)) {
            // Extract vertices of the Voronoi face
            std::vector<Point_2> face_vertices;
            Ccb_halfedge_circulator ec_start = (*f)->ccb();
            Ccb_halfedge_circulator ec = ec_start;
            do {
                if (ec->has_target()) {
                    Point_2 pt = ec->target()->point();
                    face_vertices.push_back(pt);
                } else {
                    // Optional: handle points at infinity
                }
            } while (++ec != ec_start);

            // Store the face vertices in the map
            face_vertex_map[p] = face_vertices;
        }
    }
}

inline void GeometryUtils::print_endpoint(Halfedge_handle e, bool is_src) {
    std::cout << "\t";
    if (is_src) {
        if (e->has_source())
            std::cout << e->source()->point() << std::endl;
        else
            std::cout << "point at infinity" << std::endl;
    } else {
        if (e->has_target())
            std::cout << e->target()->point() << std::endl;
        else
            std::cout << "point at infinity" << std::endl;
    }
}