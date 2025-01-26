package main

import "../../renderer"

import gl "vendor:OpenGL"
import "vendor:glfw"

import "core:c"
import "core:fmt"
import "core:math"
import la "core:math/linalg"

TITLE :: "Shaders"

GL_VERISON_MAJOR :: 3
GL_VERISON_MINOR :: 3

Vertex :: struct {
	position: la.Vector3f32,
	color:    la.Vector3f32,
}

Triangle_Indices :: [3]u32

main :: proc() {
	window := renderer.create_window(800, 600, TITLE)
	defer renderer.destroy_window(window)

	shader := renderer.create_program(#directory + "/vert.glsl", #directory + "/frag.glsl")
	defer gl.DeleteProgram(shader)

	vertices := [?]Vertex {
		{{0.5, -0.5, 0.0}, {1.0, 0.0, 0.0}},
		{{-0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}},
		{{0.0, 0.5, 0.0}, {0.0, 0.0, 1.0}},
	}

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

	gl.ClearColor(0.2, 0.3, 0.3, 1.0)

	for !renderer.window_should_close(window) {
		free_all(context.temp_allocator)

		renderer.poll_events(window)

		if window.input.key_escape == .Press {
			renderer.close_window(window)
		}

		gl.Clear(gl.COLOR_BUFFER_BIT)

		gl.UseProgram(shader)
		gl.BindVertexArray(vao)
		gl.DrawArrays(gl.TRIANGLES, 0, len(vertices))

		renderer.swap_buffers(window)
	}
}
