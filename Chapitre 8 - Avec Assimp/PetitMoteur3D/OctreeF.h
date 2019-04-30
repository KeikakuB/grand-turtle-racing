#pragma once

template <typename T>

// Simple point data type to insert into the tree.
// Have something with more interesting behavior inherit
// from this in order to store other attributes in the tree.
class OctreePoint
{
  T *data;
  physx::PxVec3 position_;
public:
  OctreePoint<T>() {}
  OctreePoint<T>(T* data, physx::PxVec3 position) : data(data), position_(position) {}
  inline const T& getData() const { return data; }
  inline void setData(T* dt) { data = dt; }
  inline const physx::PxVec3& getPosition() const { return position_; }
  inline void setPosition(const physx::PxVec3& position) { position_ = position; }
};

template <typename T>
class Octree
{
private:
  static const int MAX_CAPACITY = 8;

public:
  // Physical position/size. This implicitly defines the bounding
  // box of this node
  physx::PxVec3 origin;         //! The physical center of this node

  physx::PxVec3 halfDimension;  //! Half the width/height/depth of this node

  physx::PxVec3 Origin() const { return origin; }
  void Origin(physx::PxVec3 val) { origin = val; }

  physx::PxVec3 HalfDimension() const { return halfDimension; }
  void HalfDimension(physx::PxVec3 val) { halfDimension = val; }

  // The tree has up to eight children and can additionally store
  // a point, though in many applications only, the leaves will store data.
  std::vector<Octree<T>*> children;
  OctreePoint<T> *dataTree;   //! Data point to be stored at a node

                              /*
                              Children follow a predictable pattern to make accesses simple.
                              Here, - means less than 'origin' in that dimension, + means greater than.
                              child:	0 1 2 3 4 5 6 7
                              x:      - - - - + + + +
                              y:      - - + + - - + +
                              z:      - + - + - + - +
                              */

  Octree()
    : origin(0, 0, 0), halfDimension(1, 1, 1), dataTree(nullptr)
  {
    // Initially, there are no children
    for ( int i = 0; i < MAX_CAPACITY; i++ )
    {
      children[ i ] = nullptr;
    }
  }

  Octree(const physx::PxVec3& origin, const physx::PxVec3& halfDimension)
    : origin(origin), halfDimension(halfDimension), dataTree(nullptr)
  {
    // Initially, there are no children
    for ( int i = 0; i < MAX_CAPACITY; i++ )
    {
      children[ i ] = nullptr;
    }
  }

  Octree(const Octree& copy)
    : origin(copy.origin), halfDimension(copy.halfDimension), dataTree(copy.dataTree)
  {
    for ( int i = 0; i < MAX_CAPACITY; i++ )
    {
      children[ i ] = copy.children[ i ];
    }
  }

  ~Octree()
  {
    // Recursively destroy octants
  }

  // Determine which octant of the tree would contain 'point'
  int getOctantContainingPoint(const physx::PxVec3& point) const
  {
    int oct = 0;
    if ( point.x >= origin.x ) oct |= 4;
    if ( point.y >= origin.y ) oct |= 2;
    if ( point.z >= origin.z ) oct |= 1;
    return oct;
  }

  bool isLeafNode() const
  {
    // This is correct, but overkill. See below.
    /*
    for(int i=0; i<8; ++i)
    if(children[i] != NULL)
    return false;
    return true;
    */

    // We are a leaf iff we have no children. Since we either have none, or
    // all eight, it is sufficient to just check the first.
    // return children.front() == NULL;
    return children.size() == 0;
  }

  void insert(OctreePoint<T>* point)
  {
    // If this node doesn't have a data point yet assigned
    // and it is a leaf, then we're done!
    if ( isLeafNode() )
    {
      if ( dataTree == nullptr )
      {
        dataTree = point;
        return;
      }
      else
      {
        // We're at a leaf, but there's already something here
        // We will split this node so that it has 8 child octants
        // and then insert the old data that was here, along with
        // this new data point

        // Save this data point that was here for a later re-insert
        OctreePoint<T> *oldPoint = dataTree;
        dataTree = nullptr;

        // Split the current node and create new empty trees for each
        // child octant.
        for ( int i = 0; i < MAX_CAPACITY; ++i )
        {
          // Compute new bounding box for this child
          physx::PxVec3 newOrigin = origin;
          newOrigin.x += halfDimension.x * ( i & 4 ? .5f : -.5f );
          newOrigin.y += halfDimension.y * ( i & 2 ? .5f : -.5f );
          newOrigin.z += halfDimension.z * ( i & 1 ? .5f : -.5f );
          children[ i ] = new Octree(newOrigin, halfDimension*.5f);
        }

        // Re-insert the old point, and insert this new point
        // (We wouldn't need to insert from the root, because we already
        // know it's guaranteed to be in this section of the tree)
        children[ getOctantContainingPoint(oldPoint->getPosition()) ]->insert(oldPoint);
        children[ getOctantContainingPoint(point->getPosition()) ]->insert(point);
      }
    }
    else
    {
      // We are at an interior node. Insert recursively into the
      // appropriate child octant
      int octant = getOctantContainingPoint(point->getPosition());
      children[ octant ]->insert(point);
    }
  }

  // This is a really simple routine for querying the tree for points
  // within a bounding box defined by min/max points (bmin, bmax)
  // All results are pushed into 'results'
  void getPointsInsideBox(const physx::PxVec3& bmin, const physx::PxVec3& bmax, std::vector<OctreePoint<T>*>& results)
  {
    // If we're at a leaf node, just see if the current data point is inside
    // the query bounding box
    if ( isLeafNode() )
    {
      if ( dataTree != nullptr )
      {
        const physx::PxVec3& p = dataTree->getPosition();
        if ( p.x > bmax.x || p.y > bmax.y || p.z > bmax.z ) return;
        if ( p.x < bmin.x || p.y < bmin.y || p.z < bmin.z ) return;
        results.push_back(dataTree);
      }
    }
    else
    {
      // We're at an interior node of the tree. We will check to see if
      // the query bounding box lies outside the octants of this node.
      for ( int i = 0; i < MAX_CAPACITY; ++i )
      {
        // Compute the min/max corners of this child octant
        physx::PxVec3 cmax = children[ i ]->origin + children[ i ]->halfDimension;
        physx::PxVec3 cmin = children[ i ]->origin - children[ i ]->halfDimension;

        // If the query rectangle is outside the child's bounding box,
        // then continue
        if ( cmax.x<bmin.x || cmax.y < bmin.y || cmax.z < bmin.z ) continue;
        if ( cmin.x > bmax.x || cmin.y > bmax.y || cmin.z>bmax.z ) continue;

        // At this point, we've determined that this child is intersecting
        // the query bounding box
        children[ i ]->getPointsInsideBox(bmin, bmax, results);
      }
    }
  }
};

//"cf http://www.brandonpelfrey.com/blog/coding-a-simple-octree/ "