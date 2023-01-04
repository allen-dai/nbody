const app = document.getElementById("app");
const WIDTH = 1920;
const HEIGHT = 1080;
const N_BODY = 1000;
const MASS = 1;
const G = 20;

app.width = WIDTH;
app.height = HEIGHT;
const ctx = app.getContext("2d");
ctx.fillStyle = "black";
ctx.fillRect(0, 0, WIDTH, HEIGHT);

function make_environment(...envs) {
    return new Proxy(envs, {
        get(target, prop, receiver) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => {console.error("NOT IMPLEMENTED: "+prop, args)}
        }
    });
}

const b_arr = new Array();
var wasm;

WebAssembly.instantiateStreaming(fetch("./index.wasm"), {
    "env": make_environment()
}).then( w=> {
    wasm = w;
    init();
});

const get_info = (wasm, body) => {
    const info_ptr = wasm.instance.exports.get_info(body);
    const buffer = wasm.instance.exports.memory.buffer;
    return new Uint32Array(buffer, info_ptr, 8)
}

const draw_circle = (arr) => {
    ctx.fillStyle = "white";
    ctx.beginPath();
    r = arr[7];
    if (arr[6] === 1){
        r = 1
    }
    ctx.arc(arr[0], arr[1], r, 0, 2*Math.PI);
    ctx.fill();
}

const init = () => {
    for (let i = 0; i < N_BODY; i++) {
        const b = wasm.instance.exports.new_body(Math.random() * WIDTH, Math.random() * HEIGHT, 0,0, MASS);
        draw_circle(get_info(wasm, b));
        b_arr.push(b);
    }
    window.requestAnimationFrame(loop);
}

const loop = () => {
    for (let i = 0; i < b_arr.length; i++) {
        const b1 = b_arr[i];
        for (let j = 0; j < b_arr.length; j++) {
            if (i === j) {
                continue;
            }
            const b2 = b_arr[j];
            wasm.instance.exports.attract_body(b1, b2, G);
            wasm.instance.exports.attract_body(b2, b1, G);
        }
    }
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    b_arr.forEach( b=> {
        wasm.instance.exports.update_body(b);
        draw_circle(get_info(wasm, b));
    })
    window.requestAnimationFrame(loop);
}
