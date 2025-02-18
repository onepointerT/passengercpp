
#include "svg/svg.hpp"


namespace passenger::svg {

const char* passenger( const unsigned int passenger_num ) {
    std::string str = kPassenger;
    sprintf( str.data(), passenger_num );
    return str.data();
}

const char* node( const unsigned int cx, const unsigned int cy
                , const char* descr, const unsigned int radius = 20
) {
    std::string str = kNode;
    sprintf( str.data(), radius, cx, cy, cx, cy, descr );
    return str.data();
}


const char* edge( const unsigned int start_node_outter, const unsigned int end_node_outter
                , const char* descr
) {
    std::string str = kEdge;
    sprintf( str.data(), start_node_outter, end_node_outter
                       , start_node_outter
                       , start_node_outter
                       , descr
    );
    return str.data();
}

std::unique_ptr<lunasvg::Document> make_svg( const char* data, const char* css_addition ) {
    std::unique_ptr<lunasvg::Document> doc
        = lunasvg::Document::loadFromData( data );
    doc->applyStyleSheet( kCss );
    if ( sizeof(css_addition) > 0 ) doc->applyStyleSheet( css_addition );
    return doc;
}

} // namespace passenger::svg