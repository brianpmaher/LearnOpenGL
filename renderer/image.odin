package renderer

import stbi "vendor:stb/image"

import "core:fmt"
import "core:os"
import "core:strings"

Image :: struct {
	width:        i32,
	height:       i32,
	num_channels: i32,
	data:         [^]u8,
}

load_image :: proc(path: string, flip_y := false, allocator := context.allocator) -> Image {
	image: Image

	path_cstr := strings.clone_to_cstring(path, context.temp_allocator)
	context.allocator = allocator
	image.data = stbi.load(path_cstr, &image.width, &image.height, &image.num_channels, 0)

	if image.data == nil {
		fmt.panicf("Failed to load image \"%s\"", path)
	}

	return image
}

unload_image :: proc(image: ^Image) {
	stbi.image_free(rawptr(image.data))
}
