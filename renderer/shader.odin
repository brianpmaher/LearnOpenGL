package renderer

import gl "vendor:OpenGL"

import "core:fmt"
import "core:os"
import "core:strings"

create_program :: proc(vertex_path, fragment_path: string) -> GL_ID {
	shader := gl.CreateProgram()

	vertex_shader := compile_shader(vertex_path, .VERTEX_SHADER)
	defer gl.DeleteShader(vertex_shader)

	fragment_shader := compile_shader(fragment_path, .FRAGMENT_SHADER)
	defer gl.DeleteShader(fragment_shader)

	gl.AttachShader(shader, vertex_shader)
	gl.AttachShader(shader, fragment_shader)
	gl.LinkProgram(shader)
	success: i32
	gl.GetProgramiv(shader, gl.LINK_STATUS, &success)
	if !bool(success) {
		LOG_SIZE :: 512
		info_log: [LOG_SIZE]u8
		gl.GetProgramInfoLog(shader, LOG_SIZE, nil, raw_data(&info_log))
		info_log_str := string(cstring(raw_data(&info_log)))
		panic(info_log_str)
	}

	return shader
}

@(private = "file")
compile_shader :: proc(path: string, type: gl.GL_Enum) -> GL_ID {
	shader := gl.CreateShader(u32(type))
	source_bytes, ok := os.read_entire_file(path, context.temp_allocator)
	if !ok {
		fmt.panicf("Failed to load file \"%s\"", path)
	}
	source := string(source_bytes)
	source_cstr := strings.clone_to_cstring(source, context.temp_allocator)
	gl.ShaderSource(shader, 1, &source_cstr, nil)
	gl.CompileShader(shader)
	success: i32
	gl.GetShaderiv(shader, gl.COMPILE_STATUS, &success)
	if !bool(success) {
		LOG_SIZE :: 512
		info_log: [LOG_SIZE]u8
		gl.GetShaderInfoLog(shader, LOG_SIZE, nil, raw_data(&info_log))
		info_log_cstr := cstring(&info_log[0])
		info_log_str := string(info_log_cstr)
		panic(info_log_str)
	}
	return shader
}
