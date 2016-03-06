// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include <GLM/glm.hpp>
#include <string>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
typedef glm::fquat quat;

namespace JuEngine
{
namespace Math
{
constexpr float PI = 3.14159265359f;

float DegToRad(const float degrees);
float RadToDeg(const float radians);
float Min(const float x, const float y);
float Max(const float x, const float y);
float Clamp(const float x, const float min = 0.f, const float max = 1.f);
vec2 Clamp(const vec2 x, const float min = 0.f, const float max = 1.f);
vec3 Clamp(const vec3 x, const float min = 0.f, const float max = 1.f);
vec4 Clamp(const vec4 x, const float min = 0.f, const float max = 1.f);
float Dot(const vec2 x, const vec2 y);
float Dot(const vec3 x, const vec3 y);
float Dot(const vec4 x, const vec4 y);
float Dot(const quat x, const quat y);
vec3 Cross(const vec3 x, const vec3 y);
vec2 Normalize(const vec2 v);
vec3 Normalize(const vec3 v);
vec4 Normalize(const vec4 v);
quat Normalize(const quat q);
float Pitch(const quat q);
float Yaw(const quat q);
float Roll(const quat q);
vec3 EulerAngles(const quat q);
quat Conjugate(const quat q);
mat3 QuatToMat(const quat q);
quat MatToQuat(const mat3 m);
float Mix(const float x, const float y, float alpha);
vec2 Mix(const vec2 x, const vec2 y, float alpha);
vec3 Mix(const vec3 x, const vec3 y, float alpha);
vec4 Mix(const vec4 x, const vec4 y, float alpha);
quat Lerp(const quat start, const quat end, float alpha);
quat Slerp(const quat start, const quat end, float alpha);
quat LookAt(const vec3 posA, const vec3 posB, const vec3 worldUp = vec3(0.f, 1.f, 0.f));
std::string ToString(const vec2 v);
std::string ToString(const vec3 v);
std::string ToString(const vec4 v);
std::string ToString(const mat3 m);
std::string ToString(const mat4 m);
const float* GetDataPtr(const vec2 v);
const float* GetDataPtr(const vec3 v);
const float* GetDataPtr(const vec4 v);
const float* GetDataPtr(const mat3 m);
const float* GetDataPtr(const mat4 m);
}
}

#include "Math.inl"
