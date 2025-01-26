package main

import gl "vendor:OpenGL"
import "vendor:glfw"

import "core:c"
import "core:fmt"
import "core:math"
import la "core:math/linalg"

TITLE :: "Hello Quad"

GL_VERISON_MAJOR :: 3
GL_VERISON_MINOR :: 3

Vertex :: struct {
	position: la.Vector3f32,
}

Triangle_Indices :: [3]u32

Window_User_State :: struct {
	wireframe_mode: bool,
}

main :: proc() {
	glfw.Init()
	defer glfw.Terminate()
	glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, GL_VERISON_MAJOR)
	glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, GL_VERISON_MINOR)
	glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

	window := glfw.CreateWindow(800, 600, TITLE, nil, nil)
	defer glfw.DestroyWindow(window)
	if window == nil {
		fmt.panicf("Failed to create GLFW window\n")
	}
	glfw.MakeContextCurrent(window)

	gl.load_up_to(GL_VERISON_MAJOR, GL_VERISON_MINOR, glfw.gl_set_proc_address)

	state := Window_User_State{}
	glfw.SetFramebufferSizeCallback(window, framebuffer_size_callback)
	glfw.SetKeyCallback(window, key_callback)
	glfw.SetWindowUserPointer(window, &state)

	shader_program := gl.CreateProgram()
	defer gl.DeleteProgram(shader_program)
	{
		vertex_shader := gl.CreateShader(gl.VERTEX_SHADER)
		defer gl.DeleteShader(vertex_shader)
		{
			vertex_source: cstring = #load("./vert.glsl")
			gl.ShaderSource(vertex_shader, 1, &vertex_source, nil)
			gl.CompileShader(vertex_shader)
			success: i32
			gl.GetShaderiv(vertex_shader, gl.COMPILE_STATUS, &success)
			if !bool(success) {
				LOG_SIZE :: 512
				info_log: [LOG_SIZE]u8
				gl.GetShaderInfoLog(vertex_shader, LOG_SIZE, nil, raw_data(&info_log))
				info_log_cstr := cstring(&info_log[0])
				info_log_str := string(info_log_cstr)
				fmt.panicf(info_log_str)
			}
		}

		fragment_shader := gl.CreateShader(gl.FRAGMENT_SHADER)
		defer gl.DeleteShader(fragment_shader)
		{
			fragment_source: cstring = #load("./frag.glsl")
			gl.ShaderSource(fragment_shader, 1, &fragment_source, nil)
			gl.CompileShader(fragment_shader)
			success: i32
			gl.GetShaderiv(fragment_shader, gl.COMPILE_STATUS, &success)
			if !bool(success) {
				LOG_SIZE :: 512
				info_log: [LOG_SIZE]c.char
				gl.GetShaderInfoLog(fragment_shader, LOG_SIZE, nil, raw_data(&info_log))
				info_log_str := string(cstring(raw_data(&info_log)))
				fmt.panicf(info_log_str)
			}
		}

		gl.AttachShader(shader_program, vertex_shader)
		gl.AttachShader(shader_program, fragment_shader)
		gl.LinkProgram(shader_program)
		success: i32
		gl.GetProgramiv(shader_program, gl.LINK_STATUS, &success)
		if !bool(success) {
			LOG_SIZE :: 512
			info_log: [LOG_SIZE]c.char
			gl.GetProgramInfoLog(shader_program, LOG_SIZE, nil, raw_data(&info_log))
			info_log_str := string(cstring(raw_data(&info_log)))
			fmt.panicf(info_log_str)
		}
	}

	vertices := [?]Vertex {
		{{0.5, 0.5, 0.0}},
		{{0.5, -0.5, 0.0}},
		{{-0.5, -0.5, 0.0}},
		{{-0.5, 0.5, 0.0}},
	}
	indices := [?]Triangle_Indices{{0, 1, 3}, {1, 2, 3}}

	vao: u32
	gl.GenVertexArrays(1, &vao)
	defer gl.DeleteVertexArrays(1, &vao)
	gl.BindVertexArray(vao)

	vbo: u32
	gl.GenBuffers(1, &vbo)
	defer gl.DeleteBuffers(1, &vbo)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.BufferData(gl.ARRAY_BUFFER, size_of(vertices), &vertices, gl.STATIC_DRAW)
	offset: u32 = 0
	gl.VertexAttribPointer(
		offset,
		size_of(la.Vector3f32) / size_of(f32),
		gl.FLOAT,
		gl.FALSE,
		size_of(la.Vector3f32),
		offset_of(Vertex, position),
	)
	gl.EnableVertexAttribArray(offset)
	offset += 1

	ebo: u32
	gl.GenBuffers(1, &ebo)
	defer gl.DeleteBuffers(1, &ebo)
	gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ebo)
	gl.BufferData(gl.ELEMENT_ARRAY_BUFFER, size_of(indices), &indices, gl.STATIC_DRAW)

	gl.ClearColor(0.2, 0.3, 0.3, 1.0)

	for !glfw.WindowShouldClose(window) {
		free_all(context.temp_allocator)

		glfw.PollEvents()

		if glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS {
			glfw.SetWindowShouldClose(window, true)
		}

		gl.Clear(gl.COLOR_BUFFER_BIT)

		gl.PolygonMode(gl.FRONT_AND_BACK, state.wireframe_mode ? gl.LINE : gl.FILL)

		gl.UseProgram(shader_program)
		gl.BindVertexArray(vao)
		gl.DrawElements(
			gl.TRIANGLES,
			len(indices) * len(indices[0]),
			gl.UNSIGNED_INT,
			rawptr(uintptr(0)),
		)

		glfw.SwapBuffers(window)
	}
}

framebuffer_size_callback :: proc "c" (window: glfw.WindowHandle, width, height: c.int) {
	gl.Viewport(0, 0, width, height)
}

key_callback :: proc "c" (window: glfw.WindowHandle, key, scancode, action, mods: c.int) {
	state := cast(^Window_User_State)glfw.GetWindowUserPointer(window)

	if key == glfw.KEY_ESCAPE && action == glfw.PRESS {
		glfw.SetWindowShouldClose(window, true)
	} else if key == glfw.KEY_SPACE && action == glfw.PRESS {
		state.wireframe_mode = !state.wireframe_mode
	}
}
