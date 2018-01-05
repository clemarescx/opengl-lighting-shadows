#ifndef _GEOMETRY_MANAGER_
#define _GEOMETRY_MANAGER_

/**
 * Simple class to encapsulate vertex data for different test
 * geometries
 */
class GeometryManager {
public:
	/**
	 * @return Pointer to vertex data
	 * (see also getCubeNVertices())
	 */
	static const float* getCubeVertices();

	/**
	 * @return Pointer to vertex normal data
	 * (see also getCubeNVertices())
	 */
	static const float* getCubeNormals();

	/**
	 * @return Pointer to vertex texture coordinate
	 * data (see also getCubeNVertices()
	 */
	static const float* getCubeTexCoords();

	/**
	 * @return The number of vertices for the cube.
	 */
	static unsigned int getCubeNVertices();

	/**
	 * @return The number of triangles for the cube (defined as vertices/3)
	 */
	static unsigned int getCubeNTriangles();
};

#endif
