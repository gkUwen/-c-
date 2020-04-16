// #include <rt/solids/striangle.h>
// #include <rt/intersection.h>

// namespace rt {

// SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
// {
//     /* TODO */
// }

// SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
// {
//     /* TODO */
// }

// Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const {
//     /* TODO */ NOT_IMPLEMENTED;
// }

// }

#include "striangle.h"
#include <core/assert.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <rt/intersection.h>

namespace rt
{
	SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper * texMapper, Material * material)
	{
		this->v1 = vertices[0];
		this->v2 = vertices[1];
		this->v3 = vertices[2];

		normalVec1 = normals[0].normalize();
		normalVec2 = normals[1].normalize();
		normalVec3 = normals[2].normalize();
		
		this->bbox = BBox(min(v1, v2), max(v1, v2));
		this->bbox.extend(v3);

		this->texMapper = texMapper;
		this->material = material;
	}

	SmoothTriangle::SmoothTriangle(const Point & v1, const Point & v2, const Point & v3, const Vector & n1, const Vector & n2, const Vector & n3, CoordMapper * texMapper, Material * material)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;

		normalVec1 = n1.normalize();
		normalVec2 = n2.normalize();
		normalVec3 = n3.normalize();

		this->bbox = BBox(min(v1, v2), max(v1, v2));
		this->bbox.extend(v3);

		this->texMapper = texMapper;
		this->material = material;
	}

	Intersection SmoothTriangle::intersect(const Ray & ray, float previousBestDistance) const
	{
		Vector v1v2 = v2 - v1;
		Vector v1v3 = v3 - v1;
		Vector normalVector = cross(v1v2, v1v3);
		float triangleArea = normalVector.length() * 0.5f;

		InfinitePlane plane = InfinitePlane(v1, normalVector.normalize(), texMapper, material);
		Intersection planeIntersection = plane.intersect(ray, previousBestDistance);
		Intersection intersection;

		if (planeIntersection)
		{
			Point p = planeIntersection.hitPoint();
			Vector v3v2 = v3 - v2;
			Vector _v2 = p - v2;
            Vector _v3;
            Vector v1v3pNormal;
            Vector iNorm;

            float u, v, w;

			Vector v3v2pNormal = cross(v3v2, _v2);
			u = v3v2pNormal.length() / 2 * triangleArea;
			if (dot(normalVector, v3v2pNormal) < 0)return Intersection();

			_v3 = p - v3;
			v1v3pNormal = cross(_v3, v1v3);
			v = v1v3pNormal.length() / 2 * triangleArea;
			if (dot(normalVector, v1v3pNormal) < 0) return Intersection();

			w = 1 - u - v;
			if (w < 0 || w > 1) return Intersection();
            
			if (dot(normalVector, ray.d.normalize()) > 0) normalVector = -1.0f * normalVector;

			iNorm = u * normalVec1 + v * normalVec2 + w * normalVec3;
            intersection.localPoint = (u * v1 + v * v2 + w * v3);
			intersection = Intersection(planeIntersection.distance, ray, this, iNorm, ray.getPoint(planeIntersection.distance));
			
		}

		return intersection;
	}
}