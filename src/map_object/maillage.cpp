


#include "maillage.h"
#include "hexagone.h"
#include <math.h>
namespace rideshare{

Maillage::Maillage(int s_,int maxi,int maxj){
    this->s=s_;
    this->maxI=maxi;
    this->maxJ=maxj;   
   this->hex_grid.resize(maxI,std::vector<Hexagon>(maxJ));
}

void Maillage::createMaillage(int s){

    int max_i = this->maxI;
    int max_j = this->maxJ;

    int a = sqrt(3)*(s/2);
    
    //hex_grid[0][0] = Hexagon(100,100,0,0);


    /*for (int i=0; i<20; i++)
        for (int j=0; j<20; j++){
            this->hex_grid[i][j].setCenter((this->hex_grid[0][0].getX() + ( i % 2 ) * a + ( j * 2 * a ) ), (this->hex_grid[0][0].getY() + (i * ( (3*s) / 2 ) ) ) );
            this->hex_grid[i][j].calculateVertices(s);
  }*/

}

}