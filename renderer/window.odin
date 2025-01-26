package renderer

import gl "vendor:OpenGL"
import "vendor:glfw"

import la "core:math/linalg"
import "core:strings"

Window :: struct {
	handle: glfw.WindowHandle,
	input:  Input,
}

Key :: enum {
	Escape,
	W,
	A,
	S,
	D,
}

Key_State :: enum {
	Up,
	Down,
	Press,
	Release,
}

Input :: struct {
	input_started:   bool,
	cursor_position: la.Vector2f32,
	cursor_delta:    la.Vector2f32,
	scroll_delta:    la.Vector2f32,
	key_escape:      Key_State,
	key_w:           Key_State,
	key_a:           Key_State,
	key_s:           Key_State,
	key_d:           Key_State,
}

create_window :: proc(
	width, height: int,
	title: string,
	allocator := context.allocator,
) -> ^Window {
	glfw.Init()
	glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR)
	glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, GL_VERSION_MINOR)
	glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

	window := new(Window, allocator)
	title_cstr := strings.clone_to_cstring(title, context.temp_allocator)
	window.handle = glfw.CreateWindow(i32(width), i32(height), title_cstr, nil, nil)
	if window.handle == nil {
		panic("Failed to create GLFW window")
	}

	glfw.MakeContextCurrent(window.handle)

	gl.load_up_to(GL_VERSION_MAJOR, GL_VERSION_MINOR, glfw.gl_set_proc_address)

	glfw.SetWindowUserPointer(window.handle, window)
	glfw.SetFramebufferSizeCallback(window.handle, framebuffer_size_callback)
	glfw.SetKeyCallback(window.handle, key_callback)
	glfw.SetCursorPosCallback(window.handle, cursor_pos_callback)
	glfw.SetScrollCallback(window.handle, scroll_callback)

	return window
}

destroy_window :: proc(window: ^Window, allocator := context.allocator) {
	glfw.DestroyWindow(window.handle)
	free(window, allocator)
	glfw.Terminate()
}

poll_events :: proc(window: ^Window) {
	reset_input(window)
	glfw.PollEvents()
}

swap_buffers :: proc(window: ^Window) {
	glfw.SwapBuffers(window.handle)
}

window_should_close :: proc(window: ^Window) -> bool {
	return bool(glfw.WindowShouldClose(window.handle))
}

close_window :: proc(window: ^Window) {
	glfw.SetWindowShouldClose(window.handle, true)
}

@(private = "file")
reset_input :: proc(window: ^Window) {
	window.input.key_escape = .Up
	window.input.key_w = .Up
	window.input.key_a = .Up
	window.input.key_s = .Up
	window.input.key_d = .Up
}

@(private = "file")
framebuffer_size_callback :: proc "c" (handle: glfw.WindowHandle, width, height: i32) {
	gl.Viewport(0, 0, width, height)
}

@(private = "file")
key_callback :: proc "c" (handle: glfw.WindowHandle, key, scancode, action, mods: i32) {
	window := cast(^Window)glfw.GetWindowUserPointer(handle)

	switch key {
	case glfw.KEY_ESCAPE:
		window.input.key_escape = action == glfw.PRESS ? .Press : .Release
	case glfw.KEY_W:
		window.input.key_w = action == glfw.PRESS ? .Press : .Release
	case glfw.KEY_A:
		window.input.key_a = action == glfw.PRESS ? .Press : .Release
	case glfw.KEY_S:
		window.input.key_s = action == glfw.PRESS ? .Press : .Release
	case glfw.KEY_D:
		window.input.key_d = action == glfw.PRESS ? .Press : .Release
	}
}

@(private = "file")
cursor_pos_callback :: proc "c" (handle: glfw.WindowHandle, xpos, ypos: f64) {
	window := cast(^Window)glfw.GetWindowUserPointer(handle)

	xpos := f32(xpos)
	ypos := f32(ypos)

	if !window.input.input_started {
		window.input.cursor_position = {xpos, ypos}
		window.input.cursor_delta = {0.0, 0.0}
		window.input.input_started = true
	} else {
		prev_position := window.input.cursor_position
		window.input.cursor_position = {xpos, ypos}
		window.input.cursor_delta = window.input.cursor_position - prev_position
	}
}

@(private = "file")
scroll_callback :: proc "c" (handle: glfw.WindowHandle, x_offset, y_offset: f64) {
	window := cast(^Window)glfw.GetWindowUserPointer(handle)
	window.input.scroll_delta = {f32(x_offset), f32(y_offset)}
}
