package main

import gl "vendor:OpenGL"
import "vendor:glfw"

import "core:c"
import "core:fmt"

TITLE :: "Hello Window"

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

	gl.ClearColor(0.2, 0.3, 0.3, 1.0)

	for !glfw.WindowShouldClose(window) {
		glfw.PollEvents()

		if glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS {
			glfw.SetWindowShouldClose(window, true)
		}

		gl.Clear(gl.COLOR_BUFFER_BIT)

		glfw.SwapBuffers(window)
	}
}

framebuffer_size_callback :: proc "c" (window: glfw.WindowHandle, width, height: c.int) {
	gl.Viewport(0, 0, width, height)
}
