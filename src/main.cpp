
#include "voronoi_ui.hpp"
#include "fortunes_voronoi.hpp"

int main() {
    
    std::vector<Point> input = {
        {100, 100}, {200, 300}, {300, 100}, {400, 300}
    };

    Voronoi v(input);
    v.run();
    v.printEdges();  // ✅ Use printFaces instead of printEdges
    v.cleanup();
    
    VoronoiUI UI;

    if (!UI.Initialize()) {
        return -1;
    }

    UI.Run();
    return 0;
}