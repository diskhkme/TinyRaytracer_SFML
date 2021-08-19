#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>
#include "ObjModel.h"

ObjModel::ObjModel(const std::string& name, const char * filename, const Material & m)
	: ModelBase{ name, m }, verts{}, faces{},
	bboxMin{}, bboxMax{}
{
	LoadModel(filename);
}

// fills verts and faces arrays, supposes .obj file to have "f " entries without slashes
void ObjModel::LoadModel(const char *filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) {
		std::cerr << "Failed to open " << filename << std::endl;
		return;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			Vec3i f;
			int idx, cnt = 0;
			iss >> trash;
			while (iss >> idx) {
				idx--; // in wavefront obj all indices start at 1, not zero
				f[cnt++] = idx;
			}
			if (3 == cnt) faces.push_back(f);
		}
	}
	std::cerr << "# v# " << verts.size() << " f# " << faces.size() << std::endl;

	get_bbox(bboxMin, bboxMax);
}

bool ObjModel::ray_aabb_intersect(const Ray& ray) const
{
	float t_min_x = (bboxMin.x - ray.origin.x) / ray.direction.x;
	float t_min_y = (bboxMin.y - ray.origin.y) / ray.direction.y;
	float t_min_z = (bboxMin.z - ray.origin.z) / ray.direction.z;

	float t_max_x = (bboxMax.x - ray.origin.x) / ray.direction.x;
	float t_max_y = (bboxMax.y - ray.origin.y) / ray.direction.y;
	float t_max_z = (bboxMax.z - ray.origin.z) / ray.direction.z;

	if (ray.direction.x < 0) {
		std::swap(t_max_x, t_min_x);
	}
	if (ray.direction.y < 0) {
		std::swap(t_max_y, t_min_y);
	}
	if (ray.direction.z < 0) {
		std::swap(t_max_z, t_min_z);
	}

	float t_enter = std::max(t_min_x, std::max(t_min_y, t_min_z));
	float t_exit = std::min(t_max_x, std::min(t_max_y, t_max_z));
	return t_exit > t_enter && t_exit >= 0;
}

// Moller and Trumbore
bool ObjModel::ray_triangle_intersect(const int &fi, const Ray& ray, Hit& hit) const {
	Vec3f edge1 = point(vert(fi, 1)) - point(vert(fi, 0));
	Vec3f edge2 = point(vert(fi, 2)) - point(vert(fi, 0));
	Vec3f pvec = cross(ray.direction, edge2);
	dist_t det = edge1 * pvec;
	if (det < 1e-5) return false;

	Vec3f tvec = ray.origin - point(vert(fi, 0));
	dist_t u = tvec * pvec;
	if (u < 0 || u > det) return false;

	Vec3f qvec = cross(tvec, edge1);
	dist_t v = ray.direction * qvec;
	if (v < 0 || u + v > det) return false;

	hit.t = edge2 * qvec * (1.0f / det);
	hit.normal = cross(edge1, edge2).normalize();
	return hit.t > 1e-5f;
}


int ObjModel::nverts() const {
	return (int)verts.size();
}

int ObjModel::nfaces() const {
	return (int)faces.size();
}

void ObjModel::get_bbox(Vec3f &min, Vec3f &max) {
	min = max = verts[0];
	for (int i = 1; i < (int)verts.size(); ++i) {
		for (int j = 0; j < 3; j++) {
			min[j] = std::min(min[j], verts[i][j]);
			max[j] = std::max(max[j], verts[i][j]);
		}
	}
	std::cerr << "bbox: [" << min << " : " << max << "]" << std::endl;
}

bool ObjModel::RayIntersect(const Ray& ray, Hit& hit) const
{
	if (!ray_aabb_intersect(ray))
		return false;

	dist_t min_t = std::numeric_limits<dist_t>::max();
	Vec3f min_normal;
	bool isIntersection = false;
	Hit tmpHit;
	for (int fi = 0; fi < nfaces(); fi++)
	{
		if (ray_triangle_intersect(fi, ray, tmpHit))
		{
			if (tmpHit.t < min_t) { // 지금 교차한 삼각형의 t가 기존 min_t보다 작으면,
				min_t = tmpHit.t; // min_t를 갱신하고,
				min_normal = tmpHit.normal; // 해당 삼각형의 normal을 기록해둠
				isIntersection = true;
			}
		}
	}

	if (isIntersection && min_t < hit.t) { // 현재 obj모델이 화면에 가장 가까운 교차이면
		hit.t = min_t;
		hit.normal = min_normal;
		hit.point = ray.At(min_t);
		hit.material = material;
		return true;
	}

	return false;
}

bool ObjModel::EditModel()
{
	return material.EditMaterial();
}

const Vec3f &ObjModel::point(int i) const {
	assert(i >= 0 && i < nverts());
	return verts[i];
}

Vec3f &ObjModel::point(int i) {
	assert(i >= 0 && i < nverts());
	return verts[i];
}

int ObjModel::vert(int fi, int li) const {
	assert(fi >= 0 && fi < nfaces() && li >= 0 && li < 3);
	return faces[fi][li];
}

std::ostream& operator<<(std::ostream& out, ObjModel &m) {
	for (int i = 0; i < m.nverts(); i++) {
		out << "v " << m.point(i) << std::endl;
	}
	for (int i = 0; i < m.nfaces(); i++) {
		out << "f ";
		for (int k = 0; k < 3; k++) {
			out << (m.vert(i, k) + 1) << " ";
		}
		out << std::endl;
	}
	return out;
}