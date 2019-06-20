function canvas_resize() {
    let pr = window.devicePixelRatio;
    let w = window.innerWidth;
    let h = window.innerHeight;
    gl.canvas.width = (w * pr) | 0;
    gl.canvas.height = (h * pr) | 0;
    exports['window_resize']((w * pr) | 0, (h * pr) | 0);
    console.log("resize: [" + (w * pr | 0) + ", " + (h * pr | 0) + "] > [" + (w | 0) + ", " + (h | 0) + "]");
}

function canvas_render() {
    exports['render']();
    window.requestAnimationFrame(canvas_render);
}

function file_load(name) {
    let promise = new Promise((ok, nope) => {
        fetch(name).then(resp => {
            resp.arrayBuffer().then(arr => ok(arr));
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

    gl = document.getElementById("mcanvas").getContext("webgl2", ctxopts);
    if(!gl) {
        error_fatal("Your browser does not support WebGL 2");
    }

    files[0] = file_load("binary.wasm");
    files[1] = file_load("shader/cube_f.glsl");
    files[2] = file_load("shader/cube_v.glsl");
    for(var i=0; i<files.length; i++) {
        files[i] = await files[i];
    }

    let binary = files[0];
    imports['memory'] = new WebAssembly['Memory']({'initial':32});
    memory = new Uint8Array(imports['memory']['buffer']);
    let program = await WebAssembly['instantiate'](binary, {"env":imports});

    let instance = program['instance'];
    exports = instance['exports'];

    canvas_resize();
    window.addEventListener("resize", canvas_resize);

    if(!exports['init']()) {
        error_fatal("Engine initialization failed");
    }

    canvas_render();
}
