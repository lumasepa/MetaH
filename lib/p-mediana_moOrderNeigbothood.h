#ifndef  pmedaina_moOrderNeighborhood_h
#define  pmedaina_moOrderNeighborhood_h

#include <neighborhood/moIndexNeighborhood.h>

/**
* An ordered Neighborhood
*/
template<class Neighbor>
class moOrderNeighborhood: public moIndexNeighborhood<Neighbor> {
public:

    /**
    * Define type of a solution corresponding to Neighbor
    */
    typedef typename Neighbor::EOT EOT;

    using moIndexNeighborhood<Neighbor>::getNeighborhoodSize;

    /**
    * Empty constructor
    */
    moOrderNeighborhood() :
            moIndexNeighborhood<Neighbor>(0), currentIndex(0) {
    }

    /**
    * Constructor
    * @param _neighborhoodSize the size of the neighborhood
    */
    moOrderNeighborhood(unsigned int _neighborhoodSize) :
            moIndexNeighborhood<Neighbor> (_neighborhoodSize), currentIndex(0) {
    }

    /**
    * Test if a neighbor exists
    * @param _solution the solution to explore
    * @return true if the neighborhood was not empty
    */
    virtual bool hasNeighbor(EOT& _solution) {
        return getNeighborhoodSize() > 0;
    }

    /**
    * Initialization of the neighborhood
    * @param _solution the solution to explore
    * @param _neighbor the first neighbor
    */
    virtual void init(EOT & _solution, Neighbor & _neighbor) {
        currentIndex = 0;
        _neighbor.index(_solution, currentIndex);
    }

    /**
    * Give the next neighbor
    * @param _solution the solution to explore
    * @param _neighbor the next neighbor
    */
    virtual void next(EOT & _solution, Neighbor & _neighbor) {
        currentIndex++;
        _neighbor.index(_solution, currentIndex);
    }

    /**
    * test if all neighbors are explore or not,if false, there is no neighbor left to explore
    * currentIndex is the index which have been used before, it is not the next neighbor which can be possibly evaluated
    *
    * @param _solution the solution to explore
    * @return true if there is again a neighbor to explore
    */
    virtual bool cont(EOT & _solution) {
        return (currentIndex < getNeighborhoodSize() - 1);
    }

    /**
    * Getter
    * @return the position in the Neighborhood
    */
    inline unsigned int position() const {
        return currentIndex;
    }

    /**
    * Setter the position in the Neighborhood
    */
    void setPosition(unsigned int _currentIndex) {
        currentIndex=_currentIndex;
    }

    /**
    * Return the class Name
    * @return the class name as a std::string
    */
    virtual std::string className() const {
        return "moOrderNeighborhood";
    }

protected:
    unsigned int currentIndex;

};

#endif