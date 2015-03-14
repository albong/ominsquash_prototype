#include "collisions.h"

void doCollisions(){
    /*
    Divide the room into blocks of a given size.  Loop over all entities and bucket
    by blocks which they fall into.  Then check collisions between all entities in each
    bucket.  Check everything against wall collisions.
    
    Each entity should also have a pair of lists of hitboxes - one list for rectangular,
    one for circular.
    
    Entities also maybe need an enum so we know what kind of thing they are (eg player,
    enemy, environment, etc).  This way we can pass the entitiy to a collision method
    depending on what it is colliding with.  This will also save us having to calculate
    some collisions (enemies don't need to collide with other enemies?).  Alternatively,
    we can have each entity maintain a pointer to a collision function which takes in
    the thing it collided with.
    */
}
