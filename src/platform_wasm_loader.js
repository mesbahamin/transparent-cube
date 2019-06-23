let utf8decoder = new TextDecoder("utf-8");
let memory = null;
let exports = {};
let imports = {};
let files = [];
let gl = null;
let gl_id_freelist = [];
let gl_id_map = [null];
imports["webglAttachShader"] = function(program_id, shader_id) {
    let program = gl_id_map[program_id];
    let shader = gl_id_map[shader_id];
    gl.attachShader(program, shader);
}
imports["webglBindBuffer"] = function(target, buffer_id) {
    let buffer = gl_id_map[buffer_id];
    gl.bindBuffer(target, buffer);
}
imports["webglBindVertexArray"] = function(vao_id) {
    let vao = gl_id_map[vao_id];
    gl.bindVertexArray(vao);
}
imports["webglBlendColor"] = function(red, green, blue, alpha) {
    gl.blendColor(red, green, blue, alpha);
}
imports["webglBlendFunc"] = function(sfactor, dfactor) {
    gl.blendFunc(sfactor, dfactor);
}
imports["webglBufferData"] = function(target, size, data, usage) {
    let dataslice = memory.subarray(data, data + size);
    gl.bufferData(target, dataslice, usage);
}
imports["webglClear"] = function(mask) {
    gl.clear(mask);
}
imports["webglClearColor"] = function(red, green, blue, alpha) {
    gl.clearColor(red, green, blue, alpha);
}
imports["webglCompileShader"] = function(shader_id) {
    let shader = gl_id_map[shader_id];
    gl.compileShader(shader);
}
imports["webglCreateBuffer"] = function() {
    let buffer = gl.createBuffer();
    let buffer_id = webgl_id_new(buffer);
    return buffer_id
}
imports["webglCreateProgram"] = function() {
    let program = gl.createProgram();
    let program_id = webgl_id_new(program);
    return program_id;
}
imports["webglCreateShader"] = function(type) {
    let shader = gl.createShader(type);
    let shader_id = webgl_id_new(shader);
    return shader_id;
}
imports["webglCreateVertexArray"] = function() {
    let vao = gl.createVertexArray()
    let vao_id = webgl_id_new(vao);
    return vao_id
}
imports["webglDeleteBuffer"] = function(buffer_id) {
    let buffer = gl_id_map[buffer_id];
    gl.deleteBuffer(buffer);
    webgl_id_remove(buffer_id);
}
imports["webglDeleteShader"] = function(shader_id) {
    let shader = gl_id_map[shader_id];
    gl.deleteShader(shader);
    webgl_id_remove(shader_id);
}
imports["webglDeleteVertexArray"] = function(vao_id) {
    let vao = gl_id_map[vao_id];
    gl.deleteVertexArray(vao);
    webgl_id_remove(vao_id);
}
imports["webglDepthMask"] = function(flag) {
    gl.depthMask(flag);
}
imports["webglDisable"] = function(cap) {
    gl.disable(cap);
}
imports["webglDrawElements"] = function(mode, count, type, offset) {
    gl.drawElements(mode, count, type, offset);
}
imports["webglEnable"] = function(cap) {
    gl.enable(cap);
}
imports["webglEnableVertexAttribArray"] = function(index) {
    gl.enableVertexAttribArray(index);
}
imports["webglGetProgramInfoLog"] = function() {
}
imports["webglGetProgramParameter"] = function(program_id, param) {
    let program = gl_id_map[program_id];
    return gl.getProgramParameter(program, param);
}
imports["webglGetShaderInfoLog"] = function(shader_id, out_buf) {
    let shader = gl_id_map[shader_id];
    let info_log = gl.getShaderInfoLog(shader);

    // TODO: remove this once we get sprintf workingk
    console.log(info_log);

    let arr = memory.subarray(info_log, out_buf + info_log.byteLength);
    arr.set(new Uint8Array(info_log));
    return true;
}
imports["webglGetShaderParameter"] = function(shader_id, param) {
    let shader = gl_id_map[shader_id];
    let result = gl.getShaderParameter(shader, param);
    return result;
}
imports["webglGetUniformLocation"] = function(program_id, name_ptr, name_len) {
    let program = gl_id_map[program_id];
    let name = utf8decoder.decode(memory.subarray(name_ptr, name_ptr+name_len));
    let loc = gl.getUniformLocation(program, name);
    let location_id = webgl_id_new(loc);
    return location_id;
}
imports["webglLinkProgram"] = function(program_id) {
    let program = gl_id_map[program_id];
    gl.linkProgram(program);
}
imports["webglShaderSource"] = function(shader_id, source_ptr, source_len) {
    let shader = gl_id_map[shader_id];
    let arr = memory.subarray(source_ptr, source_ptr + source_len);
    let s = utf8decoder.decode(arr);
    gl.shaderSource(shader, s);
}
imports["webglUniform1f"] = function(location_id, value) {
    let loc = gl_id_map[location_id];
    gl['uniform1f'](loc, value);
}
imports["webglUniform1i"] = function(location_id, value) {
    let loc = gl_id_map[location_id];
    gl['uniform1i'](loc, value);
}
imports["webglUniform3f"] = function(location_id, x, y, z) {
    let loc = gl_id_map[location_id];
    gl['uniform3fv'](loc, [x, y, z]);
}
imports["webglUniformMatrix4fv"] = function(location_id, transpose, data) {
    let loc = gl_id_map[location_id];
    let dataslice = memory.slice(data, data + 4 * 16);
    gl.uniformMatrix4fv(loc, transpose, new Float32Array(dataslice.buffer));
}
imports["webglUseProgram"] = function(program_id) {
    let program = gl_id_map[program_id];
    gl.useProgram(program);
}
imports["webglVertexAttribPointer"] = function(index, size, type, normalized, stride, offset) {
    gl.vertexAttribPointer(index, size, type, normalized, stride, offset);
}
imports["webglViewport"] = function(x, y, width, height) {
    gl.viewport(x, y, width, height);
}
imports["js_read_entire_file"] = function(name, name_len, out_buf) {
    let file_name = utf8decoder.decode(memory.subarray(name, name + name_len))
    if (file_name == "shader/cube_f.glsl") {
        var file = files[1];
    } else if (file_name == "shader/cube_v.glsl") {
        var file = files[2];
    } else {
        return false;
    }
    let arr = memory.subarray(out_buf, out_buf + file.byteLength);
    let s = String.fromCharCode.apply(null, arr);
    arr.set(new Uint8Array(file));
    return true;
}
imports["js_print"] = function(s, len) {
    let arr = memory.subarray(s, s + len);
    console.log(utf8decoder.decode(arr));
}
function error_fatal(message) {
    console.log(message);
    throw message;
}
function webgl_id_new(obj) {
    if(gl_id_freelist.length == 0) {
        gl_id_map.push(obj);
        return gl_id_map.length - 1;
    } else {
        let id = gl_id_freelist.shift();
        gl_id_map[id] = obj;
        return id;
    }
}
function webgl_id_remove(id) {
    delete gl_id_map[id];
    gl_id_freelist.push(id);
}
function canvas_resize() {
    let pr = window.devicePixelRatio;
    let w = window.innerWidth;
    let h = window.innerHeight;
    // Bitwise OR does float truncation
    let w_pixels = (w * pr) | 0;
    let h_pixels = (h * pr) | 0;
    gl.canvas.width = w_pixels;
    gl.canvas.height = h_pixels
    exports['window_resize'](w_pixels, h_pixels);
    console.log("resize: (" + w_pixels + ", " + h_pixels + ")");
}
function canvas_render() {
    exports['render']();
    window.requestAnimationFrame(canvas_render);
}
function file_load(name) {
    let promise = new Promise((resolve, reject) => {
        fetch(name).then(resp => {
            resp.arrayBuffer().then(arr => resolve(arr));
        });
    });
    return promise;
}
window.onload = async function() {
    let ctxopts = {
        alpha: false,
        depth: true,
        stencil: false,
        antialias: true,
        preserveDrawingBuffer: false
    };
    gl = document.getElementById("webglcanvas").getContext("webgl2", ctxopts);
    if(!gl) {
        error_fatal("Your browser does not support WebGL 2.");
    }
    files[0] = file_load("binary.wasm");
    files[1] = file_load("shader/cube_f.glsl");
    files[2] = file_load("shader/cube_v.glsl");
    for(var i=0; i<files.length; i++) {
        files[i] = await files[i];
    }
    let binary = files[0];
    imports['memory'] = new WebAssembly.Memory({'initial':32});
    memory = new Uint8Array(imports['memory']['buffer']);
    let program = await WebAssembly.instantiate(binary, {"env":imports});
    let instance = program['instance'];
    exports = instance['exports'];
    canvas_resize();
    window.addEventListener("resize", canvas_resize);
    if(!exports['init']()) {
        error_fatal("Game initialization failed.");
    }
    canvas_render();
}
