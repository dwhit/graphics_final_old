#ifndef TERRAIN_H
#define TERRAIN_H
#include "GL/glew.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <QList>

class Terrain
{
public:
    Terrain(int decay = 2, int depth = 8, float roughness = 6 );
    virtual ~Terrain();

    /**
     * Initialize Terrain: generate VAO, vertex data, normal data, and texture coordinates, then buffer it on GPU
     *
     * Note: Should only be called after openGL has been initialized otherwise behavior is undefined!
     *
     */
    void initGL(const GLuint vertexLocation, const GLuint normalLocation, const GLuint texCoordLocation);
    void draw();
    void drawNormals();
private:
    //Methods you will need to fill in
    /**
     * You need to fill this in.
     *
     * Subdivides a square by finding the vertices at its corners, the midpoints of each side, and
     * the center (as the algorithm describes). Then recurs on each of the four sub-squares created.
     *
     * @param topLeft The grid coordinate of the top-left corner of the square to subdivide
     * @param bottomRight The grid coordinate of the bottom-right corner of the square to subdivide
     * @param depth The current recursion depth, decreasing as this function recurses deeper. The
     *              function should stop recurring when this value reaches zero.
     */
    void subdivideSquare(glm::vec2 tlg, glm::vec2 brg, GLint curDepth);
    /**
     * You need to fill this in.
     *
     * Computes the normal vector of each terrain vertex and stores it in the corresponding vertex.
     */
    void computeNormals();

    // Helper methods
    /**
      * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
      * Can be used to index into m_terrain or m_normalMap.
      * Returns -1 if the grid coordinate entered is not valid.
      */
    inline int getIndex(const glm::vec2 &c);
    /**
      * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
      * Can be used to index into m_terrain or m_normalMap.
      * Returns -1 if the grid coordinate entered is not valid.
      */
    inline int getIndex(int row, int column);

    /**
     * Computes the amount to perturb the height of the vertex currently being processed.
     * Feel free to modify this.
     *
     * @param depth The current recursion depth
     */
    double getPerturb(int cur_depth);
    /**
     * Retrieves the position of each neighbor of the given grid coordinate (i.e. all grid
     * coordinates that can be found one unit horizontally, vertically, or diagonally from
     * the specified grid coordinate).
     *
     * @param coordinate The grid coordinate whose neighbors are to be retrieved
     */
    QList<glm::vec3*> getSurroundingVertices(const glm::vec2 &coordinate);

    /**
     * Sets default values for the four corners of the terrain grid and calls subdivideSquare()
     * to begin the terrain generation process. You do not need to modify this function.
     */
    void populateTerrain();


    bool m_isInitialized;
    GLuint m_vaoID;
    GLuint m_vboID;

    /** Contains the position of the vertex at each grid point in the terrain */
    glm::vec3 *m_terrain;
    /** Contains the normal vector of the vertex at each grid point in the terrain */
    glm::vec3 *m_normalMap;
    /** The number of grid points on one side of the square terrain grid. */
    GLint m_gridLength;
    /** The number of recursion levels to use to generate terrain. Can be used as a level-of-detail parameter. */
    GLint m_depth;
    /** Controls the height of the terrain. Large values generate higher peaks and lower valleys. */
    GLfloat m_roughness;
    /** Controls how much heights can vary per recursion depth level. Higher values generate smoother terrain. */
    GLint m_decay;

    int m_numVerticesForStrip;
};

#endif // TERRAIN_H
