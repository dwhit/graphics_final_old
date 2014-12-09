#include "terrain.h"
#include <iostream>


Terrain::Terrain(int decay, int depth, float roughness)
{
    // Determines how much each vertex gets perturbed. The larger the value, the less pertubration
    // will occur per recursive value.
    m_decay = decay;

    // The number of levels of recursion your terrain uses (can be considered the level of detail
    // of your terrain).
    m_depth = depth;

    // The roughness of your terrain. Higher roughnesses entail taller mountains and deeper
    // valleys. Lower values entail small hills and shallow valleys
    m_roughness = roughness;

    // Imagining your terrain as a square grid of points, gridLength is the number of elements on
    // one side, or the length of a side of the grid.
    m_gridLength = (1 << m_depth)+1;

    // Knowing the number of elements on a side gives us the total number of elements in the grid.
    int terrain_array_size = m_gridLength * m_gridLength;

    // Most of the rows are repeated in the triangle strip. The only two that aren't are the first
    // and last.
    m_numVerticesForStrip = m_gridLength*(2*m_gridLength-2);

    // Represents all of our vertices. Picture this as a grid.
    m_terrain = new glm::vec3[terrain_array_size];

    // Represents all the normals, one per each vertex.
    m_normalMap = new glm::vec3[terrain_array_size];
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
}

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
void Terrain::subdivideSquare(glm::vec2 topleft, glm::vec2 botright, GLint curDepth)
{
    // TL--TM--TR    +---> x
    // |   |   |     |
    // ML--MM--MR    V
    // |   |   |     y
    // BL--BM--BR


    // Corner coordinates (in the grid space [x,y])
    glm::vec2 TL = glm::vec2(topleft.x, topleft.y);
    glm::vec2 TR = glm::vec2(botright.x, topleft.y);
    glm::vec2 BL = glm::vec2(topleft.x, botright.y);
    glm::vec2 BR = glm::vec2(botright.x, botright.y);

    // Corner vertices on the terrain (in the world space [x,y,z])
    glm::vec3 &vTL = m_terrain[getIndex(TL)];
    glm::vec3 &vTR = m_terrain[getIndex(TR)];
    glm::vec3 &vBL = m_terrain[getIndex(BL)];
    glm::vec3 &vBR = m_terrain[getIndex(BR)];

    glm::vec2 TM = (TL + TR) / glm::vec2(2);
    glm::vec2 BM = (BL + BR) / glm::vec2(2);
    glm::vec2 ML = (TL + BL) / glm::vec2(2);
    glm::vec2 MR = (TR + BR) / glm::vec2(2);
    glm::vec2 MM = (TL + BR) / glm::vec2(2);

    m_terrain[getIndex(TM)] = (vTL + vTR) / glm::vec3(2);
    m_terrain[getIndex(BM)] = (vBL + vBR) / glm::vec3(2);
    m_terrain[getIndex(ML)] = (vTL + vBL) / glm::vec3(2);
    m_terrain[getIndex(MR)] = (vTR + vBR) / glm::vec3(2);
    m_terrain[getIndex(MM)] = (vTL + vTR + vBL + vBR) / glm::vec3(4) + glm::vec3(0, 0.2*getPerturb(curDepth), 0);


    if(curDepth - 1 > 0) {
    subdivideSquare(TL, MM, curDepth-1);
    subdivideSquare(TM, MR, curDepth-1);
    subdivideSquare(ML, BM, curDepth-1);
    subdivideSquare(MM, BR, curDepth-1);
    }



    // @TODO: Above, we demonstrate how to find
    //            a) the coordinates in the grid of the corner vertices (in grid space)
    //            b) the actual (X,Y,Z) points in the terrain map
    //
    // Now, you just need to compute the midpoints in grid space, average the heights of the
    // neighboring vertices, and write the new points to the terrain map. Remember to perturb the
    // new center vertex by a random amount. Scale this perturbation by curDepth so that the
    // perturbation factor is less as you recur deeper.
    //
    // Remember that [X,Y] grid points map to [X,Z] in world space -- the Y coordinate in world
    // space will be used for the height.
    //
    // Once you compute these remaining 5 points, you will need to recur on the
    // four sub-squares you've just created. To do this, you'll call subdivideSquare
    // recursively, decrementing curDepth by one.


}

/**
 * You need to fill this in.
 *
 * Computes the normal vector of each terrain vertex and stores it in the corresponding vertex.
 */
void Terrain::computeNormals()
{
    // For each vertex in the 2D grid...
    for (int row = 0; row < m_gridLength; row++)
    {
        for (int column = 0; column < m_gridLength; column++)
        {
            // 2D coordinate of the vertex on the terrain grid.
            const glm::vec2 gridPosition(row, column);

            // Index into the 1D position and normal arrays.
            const int terrainIndex = getIndex(gridPosition);

            // 3D position of the vertex.
            const glm::vec3 &vertexPosition = m_terrain[terrainIndex];

            // Get the neighbors of the vertex at (a,b).
            const QList<glm::vec3*>& neighbors = getSurroundingVertices(gridPosition);
            int numNeighbors = neighbors.size();

            // @TODO: Compute vectors from vertexPosition to each neighbor.
            glm::vec3 *vertToNeighbors = new glm::vec3[numNeighbors];

            for(int i = 0; i < numNeighbors; i++)
            {
                vertToNeighbors[i] = glm::vec3(neighbors[i]->x - vertexPosition.x, neighbors[i]->y - vertexPosition.y, neighbors[i]->z - vertexPosition.z);
            }


            // @TODO: Compute cross products for each neighbor.
            glm::vec3 *normals = new glm::vec3[numNeighbors];
            for(int i = 0; i < numNeighbors; i++)
            {
                normals[i] = glm::cross(vertToNeighbors[i], vertToNeighbors[(i+1)%numNeighbors]);
            }


            // @TODO: Compute the vertex normal and store in m_normalMap.
            float magnitude = 0;
            glm::vec3 normal(0, 0, 0);

            for(int i = 0; i < numNeighbors; i++)
            {
                normal.x += normals[i].x;
                normal.y += normals[i].y;
                normal.z += normals[i].z;

                magnitude+= sqrt(normals[i].x*normals[i].x + normals[i].y*normals[i].y + normals[i].z*normals[i].z);
            }

            normal.x /= magnitude;
            normal.y /= magnitude;
            normal.z /= magnitude;

            m_normalMap[getIndex(row, column)] = normal;

            delete[] vertToNeighbors;
            delete[] normals;
        }
    }
}

/**
 * You need to fill this in.
 *
 * Fill in the buffer with the correct data. Take a look at lab 2 if you need a refresher on how to
 * do this.
 *
 * @param positionAttrib Index of the position attribute
 * @param normalAttrib Index of the normal attribute
 * @param texCoordAttrib Index of the texture coordinate attribute
 */
void Terrain::initGL(
        const GLuint positionAttrib, const GLuint normalAttrib, const GLuint texCoordAttrib)
{
    populateTerrain();
    computeNormals();


    // @TODO: Fill in an array of buffer data such that the terrain can be rendered using
    // GL_TRIANGLE_STRIP.

    GLfloat* vertexData = new GLfloat[(2*m_gridLength)*(m_gridLength-1)*8];

    //std::cout << m_gridLength << std::endl;

    //std::cout << (2*m_gridLength)*(m_gridLength-1)*6 << std::endl;

    int index;
    int count = 0;
    for (int row = 0; row < m_gridLength - 1; row++)
    {
        for (int column = 0; column < m_gridLength; column++)
        {
              index = getIndex(row, column);

              //add vertex, and add vertex BELOW it.
              //
              //  0   2   4  etc
              //  | / | / | /
              //  1   3   5
              //
              //so the first pass will fill in (1,2) then the next (3,4) and so on

              vertexData[count++] = m_terrain[index].x;
              vertexData[count++] = m_terrain[index].y;
              vertexData[count++] = m_terrain[index].z;
              vertexData[count++] = m_normalMap[index].x;
              vertexData[count++] = m_normalMap[index].y;
              vertexData[count++] = m_normalMap[index].z;
              vertexData[count++] = (float) column/m_gridLength;
              vertexData[count++] = (float) row/m_gridLength;

              index = getIndex(row+1, column);

              vertexData[count++] = m_terrain[index].x;
              vertexData[count++] = m_terrain[index].y;
              vertexData[count++] = m_terrain[index].z;
              vertexData[count++] = m_normalMap[index].x;
              vertexData[count++] = m_normalMap[index].y;
              vertexData[count++] = m_normalMap[index].z;
              vertexData[count++] = (float) column/m_gridLength;
              vertexData[count++] = (float) row/m_gridLength;

        }
    }

    //std::cout << count << std::endl

    // VAO init
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Buffer init
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


    // @TODO: Pass your data to the GPU and specify how the attributes are organized. Recall
    //        that the relevant functions are glBufferData, glEnableVertexAttribArray, and
    //        glVertexAttribPointer.

    glBufferData(GL_ARRAY_BUFFER, count*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) 0);

    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (3*sizeof(GLfloat) ));

    glEnableVertexAttribArray(texCoordAttrib);
    glVertexAttribPointer(texCoordAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (6*sizeof(GLfloat) ));



    // Clean up and unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_isInitialized = true;
    delete[] vertexData;
}

/**
 * Draws two rows of the mesh at time
 */
void Terrain::draw(){
    if (!m_isInitialized){
        std::cout << "You must call initGL() before you can draw!" << std::endl;
    } else{
        // Rebind your vertex array and draw the triangles
        glBindVertexArray(m_vaoID);
        for (int row = 0; row < m_gridLength-1; row++){
        //for (int row = 0; row < 2; row++){
            glDrawArrays(GL_TRIANGLE_STRIP, m_gridLength*2*row, m_gridLength*2);
        }

        glBindVertexArray(0);
    }
}

void Terrain::drawNormals(){
    glColor3f(1,0,0);

    for (int row = 0; row < m_gridLength; row++)
    {
        for (int column = 0; column < m_gridLength; column++)
        {
            glBegin(GL_LINES);

            glm::vec3 curVert = m_terrain[getIndex(row, column)];
            glm::vec3 curNorm = m_normalMap[getIndex(row, column)];

            glNormal3f(curNorm.x, curNorm.y, curNorm.z);
            glVertex3f(curVert.x, curVert.y, curVert.z);
            glVertex3f(curVert.x +curNorm.x,
                       curVert.y + curNorm.y,
                       curVert.z + curNorm.z);

            glEnd();
        }
    }

}

/**
 * Computes the amount to perturb the height of the vertex currently being processed.
 * Feel free to modify this.
 *
 * @param depth The current recursion depth
 */
double Terrain::getPerturb(int cur_depth)
{
    return m_roughness
           * pow((double)cur_depth / m_depth, m_decay)
           * ((rand() % 200-100) / 100.0);
}

/****************************************************************************************************************/
/**********************************DO NOT MODIFY ANY OF THE FOLLOWING CODE***************************************/
/****************************************************************************************************************/
/**
 * Sets default values for the four corners of the terrain grid and calls subdivideSquare()
 * to begin the terrain generation process. You do not need to modify this function.
 */
void Terrain::populateTerrain()
{
    // World coordinates of corners.
    glm::vec3 tl(-10, 0, -10);
    glm::vec3 tr(10, 0, -10);
    glm::vec3 bl(-10, 0, 10);
    glm::vec3 br(10, 0, 10);

    // Grid coordinates of corners.
    glm::vec2 tlg(0,0);
    glm::vec2 trg(0,m_gridLength-1);
    glm::vec2 blg(m_gridLength-1, 0);
    glm::vec2 brg(m_gridLength-1, m_gridLength-1);

    m_terrain[getIndex(tlg)] = tl;
    m_terrain[getIndex(trg)] = tr;
    m_terrain[getIndex(blg)] = bl;
    m_terrain[getIndex(brg)] = br;

    // Begin recursive terrain generation.
    subdivideSquare(tlg, brg, m_depth);
}

/**
 * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
 * Can be used to index into m_terrain or m_normalMap.
 * Returns -1 if the grid coordinate entered is not valid.
 */
inline int Terrain::getIndex(const glm::vec2 &c)
{
    return getIndex(c.x, c.y);
}

/**
 * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
 * Can be used to index into m_terrain or m_normalMap.
 * Returns -1 if the grid coordinate entered is not valid.
 */
inline int Terrain::getIndex(int row, int col)
{
    if (row < 0 || row >= m_gridLength || col < 0 || col >= m_gridLength)
        return -1;

    return row * m_gridLength + col;
}

/**
 * Retrieves the position of each neighbor of the given grid coordinate (i.e. all grid
 * coordinates that can be found one unit horizontally, vertically, or diagonally from
 * the specified grid coordinate).
 *
 * @param coordinate The grid coordinate whose neighbors are to be retrieved
 */
QList<glm::vec3*> Terrain::getSurroundingVertices(const glm::vec2 &coordinate)
{
    // List of [x,y] grid offsets to find the neighbors of a vertex. i.e. (0,-1), (1,-1), etc.
    int offsets[] = {0,-1,  +1,-1,  +1,0,  +1,+1,  0,+1,  -1,+1,  -1,0,  -1,-1};

    QList<glm::vec3*> vecs;
    for (int i = 0; i < 8; i++)
    {
        int index = getIndex(coordinate + glm::vec2(offsets[2*i], offsets[2*i+1]));
        if (index != -1)
            vecs.push_back(& m_terrain[index]);
    }

    return vecs;
}
