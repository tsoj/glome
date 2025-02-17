/// @brief Include to translate GLI enums to OpenGL enums
/// @file gli/gl.hpp

#pragma once

#include "format.hpp"
#include "target.hpp"
#include <array>

namespace gli
{
    /// Translation class to convert GLI enums into OpenGL values
    class gl
    {
    public:
        enum internal_format
        {
            INTERNAL_RGB_UNORM = 0x1907,            //GL_RGB
            INTERNAL_BGR_UNORM = 0x80E0,        //GL_BGR
            INTERNAL_RGBA_UNORM = 0x1908,        //GL_RGBA
            INTERNAL_BGRA_UNORM = 0x80E1,        //GL_BGRA
            INTERNAL_BGRA8_UNORM = 0x93A1,        //GL_BGRA8_EXT

            // unorm formats
                INTERNAL_R8_UNORM = 0x8229,            //GL_R8
            INTERNAL_RG8_UNORM = 0x822B,        //GL_RG8
            INTERNAL_RGB8_UNORM = 0x8051,        //GL_RGB8
            INTERNAL_RGBA8_UNORM = 0x8058,        //GL_RGBA8

            INTERNAL_R16_UNORM = 0x822A,        //GL_R16
            INTERNAL_RG16_UNORM = 0x822C,        //GL_RG16
            INTERNAL_RGB16_UNORM = 0x8054,        //GL_RGB16
            INTERNAL_RGBA16_UNORM = 0x805B,        //GL_RGBA16

            INTERNAL_RGB10A2_UNORM = 0x8059,    //GL_RGB10_A2
            INTERNAL_RGB10A2_SNORM_EXT = 0xFFFC,

            // snorm formats
                INTERNAL_R8_SNORM = 0x8F94,            //GL_R8_SNORM
            INTERNAL_RG8_SNORM = 0x8F95,        //GL_RG8_SNORM
            INTERNAL_RGB8_SNORM = 0x8F96,        //GL_RGB8_SNORM
            INTERNAL_RGBA8_SNORM = 0x8F97,        //GL_RGBA8_SNORM

            INTERNAL_R16_SNORM = 0x8F98,        //GL_R16_SNORM
            INTERNAL_RG16_SNORM = 0x8F99,        //GL_RG16_SNORM
            INTERNAL_RGB16_SNORM = 0x8F9A,        //GL_RGB16_SNORM
            INTERNAL_RGBA16_SNORM = 0x8F9B,        //GL_RGBA16_SNORM

            // unsigned integer formats
                INTERNAL_R8U = 0x8232,                //GL_R8UI
            INTERNAL_RG8U = 0x8238,                //GL_RG8UI
            INTERNAL_RGB8U = 0x8D7D,            //GL_RGB8UI
            INTERNAL_RGBA8U = 0x8D7C,            //GL_RGBA8UI

            INTERNAL_R16U = 0x8234,                //GL_R16UI
            INTERNAL_RG16U = 0x823A,            //GL_RG16UI
            INTERNAL_RGB16U = 0x8D77,            //GL_RGB16UI
            INTERNAL_RGBA16U = 0x8D76,            //GL_RGBA16UI

            INTERNAL_R32U = 0x8236,                //GL_R32UI
            INTERNAL_RG32U = 0x823C,            //GL_RG32UI
            INTERNAL_RGB32U = 0x8D71,            //GL_RGB32UI
            INTERNAL_RGBA32U = 0x8D70,            //GL_RGBA32UI

            INTERNAL_RGB10A2U = 0x906F,            //GL_RGB10_A2UI
            INTERNAL_RGB10A2I_EXT = 0xFFFB,

            // signed integer formats
                INTERNAL_R8I = 0x8231,                //GL_R8I
            INTERNAL_RG8I = 0x8237,                //GL_RG8I
            INTERNAL_RGB8I = 0x8D8F,            //GL_RGB8I
            INTERNAL_RGBA8I = 0x8D8E,            //GL_RGBA8I

            INTERNAL_R16I = 0x8233,                //GL_R16I
            INTERNAL_RG16I = 0x8239,            //GL_RG16I
            INTERNAL_RGB16I = 0x8D89,            //GL_RGB16I
            INTERNAL_RGBA16I = 0x8D88,            //GL_RGBA16I

            INTERNAL_R32I = 0x8235,                //GL_R32I
            INTERNAL_RG32I = 0x823B,            //GL_RG32I
            INTERNAL_RGB32I = 0x8D83,            //GL_RGB32I
            INTERNAL_RGBA32I = 0x8D82,            //GL_RGBA32I

            // Floating formats
                INTERNAL_R16F = 0x822D,                //GL_R16F
            INTERNAL_RG16F = 0x822F,            //GL_RG16F
            INTERNAL_RGB16F = 0x881B,            //GL_RGB16F
            INTERNAL_RGBA16F = 0x881A,            //GL_RGBA16F

            INTERNAL_R32F = 0x822E,                //GL_R32F
            INTERNAL_RG32F = 0x8230,            //GL_RG32F
            INTERNAL_RGB32F = 0x8815,            //GL_RGB32F
            INTERNAL_RGBA32F = 0x8814,            //GL_RGBA32F

            INTERNAL_R64F_EXT = 0xFFFA,            //GL_R64F
            INTERNAL_RG64F_EXT = 0xFFF9,        //GL_RG64F
            INTERNAL_RGB64F_EXT = 0xFFF8,        //GL_RGB64F
            INTERNAL_RGBA64F_EXT = 0xFFF7,        //GL_RGBA64F

            // sRGB formats
                INTERNAL_SR8 = 0x8FBD,                //GL_SR8_EXT
            INTERNAL_SRG8 = 0x8FBE,                //GL_SRG8_EXT
            INTERNAL_SRGB8 = 0x8C41,            //GL_SRGB8
            INTERNAL_SRGB8_ALPHA8 = 0x8C43,        //GL_SRGB8_ALPHA8

            // Packed formats
                INTERNAL_RGB9E5 = 0x8C3D,            //GL_RGB9_E5
            INTERNAL_RG11B10F = 0x8C3A,            //GL_R11F_G11F_B10F
            INTERNAL_RG3B2 = 0x2A10,            //GL_R3_G3_B2
            INTERNAL_R5G6B5 = 0x8D62,            //GL_RGB565
            INTERNAL_RGB5A1 = 0x8057,            //GL_RGB5_A1
            INTERNAL_RGBA4 = 0x8056,            //GL_RGBA4

            INTERNAL_RG4_EXT = 0xFFFE,

            // Luminance Alpha formats
                INTERNAL_LA4 = 0x8043,                //GL_LUMINANCE4_ALPHA4
            INTERNAL_L8 = 0x8040,                //GL_LUMINANCE8
            INTERNAL_A8 = 0x803C,                //GL_ALPHA8
            INTERNAL_LA8 = 0x8045,                //GL_LUMINANCE8_ALPHA8
            INTERNAL_L16 = 0x8042,                //GL_LUMINANCE16
            INTERNAL_A16 = 0x803E,                //GL_ALPHA16
            INTERNAL_LA16 = 0x8048,                //GL_LUMINANCE16_ALPHA16

            // Depth formats
                INTERNAL_D16 = 0x81A5,                //GL_DEPTH_COMPONENT16
            INTERNAL_D24 = 0x81A6,                //GL_DEPTH_COMPONENT24
            INTERNAL_D16S8_EXT = 0xFFF6,
            INTERNAL_D24S8 = 0x88F0,            //GL_DEPTH24_STENCIL8
            INTERNAL_D32 = 0x81A7,                //GL_DEPTH_COMPONENT32
            INTERNAL_D32F = 0x8CAC,                //GL_DEPTH_COMPONENT32F
            INTERNAL_D32FS8X24 = 0x8CAD,        //GL_DEPTH32F_STENCIL8
            INTERNAL_S8_EXT = 0x8D48,            //GL_STENCIL_INDEX8

            // Compressed formats
                INTERNAL_RGB_DXT1 = 0x83F0,                        //GL_COMPRESSED_RGB_S3TC_DXT1_EXT
            INTERNAL_RGBA_DXT1 = 0x83F1,                    //GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
            INTERNAL_RGBA_DXT3 = 0x83F2,                    //GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
            INTERNAL_RGBA_DXT5 = 0x83F3,                    //GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
            INTERNAL_R_ATI1N_UNORM = 0x8DBB,                //GL_COMPRESSED_RED_RGTC1
            INTERNAL_R_ATI1N_SNORM = 0x8DBC,                //GL_COMPRESSED_SIGNED_RED_RGTC1
            INTERNAL_RG_ATI2N_UNORM = 0x8DBD,                //GL_COMPRESSED_RG_RGTC2
            INTERNAL_RG_ATI2N_SNORM = 0x8DBE,                //GL_COMPRESSED_SIGNED_RG_RGTC2
            INTERNAL_RGB_BP_UNSIGNED_FLOAT = 0x8E8F,        //GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
            INTERNAL_RGB_BP_SIGNED_FLOAT = 0x8E8E,            //GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT
            INTERNAL_RGB_BP_UNORM = 0x8E8C,                    //GL_COMPRESSED_RGBA_BPTC_UNORM
            INTERNAL_RGB_PVRTC_4BPPV1 = 0x8C00,                //GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
            INTERNAL_RGB_PVRTC_2BPPV1 = 0x8C01,                //GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
            INTERNAL_RGBA_PVRTC_4BPPV1 = 0x8C02,            //GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
            INTERNAL_RGBA_PVRTC_2BPPV1 = 0x8C03,            //GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
            INTERNAL_RGBA_PVRTC_4BPPV2 = 0x9137,            //GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG
            INTERNAL_RGBA_PVRTC_2BPPV2 = 0x9138,            //GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG
            INTERNAL_ATC_RGB = 0x8C92,                        //GL_ATC_RGB_AMD
            INTERNAL_ATC_RGBA_EXPLICIT_ALPHA = 0x8C93,        //GL_ATC_RGBA_EXPLICIT_ALPHA_AMD
            INTERNAL_ATC_RGBA_INTERPOLATED_ALPHA = 0x87EE,    //GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD

            INTERNAL_RGB_ETC = 0x8D64,                        //GL_COMPRESSED_RGB8_ETC1
            INTERNAL_RGB_ETC2 = 0x9274,                        //GL_COMPRESSED_RGB8_ETC2
            INTERNAL_RGBA_PUNCHTHROUGH_ETC2 = 0x9276,        //GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2
            INTERNAL_RGBA_ETC2 = 0x9278,                    //GL_COMPRESSED_RGBA8_ETC2_EAC
            INTERNAL_R11_EAC = 0x9270,                        //GL_COMPRESSED_R11_EAC
            INTERNAL_SIGNED_R11_EAC = 0x9271,                //GL_COMPRESSED_SIGNED_R11_EAC
            INTERNAL_RG11_EAC = 0x9272,                        //GL_COMPRESSED_RG11_EAC
            INTERNAL_SIGNED_RG11_EAC = 0x9273,                //GL_COMPRESSED_SIGNED_RG11_EAC

            INTERNAL_RGBA_ASTC_4x4 = 0x93B0,                //GL_COMPRESSED_RGBA_ASTC_4x4_KHR
            INTERNAL_RGBA_ASTC_5x4 = 0x93B1,                //GL_COMPRESSED_RGBA_ASTC_5x4_KHR
            INTERNAL_RGBA_ASTC_5x5 = 0x93B2,                //GL_COMPRESSED_RGBA_ASTC_5x5_KHR
            INTERNAL_RGBA_ASTC_6x5 = 0x93B3,                //GL_COMPRESSED_RGBA_ASTC_6x5_KHR
            INTERNAL_RGBA_ASTC_6x6 = 0x93B4,                //GL_COMPRESSED_RGBA_ASTC_6x6_KHR
            INTERNAL_RGBA_ASTC_8x5 = 0x93B5,                //GL_COMPRESSED_RGBA_ASTC_8x5_KHR
            INTERNAL_RGBA_ASTC_8x6 = 0x93B6,                //GL_COMPRESSED_RGBA_ASTC_8x6_KHR
            INTERNAL_RGBA_ASTC_8x8 = 0x93B7,                //GL_COMPRESSED_RGBA_ASTC_8x8_KHR
            INTERNAL_RGBA_ASTC_10x5 = 0x93B8,                //GL_COMPRESSED_RGBA_ASTC_10x5_KHR
            INTERNAL_RGBA_ASTC_10x6 = 0x93B9,                //GL_COMPRESSED_RGBA_ASTC_10x6_KHR
            INTERNAL_RGBA_ASTC_10x8 = 0x93BA,                //GL_COMPRESSED_RGBA_ASTC_10x8_KHR
            INTERNAL_RGBA_ASTC_10x10 = 0x93BB,                //GL_COMPRESSED_RGBA_ASTC_10x10_KHR
            INTERNAL_RGBA_ASTC_12x10 = 0x93BC,                //GL_COMPRESSED_RGBA_ASTC_12x10_KHR
            INTERNAL_RGBA_ASTC_12x12 = 0x93BD,                //GL_COMPRESSED_RGBA_ASTC_12x12_KHR

            // sRGB formats
                INTERNAL_SRGB_DXT1 = 0x8C4C,                    //GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
            INTERNAL_SRGB_ALPHA_DXT1 = 0x8C4D,                //GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
            INTERNAL_SRGB_ALPHA_DXT3 = 0x8C4E,                //GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
            INTERNAL_SRGB_ALPHA_DXT5 = 0x8C4F,                //GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
            INTERNAL_SRGB_BP_UNORM = 0x8E8D,                //GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM
            INTERNAL_SRGB_PVRTC_2BPPV1 = 0x8A54,            //GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT
            INTERNAL_SRGB_PVRTC_4BPPV1 = 0x8A55,            //GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT
            INTERNAL_SRGB_ALPHA_PVRTC_2BPPV1 = 0x8A56,        //GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT
            INTERNAL_SRGB_ALPHA_PVRTC_4BPPV1 = 0x8A57,        //GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT
            INTERNAL_SRGB_ALPHA_PVRTC_2BPPV2 = 0x93F0,        //COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV2_IMG
            INTERNAL_SRGB_ALPHA_PVRTC_4BPPV2 = 0x93F1,        //GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV2_IMG
            INTERNAL_SRGB8_ETC2 = 0x9275,                        //GL_COMPRESSED_SRGB8_ETC2
            INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 = 0x9277,    //GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2
            INTERNAL_SRGB8_ALPHA8_ETC2_EAC = 0x9279,            //GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC
            INTERNAL_SRGB8_ALPHA8_ASTC_4x4 = 0x93D0,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_5x4 = 0x93D1,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_5x5 = 0x93D2,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_6x5 = 0x93D3,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_6x6 = 0x93D4,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_8x5 = 0x93D5,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_8x6 = 0x93D6,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_8x8 = 0x93D7,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_10x5 = 0x93D8,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_10x6 = 0x93D9,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_10x8 = 0x93DA,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_10x10 = 0x93DB,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_12x10 = 0x93DC,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR
            INTERNAL_SRGB8_ALPHA8_ASTC_12x12 = 0x93DD,        //GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR

            INTERNAL_ALPHA8 = 0x803C,
            INTERNAL_ALPHA16 = 0x803E,
            INTERNAL_LUMINANCE8 = 0x8040,
            INTERNAL_LUMINANCE16 = 0x8042,
            INTERNAL_LUMINANCE8_ALPHA8 = 0x8045,
            INTERNAL_LUMINANCE16_ALPHA16 = 0x8048,

            INTERNAL_R8_USCALED_GTC = 0xF000,
            INTERNAL_R8_SSCALED_GTC,
            INTERNAL_RG8_USCALED_GTC,
            INTERNAL_RG8_SSCALED_GTC,
            INTERNAL_RGB8_USCALED_GTC,
            INTERNAL_RGB8_SSCALED_GTC,
            INTERNAL_RGBA8_USCALED_GTC,
            INTERNAL_RGBA8_SSCALED_GTC,
            INTERNAL_RGB10A2_USCALED_GTC,
            INTERNAL_RGB10A2_SSCALED_GTC,
            INTERNAL_R16_USCALED_GTC,
            INTERNAL_R16_SSCALED_GTC,
            INTERNAL_RG16_USCALED_GTC,
            INTERNAL_RG16_SSCALED_GTC,
            INTERNAL_RGB16_USCALED_GTC,
            INTERNAL_RGB16_SSCALED_GTC,
            INTERNAL_RGBA16_USCALED_GTC,
            INTERNAL_RGBA16_SSCALED_GTC,
        };

        enum external_format
        {
            EXTERNAL_NONE = 0,                    //GL_NONE
            EXTERNAL_RED = 0x1903,                //GL_RED
            EXTERNAL_RG = 0x8227,                //GL_RG
            EXTERNAL_RGB = 0x1907,                //GL_RGB
            EXTERNAL_BGR = 0x80E0,                //GL_BGR
            EXTERNAL_RGBA = 0x1908,                //GL_RGBA
            EXTERNAL_BGRA = 0x80E1,                //GL_BGRA
            EXTERNAL_RED_INTEGER = 0x8D94,        //GL_RED_INTEGER
            EXTERNAL_RG_INTEGER = 0x8228,        //GL_RG_INTEGER
            EXTERNAL_RGB_INTEGER = 0x8D98,        //GL_RGB_INTEGER
            EXTERNAL_BGR_INTEGER = 0x8D9A,        //GL_BGR_INTEGER
            EXTERNAL_RGBA_INTEGER = 0x8D99,        //GL_RGBA_INTEGER
            EXTERNAL_BGRA_INTEGER = 0x8D9B,        //GL_BGRA_INTEGER
            EXTERNAL_DEPTH = 0x1902,            //GL_DEPTH_COMPONENT
            EXTERNAL_DEPTH_STENCIL = 0x84F9,    //GL_DEPTH_STENCIL
            EXTERNAL_STENCIL = 0x1901,            //GL_STENCIL_INDEX

            EXTERNAL_LUMINANCE = 0x1909,                //GL_LUMINANCE
            EXTERNAL_ALPHA = 0x1906,                    //GL_ALPHA
            EXTERNAL_LUMINANCE_ALPHA = 0x190A,            //GL_LUMINANCE_ALPHA

            EXTERNAL_SRGB_EXT = 0x8C40,                    //SRGB_EXT
            EXTERNAL_SRGB_ALPHA_EXT = 0x8C42            //SRGB_ALPHA_EXT
        };

        enum type_format
        {
            TYPE_NONE = 0,                        //GL_NONE
            TYPE_I8 = 0x1400,                    //GL_BYTE
            TYPE_U8 = 0x1401,                    //GL_UNSIGNED_BYTE
            TYPE_I16 = 0x1402,                    //GL_SHORT
            TYPE_U16 = 0x1403,                    //GL_UNSIGNED_SHORT
            TYPE_I32 = 0x1404,                    //GL_INT
            TYPE_U32 = 0x1405,                    //GL_UNSIGNED_INT
            TYPE_I64 = 0x140E,                    //GL_INT64_ARB
            TYPE_U64 = 0x140F,                    //GL_UNSIGNED_INT64_ARB
            TYPE_F16 = 0x140B,                    //GL_HALF_FLOAT
            TYPE_F16_OES = 0x8D61,                //GL_HALF_FLOAT_OES
            TYPE_F32 = 0x1406,                    //GL_FLOAT
            TYPE_F64 = 0x140A,                    //GL_DOUBLE
            TYPE_UINT32_RGB9_E5_REV = 0x8C3E,    //GL_UNSIGNED_INT_5_9_9_9_REV
            TYPE_UINT32_RG11B10F_REV = 0x8C3B,    //GL_UNSIGNED_INT_10F_11F_11F_REV
            TYPE_UINT8_RG3B2 = 0x8032,            //GL_UNSIGNED_BYTE_3_3_2
            TYPE_UINT8_RG3B2_REV = 0x8362,        //GL_UNSIGNED_BYTE_2_3_3_REV
            TYPE_UINT16_RGB5A1 = 0x8034,        //GL_UNSIGNED_SHORT_5_5_5_1
            TYPE_UINT16_RGB5A1_REV = 0x8366,    //GL_UNSIGNED_SHORT_1_5_5_5_REV
            TYPE_UINT16_R5G6B5 = 0x8363,        //GL_UNSIGNED_SHORT_5_6_5
            TYPE_UINT16_R5G6B5_REV = 0x8364,    //GL_UNSIGNED_SHORT_5_6_5_REV
            TYPE_UINT16_RGBA4 = 0x8033,            //GL_UNSIGNED_SHORT_4_4_4_4
            TYPE_UINT16_RGBA4_REV = 0x8365,        //GL_UNSIGNED_SHORT_4_4_4_4_REV
            TYPE_UINT32_RGBA8 = 0x8035,            //GL_UNSIGNED_SHORT_8_8_8_8
            TYPE_UINT32_RGBA8_REV = 0x8367,        //GL_UNSIGNED_SHORT_8_8_8_8_REV
            TYPE_UINT32_RGB10A2 = 0x8036,        //GL_UNSIGNED_INT_10_10_10_2
            TYPE_UINT32_RGB10A2_REV = 0x8368,    //GL_UNSIGNED_INT_2_10_10_10_REV

            TYPE_UINT8_RG4_REV_GTC = 0xFFFD,
            TYPE_UINT16_A1RGB5_GTC = 0xFFFC
        };

        enum target
        {
            TARGET_1D = 0x0DE0,
            TARGET_1D_ARRAY = 0x8C18,
            TARGET_2D = 0x0DE1,
            TARGET_2D_ARRAY = 0x8C1A,
            TARGET_3D = 0x806F,
            TARGET_RECT = 0x84F5,
            TARGET_RECT_ARRAY = 0x84F5, // Not supported by OpenGL
            TARGET_CUBE = 0x8513,
            TARGET_CUBE_ARRAY = 0x9009
        };

        enum swizzle
        {
            SWIZZLE_RED = 0x1903,        //GL_RED
            SWIZZLE_GREEN = 0x1904,        //GL_GREEN
            SWIZZLE_BLUE = 0x1905,        //GL_BLUE
            SWIZZLE_ALPHA = 0x1906,        //GL_ALPHA
            SWIZZLE_ZERO = 0x0000,        //GL_ZERO
            SWIZZLE_ONE = 0x0001,        //GL_ONE
        };

        enum profile
        {
            PROFILE_ES20,
            PROFILE_ES30,
            PROFILE_GL32,
            PROFILE_GL33,
            PROFILE_KTX
        };

        typedef glm::vec<4, int> swizzles;

        struct format
        {
            internal_format Internal;
            external_format External;
            type_format Type;
            swizzles Swizzles;
        };

        gl(profile Profile);

        /// Convert GLI targets into OpenGL texture targets
        target const& translate(gli::target Target) const;

        /// Convert GLI formats into OpenGL texture formats
        format translate(gli::format Format, gli::swizzles const& Swizzle) const;

        /// Convert an OpenGL format into a GLI format
        gli::format find(internal_format InternalFormat, external_format ExternalFormat, type_format Type);

    private:
        struct format_desc
        {
            internal_format Internal;
            external_format External;
            type_format Type;
            unsigned int Properties;
        };

        bool has_swizzle(profile Profile) const
        {
            return Profile == PROFILE_ES30 || Profile == PROFILE_GL33;
        }

        gl::swizzles compute_swizzle(format_desc const& FormatDesc, gli::swizzles const& Swizzle) const;

        std::array<format_desc, FORMAT_COUNT> FormatDesc;
        profile Profile;
    };
}//namespace gli

#include "./core/gl.inl"
