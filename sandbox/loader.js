Factory = {
    "memory": null,
    "heap": null,
    "data": {},
    "import": {
        "env": {
            "emscripten_notify_memory_growth": function(){},

            "fetch_parse_json_from": function(p_name, p_url){
                const strname = readString(p_name);
                const strurl = readString(p_url);
                var request = new XMLHttpRequest();
                request.open('GET', strurl, false);  // `false` makes the request synchronous
                request.send(null);

                if (request.status === 200) {
                    console.log(request.responseText);
                    try {
                        Factory.data[strname] = JSON.parse(request.responseText);
                    } catch (err) {
                        Factory.data[strname] = null;
                        console.log("Error: fetched data from " + strurl + " is not a valid JSON");
                    }
                } else {
                    Factory.data[strname] = null;
                    console.log("Error: data from " + strurl + " cannot be fetched");
                }
            },
            "try_read_fetched_value": function(str_ptr) {
                const path = readString(str_ptr);
                const path_elems = path.split(".");
                var obj = Factory.data;
                for(var i = 0; i < path_elems.length; i++) {
                    const elem = path_elems[i];
                    obj = obj[elem];
                    if(obj == undefined) {
                        return makeString("[element '*." + elem + "' does not exist]");
                    }
                }
                if(typeof obj != String) {
                    obj = JSON.stringify(obj);
                }
                return makeString(obj);
                
            },
            "console_print": function(ptr) {
                console.log(readString(ptr));
            }
        }
    }
}

async function blockawait(promise){
    var v = null;
    promise.then(res => {
        v = res;
    })
    while(v == null);
    return v;
}

function makeString(str) {
    const ptr = Factory.malloc_string(str.length);
    for(i = 0; i < str.length; i++) {
        Factory.heap[ptr + i] = str.charCodeAt(i);
    }
    return ptr;
}

function readString(addr) {
    var current_char = Factory.heap[addr];
    var current_length = 0;
    var chars = [];
    while(current_char != 0) {
        current_length++;
        chars.push(String.fromCharCode(current_char));
        current_char = Factory.heap[addr + current_length];
    }

    return chars.join("");
}

function takeString(addr) {
    const str = readString(addr);
    Factory.free_string(addr);
    return str;
}

WebAssembly.instantiateStreaming(fetch('./factory.wasm'), Factory.import)
.then(wasm => {
    // init
    wasm.instance.exports._initialize();
    Factory.memory = wasm.instance.exports.memory;
    Factory.heap = new Uint8Array(Factory.memory.buffer);
    // get basic exported functions
    Factory["malloc_string"] = wasm.instance.exports.malloc_string;
    Factory["free_string"] = wasm.instance.exports.free_string;
    // get and wrap custom exported function
    Factory["test"] = function(input) {
        return takeString(wasm.instance.exports.test(makeString(input)));
    }
});