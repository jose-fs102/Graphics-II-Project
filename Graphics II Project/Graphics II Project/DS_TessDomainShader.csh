#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer MATRIX_BUFFER
// {
//
//   row_major float4x4 worldMatrix;    // Offset:    0 Size:    64
//   row_major float4x4 viewMatrix;     // Offset:   64 Size:    64
//   row_major float4x4 projectionMatrix;// Offset:  128 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// MATRIX_BUFFER                     cbuffer      NA          NA    0        1
//
//
//
// Patch Constant signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TessFactor            0   x           0  TRIEDGE   float       
// SV_TessFactor            1   x           1  TRIEDGE   float       
// SV_TessFactor            2   x           2  TRIEDGE   float       
// SV_InsideTessFactor      0   x           3   TRIINT   float       
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyz 
// UVS                      0   xy          1     NONE   float   xy  
// AMOUNT                   0     z         1     NONE   float       
// NORMALS                  0   xyz         2     NONE   float   xyz 
// WORLDPOS                 0   xyz         3     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// UVS                      0   xy          1     NONE   float   xy  
// AMOUNT                   0     z         1     NONE   float       
// NORMALS                  0   xyz         2     NONE   float   xyz 
// WORLDPOS                 0   xyz         3     NONE   float   xyz 
//
// Tessellation Domain   # of control points
// -------------------- --------------------
// Triangle                                3
//
ds_5_0
dcl_input_control_point_count 3
dcl_tessellator_domain domain_tri
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[12], immediateIndexed
dcl_input vDomain.xyz
dcl_input vicp[3][0].xyz
dcl_input vicp[3][1].xy
dcl_input vicp[3][2].xyz
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output o2.xyz
dcl_output o3.xyz
dcl_temps 2
mul r0.xyz, vDomain.yyyy, vicp[1][0].xyzx
mad r0.xyz, vDomain.xxxx, vicp[0][0].xyzx, r0.xyzx
mad r0.xyz, vDomain.zzzz, vicp[2][0].xyzx, r0.xyzx
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[2].xyzw, r1.xyzw
add r0.xyzw, r0.xyzw, cb0[3].xyzw
mul r1.xyzw, r0.yyyy, cb0[5].xyzw
mad r1.xyzw, r0.xxxx, cb0[4].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb0[6].xyzw, r1.xyzw
mad r1.xyzw, r0.wwww, cb0[7].xyzw, r1.xyzw
mov o3.xyz, r0.xyzx
mul r0.xyzw, r1.yyyy, cb0[9].xyzw
mad r0.xyzw, r1.xxxx, cb0[8].xyzw, r0.xyzw
mad r0.xyzw, r1.zzzz, cb0[10].xyzw, r0.xyzw
mad o0.xyzw, r1.wwww, cb0[11].xyzw, r0.xyzw
mul r0.xy, vDomain.yyyy, vicp[1][1].xyxx
mad r0.xy, vDomain.xxxx, vicp[0][1].xyxx, r0.xyxx
mad o1.xy, vDomain.zzzz, vicp[2][1].xyxx, r0.xyxx
dp3 o2.x, cb0[0].xyzx, vicp[0][2].xyzx
dp3 o2.y, cb0[1].xyzx, vicp[0][2].xyzx
dp3 o2.z, cb0[2].xyzx, vicp[0][2].xyzx
ret 
// Approximately 23 instruction slots used
#endif

const BYTE DS_TessDomainShader[] =
{
     68,  88,  66,  67,  64,  31, 
     69, 166, 153, 250,  88,  66, 
    144,  45, 118, 250,  73, 150, 
    220,  21,   1,   0,   0,   0, 
      4,   8,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    200,   1,   0,   0, 120,   2, 
      0,   0,  12,   3,   0,   0, 
    188,   3,   0,   0, 104,   7, 
      0,   0,  82,  68,  69,  70, 
    136,   1,   0,   0,   1,   0, 
      0,   0, 108,   0,   0,   0, 
      1,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  68, 
      0,   1,   0,   0,  84,   1, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     77,  65,  84,  82,  73,  88, 
     95,  66,  85,  70,  70,  69, 
     82,   0, 171, 171,  92,   0, 
      0,   0,   3,   0,   0,   0, 
    132,   0,   0,   0, 192,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 252,   0, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,  20,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  56,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     20,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     67,   1,   0,   0, 128,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  20,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 119, 111, 
    114, 108, 100,  77,  97, 116, 
    114, 105, 120,   0, 102, 108, 
    111,  97, 116,  52, 120,  52, 
      0, 171, 171, 171,   2,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   1,   0,   0, 118, 105, 
    101, 119,  77,  97, 116, 114, 
    105, 120,   0, 112, 114, 111, 
    106, 101,  99, 116, 105, 111, 
    110,  77,  97, 116, 114, 105, 
    120,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0, 171, 171, 
     73,  83,  71,  78, 168,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   7,   0,   0, 137,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   3,   0,   0, 141,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      4,   0,   0,   0, 148,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   7,   0,   0, 156,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   0,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  85,  86,  83,   0,  65, 
     77,  79,  85,  78,  84,   0, 
     78,  79,  82,  77,  65,  76, 
     83,   0,  87,  79,  82,  76, 
     68,  80,  79,  83,   0, 171, 
    171, 171,  80,  67,  83,  71, 
    140,   0,   0,   0,   4,   0, 
      0,   0,   8,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,  13,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    104,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    104,   0,   0,   0,   2,   0, 
      0,   0,  13,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
    118,   0,   0,   0,   0,   0, 
      0,   0,  14,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     83,  86,  95,  84, 101, 115, 
    115,  70,  97,  99, 116, 111, 
    114,   0,  83,  86,  95,  73, 
    110, 115, 105, 100, 101,  84, 
    101, 115, 115,  70,  97,  99, 
    116, 111, 114,   0, 171, 171, 
     79,  83,  71,  78, 168,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 140,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,  12,   0,   0, 144,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      4,  15,   0,   0, 151,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0, 159,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  85,  86, 
     83,   0,  65,  77,  79,  85, 
     78,  84,   0,  78,  79,  82, 
     77,  65,  76,  83,   0,  87, 
     79,  82,  76,  68,  80,  79, 
     83,   0,  83,  72,  69,  88, 
    164,   3,   0,   0,  80,   0, 
      4,   0, 233,   0,   0,   0, 
    147,  24,   0,   1, 149,  16, 
      0,   1, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     12,   0,   0,   0,  95,   0, 
      0,   2, 114, 192,   1,   0, 
     95,   0,   0,   4, 114, 144, 
     33,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   4,  50, 144,  33,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  95,   0,   0,   4, 
    114, 144,  33,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
    103,   0,   0,   4, 242,  32, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3,  50,  32,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      3,   0,   0,   0, 104,   0, 
      0,   2,   2,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     86, 197,   1,   0,  70, 146, 
     33,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,   6, 192, 
      1,   0,  70, 146,  33,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0, 166, 202, 
      1,   0,  70, 146,  33,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  50,   0,  16,   0, 
      0,   0,   0,   0,  86, 197, 
      1,   0,  70, 144,  33,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,   9, 
     50,   0,  16,   0,   0,   0, 
      0,   0,   6, 192,   1,   0, 
     70, 144,  33,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
     50,  32,  16,   0,   1,   0, 
      0,   0, 166, 202,   1,   0, 
     70, 144,  33,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,   9, 
     18,  32,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 146,  33,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  16,   0,   0,   9, 
     34,  32,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  70, 146,  33,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  16,   0,   0,   9, 
     66,  32,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70, 146,  33,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  23,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     21,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
