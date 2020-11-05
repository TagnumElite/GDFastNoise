tool
extends EditorPlugin

const FN := preload("res://addons/gdfastnoise/bin/fastnoise.gdns")
const FNIcon := preload("res://addons/gdfastnoise/icons/FastNoise.svg")
const FNHandler := "res://addons/gdfastnoise/scripts/FNHandler.gd"

func _enter_tree() -> void:
	add_autoload_singleton("FNHandler", FNHandler)
	add_custom_type("FastNoise", "Resource", FN, FNIcon)
	

func _exit_tree() -> void:
	remove_autoload_singleton("FNHandler")
	remove_custom_type("FastNoise")
	
