package main

import gl "vendor:OpenGL"
import "vendor:glfw"

import "core:c"
import "core:fmt"
import "core:math"

TITLE :: "Hello Triangle"

GL_VERISON_MAJOR :: 3
GL_VERISON_MINOR :: 3

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

	glfw.SetFramebufferSizeCallback(window, framebuffer_size_callback)

	shader_program := gl.CreateProgram()
	defer gl.DeleteProgram(shader_program)
	{
		vertex_source: cstring = #load("./vert.glsl")
		vertex_shader := gl.CreateShader(gl.VERTEX_SHADER)
		gl.ShaderSource(vertex_shader, 1, &vertex_source, nil)
		gl.CompileShader(vertex_shader)
		success: i32
		gl.GetShaderiv(vertex_shader, gl.COMPILE_STATUS, &success)
		if !bool(success) {
			LOG_SIZE :: 512
			info_log: [LOG_SIZE]c.char
			gl.GetShaderInfoLog(vertex_shader, LOG_SIZE, nil, raw_data(info_log[:]))
			// TODO: Here, need to alloc string or something
		}
	}

	for !glfw.WindowShouldClose(window) {
		glfw.PollEvents()

		if glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS {
			glfw.SetWindowShouldClose(window, true)
		}

		// Source: https://justinparrtech.com/JustinParr-Tech/spectrum-generating-color-function-using-sine-waves/
		{
			time := glfw.GetTime()
			red := (math.sin(time) + 1) / 2
			green := (math.sin(time - 2.0 * math.PI / 3.0) + 1) / 2
			blue := (math.sin(time - 4.0 * math.PI / 3.0) + 1) / 2
			gl.ClearColor(f32(red), f32(green), f32(blue), 1.0)
			gl.Clear(gl.COLOR_BUFFER_BIT)
		}

		gl.Clear(gl.COLOR_BUFFER_BIT)

		glfw.SwapBuffers(window)
	}
}

framebuffer_size_callback :: proc "c" (window: glfw.WindowHandle, width, height: c.int) {
	gl.Viewport(0, 0, width, height)
}
