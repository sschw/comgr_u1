#pragma once

#include <SDL.h>
#include <random>

#include "Figure.h"
#include "Light.h"
#include "Vector3.h"
#include "Ray.h"
#include "Color.h"
#include "Scene.h"

class SceneRenderer {
	const float PDF = 1.0f / (2 * F_PI);
public:
	Scene& scene;
	bool antialiasing, depthOfField, usePathTracing;
	float depthOfFieldApart;
	Uint32 width, height, antialiasingFactor, depthOfFieldFactor, noPtRays, noPtDepth, reflectionDepth;

	SceneRenderer(Scene& scene, Uint32 width, Uint32 height, bool aa, Uint32 aaF, bool dof, Uint32 dofF, float dofA, bool ufl, Uint32 noRays, Uint32 noRec, Uint32 ref) : scene(scene), width(width), height(height), antialiasing(aa), antialiasingFactor(aaF), depthOfField(dof), depthOfFieldFactor(dofF), depthOfFieldApart(dofA), usePathTracing(ufl), noPtRays(noRays), noPtDepth(noRec), reflectionDepth(ref) {}

	Color renderPixel(Uint32 w, Uint32 h) {
		float xScale = (w - (width / 2.0f)) / (width / 2.0f);
		float yScale = (h - (height / 2.0f)) / (height / 2.0f);
		if (!usePathTracing) {
			Color c;
			if (antialiasing) {
				float sigma = 0.5;
				float weightSum = 0;
				for (Uint32 i = 0; i < antialiasingFactor; i++) {
					float randX = n_dist() / (width / 2.0f);
					float randY = n_dist() / (height / 2.0f);
					float xScaleA = xScale + randX;
					float yScaleA = yScale + randY;
					float weight = pow(F_E, -((randX*randX + randY*randY) / (2.0f * sigma*sigma))); // *(1.0 / (2.0 * F_PI*sigma*sigma))
					Ray r(scene.eye, scene.lookat, Vector2(xScaleA, yScaleA), scene.fov);
					weightSum += weight;
					c.color = c.color + ((Vector3)calcColor(r, reflectionDepth)) * weight;
				}

				c.color = c.color / weightSum;
			}
			else {
				Ray r(scene.eye, scene.lookat, Vector2(xScale, yScale), scene.fov);
				c = calcColor(r, reflectionDepth);
			}

			if (depthOfField) {
				for (Uint32 i = 0; i < depthOfFieldFactor; i++) {
					Ray r(scene.eye, scene.lookat, Vector2(xScale, yScale), scene.fov, depthOfFieldApart);
					c = ((Vector3)c) + ((Vector3)calcColor(r, reflectionDepth));
				}
				c = ((Vector3)c).divideElements((float)depthOfFieldFactor + 1);
			}
			return c;
		}
		else {
			float xScale = (w - (width / 2.0f)) / (width / 2.0f);
			float yScale = (h - (height / 2.0f)) / (height / 2.0f);
			Ray r(scene.eye, scene.lookat, Vector2(xScale, yScale), scene.fov);
			return calcColorPT(r, noPtRays, noPtDepth);
		}
	}

	HitPoint calcHitPoint(Ray& r) {
		HitPoint h = { nullptr, r, -1, Color(0) };
		for (const Figure* figure : scene.figures) {
			HitPoint j = figure->intersect(r);
			if (j.rayMultiplier >= 0 && (h.rayMultiplier == -1 || j.rayMultiplier < h.rayMultiplier)) {
				h.figure = j.figure;
				h.rayMultiplier = j.rayMultiplier;
				h.hitColor = j.hitColor;
			}
		}
		return h;
	}

	Color calcColor(Ray& r, Uint32 c) {
		HitPoint p = calcHitPoint(r);
		float ambientLight = 0.3f;
		if (p.rayMultiplier > 0) {
			Color color = p.hitColor;
			if (!p.figure->emiting)
				return color;
			Vector3 hitPointPos = p.ray.origin + p.ray.ray*p.rayMultiplier;
			Vector3 n = (hitPointPos - p.figure->pos).norm();

			Vector3 i(0, 0, 0);

			for (const Light* li : scene.light) {
				Vector3 l = (li->pos - hitPointPos).norm();
				// Add Shadow
				float shadow = 1;
				HitPoint shadowHit = calcHitPoint(Ray(hitPointPos + n*0.0001f, l));
				if (shadowHit.rayMultiplier >= 0 && shadowHit.rayMultiplier <= l.distance())
					shadow = 0.5;
				Vector3 id = (li->diffuseLightIntensity(p.figure->diffuseInt, n, l)*shadow).multiplyElements(color);
				Vector3 is = li->spectularLightIntensity(p.figure->specularInt, p.figure->specularReflection, n, l, (-r.ray).norm());
				Vector3 refVec = (p.ray.ray).reflect(n);
				i = i + li->attenuation(id, hitPointPos) + li->attenuation(is, hitPointPos);
				if (c > 0 && p.figure->reflection > 0) {
					Color ir = calcColor(Ray(hitPointPos + refVec*0.001f, refVec), c - 1);
					// REFLECT CONSTANT 0.1
					i = i + ((Vector3)ir).multiplyElements(p.figure->reflection);
				}
			}
			i = i + ambientLight;
			return Color(i);
		}
		return 0;
	}

	Color calcColorPT(Ray& r, Uint32 noRays, Uint32 noRec) {
		HitPoint p = calcHitPoint(r);
		Vector3 c(0, 0, 0);
		if (p.rayMultiplier > 0) {
			Vector3 hitPointPos = p.ray.origin + p.ray.ray*p.rayMultiplier;
			Vector3 n = (hitPointPos - p.figure->pos).norm();
			Color matEmmission = ((Vector3)p.hitColor) * p.figure->emission;
			Vector3 cRef(0, 0, 0);
			if (noRec > 1) {
				for (Uint32 i = 0; i < noRays; i++) {
					Vector3 refVec(u_11dist(), u_11dist(), u_11dist());
					while (refVec.distanceSquared() > 1) {
						refVec = Vector3(u_11dist(), u_11dist(), u_11dist());
					}
					refVec = refVec.norm();
					float refDot = n * refVec;
					if (refDot <= 0)
						refVec = -refVec, refDot = -refDot;
					Vector3 col = ((((Vector3)calcColorPT(Ray(hitPointPos + refVec*0.001f, refVec), 4, noRec - 1))*refDot).multiplyElements((Vector3)p.hitColor));
					cRef = cRef + col/PDF;
				}
				c = ((Vector3) matEmmission) + cRef / ((float) noRays);
			}
		}
		return c;
	}
};