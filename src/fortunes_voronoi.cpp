#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include "fortunes_voronoi.hpp"

const double EPS = 1e-9;
const double INF = 1e9;
const double minX = 0, maxX = 500, minY = 0, maxY = 500;

Voronoi::Voronoi(const std::vector<Point>& pts) : points(pts) {}

void Voronoi::run() {
    for (const auto& p : points) {
        Event* e = new Event{p, true};
        eventQueue.push(e);
        allocatedEvents.push_back(e);
    }

    while (!eventQueue.empty()) {
        Event* e = eventQueue.top();
        eventQueue.pop();

        if (e->isSite) {
            handleSite(e->p);
        } else if (e->valid) {
            handleCircle(e);
        }
    }

    clipEdgesToBoundingBox();
}

void Voronoi::handleSite(const Point& p) {
    if (!beachline) {
        beachline = new Arc{p};
        allocatedArcs.push_back(beachline);

        Face* face = new Face{new Point(p)};
        allocatedPoints.push_back(face->site);
        faces.push_back(face);
        beachline->face = face;

        std::cout << "Created first beachline arc at: (" << p.x << ", " << p.y << ")\n";
        return;
    }

    double ly = p.y;
    Arc* arc = findArcAbove(p.x, ly);

    if (arc->circleEvent) {
        arc->circleEvent->valid = false;
        arc->circleEvent = nullptr;
    }

    Point* start = new Point{p.x, getY(arc->site, p.x, ly)};
    allocatedPoints.push_back(start);

    Vertex* vertex = new Vertex{start};
    vertices.push_back(vertex);

    Face* newFace = new Face{new Point(p)};
    allocatedPoints.push_back(newFace->site);
    faces.push_back(newFace);

    Arc* newArc = new Arc{p};
    Arc* arcNext = new Arc{arc->site};

    allocatedArcs.push_back(newArc);
    allocatedArcs.push_back(arcNext);

    HalfEdge* e1 = createHalfEdge(vertex, nullptr, newFace, arc->face);
    HalfEdge* e2 = e1->twin;

    Edge* edge = new Edge{start, nullptr, newFace->site, arc->face->site};
    allocatedEdges.push_back(edge);

    arc->edge = newArc->edge = edge;

    arcNext->next = arc->next;
    if (arc->next) arc->next->prev = arcNext;

    arc->next = newArc;
    newArc->prev = arc;
    newArc->next = arcNext;
    arcNext->prev = newArc;

    newArc->face = newFace;
    arcNext->face = arc->face;

    newArc->edge = arc->edge;

    arc->face->outerComponent = e2;
    newFace->outerComponent = e1;

    checkCircle(arc, ly);
    checkCircle(arcNext, ly);
}

void Voronoi::handleCircle(Event* e) {
    Arc* arc = e->arc;
    if (!arc || !arc->prev || !arc->next) return;

    Point* vert = new Point{e->p.x, e->p.y};
    allocatedPoints.push_back(vert);
    Vertex* v = new Vertex{vert};
    vertices.push_back(v);

    if (arc->prev->edge) arc->prev->edge->end = vert;
    if (arc->next->edge) arc->next->edge->end = vert;

    HalfEdge* newEdge = createHalfEdge(v, nullptr, arc->prev->face, arc->next->face);
    arc->prev->face->outerComponent = newEdge;

    Edge* edge = new Edge{vert, nullptr, arc->prev->face->site, arc->next->face->site};
    allocatedEdges.push_back(edge);

    arc->prev->edge = edge;

    arc->prev->next = arc->next;
    arc->next->prev = arc->prev;

    checkCircle(arc->prev, e->p.y);
    checkCircle(arc->next, e->p.y);
}

void Voronoi::checkCircle(Arc* arc, double ly) {
    if (!arc || !arc->prev || !arc->next) return;

    Point a = arc->prev->site;
    Point b = arc->site;
    Point c = arc->next->site;

    if ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x) >= 0) return;

    Point o;
    if (!getCircleCenter(a, b, c, o)) return;

    double r = distance(o, b);
    Point p = {o.x, o.y - r};

    if (p.y >= ly) return;

    Event* e = new Event{p, false};
    e->arc = arc;
    arc->circleEvent = e;

    eventQueue.push(e);
    allocatedEvents.push_back(e);
}

bool Voronoi::getCircleCenter(const Point& a, const Point& b, const Point& c, Point& o) {
    double A = b.x - a.x;
    double B = b.y - a.y;
    double C = c.x - a.x;
    double D = c.y - a.y;
    double E = A * (a.x + b.x) + B * (a.y + b.y);
    double F = C * (a.x + c.x) + D * (a.y + c.y);
    double G = 2.0 * (A * (c.y - b.y) - B * (c.x - b.x));

    if (std::abs(G) < EPS) return false;

    o.x = (D * E - B * F) / G;
    o.y = (A * F - C * E) / G;
    return true;
}

double Voronoi::getY(const Point& p, double x, double ly) {
    double dp = 2.0 * (p.y - ly);
    if (std::abs(dp) < EPS)
        return p.y;

    double a = 1.0 / dp;
    double b = -2.0 * p.x / dp;
    double c = (p.x * p.x + p.y * p.y - ly * ly) / dp;

    return a * x * x + b * x + c;
}

void Voronoi::clipEdgesToBoundingBox() {
    for (auto& edge : allocatedArcs) {
        if (!edge || !edge->edge) continue;

        Point* start = edge->edge->start;
        Point* end = edge->edge->end;

        if (!start || !end) {
            if (edge->next && edge->next->site.x != edge->site.x) {
                double slope = (edge->next->site.y - edge->site.y) / (edge->next->site.x - edge->site.x);
                double intercept = start ? start->y - slope * start->x : 0;
                double x1 = minX, x2 = maxX;
                double y1 = slope * x1 + intercept;
                double y2 = slope * x2 + intercept;

                start = start ? start : new Point{x1, y1};
                end = end ? end : new Point{x2, y2};
            } else {
                double x = edge->site.x;
                start = start ? start : new Point{x, minY};
                end = end ? end : new Point{x, maxY};
            }

            if (!edge->edge->start) edge->edge->start = start;
            if (!edge->edge->end) edge->edge->end = end;

            allocatedPoints.push_back(start);
            allocatedPoints.push_back(end);
        }
    }
}

Arc* Voronoi::findArcAbove(double x, double ly) {
    Arc* arc = beachline;
    double y = 0.0;

    while (arc) {
        y = getY(arc->site, x, ly);
        if (!arc->next || getY(arc->next->site, x, ly) > y)
            break;
        arc = arc->next;
    }

    return arc;
}

HalfEdge* Voronoi::createHalfEdge(Vertex* from, Vertex* to, Face* leftFace, Face* rightFace) {
    HalfEdge* e1 = new HalfEdge();
    HalfEdge* e2 = new HalfEdge();

    e1->origin = from;
    e2->origin = to;

    e1->twin = e2;
    e2->twin = e1;

    e1->face = leftFace;
    e2->face = rightFace;

    halfEdges.push_back(e1);
    halfEdges.push_back(e2);

    return e1;
}

double Voronoi::distance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

void Voronoi::printEdges() {
    std::cout << "\n========== Voronoi Edges ==========\n";
    int count = 0;
    for (const Arc* arc : allocatedArcs) {
        if (!arc || !arc->edge || !arc->edge->start || !arc->edge->end) continue;

        const Point* a = arc->edge->start;
        const Point* b = arc->edge->end;

        std::cout << "Edge " << count++ << ": (" << a->x << ", " << a->y << ") -> (" << b->x << ", " << b->y << ")\n";
    }
    std::cout << "===================================\n";
}

void Voronoi::cleanup() {
    std::unordered_set<Point*> deletedPoints;

    for (Point* p : allocatedPoints) {
        if (p && deletedPoints.find(p) == deletedPoints.end()) {
            deletedPoints.insert(p);
            delete p;
        }
    }
    allocatedPoints.clear();

    for (Arc* arc : allocatedArcs)
        delete arc;
    allocatedArcs.clear();

    for (Event* e : allocatedEvents)
        delete e;
    allocatedEvents.clear();

    for (Vertex* v : vertices)
        delete v;
    vertices.clear();

    for (HalfEdge* he : halfEdges)
        delete he;
    halfEdges.clear();

    for (Face* f : faces)
        delete f;
    faces.clear();

    for (Edge* edge : allocatedEdges)
        delete edge;
    allocatedEdges.clear();

    while (!eventQueue.empty()) {
        eventQueue.pop();
    }

    beachline = nullptr;
}

