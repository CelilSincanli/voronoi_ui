#include "voronoi_ui.hpp"
#include <set>
#include <iostream>

int main() {

    // Optional GUI runner (commented)
    VoronoiUI UI;
    if (!UI.Initialize()) return -1;
    UI.Run();

    return 0;
}
