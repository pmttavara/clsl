#include <stdio.h>
#undef bool
#undef true
#undef false
enum { false, true }; typedef _Bool bool;
typedef unsigned int uint;
#define VECDEF(T) \
typedef union T##2 { T raw[2]; struct { T x, y; }; } T##2; \
typedef union T##3 { \
    T raw[3]; \
    T##2 xy; \
    struct { T x; union { T y, z; T##2 yz; }; }; \
} T##3; \
typedef union T##4 { \
    T raw[4]; \
    T##3 xyz; \
    struct { T x, y, z, w; }; \
    struct { T##2 xy, zw; }; \
    struct { T x_; union { T##2 yz; T##3 yzw; }; }; \
} T##4;
VECDEF(float) VECDEF(double) VECDEF(int) VECDEF(uint) VECDEF(bool);
#define VECT2U(T,U,...) static inline T U##_to_##T(U v) { return (T)__VA_ARGS__; }
#define DEF2VEC(T,N,...) VECT2U(T,float##N,__VA_ARGS__) VECT2U(T,double##N,__VA_ARGS__) \
VECT2U(T,int##N,__VA_ARGS__) VECT2U(T,uint##N,__VA_ARGS__) VECT2U(T,bool##N,__VA_ARGS__)
#define DEF2VEC2(T) DEF2VEC(T,,{v}) DEF2VEC(T,2,{v.x, v.y})
#define DEF2VEC3(T) DEF2VEC2(T) DEF2VEC(T,3,{v.x, v.y, v.z})
#define DEF2VEC4(T) DEF2VEC3(T) DEF2VEC(T,4,{v.x, v.y, v.z, v.w})
#define DEF2VECA(T) DEF2VEC(T,,{v}) DEF2VEC2(T##2) DEF2VEC3(T##3) DEF2VEC4(T##4)
DEF2VECA(float);
DEF2VECA(double);
DEF2VECA(int);
DEF2VECA(uint);
DEF2VECA(bool);
#define GEN_TO_VEC(T,N) float##N: float##N##_to_##T, double##N: double##N##_to_##T, \
int##N: int##N##_to_##T, uint##N: uint##N##_to_##T, bool##N: bool##N##_to_##T
#define GEN_TO_VEC2(T) GEN_TO_VEC(T,), GEN_TO_VEC(T,2)
#define GEN_TO_VEC3(T) GEN_TO_VEC2(T), GEN_TO_VEC(T,3)
#define GEN_TO_VEC4(T) GEN_TO_VEC3(T), GEN_TO_VEC(T,4)
#define TO_VEC(T, x) _Generic(x, GEN_TO_VEC(T,))
#define TO_VEC2(T, x) _Generic(x, GEN_TO_VEC2(T))
#define TO_VEC3(T, x) _Generic(x, GEN_TO_VEC3(T))
#define TO_VEC4(T, x) _Generic(x, GEN_TO_VEC4(T))
#define VEC3_x_y(T)    static inline T##3 T##3##_from_x_y   (T x, T y) { return (T##3){.x = x, .y = y }; }
#define VEC3_x_yz(T)   static inline T##3 T##3##_from_x_yz  (T x, T##2 yz) { return (T##3){.x = x, .yz = yz }; }
#define VEC3_xy_z(T)   static inline T##3 T##3##_from_xy_z  (T##2 xy, T z) { return (T##3){.xy = xy, .z = z }; }
#define VEC4_x_y(T)    static inline T##4 T##4##_from_x_y   (T x, T y) { return (T##4){.x = x, .y = y }; }
#define VEC4_x_yz(T)   static inline T##4 T##4##_from_x_yz  (T x, T##2 yz) { return (T##4){.x = x, .yz = yz }; }
#define VEC4_xy_z(T)   static inline T##4 T##4##_from_xy_z  (T##2 xy, T z) { return (T##4){.xy = xy, .z = z }; }
#define VEC4_x_y_z(T)  static inline T##4 T##4##_from_x_y_z (T x, T y, T z) { return (T##4){.x = x, .y = y, .z = z }; }
#define VEC4_x_y_zw(T) static inline T##4 T##4##_from_x_y_zw(T x, T y, T##2 zw) { return (T##4){.x = x, .y = y, .zw = zw}; }
#define VEC4_x_yz_w(T) static inline T##4 T##4##_from_x_yz_w(T x, T##2 yz, T w) { return (T##4){.x = x, .yz = yz, .w = w}; }
#define VEC4_xy_z_w(T) static inline T##4 T##4##_from_xy_z_w(T##2 xy, T z, T w) { return (T##4){.xy = xy, .z = z, .w = w}; }
#define VEC4_x_yzw(T)  static inline T##4 T##4##_from_x_yzw (T x, T##3 yzw)     { return (T##4){.x = x, .yzw = yzw}; }
#define VEC4_xy_zw(T)  static inline T##4 T##4##_from_xy_zw (T##2 xy, T##2 zw)  { return (T##4){.xy = xy, .zw = zw}; }
#define VEC4_xyz_w(T)  static inline T##4 T##4##_from_xyz_w (T##3 xyz, T w)     { return (T##4){.xyz = xyz, .w = w}; }
#define VEC_PERMUTED_ARGS(T) VEC3_x_y(T) VEC3_x_yz(T) VEC3_xy_z(T) VEC4_x_y(T) VEC4_x_yz(T) VEC4_xy_z(T) VEC4_x_y_zw(T)  VEC4_x_yz_w(T) VEC4_xy_z_w(T) VEC4_x_yzw(T) VEC4_xy_zw(T) VEC4_xyz_w(T)
VEC_PERMUTED_ARGS(float)
VEC_PERMUTED_ARGS(double)
VEC_PERMUTED_ARGS(int)
VEC_PERMUTED_ARGS(uint)
VEC_PERMUTED_ARGS(bool) 
#define GEN_TO_TSIZE(T, N, x) \
float##N: TO_VEC##N(T##N, x), \
double##N: TO_VEC##N(T##N, x), \
int##N: TO_VEC##N(T##N, x), \
uint##N: TO_VEC##N(T##N, x), \
bool##N: TO_VEC##N(T##N, x)
#define GEN_TO_TSIZES(T, x) GEN_TO_VEC(T,), GEN_TO_VEC(T##2,2), GEN_TO_VEC(T##3,3), GEN_TO_VEC(T##4,4)
#define TO_TSIZE(T, x) _Generic(x, GEN_TO_TSIZES(T, x))
#define VEC2_0(T, dummy) ((T##2){0})
#define VEC2_1(T, x) TO_VEC2(T##2, x)(x)
#define VEC2_2(T, x, y) ((T##2){.raw = {(x), (y)}})
#define VEC3_0(T, dummy) ((T##3){0})
#define VEC3_1(T, x) TO_VEC3(T##3, x)(x)
#define VEC3_2(T, first, second) \
_Generic(TO_TSIZE(T, first)(first), \
    T: _Generic(TO_TSIZE(T, second)(second), \
        T: T##3##_from_x_y, \
        T##2: T##3##_from_x_yz \
    ), \
    T##2: T##3##_from_xy_z \
)(TO_TSIZE(T, first)(first), TO_TSIZE(T, second)(second))
#define VEC3_3(T, x, y, z) ((T##3){.raw = {(x), (y), (z)}})
#define VEC4_0(T, dummy) ((T##4){0})
#define VEC4_1(T, x) TO_VEC4(T##4, x)(x)
#define VEC4_2(T, first, second) \
_Generic(TO_TSIZE(T, first)(first), \
    T: _Generic(TO_TSIZE(T, second)(second), \
        T: T##4##_from_x_y, \
        T##2: T##4##_from_x_yz, \
        T##3: T##4##_from_x_yzw \
    ), \
    T##2: _Generic(TO_TSIZE(T, second)(second), \
        T: T##4##_from_xy_z, \
        T##2: T##4##_from_xy_zw \
    ), \
    T##3: _Generic(TO_TSIZE(T, second)(second), \
        T: T##4##_from_xyz_w, \
        T##2: T##4##_from_xyz_w \
    ) \
)(TO_TSIZE(T, first)(first), TO_TSIZE(T, second)(second))
// @Todo: #define VEC4_3(T, first, second, third)
#define VEC4_4(T, x, y, z, w) ((T##4){.raw = {(x), (y), (z), (w)}})
#define PP_ARG_N(_1, _2, _3, _4, _5, N, ...) N
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)    
#define PP_NARG(...)  PP_NARG_(_, ##__VA_ARGS__, 4, 3, 2, 1, 0)
#define PASTE_(a, b) a ## b
#define PASTE(a, b) PASTE_(a, b)
#define float(...) ((float)(__VA_ARGS__))
#define float2(...) PASTE(VEC2_, PP_NARG(__VA_ARGS__))(float, __VA_ARGS__)
#define float3(...) PASTE(VEC3_, PP_NARG(__VA_ARGS__))(float, __VA_ARGS__)
#define float4(...) PASTE(VEC4_, PP_NARG(__VA_ARGS__))(float, __VA_ARGS__)
#define double(...) ((double)(__VA_ARGS__))
#define double2(...) PASTE(VEC2_, PP_NARG(__VA_ARGS__))(double, __VA_ARGS__)
#define double3(...) PASTE(VEC3_, PP_NARG(__VA_ARGS__))(double, __VA_ARGS__)
#define double4(...) PASTE(VEC4_, PP_NARG(__VA_ARGS__))(double, __VA_ARGS__)
#define int(...) ((int)(__VA_ARGS__))
#define int2(...) PASTE(VEC2_, PP_NARG(__VA_ARGS__))(int, __VA_ARGS__)
#define int3(...) PASTE(VEC3_, PP_NARG(__VA_ARGS__))(int, __VA_ARGS__)
#define int4(...) PASTE(VEC4_, PP_NARG(__VA_ARGS__))(int, __VA_ARGS__)
#define uint(...) ((uint)(__VA_ARGS__))
#define uint2(...) PASTE(VEC2_, PP_NARG(__VA_ARGS__))(uint, __VA_ARGS__)
#define uint3(...) PASTE(VEC3_, PP_NARG(__VA_ARGS__))(uint, __VA_ARGS__)
#define uint4(...) PASTE(VEC4_, PP_NARG(__VA_ARGS__))(uint, __VA_ARGS__)
#define bool(...) ((bool)(__VA_ARGS__))
#define bool2(...) PASTE(VEC2_, PP_NARG(__VA_ARGS__))(bool, __VA_ARGS__)
#define bool3(...) PASTE(VEC3_, PP_NARG(__VA_ARGS__))(bool, __VA_ARGS__)
#define bool4(...) PASTE(VEC4_, PP_NARG(__VA_ARGS__))(bool, __VA_ARGS__)
#define vec2 float2
#define dvec2 double2
#define ivec2 int2
#define uvec2 uint2
#define bvec2 bool2
#define vec3 float3
#define dvec3 double3
#define ivec3 int3
#define uvec3 uint3
#define bvec3 bool3
#define vec4 float4
#define dvec4 double4
#define ivec4 int4
#define uvec4 uint4
#define bvec4 bool4



int main(int argc, char **argv) {

    bvec4 x = {bool(argc > 0), bool(argc > 1), bool(argc > 2), bool(argc > 4)};
    ivec3 x3 = int3(vec4(x).xyz);

    ivec2 twodee = ivec2(3.5f, 75.3f);

    vec4 v = vec4(x.z, twodee);
    vec4 vasdf = vec4(x.xyz, 3);

    printf("v is %f, %f, %f, %f.\n", v.x, v.y, v.z, v.w);

}
