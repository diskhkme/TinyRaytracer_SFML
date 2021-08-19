#pragma once
// From https://github.com/ssloy/tinyraytracer/blob/homework_assignment/model.h
#include <vector>
#include <string>
#include "../Geometry.h"
#include "ModelBase.h"

class ObjModel : public ModelBase {
private:
	std::vector<Vec3f> verts;
	std::vector<Vec3i> faces;
	Vec3f bboxMin;
	Vec3f bboxMax;
public:
	ObjModel(const std::string& name, const char* filename, const Material& m);
	void LoadModel(const char *filename);

	int nverts() const;                          // number of vertices
	int nfaces() const;                          // number of triangles

	bool ray_triangle_intersect(const int &fi, const Ray& ray, Hit& hit) const;
	bool ray_aabb_intersect(const Ray& ray) const;

	const Vec3f &point(int i) const;                   // coordinates of the vertex i
	Vec3f &point(int i);                   // coordinates of the vertex i
	int vert(int fi, int li) const;              // index of the vertex for the triangle fi and local index li
	void get_bbox(Vec3f &min, Vec3f &max); // bounding box for all the vertices, including isolated ones

public:
	virtual bool RayIntersect(const Ray& ray, Hit& hit) const override;

	virtual bool EditModel() override;
};

std::ostream& operator<<(std::ostream& out, ObjModel &m);