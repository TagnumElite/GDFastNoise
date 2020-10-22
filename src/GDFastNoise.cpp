#include "GDFastNoise.h"

#define REG_METH(name) godot::register_method(#name, &GDFastNoise::##name)
#define REG_PROP(name, type, _default) godot::register_property<GDFastNoise, type>\
(#name, &GDFastNoise::set_##name, &GDFastNoise::get_##name, _default)
#define REG_PROP_INT(name, _default) REG_PROP(name, int, _default);
#define REG_PROP_FLOAT(name, _default) REG_PROP(name, float, _default);
#define REG_PROP_ENUM(name, _default, hint_string) godot::register_property<GDFastNoise, int>\
(#name, &GDFastNoise::set_##name, &GDFastNoise::get_##name, (int)_default, GODOT_METHOD_RPC_MODE_DISABLED,\
GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, hint_string)

#define emit_changed() emit_signal("changed")

using namespace godot;

void GDFastNoise::_register_methods() {
	REG_METH(_process);
	REG_METH(get_image);
	REG_METH(get_seamless_image);
	REG_METH(get_noise_2d);
	REG_METH(get_noise_3d);
	REG_METH(warp_domain_2d);
	REG_METH(warp_domain_3d);

	// godot::Dictionary::make() is required for some stupidity with compiler
	godot::register_signal<GDFastNoise>("changed", godot::Dictionary::make());

	REG_PROP_INT(seed, 1337);
	REG_PROP_FLOAT(frequency, 0.01f);
	REG_PROP_ENUM(noise_type, NoiseType::NoiseType_OpenSimplex2, "OpenSimplex2,OpenSimplex2S,Cellular,Perlin,Value Cubic,Value");
	REG_PROP_ENUM(rotation_type_3d, RotationType3D::RotationType3D_None, "None,Improve XY Planes,Improve XZ Planes");
	REG_PROP_ENUM(fractal_type, FractalType::FractalType_None, "None,FBm,Ridged,Ping Pong,Domain Warp Progressive,Domain Warp Independent");
	REG_PROP_INT(fractal_octaves, 3);
	REG_PROP_FLOAT(fractal_lacunarity, 2.0f);
	REG_PROP_FLOAT(fractal_gain, 0.5f);
	REG_PROP_FLOAT(fractal_weighted_strength, 0.0f);
	REG_PROP_FLOAT(fractal_ping_pong_strength, 2.0f);
	REG_PROP_ENUM(cellular_distance_function, CellularDistanceFunction::CellularDistanceFunction_Euclidean, "Euclidean,EuclideanSq,Manhattan,Hybrid");
	REG_PROP_ENUM(cellular_return_type, CellularReturnType::CellularReturnType_CellValue, "Cell Value,Distance,Distance 2,Distance 2 Add,Distance 2 Sub,Distance 2 Mul,Distance 2 Div");
	REG_PROP_FLOAT(cellular_jitter, 1.0f);
	REG_PROP_ENUM(domain_warp_type, DomainWarpType::DomainWarpType_OpenSimplex2, "OpenSimplex2,OpenSimplex2 Reduced,Basic Grid");
	REG_PROP_FLOAT(domain_warp_amp, 1.0f);
}

void GDFastNoise::_init() {
	noise = FN();
}

void GDFastNoise::_process(float delta) {}

void GDFastNoise::set_seed(int seed)
{
	noise.SetSeed(seed);
	emit_changed();
}

void GDFastNoise::set_frequency(float frequency)
{
	noise.SetFrequency(frequency);
	emit_changed();
}

void GDFastNoise::set_noise_type(int noiseType)
{
	noise.SetNoiseType((NoiseType)noiseType);
	emit_changed();
}

void GDFastNoise::set_rotation_type_3d(int rotationType3D)
{
	noise.SetRotationType3D((RotationType3D)rotationType3D);
	emit_changed();
}

void GDFastNoise::set_fractal_type(int fractalType)
{
	noise.SetFractalType((FractalType)fractalType);
	emit_changed();
}

void GDFastNoise::set_fractal_octaves(int octaves)
{
	noise.SetFractalOctaves(octaves);
	emit_changed();
}

void GDFastNoise::set_fractal_lacunarity(float lacunarity)
{
	noise.SetFractalLacunarity(lacunarity);
	emit_changed();
}

void GDFastNoise::set_fractal_gain(float gain)
{
	noise.SetFractalGain(gain);
	emit_changed();
}

void GDFastNoise::set_fractal_weighted_strength(float weightedStrength)
{
	noise.SetFractalWeightedStrength(weightedStrength);
	emit_changed();
}

void GDFastNoise::set_fractal_ping_pong_strength(float pingPongStrength)
{
	noise.SetFractalPingPongStrength(pingPongStrength);
	emit_changed();
}

void GDFastNoise::set_cellular_distance_function(int cellularDistanceFunction)
{
	noise.SetCellularDistanceFunction((CellularDistanceFunction)cellularDistanceFunction);
	emit_changed();
}

void GDFastNoise::set_cellular_return_type(int cellularReturnType)
{
	noise.SetCellularReturnType((CellularReturnType)cellularReturnType);
	emit_changed();
}

void GDFastNoise::set_cellular_jitter(float cellularJitter)
{
	noise.SetCellularJitter(cellularJitter);
	emit_changed();
}

void GDFastNoise::set_domain_warp_type(int domainWarpType)
{
	noise.SetDomainWarpType((DomainWarpType)domainWarpType);
	emit_changed();
}

void GDFastNoise::set_domain_warp_amp(float domainWarpAmp)
{
	noise.SetDomainWarpAmp(domainWarpAmp);
	emit_changed();
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
	emit_changed();
}

void GDFastNoise::warp_domain_3d(float x, float y, float z)
{
	noise.DomainWarp(x, y, z);
	emit_changed();
}
