tool
class_name FastNoiseTexture, "res://addons/gdfastnoise/icons/FastNoiseTexture.svg"
extends ProxyTexture

const FN := preload("res://addons/gdfastnoise/bin/fastnoise.gdns")
const FNHandler := preload("res://addons/gdfastnoise/FNHandler.gd")

export(int, 1,2048,1) var width := 256 setget _set_width
export(int, 1,2048,1) var height := 256 setget _set_height
export var seamless := false
export var as_normalmap := false setget _set_as_normalmap
var bump_strength := 8.0
var noise setget _set_noise

var data : Image
var thread := Thread.new()
var update_queued := false
var regen_queued := false
var first_time := true
var texture : RID

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	_queue_update()


func _queue_update():
	if update_queued: return
	update_queued = true
	call_deferred("_update_texture")


func _update_texture():
	var use_thread := true
	if first_time:
		use_thread = false
		first_time = false
	if use_thread:
		if !thread:
			thread = Thread.new()
		if thread.is_active():
			regen_queued = true
		else:
			thread.start(self, '_thread_function')
			regen_queued = false
	else:
		var image := _generate_texture()
		_set_texture_data(image)
	update_queued = false


func _set_texture_data(image : Image):
	if is_instance_valid(image):
		var image_tex = ImageTexture.new()
		image_tex.create_from_image(image)
		set_base(image_tex)
	emit_signal("changed")


func _thread_function(_v):
	call_deferred("_thread_done", _generate_texture())


func _thread_done(image : Image):
	_set_texture_data(image)
	thread.wait_to_finish()
	if (regen_queued):
		thread.start(self, "_thread_function")
		regen_queued = false


func _generate_texture() -> Image:
	if !noise: return Image.new()
	
	var image : Image
	
	if seamless:
		image = noise.get_seamless_image(width)
	else:
		image = noise.get_image(width, height)
	
	if as_normalmap:
		image.bumpmap_to_normalmap(bump_strength)
	
	return image


func _set_noise(noise_settings):
	if noise == noise_settings: return
	if noise:
		noise.disconnect("changed", self, "_queue_update")
	if noise_settings:
		noise = FNHandler.convert_to_fastnoise(noise_settings)
		noise.connect("changed", self, "_queue_update")
	else:
		noise = null
	_queue_update()


func has_alpha() -> bool: return false
func get_data() -> Image: return data
func get_width() -> int: return width
func get_height() -> int: return height

func _set_as_normalmap(p_as_normalmap : bool):
	as_normalmap = p_as_normalmap
	property_list_changed_notify()


func _set_width(val):
	width = val
	_queue_update()


func _set_height(val):
	height = val
	_queue_update()


func _get_property_list() -> Array:
	var props = []
	
	if as_normalmap:
		props.append({
			'name': "bump_strength",
			'type': TYPE_REAL,
			'hint': PROPERTY_HINT_RANGE,
			'hint_string': "0,32,0.1,or_greater"
		})
	
	props.append({
		'name': 'noise',
		'type': TYPE_OBJECT,
		'hint': PROPERTY_HINT_RESOURCE_TYPE,
		'hint_string': 'FastNoise'
	})
	
	return props


func _exit_tree():
	if base: base.free()
	if data: data.free()
	if thread: thread.wait_to_finish()
