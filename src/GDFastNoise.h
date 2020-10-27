#pragma once

#include <Godot.hpp>
#include <Resource.hpp>
#include <Image.hpp>
#include <Reference.hpp>
#include <Dictionary.hpp>

#include <FastNoiseLite.h>

typedef FastNoiseLite FN;
typedef FN::NoiseType NoiseType;
typedef FN::RotationType3D RotationType3D;
typedef FN::FractalType FractalType;
typedef FN::CellularDistanceFunction CellularDistanceFunction;
typedef FN::CellularReturnType CellularReturnType;
typedef FN::DomainWarpType DomainWarpType;

class GDFastNoise : public godot::Resource {
    GODOT_CLASS(GDFastNoise, godot::Resource);

    FastNoiseLite noise;

    // Variables for storing
    int _seed;
    float _frequency;
    int _noiseType;
    int _rotationType3D;
    int _fractalType;
    int _fractalOctaves;
    float _fractalLacunarity;
    float _fractalGain;
    float _fractalWeightedStrength;
    float _fractalPingPongStrength;
    int _cellularDistanceFunction;
    int _cellularReturnType;
    float _cellularJitter;
    int _domainWarpType;
    float _domainWarpAmp;

public:
    static void _register_methods();

    void _init();

    void set_seed(int seed);
    int get_seed() const { return _seed; }

    void set_frequency(float frequency);
    float get_frequency() const { return _frequency; }

    void set_noise_type(int noiseType);
    int get_noise_type() const { return _noiseType; }

    void set_rotation_type_3d(int rotationType3D);
    int get_rotation_type_3d() const { return _rotationType3D; }

    void set_fractal_type(int fractalType);
    int get_fractal_type() const { return _fractalType; }

    void set_fractal_octaves(int octaves);
    int get_fractal_octaves() const { return _fractalOctaves; }

    void set_fractal_lacunarity(float lacunarity);
    float get_fractal_lacunarity() const { return _fractalLacunarity; }

    void set_fractal_gain(float gain);
    float get_fractal_gain() const { return _fractalGain; }

    void set_fractal_weighted_strength(float weightedStrength);
    float get_fractal_weighted_strength() const { return _fractalWeightedStrength; }

    void set_fractal_ping_pong_strength(float pingPongStrength);
    float get_fractal_ping_pong_strength() const { return _fractalPingPongStrength; }

    void set_cellular_distance_function(int cellularDistanceFunction);
    int get_cellular_distance_function() const { return _cellularDistanceFunction; }

    void set_cellular_return_type(int cellularReturnType);
    int get_cellular_return_type() const { return _cellularReturnType; }

    void set_cellular_jitter(float cellularJitter);
    float get_cellular_jitter() const { return _cellularJitter; }

    void set_domain_warp_type(int domainWarpType);
    int get_domain_warp_type() const { return _domainWarpType; }

    void set_domain_warp_amp(float domainWarpAmp);
    float get_domain_warp_amp() const { return _domainWarpAmp; }

    godot::Ref<godot::Image> get_image(int p_width, int p_height);
    godot::Ref<godot::Image> get_seamless_image(int p_size);

    float get_noise_2d(float x, float y);
    float get_noise_3d(float x, float y, float z);

    void warp_domain_2d(float x, float y);
    void warp_domain_3d(float x, float y, float z);

    void _process(float delta);

    GDFastNoise();
    ~GDFastNoise();
};
