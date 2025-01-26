package main

import "../../renderer"

import gl "vendor:OpenGL"
import "vendor:glfw"

import "core:c"
import "core:fmt"
import "core:math"
import la "core:math/linalg"

TITLE :: "Textures"

Vertex :: struct {
	position:      la.Vector3f32,
	color:         la.Vector3f32,
	texture_coord: la.Vector2f32,
}

Triangle_Indices :: [3]u32

main :: proc() {
	window := renderer.create_window(800, 600, TITLE)
	defer renderer.destroy_window(window)

	shader := renderer.create_program(#directory + "/vert.glsl", #directory + "/frag.glsl")
	defer gl.DeleteProgram(shader)

	container_texture: u32
	{
		gl.GenTextures(1, &container_texture)
		gl.BindTexture(gl.TEXTURE_2D, container_texture)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)

		image := renderer.load_image("res/container.jpg", false, context.temp_allocator)
		gl.TexImage2D(
			gl.TEXTURE_2D,
			0,
			gl.RGB,
			image.width,
			image.height,
			0,
			gl.RGB,
			gl.UNSIGNED_BYTE,
			image.data,
		)
		gl.GenerateMipmap(gl.TEXTURE_2D)
	}

	awesome_face_texture: u32
	{
		gl.GenTextures(1, &awesome_face_texture)
		gl.BindTexture(gl.TEXTURE_2D, awesome_face_texture)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR)
		gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)

		image := renderer.load_image("res/awesome_face.png", true, context.temp_allocator)
		gl.TexImage2D(
			gl.TEXTURE_2D,
			0,
			gl.RGB,
			image.width,
			image.height,
			0,
			gl.RGBA,
			gl.UNSIGNED_BYTE,
			image.data,
		)
		gl.GenerateMipmap(gl.TEXTURE_2D)
	}


	vertices := [?]Vertex {
		{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0}},
		{{0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0}},
		{{-0.5, -0.5, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}},
		{{-0.5, 0.5, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0}},
	}
	indices := [?]u32{0, 1, 3, 1, 2, 3}

	vao: u32
	gl.GenVertexArrays(1, &vao)
	defer gl.DeleteVertexArrays(1, &vao)
	gl.BindVertexArray(vao)

	vbo: u32
	gl.GenBuffers(1, &vbo)
	defer gl.DeleteBuffers(1, &vbo)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.BufferData(gl.ARRAY_BUFFER, size_of(vertices), &vertices, gl.STATIC_DRAW)

	location: u32 = 0
	stride := i32(size_of(Vertex))

	// Position attribute
	{
		gl.VertexAttribPointer(
			location,
			len(la.Vector3f32),
			gl.FLOAT,
			gl.FALSE,
			stride,
			offset_of(Vertex, position),
		)
		gl.EnableVertexAttribArray(location)
		location += 1
	}

	// Color attribute
	{
		gl.VertexAttribPointer(
			location,
			len(la.Vector3f32),
			gl.FLOAT,
			gl.FALSE,
			stride,
			offset_of(Vertex, color),
		)
		gl.EnableVertexAttribArray(location)
		location += 1
	}

	// Texture coord attribute
	{
		gl.VertexAttribPointer(
			location,
			len(la.Vector2f32),
			gl.FLOAT,
			gl.FALSE,
			stride,
			offset_of(Vertex, texture_coord),
		)
		gl.EnableVertexAttribArray(location)
		location += 1
	}

	gl.UseProgram(shader)
	gl.Uniform1i(gl.GetUniformLocation(shader, "texture0"), 0)
	gl.Uniform1i(gl.GetUniformLocation(shader, "texture1"), 1)

	gl.ClearColor(0.2, 0.3, 0.3, 1.0)

	for !renderer.window_should_close(window) {
		free_all(context.temp_allocator)

		renderer.poll_events(window)

		if window.input.key_escape == .Press {
			renderer.close_window(window)
		}

		gl.Clear(gl.COLOR_BUFFER_BIT)

		gl.UseProgram(shader)

		gl.ActiveTexture(gl.TEXTURE0)
		gl.BindTexture(gl.TEXTURE_2D, container_texture)

		gl.ActiveTexture(gl.TEXTURE1)
		gl.BindTexture(gl.TEXTURE_2D, awesome_face_texture)

		gl.BindVertexArray(vao)

		gl.BindVertexArray(vao)
		gl.DrawElements(gl.TRIANGLES, len(indices), gl.UNSIGNED_INT, rawptr(uintptr(0)))

		gl.BindVertexArray(gl.NONE)

		renderer.swap_buffers(window)
	}
}
