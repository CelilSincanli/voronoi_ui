#ifndef FORTUNES_VORONOI_HPP
#define FORTUNES_VORONOI_HPP

#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include <limits>
#include <unordered_set>


// Basic point structure
struct Point {
    double x, y;
};

// Forward declarations
struct Arc;
struct Event;
struct Edge;

// DCEL structures
struct Vertex {
    Point* point = nullptr;
};

struct HalfEdge {
    Vertex* origin = nullptr;
    HalfEdge* twin = nullptr;
    HalfEdge* next = nullptr;
    HalfEdge* prev = nullptr;
    struct Face* face = nullptr;
};

struct Face {
    Point* site = nullptr;
    HalfEdge* outerComponent = nullptr;
};

// Event structure
struct Event {
    Point p;
    bool isSite;
    Arc* arc = nullptr;
    bool valid = true;

    bool operator<(const Event& other) const {
        return p.y == other.p.y ? p.x > other.p.x : p.y < other.p.y;
    }
};

// Arc structure for beachline
struct Arc {
    Point site;
    Arc* prev = nullptr;
    Arc* next = nullptr;
    Event* circleEvent = nullptr;
    Edge* edge = nullptr;
    Face* face = nullptr;
};

// Edge structure
struct Edge {
    Point* start = nullptr;
    Point* end = nullptr;
    Point* left = nullptr;
    Point* right = nullptr;
};

struct EventCompare {
    bool operator()(const Event* a, const Event* b) const {
        return a->p.y == b->p.y ? a->p.x > b->p.x : a->p.y < b->p.y;
    }
};

class Voronoi {
public:
    Voronoi(const std::vector<Point>& pts);
    void run();
    void printFaces();
    void cleanup();
    void printEdges();

private:
    std::vector<Point> points;
    std::priority_queue<Event*, std::vector<Event*>, EventCompare> eventQueue;

    Arc* beachline = nullptr;

    std::vector<Vertex*> vertices;
    std::vector<HalfEdge*> halfEdges;
    std::vector<Face*> faces;

    std::vector<Point*> allocatedPoints;
    std::vector<Arc*> allocatedArcs;
    std::vector<Event*> allocatedEvents;
    std::vector<Edge*> allocatedEdges;

    void handleSite(const Point& p);
    void handleCircle(Event* e);
    Arc* findArcAbove(double x, double ly);
    void checkCircle(Arc* arc, double ly);
    double getXBreak(Arc* arc, double ly);
    double getY(const Point& site, double x, double ly);
    double distance(const Point& a, const Point& b);
    double ccw(const Point& a, const Point& b, const Point& c);
    bool circleCenter(const Point& a, const Point& b, const Point& c, Point& center);
    HalfEdge* createHalfEdge(Vertex* from, Vertex* to, Face* leftFace, Face* rightFace);
    void clipEdgesToBoundingBox();
    bool getCircleCenter(const Point& a, const Point& b, const Point& c, Point& o);
};

#endif // FORTUNES_VORONOI_HPP
