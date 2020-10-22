tool
extends Control

const FN := preload("res://addons/gdfastnoise/bin/fastnoise.gdns")
const FNHandler := preload("res://addons/gdfastnoise/FNHandler.gd")

var noise setget set_noise
export(int,8,2048,2) var size = 256
export var add_color := false

onready var tex_rec = $TextureRect

func _ready() -> void:
	get_texture()


func _input(event: InputEvent) -> void:
	if event.is_action("ui_accept"):
		get_texture()


func get_texture() -> void:
	if noise:
		var image = noise.get_image(size, size)
		var image_tex = ImageTexture.new()
		image_tex.create_from_image(image)
		$TextureRect.texture = image_tex


func set_noise(val):
	if noise == val:
		return
	if noise:
		noise.disconnect("changed", self, "get_texture")
	if val:
		val = FNHandler.convert_to_fastnoise(val)
		val.connect("changed", self, "get_texture")
	noise = val


func _get_property_list() -> Array:
	return [
		{
			'name': "noise",
			'type': TYPE_OBJECT,
			'hint': PROPERTY_HINT_RESOURCE_TYPE,
			'hint_string': "FastNoise"
		}
	]
