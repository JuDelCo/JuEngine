// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/string_cast.hpp>

namespace JuEngine
{
namespace Math
{
inline float __attribute__((always_inline)) DegToRad(const float degrees)
{
	return degrees * 0.01745329251994329576923690768489;
}

inline float __attribute__((always_inline)) RadToDeg(const float radians)
{
	return radians * 57.295779513082320876798154814105;
}

inline float __attribute__((always_inline)) Min(const float x, const float y)
{
	return x <= y ? x : y;
}

inline float __attribute__((always_inline)) Max(const float x, const float y)
{
	return x >= y ? x : y;
}

inline float __attribute__((always_inline)) Clamp(const float x, const float min, const float max)
{
	return Min(Max(x, min), max);
}

inline vec2 __attribute__((always_inline)) Clamp(const vec2 x, const float min, const float max)
{
	return vec2(Min(Max(x.x, min), max), Min(Max(x.y, min), max));
}

inline vec3 __attribute__((always_inline)) Clamp(const vec3 x, const float min, const float max)
{
	return vec3(Min(Max(x.x, min), max), Min(Max(x.y, min), max), Min(Max(x.z, min), max));
}

inline vec4 __attribute__((always_inline)) Clamp(const vec4 x, const float min, const float max)
{
	return vec4(Min(Max(x.x, min), max), Min(Max(x.y, min), max), Min(Max(x.z, min), max), Min(Max(x.w, min), max));
}

inline float __attribute__((always_inline)) Dot(const vec2 x, const vec2 y)
{
	vec2 temp(x * y);

	return temp.x + temp.y;
}

inline float __attribute__((always_inline)) Dot(const vec3 x, const vec3 y)
{
	vec3 temp(x * y);

	return temp.x + temp.y + temp.z;
}

inline float __attribute__((always_inline)) Dot(const vec4 x, const vec4 y)
{
	vec4 temp(x * y);

	return (temp.x + temp.y) + (temp.z + temp.w);
}

inline float __attribute__((always_inline)) Dot(const quat x, const quat y)
{
	vec4 temp(x.x * y.x, x.y * y.y, x.z * y.z, x.w * y.w);

	return (temp.x + temp.y) + (temp.z + temp.w);
}

inline vec3 __attribute__((always_inline)) Cross(const vec3 x, const vec3 y)
{
	return vec3(x.y * y.z - y.y * x.z, x.z * y.x - y.z * x.x, x.x * y.y - y.x * x.y);
}

inline vec2 __attribute__((always_inline)) Normalize(const vec2 v)
{
	return v * (float)(1.f / sqrt(Dot(v, v)));
}

inline vec3 __attribute__((always_inline)) Normalize(const vec3 v)
{
	return v * (float)(1.f / sqrt(Dot(v, v)));
}

inline vec4 __attribute__((always_inline)) Normalize(const vec4 v)
{
	return v * (float)(1.f / sqrt(Dot(v, v)));
}

inline quat __attribute__((always_inline)) Normalize(const quat q)
{
	return q * (float)(1.f / sqrt(Dot(q, q)));
}

inline float __attribute__((always_inline)) Pitch(const quat q)
{
	return atan2(2.f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
}

inline float __attribute__((always_inline)) Yaw(const quat q)
{
	return asin(-2.f * (q.x * q.z - q.w * q.y));
}

inline float __attribute__((always_inline)) Roll(const quat q)
{
	return atan2(2.f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}

inline vec3 __attribute__((always_inline)) EulerAngles(const quat q)
{
	return vec3(Pitch(q), Yaw(q), Roll(q));
}

inline quat __attribute__((always_inline)) Conjugate(const quat q)
{
	return quat(q.w, -q.x, -q.y, -q.z);
}

inline mat3 __attribute__((always_inline)) QuatToMat(const quat q)
{
	return glm::mat3_cast(q);
}

inline quat __attribute__((always_inline)) MatToQuat(const mat3 m)
{
	return glm::quat_cast(m);
}

inline float __attribute__((always_inline)) Mix(const float x, const float y, float alpha)
{
	if(alpha > 1.f) alpha = 1.f;

	return x * (1.f - alpha) + y * alpha;
}

inline vec2 __attribute__((always_inline)) Mix(const vec2 x, const vec2 y, float alpha)
{
	if(alpha > 1.f) alpha = 1.f;

	return x * (1.f - alpha) + y * alpha;
}

inline vec3 __attribute__((always_inline)) Mix(const vec3 x, const vec3 y, float alpha)
{
	if(alpha > 1.f) alpha = 1.f;

	return x * (1.f - alpha) + y * alpha;
}

inline vec4 __attribute__((always_inline)) Mix(const vec4 x, const vec4 y, float alpha)
{
	if(alpha > 1.f) alpha = 1.f;

	return x * (1.f - alpha) + y * alpha;
}

inline quat __attribute__((always_inline)) Lerp(const quat start, const quat end, float alpha)
{
	vec4 start_v = vec4(start.x, start.y, start.z, start.w);
	vec4 end_v = vec4(end.x, end.y, end.z, end.w);
	vec4 interpolation = Normalize(Mix(start_v, end_v, alpha));

	return quat(interpolation.w, interpolation.x, interpolation.y, interpolation.z);
}

inline quat __attribute__((always_inline)) Slerp(const quat start, const quat end, float alpha)
{
	// http://number-none.com/product/Understanding Slerp, Then Not Using It/
	// Note: Excepted start & end to be normalized

	float dot = Dot(start, end); // Compute the cosine of the angle between the two vectors.

	if (dot > 0.9995f)
	{
		return Lerp(start, end, alpha);
	}

	if(alpha > 1.f) alpha = 1.f;

	Clamp(dot, -1.f, 1.f);       // Robustness: Stay within domain of acos()
	float theta_0 = acosf(dot);    // theta_0 = angle between input vectors
	float theta = theta_0 * alpha; // theta = angle between v0 (start) and result

	vec4 vStart = vec4(start.x, start.y, start.z, start.w);
	vec4 vEnd = vec4(end.x, end.y, end.z, end.w);
	vec4 res = vStart * (float)cos(theta) + Normalize(vEnd - vStart * dot) * (float)sin(theta);

	return quat(res.w, res.x, res.y, res.z);
}

inline quat __attribute__((always_inline)) LookAt(const vec3 pos, const vec3 target, const vec3 worldUp)
{
	// http://www.macaronikazoo.com/?p=491
	// http://www.falstad.com/dotproduct/

	vec3 aimZ = Normalize(target - pos);
	vec3 aimY = Normalize(worldUp - aimZ * Dot(worldUp, aimZ));
	vec3 aimX = Normalize(Cross(aimY, aimZ));

	mat3 aimRotationMatrix = mat3(1.f);
	aimRotationMatrix[0] = aimX;
	aimRotationMatrix[1] = aimY;
	aimRotationMatrix[2] = aimZ;

	return MatToQuat(aimRotationMatrix);
}

inline std::string __attribute__((always_inline)) ToString(const vec2 v)
{
	return glm::to_string(v);
}

inline std::string __attribute__((always_inline)) ToString(const vec3 v)
{
	return glm::to_string(v);
}

inline std::string __attribute__((always_inline)) ToString(const vec4 v)
{
	return glm::to_string(v);
}

inline std::string __attribute__((always_inline)) ToString(const mat3 m)
{
	return glm::to_string(m);
}

inline std::string __attribute__((always_inline)) ToString(const mat4 m)
{
	return glm::to_string(m);
}

inline const float* __attribute__((always_inline)) GetDataPtr(const vec2 v)
{
	return glm::value_ptr(v);
}

inline const float* __attribute__((always_inline)) GetDataPtr(const vec3 v)
{
	return glm::value_ptr(v);
}

inline const float* __attribute__((always_inline)) GetDataPtr(const vec4 v)
{
	return glm::value_ptr(v);
}

inline const float* __attribute__((always_inline)) GetDataPtr(const mat3 m)
{
	return glm::value_ptr(m);
}

inline const float* __attribute__((always_inline)) GetDataPtr(const mat4 m)
{
	return glm::value_ptr(m);
}
}
}
