#include <algorithm>

namespace gli
{
    namespace detail
    {
        inline gl::swizzles translate(gli::swizzles const& Swizzles)
        {
            static gl::swizzle const Table[] =
                {
                    gl::SWIZZLE_RED,
                    gl::SWIZZLE_GREEN,
                    gl::SWIZZLE_BLUE,
                    gl::SWIZZLE_ALPHA,
                    gl::SWIZZLE_ZERO,
                    gl::SWIZZLE_ONE
                };
            static_assert(sizeof(Table) / sizeof(Table[0]) == SWIZZLE_COUNT,
                          "GLI error: swizzle descriptor list doesn't match number of supported swizzles");

            return gl::swizzles(Table[Swizzles.r], Table[Swizzles.g], Table[Swizzles.b], Table[Swizzles.a]);
        }

        enum format_property
        {
            FORMAT_PROPERTY_BGRA_FORMAT_BIT = (1 << 0),
            FORMAT_PROPERTY_BGRA_TYPE_BIT = (1 << 1)
        };
    }//namespace detail

    inline gl::gl(profile Profile)
        : Profile(Profile)
    {
        bool const HasSwizzle = has_swizzle(Profile);
        external_format const ExternalBGR = HasSwizzle ? EXTERNAL_RGB : EXTERNAL_BGR;
        external_format const ExternalBGRA = HasSwizzle ? EXTERNAL_RGBA : EXTERNAL_BGRA;
        external_format const ExternalBGRInt = HasSwizzle ? EXTERNAL_RGB_INTEGER : EXTERNAL_BGR_INTEGER;
        external_format const ExternalBGRAInt = HasSwizzle ? EXTERNAL_RGBA_INTEGER : EXTERNAL_BGRA_INTEGER;

        external_format const ExternalSRGB8 = Profile != PROFILE_ES20 ? EXTERNAL_RGB : EXTERNAL_SRGB_EXT;
        external_format const ExternalSRGB8_A8 = Profile != PROFILE_ES20 ? EXTERNAL_RGBA : EXTERNAL_SRGB_ALPHA_EXT;

        internal_format const InternalBGRA = Profile == PROFILE_ES20 ? INTERNAL_BGRA8_UNORM : INTERNAL_RGBA8_UNORM;
        internal_format const InternalRGBETC = Profile == PROFILE_ES20 ? INTERNAL_RGB_ETC : INTERNAL_RGB_ETC2;

        internal_format const InternalLuminance8 = HasSwizzle ? INTERNAL_R8_UNORM : INTERNAL_LUMINANCE8;
        internal_format const InternalAlpha8 = HasSwizzle ? INTERNAL_R8_UNORM : INTERNAL_ALPHA8;
        internal_format const InternalLuminanceAlpha8 = HasSwizzle ? INTERNAL_RG8_UNORM : INTERNAL_LUMINANCE8_ALPHA8;

        internal_format const InternalLuminance16 = HasSwizzle ? INTERNAL_R16_UNORM : INTERNAL_LUMINANCE16;
        internal_format const InternalAlpha16 = HasSwizzle ? INTERNAL_R16_UNORM : INTERNAL_ALPHA16;
        internal_format const InternalLuminanceAlpha16 = HasSwizzle ? INTERNAL_RG16_UNORM : INTERNAL_LUMINANCE16_ALPHA16;

        external_format const ExternalLuminance = HasSwizzle ? EXTERNAL_RED : EXTERNAL_LUMINANCE;
        external_format const ExternalAlpha = HasSwizzle ? EXTERNAL_RED : EXTERNAL_ALPHA;
        external_format const ExternalLuminanceAlpha = HasSwizzle ? EXTERNAL_RG : EXTERNAL_LUMINANCE_ALPHA;

        type_format const TypeF16 = Profile == PROFILE_ES20 ? TYPE_F16_OES : TYPE_F16;

        format_desc const Table[] =
            {
                {INTERNAL_RG4_EXT,                        EXTERNAL_RG,            TYPE_UINT8_RG4_REV_GTC,   0},                                        //FORMAT_R4G4_UNORM,
                {INTERNAL_RGBA4,                          EXTERNAL_RGBA,          TYPE_UINT16_RGBA4_REV,    0},                                        //FORMAT_RGBA4_UNORM,
                {INTERNAL_RGBA4,                          EXTERNAL_RGBA,          TYPE_UINT16_RGBA4,        detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_BGRA4_UNORM,
                {INTERNAL_R5G6B5,                         EXTERNAL_RGB,           TYPE_UINT16_R5G6B5_REV,   0},                                        //FORMAT_R5G6B5_UNORM,
                {INTERNAL_R5G6B5,                         EXTERNAL_RGB,           TYPE_UINT16_R5G6B5,       detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_B5G6R5_UNORM,
                {INTERNAL_RGB5A1,                         EXTERNAL_RGBA,          TYPE_UINT16_RGB5A1_REV,   0},                                    //FORMAT_RGB5A1_UNORM,
                {INTERNAL_RGB5A1,                         EXTERNAL_RGBA,          TYPE_UINT16_RGB5A1,       detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},    //FORMAT_BGR5A1_UNORM,
                {INTERNAL_RGB5A1,                         EXTERNAL_RGBA,          TYPE_UINT16_A1RGB5_GTC,   0},                                    //FORMAT_A1RGB5_UNORM,

                {INTERNAL_R8_UNORM,                       EXTERNAL_RED,           TYPE_U8,                  0},                    //FORMAT_R8_UNORM,
                {INTERNAL_R8_SNORM,                       EXTERNAL_RED,           TYPE_I8,                  0},                    //FORMAT_R8_SNORM,
                {INTERNAL_R8_USCALED_GTC,                 EXTERNAL_RED,           TYPE_U8,                  0},            //FORMAT_R8_USCALED,
                {INTERNAL_R8_SSCALED_GTC,                 EXTERNAL_RED,           TYPE_I8,                  0},            //FORMAT_R8_SSCALED,
                {INTERNAL_R8U,                            EXTERNAL_RED_INTEGER,   TYPE_U8,                  0},                //FORMAT_R8_UINT,
                {INTERNAL_R8I,                            EXTERNAL_RED_INTEGER,   TYPE_I8,                  0},                //FORMAT_R8_SINT,
                {INTERNAL_SR8,                            EXTERNAL_RED,           TYPE_U8,                  0},                        //FORMAT_R8_SRGB,

                {INTERNAL_RG8_UNORM,                      EXTERNAL_RG,            TYPE_U8,                  0},                    //FORMAT_RG8_UNORM,
                {INTERNAL_RG8_SNORM,                      EXTERNAL_RG,            TYPE_I8,                  0},                    //FORMAT_RG8_SNORM,
                {INTERNAL_RG8_USCALED_GTC,                EXTERNAL_RG,            TYPE_U8,                  0},            //FORMAT_RG8_USCALED,
                {INTERNAL_RG8_SSCALED_GTC,                EXTERNAL_RG,            TYPE_I8,                  0},            //FORMAT_RG8_SSCALED,
                {INTERNAL_RG8U,                           EXTERNAL_RG_INTEGER,    TYPE_U8,                  0},                //FORMAT_RG8_UINT,
                {INTERNAL_RG8I,                           EXTERNAL_RG_INTEGER,    TYPE_I8,                  0},                //FORMAT_RG8_SINT,
                {INTERNAL_SRG8,                           EXTERNAL_RG,            TYPE_U8,                  0},                        //FORMAT_RG8_SRGB,

                {INTERNAL_RGB8_UNORM,                     EXTERNAL_RGB,           TYPE_U8,                  0},                //FORMAT_RGB8_UNORM,
                {INTERNAL_RGB8_SNORM,                     EXTERNAL_RGB,           TYPE_I8,                  0},                //FORMAT_RGB8_SNORM,
                {INTERNAL_RGB8_USCALED_GTC,               EXTERNAL_RGB,           TYPE_U8,                  0},            //FORMAT_RGB8_USCALED,
                {INTERNAL_RGB8_SSCALED_GTC,               EXTERNAL_RGB,           TYPE_I8,                  0},            //FORMAT_RGB8_SSCALED,
                {INTERNAL_RGB8U,                          EXTERNAL_RGB_INTEGER,   TYPE_U8,                  0},                //FORMAT_RGB8_UINT,
                {INTERNAL_RGB8I,                          EXTERNAL_RGB_INTEGER,   TYPE_I8,                  0},                //FORMAT_RGB8_SINT,
                {INTERNAL_SRGB8,                          ExternalSRGB8,          TYPE_U8,                  0},                    //FORMAT_RGB8_SRGB,

                {INTERNAL_RGB8_UNORM,                     ExternalBGR,            TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},            //FORMAT_BGR8_UNORM_PACK8,
                {INTERNAL_RGB8_SNORM,                     ExternalBGR,            TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},            //FORMAT_BGR8_SNORM_PACK8,
                {INTERNAL_RGB8_USCALED_GTC,               ExternalBGR,            TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},        //FORMAT_BGR8_USCALED_PACK8,
                {INTERNAL_RGB8_SSCALED_GTC,               ExternalBGR,            TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},        //FORMAT_BGR8_SSCALED_PACK8,
                {INTERNAL_RGB8U,                          ExternalBGRInt,         TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGR8_UINT_PACK8,
                {INTERNAL_RGB8I,                          ExternalBGRInt,         TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGR8_SINT_PACK8,
                {INTERNAL_SRGB8,                          ExternalBGR,            TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGR8_SRGB_PACK8,

                {INTERNAL_RGBA8_UNORM,                    EXTERNAL_RGBA,          TYPE_U8,                  0},                //FORMAT_RGBA8_UNORM_PACK8,
                {INTERNAL_RGBA8_SNORM,                    EXTERNAL_RGBA,          TYPE_I8,                  0},                //FORMAT_RGBA8_SNORM_PACK8,
                {INTERNAL_RGBA8_USCALED_GTC,              EXTERNAL_RGBA,          TYPE_U8,                  0},        //FORMAT_RGBA8_USCALED_PACK8,
                {INTERNAL_RGBA8_SSCALED_GTC,              EXTERNAL_RGBA,          TYPE_I8,                  0},        //FORMAT_RGBA8_SSCALED_PACK8,
                {INTERNAL_RGBA8U,                         EXTERNAL_RGBA_INTEGER,  TYPE_U8,                  0},            //FORMAT_RGBA8_UINT_PACK8,
                {INTERNAL_RGBA8I,                         EXTERNAL_RGBA_INTEGER,  TYPE_I8,                  0},            //FORMAT_RGBA8_SINT_PACK8,
                {INTERNAL_SRGB8_ALPHA8,                   ExternalSRGB8_A8,       TYPE_U8,                  0},            //FORMAT_RGBA8_SRGB_PACK8,

                {InternalBGRA,                            ExternalBGRA,           TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                        //FORMAT_BGRA8_UNORM_PACK8,
                {INTERNAL_RGBA8_SNORM,                    ExternalBGRA,           TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGRA8_SNORM_PACK8,
                {INTERNAL_RGBA8_USCALED_GTC,              ExternalBGRA,           TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},        //FORMAT_BGRA8_USCALED_PACK8,
                {INTERNAL_RGBA8_SSCALED_GTC,              ExternalBGRA,           TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},        //FORMAT_BGRA8_SSCALED_PACK8,
                {INTERNAL_RGBA8U,                         ExternalBGRAInt,        TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGRA8_UINT_PACK8,
                {INTERNAL_RGBA8I,                         ExternalBGRAInt,        TYPE_I8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},                //FORMAT_BGRA8_SINT_PACK8,
                {INTERNAL_SRGB8_ALPHA8,                   ExternalBGRA,           TYPE_U8,                  detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT},            //FORMAT_BGRA8_SRGB_PACK8,

                {INTERNAL_RGBA8_UNORM,                    EXTERNAL_RGBA,          TYPE_UINT32_RGBA8_REV,    0},            //FORMAT_ABGR8_UNORM_PACK32,
                {INTERNAL_RGBA8_SNORM,                    EXTERNAL_RGBA,          TYPE_UINT32_RGBA8_REV,    0},            //FORMAT_ABGR8_SNORM_PACK32,
                {INTERNAL_RGBA8_USCALED_GTC,              EXTERNAL_RGBA,          TYPE_UINT32_RGBA8_REV,    0},        //FORMAT_ABGR8_USCALED_PACK32,
                {INTERNAL_RGBA8_SSCALED_GTC,              EXTERNAL_RGBA,          TYPE_UINT32_RGBA8_REV,    0},        //FORMAT_ABGR8_SSCALED_PACK32,
                {INTERNAL_RGBA8U,                         EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGBA8_REV,    0},            //FORMAT_ABGR8_UINT_PACK32,
                {INTERNAL_RGBA8I,                         EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGBA8_REV,    0},            //FORMAT_ABGR8_SINT_PACK32,
                {INTERNAL_SRGB8_ALPHA8,                   EXTERNAL_RGBA,          TYPE_UINT32_RGBA8_REV,    0},            //FORMAT_ABGR8_SRGB_PACK32,

                {INTERNAL_RGB10A2_UNORM,                  EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2_REV,  0},            //FORMAT_RGB10A2_UNORM_PACK32,
                {INTERNAL_RGB10A2_SNORM_EXT,              EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2_REV,  0},        //FORMAT_RGB10A2_SNORM_PACK32,
                {INTERNAL_RGB10A2_USCALED_GTC,            EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2_REV,  0},        //FORMAT_RGB10A2_USCALE_PACK32,
                {INTERNAL_RGB10A2_SSCALED_GTC,            EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2_REV,  0},        //FORMAT_RGB10A2_SSCALE_PACK32,
                {INTERNAL_RGB10A2U,                       EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGB10A2_REV,  0},            //FORMAT_RGB10A2_UINT_PACK32,
                {INTERNAL_RGB10A2I_EXT,                   EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGB10A2_REV,  0},        //FORMAT_RGB10A2_SINT_PACK32,

                {INTERNAL_RGB10A2_UNORM,                  EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},            //FORMAT_BGR10A2_UNORM_PACK32,
                {INTERNAL_RGB10A2_SNORM_EXT,              EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_BGR10A2_SNORM_PACK32,
                {INTERNAL_RGB10A2_USCALED_GTC,            EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_BGR10A2_USCALE_PACK32,
                {INTERNAL_RGB10A2_SSCALED_GTC,            EXTERNAL_RGBA,          TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_BGR10A2_SSCALE_PACK32,
                {INTERNAL_RGB10A2U,                       EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},            //FORMAT_BGR10A2_UINT_PACK32,
                {INTERNAL_RGB10A2I_EXT,                   EXTERNAL_RGBA_INTEGER,  TYPE_UINT32_RGB10A2,      detail::FORMAT_PROPERTY_BGRA_TYPE_BIT},        //FORMAT_BGR10A2_SINT_PACK32,

                {INTERNAL_R16_UNORM,                      EXTERNAL_RED,           TYPE_U16,                 0},                    //FORMAT_R16_UNORM_PACK16,
                {INTERNAL_R16_SNORM,                      EXTERNAL_RED,           TYPE_I16,                 0},                    //FORMAT_R16_SNORM_PACK16,
                {INTERNAL_R16_USCALED_GTC,                EXTERNAL_RED,           TYPE_U16,                 0},                //FORMAT_R16_USCALED_PACK16,
                {INTERNAL_R16_SSCALED_GTC,                EXTERNAL_RED,           TYPE_I16,                 0},                //FORMAT_R16_SSCALED_PACK16,
                {INTERNAL_R16U,                           EXTERNAL_RED_INTEGER,   TYPE_U16,                 0},                    //FORMAT_R16_UINT_PACK16,
                {INTERNAL_R16I,                           EXTERNAL_RED_INTEGER,   TYPE_I16,                 0},                    //FORMAT_R16_SINT_PACK16,
                {INTERNAL_R16F,                           EXTERNAL_RED,           TypeF16,                  0},                            //FORMAT_R16_SFLOAT_PACK16,

                {INTERNAL_RG16_UNORM,                     EXTERNAL_RG,            TYPE_U16,                 0},                    //FORMAT_RG16_UNORM_PACK16,
                {INTERNAL_RG16_SNORM,                     EXTERNAL_RG,            TYPE_I16,                 0},                    //FORMAT_RG16_SNORM_PACK16,
                {INTERNAL_RG16_USCALED_GTC,               EXTERNAL_RG,            TYPE_U16,                 0},                //FORMAT_RG16_USCALED_PACK16,
                {INTERNAL_RG16_SSCALED_GTC,               EXTERNAL_RG,            TYPE_I16,                 0},                //FORMAT_RG16_SSCALED_PACK16,
                {INTERNAL_RG16U,                          EXTERNAL_RG_INTEGER,    TYPE_U16,                 0},                    //FORMAT_RG16_UINT_PACK16,
                {INTERNAL_RG16I,                          EXTERNAL_RG_INTEGER,    TYPE_I16,                 0},                    //FORMAT_RG16_SINT_PACK16,
                {INTERNAL_RG16F,                          EXTERNAL_RG,            TypeF16,                  0},                            //FORMAT_RG16_SFLOAT_PACK16,

                {INTERNAL_RGB16_UNORM,                    EXTERNAL_RGB,           TYPE_U16,                 0},                    //FORMAT_RGB16_UNORM_PACK16,
                {INTERNAL_RGB16_SNORM,                    EXTERNAL_RGB,           TYPE_I16,                 0},                    //FORMAT_RGB16_SNORM_PACK16,
                {INTERNAL_RGB16_USCALED_GTC,              EXTERNAL_RGB,           TYPE_U16,                 0},            //FORMAT_RGB16_USCALED_PACK16,
                {INTERNAL_RGB16_SSCALED_GTC,              EXTERNAL_RGB,           TYPE_I16,                 0},            //FORMAT_RGB16_USCALED_PACK16,
                {INTERNAL_RGB16U,                         EXTERNAL_RGB_INTEGER,   TYPE_U16,                 0},                //FORMAT_RGB16_UINT_PACK16,
                {INTERNAL_RGB16I,                         EXTERNAL_RGB_INTEGER,   TYPE_I16,                 0},                //FORMAT_RGB16_SINT_PACK16,
                {INTERNAL_RGB16F,                         EXTERNAL_RGB,           TypeF16,                  0},                        //FORMAT_RGB16_SFLOAT_PACK16,

                {INTERNAL_RGBA16_UNORM,                   EXTERNAL_RGBA,          TYPE_U16,                 0},                //FORMAT_RGBA16_UNORM_PACK16,
                {INTERNAL_RGBA16_SNORM,                   EXTERNAL_RGBA,          TYPE_I16,                 0},                //FORMAT_RGBA16_SNORM_PACK16,
                {INTERNAL_RGBA16_USCALED_GTC,             EXTERNAL_RGBA,          TYPE_U16,                 0},            //FORMAT_RGBA16_USCALED_PACK16,
                {INTERNAL_RGBA16_SSCALED_GTC,             EXTERNAL_RGBA,          TYPE_I16,                 0},            //FORMAT_RGBA16_SSCALED_PACK16,
                {INTERNAL_RGBA16U,                        EXTERNAL_RGBA_INTEGER,  TYPE_U16,                 0},                //FORMAT_RGBA16_UINT_PACK16,
                {INTERNAL_RGBA16I,                        EXTERNAL_RGBA_INTEGER,  TYPE_I16,                 0},                //FORMAT_RGBA16_SINT_PACK16,
                {INTERNAL_RGBA16F,                        EXTERNAL_RGBA,          TypeF16,                  0},                        //FORMAT_RGBA16_SFLOAT_PACK16,

                {INTERNAL_R32U,                           EXTERNAL_RED_INTEGER,   TYPE_U32,                 0},                    //FORMAT_R32_UINT_PACK32,
                {INTERNAL_R32I,                           EXTERNAL_RED_INTEGER,   TYPE_I32,                 0},                    //FORMAT_R32_SINT_PACK32,
                {INTERNAL_R32F,                           EXTERNAL_RED,           TYPE_F32,                 0},                            //FORMAT_R32_SFLOAT_PACK32,

                {INTERNAL_RG32U,                          EXTERNAL_RG_INTEGER,    TYPE_U32,                 0},                    //FORMAT_RG32_UINT_PACK32,
                {INTERNAL_RG32I,                          EXTERNAL_RG_INTEGER,    TYPE_I32,                 0},                    //FORMAT_RG32_SINT_PACK32,
                {INTERNAL_RG32F,                          EXTERNAL_RG,            TYPE_F32,                 0},                            //FORMAT_RG32_SFLOAT_PACK32,

                {INTERNAL_RGB32U,                         EXTERNAL_RGB_INTEGER,   TYPE_U32,                 0},                //FORMAT_RGB32_UINT_PACK32,
                {INTERNAL_RGB32I,                         EXTERNAL_RGB_INTEGER,   TYPE_I32,                 0},                //FORMAT_RGB32_SINT_PACK32,
                {INTERNAL_RGB32F,                         EXTERNAL_RGB,           TYPE_F32,                 0},                        //FORMAT_RGB32_SFLOAT_PACK32,

                {INTERNAL_RGBA32U,                        EXTERNAL_RGBA_INTEGER,  TYPE_U32,                 0},                //FORMAT_RGBA32_UINT_PACK32,
                {INTERNAL_RGBA32I,                        EXTERNAL_RGBA_INTEGER,  TYPE_I32,                 0},                //FORMAT_RGBA32_SINT_PACK32,
                {INTERNAL_RGBA32F,                        EXTERNAL_RGBA,          TYPE_F32,                 0},                        //FORMAT_RGBA32_SFLOAT_PACK32,

                {INTERNAL_R64F_EXT,                       EXTERNAL_RED,           TYPE_U64,                 0},                        //FORMAT_R64_UINT_PACK64,
                {INTERNAL_R64F_EXT,                       EXTERNAL_RED,           TYPE_I64,                 0},                        //FORMAT_R64_SINT_PACK64,
                {INTERNAL_R64F_EXT,                       EXTERNAL_RED,           TYPE_F64,                 0},                        //FORMAT_R64_SFLOAT_PACK64,

                {INTERNAL_RG64F_EXT,                      EXTERNAL_RG,            TYPE_U64,                 0},                        //FORMAT_RG64_UINT_PACK64,
                {INTERNAL_RG64F_EXT,                      EXTERNAL_RG,            TYPE_I64,                 0},                        //FORMAT_RG64_SINT_PACK64,
                {INTERNAL_RG64F_EXT,                      EXTERNAL_RG,            TYPE_F64,                 0},                        //FORMAT_RG64_SFLOAT_PACK64,

                {INTERNAL_RGB64F_EXT,                     EXTERNAL_RGB,           TYPE_U64,                 0},                    //FORMAT_RGB64_UINT_PACK64,
                {INTERNAL_RGB64F_EXT,                     EXTERNAL_RGB,           TYPE_I64,                 0},                    //FORMAT_RGB64_SINT_PACK64,
                {INTERNAL_RGB64F_EXT,                     EXTERNAL_RGB,           TYPE_F64,                 0},                    //FORMAT_RGB64_SFLOAT_PACK64,

                {INTERNAL_RGBA64F_EXT,                    EXTERNAL_RGBA,          TYPE_U64,                 0},                    //FORMAT_RGBA64_UINT_PACK64,
                {INTERNAL_RGBA64F_EXT,                    EXTERNAL_RGBA,          TYPE_I64,                 0},                    //FORMAT_RGBA64_SINT_PACK64,
                {INTERNAL_RGBA64F_EXT,                    EXTERNAL_RGBA,          TYPE_F64,                 0},                    //FORMAT_RGBA64_SFLOAT_PACK64,

                {INTERNAL_RG11B10F,                       EXTERNAL_RGB,           TYPE_UINT32_RG11B10F_REV, 0},        //FORMAT_RG11B10_UFLOAT_PACK32,
                {INTERNAL_RGB9E5,                         EXTERNAL_RGB,           TYPE_UINT32_RGB9_E5_REV,  0},        //FORMAT_RGB9E5_UFLOAT_PACK32,

                {INTERNAL_D16,                            EXTERNAL_DEPTH,         TYPE_NONE,                0},                        //FORMAT_D16_UNORM_PACK16,
                {INTERNAL_D24,                            EXTERNAL_DEPTH,         TYPE_NONE,                0},                        //FORMAT_D24_UNORM,
                {INTERNAL_D32F,                           EXTERNAL_DEPTH,         TYPE_NONE,                0},                        //FORMAT_D32_UFLOAT,
                {INTERNAL_S8_EXT,                         EXTERNAL_STENCIL,       TYPE_NONE,                0},                    //FORMAT_S8_UNORM,
                {INTERNAL_D16S8_EXT,                      EXTERNAL_DEPTH,         TYPE_NONE,                0},                    //FORMAT_D16_UNORM_S8_UINT_PACK32,
                {INTERNAL_D24S8,                          EXTERNAL_DEPTH_STENCIL, TYPE_NONE,                0},                //FORMAT_D24_UNORM_S8_UINT_PACK32,
                {INTERNAL_D32FS8X24,                      EXTERNAL_DEPTH_STENCIL, TYPE_NONE,                0},            //FORMAT_D32_SFLOAT_S8_UINT_PACK64,

                {INTERNAL_RGB_DXT1,                       EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_DXT1_UNORM_BLOCK8,
                {INTERNAL_SRGB_DXT1,                      EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_DXT1_SRGB_BLOCK8,
                {INTERNAL_RGBA_DXT1,                      EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_DXT1_UNORM_BLOCK8,
                {INTERNAL_SRGB_ALPHA_DXT1,                EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_DXT1_SRGB_BLOCK8,
                {INTERNAL_RGBA_DXT3,                      EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_DXT3_UNORM_BLOCK16,
                {INTERNAL_SRGB_ALPHA_DXT3,                EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_DXT3_SRGB_BLOCK16,
                {INTERNAL_RGBA_DXT5,                      EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_DXT5_UNORM_BLOCK16,
                {INTERNAL_SRGB_ALPHA_DXT5,                EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_DXT5_SRGB_BLOCK16,
                {INTERNAL_R_ATI1N_UNORM,                  EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_R_ATI1N_UNORM_BLOCK8,
                {INTERNAL_R_ATI1N_SNORM,                  EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_R_ATI1N_SNORM_BLOCK8,
                {INTERNAL_RG_ATI2N_UNORM,                 EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RG_ATI2N_UNORM_BLOCK16,
                {INTERNAL_RG_ATI2N_SNORM,                 EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RG_ATI2N_SNORM_BLOCK16,
                {INTERNAL_RGB_BP_UNSIGNED_FLOAT,          EXTERNAL_NONE,          TYPE_NONE,                0},        //FORMAT_RGB_BP_UFLOAT_BLOCK16,
                {INTERNAL_RGB_BP_SIGNED_FLOAT,            EXTERNAL_NONE,          TYPE_NONE,                0},        //FORMAT_RGB_BP_SFLOAT_BLOCK16,
                {INTERNAL_RGB_BP_UNORM,                   EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGB_BP_UNORM,
                {INTERNAL_SRGB_BP_UNORM,                  EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGB_BP_SRGB,

                {InternalRGBETC,                          EXTERNAL_NONE,          TYPE_NONE,                0},                            //FORMAT_RGB_ETC2_UNORM_BLOCK8,
                {INTERNAL_SRGB8_ETC2,                     EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGB_ETC2_SRGB_BLOCK8,
                {INTERNAL_RGBA_PUNCHTHROUGH_ETC2,         EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ETC2_PUNCHTHROUGH_UNORM,
                {INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2, EXTERNAL_NONE,          TYPE_NONE,                0},    //FORMAT_RGBA_ETC2_PUNCHTHROUGH_SRGB,
                {INTERNAL_RGBA_ETC2,                      EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ETC2_UNORM_BLOCK16,
                {INTERNAL_SRGB8_ALPHA8_ETC2_EAC,          EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ETC2_SRGB_BLOCK16,
                {INTERNAL_R11_EAC,                        EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_R11_EAC_UNORM,
                {INTERNAL_SIGNED_R11_EAC,                 EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_R11_EAC_SNORM,
                {INTERNAL_RG11_EAC,                       EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RG11_EAC_UNORM,
                {INTERNAL_SIGNED_RG11_EAC,                EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RG11_EAC_SNORM,

                {INTERNAL_RGBA_ASTC_4x4,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC4X4_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_4x4,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC4X4_SRGB,
                {INTERNAL_RGBA_ASTC_5x4,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC5X4_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_5x4,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC5X4_SRGB,
                {INTERNAL_RGBA_ASTC_5x5,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC5X5_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_5x5,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC5X5_SRGB,
                {INTERNAL_RGBA_ASTC_6x5,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC6X5_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_6x5,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC6X5_SRGB,
                {INTERNAL_RGBA_ASTC_6x6,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC6X6_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_6x6,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC6X6_SRGB,
                {INTERNAL_RGBA_ASTC_8x5,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC8X5_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_8x5,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC8X5_SRGB,
                {INTERNAL_RGBA_ASTC_8x6,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC8X6_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_8x6,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC8X6_SRGB,
                {INTERNAL_RGBA_ASTC_8x8,                  EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC8X8_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_8x8,          EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC8X8_SRGB,
                {INTERNAL_RGBA_ASTC_10x5,                 EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC10X5_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_10x5,         EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC10X5_SRGB,
                {INTERNAL_RGBA_ASTC_10x6,                 EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC10X6_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_10x6,         EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC10X6_SRGB,
                {INTERNAL_RGBA_ASTC_10x8,                 EXTERNAL_NONE,          TYPE_NONE,                0},                        //FORMAT_RGBA_ASTC10X8_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_10x8,         EXTERNAL_NONE,          TYPE_NONE,                0},                //FORMAT_RGBA_ASTC10X8_SRGB,
                {INTERNAL_RGBA_ASTC_10x10,                EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_ASTC10X10_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_10x10,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ASTC10X10_SRGB,
                {INTERNAL_RGBA_ASTC_12x10,                EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_ASTC12X10_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_12x10,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ASTC12X10_SRGB,
                {INTERNAL_RGBA_ASTC_12x12,                EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_ASTC12X12_UNORM,
                {INTERNAL_SRGB8_ALPHA8_ASTC_12x12,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ASTC12X12_SRGB,

                {INTERNAL_RGB_PVRTC_4BPPV1,               EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32,
                {INTERNAL_SRGB_PVRTC_2BPPV1,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32,
                {INTERNAL_RGB_PVRTC_2BPPV1,               EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32,
                {INTERNAL_SRGB_PVRTC_4BPPV1,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32,
                {INTERNAL_RGBA_PVRTC_4BPPV1,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32,
                {INTERNAL_SRGB_ALPHA_PVRTC_2BPPV1,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32,
                {INTERNAL_RGBA_PVRTC_2BPPV1,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32,
                {INTERNAL_SRGB_ALPHA_PVRTC_4BPPV1,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32,
                {INTERNAL_RGBA_PVRTC_4BPPV2,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8,
                {INTERNAL_SRGB_ALPHA_PVRTC_4BPPV2,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8,
                {INTERNAL_RGBA_PVRTC_2BPPV2,              EXTERNAL_NONE,          TYPE_NONE,                0},                    //FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8,
                {INTERNAL_SRGB_ALPHA_PVRTC_2BPPV2,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8,

                {INTERNAL_RGB_ETC,                        EXTERNAL_NONE,          TYPE_NONE,                0},                            //FORMAT_RGB_ETC_UNORM_BLOCK8,
                {INTERNAL_ATC_RGB,                        EXTERNAL_NONE,          TYPE_NONE,                0},                            //FORMAT_RGB_ATC_UNORM_BLOCK8,
                {INTERNAL_ATC_RGBA_EXPLICIT_ALPHA,        EXTERNAL_NONE,          TYPE_NONE,                0},            //FORMAT_RGBA_ATCA_UNORM_BLOCK16,
                {INTERNAL_ATC_RGBA_INTERPOLATED_ALPHA,    EXTERNAL_NONE,          TYPE_NONE,                0},        //FORMAT_RGBA_ATCI_UNORM_BLOCK16,

                {InternalLuminance8,                      ExternalLuminance,      TYPE_U8,                  0},                        //FORMAT_L8_UNORM_PACK8,
                {InternalAlpha8,                          ExternalAlpha,          TYPE_U8,                  0},                                //FORMAT_A8_UNORM_PACK8,
                {InternalLuminanceAlpha8,                 ExternalLuminanceAlpha, TYPE_U8,                  0},                //FORMAT_LA8_UNORM_PACK8,
                {InternalLuminance16,                     ExternalLuminance,      TYPE_U16,                 0},                        //FORMAT_L16_UNORM_PACK16,
                {InternalAlpha16,                         ExternalAlpha,          TYPE_U16,                 0},                                //FORMAT_A16_UNORM_PACK16,
                {InternalLuminanceAlpha16,                ExternalLuminanceAlpha, TYPE_U16,                 0},            //FORMAT_LA16_UNORM_PACK16,

                {INTERNAL_RGB8_UNORM,                     ExternalBGRA,           TYPE_U8,                  0},                            //FORMAT_BGRX8_UNORM,
                {INTERNAL_SRGB8,                          ExternalBGRA,           TYPE_U8,                  0},                                    //FORMAT_BGRX8_SRGB,

                {INTERNAL_RG3B2,                          EXTERNAL_RGB,           TYPE_UINT8_RG3B2_REV,     0},                    //FORMAT_RG3B2_UNORM,
            };

        static_assert(sizeof(Table) / sizeof(Table[0]) == FORMAT_COUNT,
                      "GLI error: format descriptor list doesn't match number of supported formats");

        std::copy(&Table[0], &Table[0] + FORMAT_COUNT, this->FormatDesc.begin());
    }

    inline gl::target const& gl::translate(gli::target Target) const
    {
        static gl::target const Table[] =
            {
                gl::TARGET_1D,
                gl::TARGET_1D_ARRAY,
                gl::TARGET_2D,
                gl::TARGET_2D_ARRAY,
                gl::TARGET_3D,
                gl::TARGET_RECT,
                gl::TARGET_RECT_ARRAY,
                gl::TARGET_CUBE,
                gl::TARGET_CUBE_ARRAY
            };
        static_assert(sizeof(Table) / sizeof(Table[0]) == TARGET_COUNT,
                      "GLI error: target descriptor list doesn't match number of supported targets");

        return Table[Target];
    }

    inline gl::format gl::translate(gli::format Format, gli::swizzles const& Swizzles) const
    {
        GLI_ASSERT(Format >= FORMAT_FIRST && Format <= FORMAT_LAST);

        gl::format_desc const& FormatDesc = this->FormatDesc[Format - FORMAT_FIRST];

        gl::format FormatGL;
        FormatGL.Internal = FormatDesc.Internal;
        FormatGL.External = FormatDesc.External;
        FormatGL.Type = FormatDesc.Type;
        FormatGL.Swizzles = this->compute_swizzle(FormatDesc, Swizzles);
        return FormatGL;
    }

    inline gli::format gl::find(gl::internal_format InternalFormat, gl::external_format ExternalFormat, gl::type_format Type)
    {
        for (int FormatIndex = FORMAT_FIRST; FormatIndex <= FORMAT_LAST; ++FormatIndex)
        {
            std::size_t const Index = FormatIndex - FORMAT_FIRST;
            if (this->FormatDesc[Index].Internal != InternalFormat)
                continue;
            if (this->FormatDesc[Index].External != ExternalFormat)
                continue;
            if (this->FormatDesc[Index].Type != Type)
                continue;

            return static_cast<gli::format>(FormatIndex);
        }
        return gli::FORMAT_UNDEFINED;
    }

    inline gl::swizzles gl::compute_swizzle(format_desc const& FormatDesc, gli::swizzles const& Swizzles) const
    {
        if (!this->has_swizzle(this->Profile))
            return swizzles(gl::SWIZZLE_RED, gl::SWIZZLE_GREEN, gl::SWIZZLE_BLUE, gl::SWIZZLE_ALPHA);

        bool const IsExternalBGRA = ((FormatDesc.Properties & detail::FORMAT_PROPERTY_BGRA_FORMAT_BIT) && !has_swizzle(this->Profile)) ||
                                    (FormatDesc.Properties & detail::FORMAT_PROPERTY_BGRA_TYPE_BIT);

        return detail::translate(IsExternalBGRA ? gli::swizzles(Swizzles.b, Swizzles.g, Swizzles.r, Swizzles.a) : Swizzles);
    }
}//namespace gli
