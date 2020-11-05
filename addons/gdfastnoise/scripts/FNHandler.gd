extends Node

const FN := preload("res://addons/gdfastnoise/bin/fastnoise.gdns")

# All this stupidity because godot can't do things properly
# Really need to make a pull request to sort this out
static func convert_to_fastnoise(obj) -> FN:
	var noise = FN.new()
	
	var _seed : int = obj.get("seed")
	if _seed: noise.seed = _seed
	var frequency : float = obj.get("frequency")
	if frequency: noise.frequency = frequency
	var noise_type : int = obj.get("noise_type")
	if noise_type: noise.noise_type = noise_type
	var rotation_type_3d : int = obj.get("rotation_type_3d")
	if rotation_type_3d: noise.rotation_type_3d = rotation_type_3d
	var fractal_type : int = obj.get("fractal_type")
	if fractal_type: noise.fractal_type = fractal_type
	var fractal_octaves : int = obj.get("fractal_octaves")
	if fractal_octaves: noise.fractal_octaves = fractal_octaves
	var fractal_lacunarity : float = obj.get("fractal_lacunarity")
	if fractal_lacunarity: noise.fractal_lacunarity = fractal_lacunarity
	var fractal_gain : float = obj.get("fractal_gain")
	if fractal_gain: noise.fractal_gain = fractal_gain
	var fractal_weighted_strength : float = obj.get("fractal_weighted_strength")
	if fractal_weighted_strength: noise.fractal_weighted_strength = fractal_weighted_strength
	var fractal_ping_pong_strength : float = obj.get("fractal_ping_pong_strength")
	if fractal_ping_pong_strength: noise.fractal_ping_pong_strength = fractal_ping_pong_strength
	var cellular_distance_function : int = obj.get("cellular_distance_function")
	if cellular_distance_function: noise.cellular_distance_function = cellular_distance_function
	var cellular_return_type : int = obj.get("cellular_return_type")
	if cellular_return_type: noise.cellular_return_type = cellular_return_type
	var cellular_jitter : float = obj.get("cellular_jitter")
	if cellular_jitter: noise.cellular_jitter = cellular_jitter
	var domain_warp_type : int = obj.get("domain_warp_type")
	if domain_warp_type: noise.domain_warp_type = domain_warp_type
	var domain_warp_amp : float = obj.get("domain_warp_amp")
	if domain_warp_amp: noise.domain_warp_amp = domain_warp_amp
	
	return noise
