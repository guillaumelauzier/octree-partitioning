#include <vector>

struct Point3D {
    double x, y, z;
    // constructor
    Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

struct OctreeNode {
    std::vector<Point3D> points;
    double size;
    Point3D center;
    OctreeNode* children[8];
    // constructor
    OctreeNode(double size_, Point3D center_) : size(size_), center(center_) {
        for (int i = 0; i < 8; i++) {
            children[i] = nullptr;
        }
    }
};

void insertPoint(OctreeNode* node, Point3D point) {
    // check if point is inside the node's bounds
    double halfSize = node->size / 2;
    if (point.x >= node->center.x - halfSize && point.x <= node->center.x + halfSize &&
        point.y >= node->center.y - halfSize && point.y <= node->center.y + halfSize &&
        point.z >= node->center.z - halfSize && point.z <= node->center.z + halfSize) {
        // if node is at maximum capacity, subdivide it
        if (node->points.size() >= 8) {
            double childSize = node->size / 2;
            double x = node->center.x;
            double y = node->center.y;
            double z = node->center.z;
            node->children[0] = new OctreeNode(childSize, Point3D(x - halfSize, y - halfSize, z - halfSize));
            node->children[1] = new OctreeNode(childSize, Point3D(x + halfSize, y - halfSize, z - halfSize));
            node->children[2] = new OctreeNode(childSize, Point3D(x - halfSize, y + halfSize, z - halfSize));
            node->children[3] = new OctreeNode(childSize, Point3D(x + halfSize, y + halfSize, z - halfSize));
            node->children[4] = new OctreeNode(childSize, Point3D(x - halfSize, y - halfSize, z + halfSize));
            node->children[5] = new OctreeNode(childSize, Point3D(x + halfSize, y - halfSize, z + halfSize));
            node->children[6] = new OctreeNode(childSize, Point3D(x - halfSize, y + halfSize, z + halfSize));
            node->children[7] = new OctreeNode(childSize, Point3D(x + halfSize, y + halfSize, z + halfSize));
            // redistribute points to children
            for (Point3D p : node->points) {
                for (int i = 0; i < 8; i++) {
                    insertPoint(node->children[i], p);
                }
            }
            node->points.clear();
        }
        // add point to node
        node->points.push_back(point);
    }
}

OctreeNode* buildOctree(std::vector<Point3D>& points, double size, Point3D center) {
    OctreeNode* root = new OctreeNode(size, center);
    for (Point3D point : points) {
        insertPoint(root, point);
    }
    return root;
}
