
#pragma once

#include <string>

#include <lunasvg.h>


namespace passenger::svg {

static const char kPassenger[] = R"SVG(
<svg xmlns="https://www.w3.org/2000/svg" viewBox="0 0 18 18" width="18" height="18">
    <!-- A passenger marker -->
    <polygon points="0,9 0,9 9,0 9,0" class="passenger" />

    <!-- A description %i for the marker -->
    <text x="5" y="5" dx="5" dy="5" textLength="10" class="passenger text">%i</text>
</svg>
)SVG";

static const char kEdge[] = R"SVG(
<svg xmlns="https://www.w3.org/2000/svg" viewBox="0 0 18 18" width="18" height="18">
    <!-- A edge from i1 to i2 -->
    <path points="M%i LS%i Z" class="edge" />

    <!-- A description for the edge at i3,i4 with the value s -->
    <text class="edge text" x="%i" y="%i">
        %s
    </text>
</svg>
)SVG";

static const char kNode[] = R"SVG(
<svg xmlns="https://www.w3.org/2000/svg" viewBox="0 0 18 18" width="18" height="18">
    <!-- A node of the radius size i1 at position i2,i3 -->
    <circle r="%i" cx="%i" cy="%i" class="node" />

    <!-- A description for the edge -->
    <text class="node text" x="%i-5" y="%i-5">%s</text>
</svg>
)SVG";

static const char kCss[] = R"CSS(
.text { font-size: 10; font-palette: #000000; font-family: system-ui; font: bold; font-optical-sizing: auto; font-display: block; text-align: center; text-anchor: middle; }
.passenger { fill: #000000; stroke: #00ffff; font-palette: #0000bb; font-size: 5; }
.edge { fill: #000000; stroke: #ff0000; font-palette: #ff00; }
.node { fill: #00ffff; stroke: #fff000; font-palette: #ff00; }
)CSS";


const char* passenger( const unsigned int passenger_num );
const char* node( const unsigned int cx, const unsigned int cy
                 , const char* descr, const unsigned int radius = 20
);
const char* edge( const unsigned int start_node_outter, const unsigned int end_node_outter
                 , const char* descr
);

std::unique_ptr<lunasvg::Document> make_svg( const char* data, const char* css_addition = "" );

} // namespace passenger::svg

