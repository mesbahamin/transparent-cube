let utf8decoder = new TextDecoder("utf-8");
let memory = null;
let exports = {};
let imports = {};
let files = [];
let gl = null;
let gl_id_freelist = [];
let gl_id_map = [null];
