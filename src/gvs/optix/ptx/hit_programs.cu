// ///////////////////////////////////////////////////////////////////////////////////////
// Geometry Visualization Server
// Copyright (c) 2018 Logan Barnes - All Rights Reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ///////////////////////////////////////////////////////////////////////////////////////
// This define shouldn't be here. TODO: Figure out how to include the header properly
#define __CUDA_INCLUDE_COMPILER_INTERNAL_HEADERS__
#include <optix.h>
#include <optixu/optixu_math_namespace.h>

struct PerRayData {
    float3 result;
    float depth;
};

rtDeclareVariable(float3, eye, , );
rtDeclareVariable(float, scene_epsilon, , );
//rtDeclareVariable(rtObject, top_shadower, , );
//rtDeclareVariable(unsigned int, shadow_ray_type, , );

rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );

rtDeclareVariable(PerRayData, prd_current, rtPayload, );
//rtDeclareVariable(PerRayData_shadow, prd_shadow, rtPayload, );

rtDeclareVariable(float, t_hit, rtIntersectionDistance, );

rtBuffer<float4> lights;

// Material variables
rtDeclareVariable(float3, albedo, , );

RT_PROGRAM
void closest_hit_normal() {
    float3 world_geom_normal = optix::normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));
    float3 world_shading_normal = optix::normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
    float3 ffnormal = optix::faceforward(world_shading_normal, -ray.direction, world_geom_normal);

    prd_current.result = ffnormal * 0.5f + 0.5f;
}