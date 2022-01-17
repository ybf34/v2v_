


#include "maillage.h"
#include "hexagone.h"
#include "vehicle.h"
#include <math.h>
namespace  v2v{

void v2v::Maillage::createMaillage(){

    int a = sqrt(3)*(s/2);
    hex_grid[0][0] = Hexagon(0,0,0,0);
        for (int i=0; i<20; i++)
            for (int j=0; j<20; j++){
                hex_grid[i][j].setCenter((hex_grid[0][0].getX() + ( i % 2 ) * a + ( j * 2 * a ) ), (hex_grid[0][0].getY() + (i * ( (3*s) / 2 ) ) ) );
                hex_grid[i][j].calculateVertices(s);
            }
        }
}