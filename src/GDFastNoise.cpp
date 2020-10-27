#include "GDFastNoise.h"

using namespace godot;

void GDFastNoise::_register_methods() {
	register_method("_process", &GDFastNoise::_process);
	register_method("get_image", &GDFastNoise::get_image);
	register_method("get_seamless_image", &GDFastNoise::get_seamless_image);
	register_method("get_noise_2d", &GDFastNoise::get_noise_2d);
	register_method("get_noise_3d", &GDFastNoise::get_noise_3d);
	register_method("warp_domain_2d", &GDFastNoise::warp_domain_2d);
	register_method("warp_domain_3d", &GDFastNoise::warp_domain_3d);

	// godot::Dictionary::make() is required for some stupidity with compiler
	register_signal<GDFastNoise>("changed", godot::Dictionary::make());

	register_property<GDFastNoise, int>("seed", &GDFastNoise::set_seed, &GDFastNoise::get_seed, 1337);
	register_property<GDFastNoise, float>("frequency", &GDFastNoise::set_frequency, &GDFastNoise::get_frequency, 0.01f);

	register_property<GDFastNoise, int>("noise_type", &GDFastNoise::set_noise_type, &GDFastNoise::get_noise_type, (int)NoiseType::NoiseType_OpenSimplex2, \
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "OpenSimplex2,OpenSimplex2S,Cellular,Perlin,Value Cubic,Value");
	register_property<GDFastNoise, int>("rotation_type_3d", &GDFastNoise::set_rotation_type_3d, &GDFastNoise::get_rotation_type_3d, (int)RotationType3D::RotationType3D_None,\
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "None,Improve XY Planes,Improve XZ Planes");
	register_property<GDFastNoise, int>("fractal_type", &GDFastNoise::set_fractal_type, &GDFastNoise::get_fractal_type, (int)FractalType::FractalType_None, \
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "None,FBm,Ridged,Ping Pong,Domain Warp Progressive,Domain Warp Independent");

	register_property<GDFastNoise, int>("fractal_octaves", &GDFastNoise::set_fractal_octaves, &GDFastNoise::get_fractal_octaves, 3, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "1,9,1,or_greater");
	register_property<GDFastNoise, float>("fractal_lacunarity", &GDFastNoise::set_fractal_lacunarity, &GDFastNoise::get_fractal_lacunarity, 2.0f);
	register_property<GDFastNoise, float>("fractal_gain", &GDFastNoise::set_fractal_gain, &GDFastNoise::get_fractal_gain, 0.5f);
	register_property<GDFastNoise, float>("fractal_weighted_strength", &GDFastNoise::set_fractal_weighted_strength, &GDFastNoise::get_fractal_weighted_strength, 0.0f);
	register_property<GDFastNoise, float>("fractal_ping_pong_strength", &GDFastNoise::set_fractal_ping_pong_strength, &GDFastNoise::get_fractal_ping_pong_strength, 2.0f);

	register_property<GDFastNoise, int>("cellular_distance_function", &GDFastNoise::set_cellular_distance_function, &GDFastNoise::get_cellular_distance_function, (int)CellularDistanceFunction::CellularDistanceFunction_Euclidean, \
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "Euclidean,EuclideanSq,Manhattan,Hybrid");
	register_property<GDFastNoise, int>("cellular_return_type", &GDFastNoise::set_cellular_return_type, &GDFastNoise::get_cellular_return_type, (int)CellularReturnType::CellularReturnType_CellValue, \
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "Cell Value,Distance,Distance 2,Distance 2 Add,Distance 2 Sub,Distance 2 Mul,Distance 2 Div");
	
	register_property<GDFastNoise, float>("cellular_jitter", &GDFastNoise::set_cellular_jitter, &GDFastNoise::get_cellular_jitter, 1.0f);
	
	register_property<GDFastNoise, int>("domain_warp_type", &GDFastNoise::set_domain_warp_type, &GDFastNoise::get_domain_warp_type, (int)DomainWarpType::DomainWarpType_OpenSimplex2, \
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "OpenSimplex2,OpenSimplex2 Reduced,Basic Grid");
	
	register_property<GDFastNoise, float>("domain_warp_amp", &GDFastNoise::set_domain_warp_amp, &GDFastNoise::get_domain_warp_amp, 1.0f);
}

void GDFastNoise::_init() {

}

GDFastNoise::GDFastNoise() {
	noise = FN();
	_seed = 1337;
	_frequency = 0.01f;
	_noiseType = NoiseType::NoiseType_OpenSimplex2;
	_rotationType3D = RotationType3D::RotationType3D_None;
	_fractalType = FractalType::FractalType_None;
	_fractalOctaves = 3;
	_fractalLacunarity = 2.0f;
	_fractalGain = 0.5f;
	_fractalWeightedStrength = 0.0f;
	_fractalPingPongStrength = 2.0f;
	_cellularDistanceFunction = CellularDistanceFunction::CellularDistanceFunction_Euclidean;
	_cellularReturnType = CellularReturnType::CellularReturnType_CellValue;
	_cellularJitter = 1.0f;
	_domainWarpType = DomainWarpType::DomainWarpType_OpenSimplex2;
	_domainWarpAmp = 1.0f;
}

GDFastNoise::~GDFastNoise() {}

void GDFastNoise::_process(float delta) {}

void GDFastNoise::set_seed(int seed)
{
	_seed = seed;
	noise.SetSeed(seed);
	emit_signal("changed");
}

void GDFastNoise::set_frequency(float frequency)
{
	_frequency = frequency;
	noise.SetFrequency(frequency);
	emit_signal("changed");
}

void GDFastNoise::set_noise_type(int noiseType)
{
	_noiseType = noiseType;
	noise.SetNoiseType((NoiseType)noiseType);
	emit_signal("changed");
}

void GDFastNoise::set_rotation_type_3d(int rotationType3D)
{
	_rotationType3D = rotationType3D;
	noise.SetRotationType3D((RotationType3D)rotationType3D);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_type(int fractalType)
{
	_fractalType = fractalType;
	noise.SetFractalType((FractalType)fractalType);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_octaves(int octaves)
{
	_fractalOctaves = octaves;
	noise.SetFractalOctaves(octaves);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_lacunarity(float lacunarity)
{
	_fractalLacunarity = lacunarity;
	noise.SetFractalLacunarity(lacunarity);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_gain(float gain)
{
	_fractalGain = gain;
	noise.SetFractalGain(gain);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_weighted_strength(float weightedStrength)
{
	_fractalWeightedStrength = weightedStrength;
	noise.SetFractalWeightedStrength(weightedStrength);
	emit_signal("changed");
}

void GDFastNoise::set_fractal_ping_pong_strength(float pingPongStrength)
{
	_fractalPingPongStrength = pingPongStrength;
	noise.SetFractalPingPongStrength(pingPongStrength);
	emit_signal("changed");
}

void GDFastNoise::set_cellular_distance_function(int cellularDistanceFunction)
{
	_cellularDistanceFunction = cellularDistanceFunction;
	noise.SetCellularDistanceFunction((CellularDistanceFunction)cellularDistanceFunction);
	emit_signal("changed");
}

void GDFastNoise::set_cellular_return_type(int cellularReturnType)
{
	_cellularReturnType = cellularReturnType;
	noise.SetCellularReturnType((CellularReturnType)cellularReturnType);
	emit_signal("changed");
}

void GDFastNoise::set_cellular_jitter(float cellularJitter)
{
	_cellularJitter = cellularJitter;
	noise.SetCellularJitter(cellularJitter);
	emit_signal("changed");
}

void GDFastNoise::set_domain_warp_type(int domainWarpType)
{
	_domainWarpType = domainWarpType;
	noise.SetDomainWarpType((DomainWarpType)domainWarpType);
	emit_signal("changed");
}

void GDFastNoise::set_domain_warp_amp(float domainWarpAmp)
{
	_domainWarpAmp = domainWarpAmp;
	noise.SetDomainWarpAmp(domainWarpAmp);
	emit_signal("changed");
}

Ref<Image> GDFastNoise::get_image(int width, int height) {
	PoolByteArray data;
	data.resize(width * height * 3);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float noise = get_noise_2d(x, y);
			noise = noise * 0.5f + 0.5f;
			uint8_t value = uint8_t(Math::clamp((int)(255 * noise), 0, 255));
			data.set((y * width + x) * 3 + 0, value);
			data.set((y * width + x) * 3 + 1, value);
			data.set((y * width + x) * 3 + 2, value);
		}
	}

	Ref<Image> image;
	image.instance();
	image->create_from_data(width, height, false, Image::FORMAT_RGB8, data);

	return image;
}

Ref<Image> GDFastNoise::get_seamless_image(int size) {
	PoolByteArray data;
	data.resize(size * size * 3);

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {/*
			float yy = (float)y / (float)size;
			float xx = (float)x / (float)size;

			yy *= 2.0 * Math_PI;
			xx *= 2.0 * Math_PI;

			float radius = size / (2.0 * Math_PI);

			float x = radius * Math::sin(xx);
			float y = radius * Math::cos(xx);
			float z = radius * Math::sin(yy);
			float w = radius * Math::cos(yy);
			float v = get_noise_4d(x, y, z, w);
			*/
			float noise = get_noise_2d(x, y);
			noise = noise * 0.5f + 0.5f; // Normalize [0..1]
			uint8_t value = uint8_t(Math::clamp((int)(noise * 255.0), 0, 255));
			data.set(((y * size + x) * 3 + 0), value);
			data.set(((y * size + x) * 3 + 1), value);
			data.set(((y * size + x) * 3 + 2), value);
		}
	}

	Ref<Image> image;
	image.instance();
	image->create_from_data(size, size, false, Image::FORMAT_RGB8, data);

	return image;
}

float GDFastNoise::get_noise_2d(float x, float y)
{
	return noise.GetNoise(x, y);
}

float GDFastNoise::get_noise_3d(float x, float y, float z)
{
	return noise.GetNoise(x, y, z);
}

void GDFastNoise::warp_domain_2d(float x, float y)
{
	noise.DomainWarp(x, y);
	emit_signal("changed");
}

void GDFastNoise::warp_domain_3d(float x, float y, float z)
{
	noise.DomainWarp(x, y, z);
	emit_signal("changed");
}
