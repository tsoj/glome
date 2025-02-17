#include <gli/sampler2d.hpp>
#include <gli/comparison.hpp>
#include <glm/ext/vector_relational.hpp>
#include <ctime>
#include <limits>
#include <array>

namespace load
{
    int test()
    {
        int Error(0);

        gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4, 2), 1);
        *(Texture.data<glm::u8vec4>() + 0) = glm::u8vec4(255, 0, 0, 255);
        *(Texture.data<glm::u8vec4>() + 1) = glm::u8vec4(255, 128, 0, 255);
        *(Texture.data<glm::u8vec4>() + 2) = glm::u8vec4(255, 255, 0, 255);
        *(Texture.data<glm::u8vec4>() + 3) = glm::u8vec4(128, 255, 0, 255);
        *(Texture.data<glm::u8vec4>() + 4) = glm::u8vec4(0, 255, 0, 255);
        *(Texture.data<glm::u8vec4>() + 5) = glm::u8vec4(0, 255, 255, 255);
        *(Texture.data<glm::u8vec4>() + 6) = glm::u8vec4(0, 0, 255, 255);
        *(Texture.data<glm::u8vec4>() + 7) = glm::u8vec4(255, 0, 255, 255);

        glm::u8vec4 Data0 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(0, 0), 0);
        glm::u8vec4 Data1 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(1, 0), 0);
        glm::u8vec4 Data2 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(2, 0), 0);
        glm::u8vec4 Data3 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(3, 0), 0);
        glm::u8vec4 Data4 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(0, 1), 0);
        glm::u8vec4 Data5 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(1, 1), 0);
        glm::u8vec4 Data6 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(2, 1), 0);
        glm::u8vec4 Data7 = Texture.load<glm::u8vec4>(gli::texture2d::extent_type(3, 1), 0);

        Error += Data0 == glm::u8vec4(255, 0, 0, 255) ? 0 : 1;
        Error += Data1 == glm::u8vec4(255, 128, 0, 255) ? 0 : 1;
        Error += Data2 == glm::u8vec4(255, 255, 0, 255) ? 0 : 1;
        Error += Data3 == glm::u8vec4(128, 255, 0, 255) ? 0 : 1;
        Error += Data4 == glm::u8vec4(0, 255, 0, 255) ? 0 : 1;
        Error += Data5 == glm::u8vec4(0, 255, 255, 255) ? 0 : 1;
        Error += Data6 == glm::u8vec4(0, 0, 255, 255) ? 0 : 1;
        Error += Data7 == glm::u8vec4(255, 0, 255, 255) ? 0 : 1;

        return Error;
    }
}//namespace load

namespace texture_lod
{
    int test()
    {
        int Error = 0;

        {
            gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4), 1);
            Texture.clear(gli::u8vec4(0, 0, 0, 255));
            Texture.store(gli::extent2d(0, 0), 0, gli::u8vec4(255, 127, 0, 255));
            Texture.store(gli::extent2d(1, 0), 0, gli::u8vec4(255, 127, 0, 255));
            Texture.store(gli::extent2d(0, 1), 0, gli::u8vec4(255, 127, 0, 255));
            Texture.store(gli::extent2d(1, 1), 0, gli::u8vec4(255, 127, 0, 255));
            Texture.store(gli::extent2d(2, 2), 0, gli::u8vec4(0, 127, 255, 255));
            Texture.store(gli::extent2d(3, 2), 0, gli::u8vec4(0, 127, 255, 255));
            Texture.store(gli::extent2d(2, 3), 0, gli::u8vec4(0, 127, 255, 255));
            Texture.store(gli::extent2d(3, 3), 0, gli::u8vec4(0, 127, 255, 255));

            gli::sampler2d<float> const Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR,
                                                gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));

            gli::vec4 const SampleA = Sampler.texture_lod(gli::fsampler2D::normalized_type(0.25f), 0.0f);
            Error += gli::all(glm::equal(SampleA, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;

            gli::vec4 const SampleB = Sampler.texture_lod(gli::fsampler2D::normalized_type(0.8f), 0.0f);
            Error += gli::all(glm::equal(SampleB, gli::vec4(0.0f, 0.5f, 1.0f, 1.0f), 0.01f)) ? 0 : 1;

            gli::vec4 const SampleC = Sampler.texture_lod(gli::fsampler2D::normalized_type(0.20f, 0.8f), 0.0f);
            Error += gli::all(glm::equal(SampleC, gli::vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;

            gli::vec4 const SampleD = Sampler.texture_grad(gli::fsampler2D::normalized_type(0.25f), gli::fsampler2D::normalized_type(0.0f),
                                                           gli::fsampler2D::normalized_type(0.0f));
            Error += gli::all(glm::equal(SampleA, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
        }

        return Error;
    }
}//namespace texture_lod

namespace sampler_type
{
    int test()
    {
        int Error = 0;

        {
            gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4), 1);
            gli::sampler2d<float> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
        }

        {
            gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4), 1);
            gli::sampler2d<double> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
        }

        {
            gli::texture2d Texture(gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture2d::extent_type(4), 1);
            gli::sampler2d<int> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_NEAREST, gli::FILTER_NEAREST);
        }

        return Error;
    }
}//namespace sampler_type

int main()
{
    int Error(0);

    Error += texture_lod::test();
    Error += load::test();
    Error += sampler_type::test();

    return Error;
}

