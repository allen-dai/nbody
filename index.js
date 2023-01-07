var svg = document.createElementNS("http://www.w3.org/2000/svg",'svg');
var xform = svg.createSVGMatrix();
var trail = false;
const WIDTH = 1800;
const HEIGHT = 930;
const NUM_BODY = 0;
const MASS = 1;
const G = 20;

const canvas = document.getElementById("canvas");
canvas.width = WIDTH;
canvas.height = HEIGHT;
const ctx = canvas.getContext("2d");
ctx.fillStyle = "black";
ctx.fillRect(0, 0, WIDTH, HEIGHT);

const p_g = document.getElementById("gravity");
const p_b = document.getElementById("body");
const p_m = document.getElementById("mass");
const p_t = document.getElementById("trail");
p_g.innerHTML = G;
p_b.innerHTML = NUM_BODY;
p_m.innerHTML = MASS;
p_t.innerHTML = trail.toString();

var b_arr = new Array();
var wasm;
var mouse_x = 0;
var mouse_y = 0;
var mouse_mass = 1;
var mouse_down = false;

document.addEventListener("keydown", e=> {
    switch (e.key) {
        case "r":
            b_arr.forEach( b => {
                wasm.instance.exports.free_body(b);
            })
            b_arr = new Array();
            p_b.innerHTML = 0;
            break;
        case "a":
            mouse_mass += 1;
            p_m.innerHTML = mouse_mass;
            break;
        case "A":
            mouse_mass += 100;
            p_m.innerHTML = mouse_mass;
            break;
        case "d":
            p_m.innerHTML = mouse_mass;
            break;
        case "D":
            mouse_mass -= 100;
            if (mouse_mass <= 0){
                mouse_mass = 1;
            }
            p_m.innerHTML = mouse_mass;
            break;
        case "t":
            trail = !trail;
            p_t.innerHTML = trail.toString();
            break;
        default:
            break;
    }
})
var pt = svg.createSVGPoint();
ctx.transformedPoint = function(x,y){
    pt.x=x; pt.y=y;
    return pt.matrixTransform(xform.inverse());
}
var scaleFactor = 1.1;
var zoom = function(clicks){
    var pt = ctx.transformedPoint(mouse_x,mouse_y);
    ctx.translate(pt.x,pt.y);
    var factor = Math.pow(scaleFactor,clicks);
    ctx.scale(factor,factor);
    ctx.translate(-pt.x,-pt.y);
}

var handleScroll = function(evt){
    var delta = evt.wheelDelta ? evt.wheelDelta/40 : evt.detail ? -evt.detail : 0;
    if (delta) zoom(delta);
    return evt.preventDefault() && false;
};
canvas.addEventListener('DOMMouseScroll',handleScroll,false);
canvas.addEventListener('mousewheel',handleScroll,false);

function getTransformedPoint(x, y) {
    const inverseTransform = ctx.getTransform().invertSelf();
    const transformedX = inverseTransform.a * x + inverseTransform.c * y + inverseTransform.e;
    const transformedY = inverseTransform.b * x + inverseTransform.d * y + inverseTransform.f;
    mouse_x = transformedX;
    mouse_y = transformedY;
  //return { x: transformedX, y: transformedY };
}

canvas.addEventListener("mousemove", function (e) {
    getTransformedPoint(e.offsetX, e.offsetY);
});

canvas.addEventListener("mousedown", function (e) {
    getTransformedPoint(e.offsetX, e.offsetY);
    mouse_down = true;
});

canvas.addEventListener("mouseup", function (e) {
    getTransformedPoint(e.offsetX, e.offsetY);
    mouse_down = false;
});

canvas.addEventListener("click", function (e) {
    const b = wasm.instance.exports.new_body(mouse_x, mouse_y, 0,0, mouse_mass);
    b_arr.push(b);
    p_b.innerHTML = b_arr.length;
});

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


WebAssembly.instantiateStreaming(fetch("./index.wasm"), {
    "env": make_environment()
}).then( w=> {
    wasm = w;
    init();
});

const get_info = (wasm, body) => {
    const info_ptr = wasm.instance.exports.get_info(body);
    const buffer = wasm.instance.exports.memory.buffer;
    return new Int32Array(buffer, info_ptr, 8)
}

const draw_circle = (arr) => {
    ctx.fillStyle = "white";
    ctx.beginPath();
    r = arr[7];
    ctx.arc(arr[0], arr[1], r, 0, 2*Math.PI);
    ctx.fill();
}

const init = () => {
    for (let i = 0; i < NUM_BODY; i++) {
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
    if (!trail){
        ctx.save();
        ctx.setTransform(1,0,0,1,0,0);
        ctx.fillStyle = "black";
        ctx.fillRect(0,0,canvas.width,canvas.height);
        ctx.restore();
    }
    draw_circle([mouse_x, mouse_y, 0,0,0,0,0,Math.sqrt(mouse_mass)*2]);
    b_arr.forEach( b=> {
        wasm.instance.exports.update_body(b);
        draw_circle(get_info(wasm, b));
    })
    window.requestAnimationFrame(loop);
}

function mouse_pos(canvas, e) {
    const rect = canvas.getBoundingClientRect();
    return {
        x: e.clientX - rect.left,
        y: e.clientY - rect.top
    };
}
