#include "emscripten.h"
#include "malloc.h"
#include "strings.h"


// functions to import ==> defined in loader.js
extern char* console_print(char* text);
extern void fetch_parse_json_from(char* name, char* url);
extern char* try_read_fetched_value(char* name);



EMSCRIPTEN_KEEPALIVE
char* malloc_string(int length) {
    char* str = malloc(sizeof(char) * (length + 1));
    str[length] = '\0';
    return str;
}

EMSCRIPTEN_KEEPALIVE
void free_string(char* ptr) {
    free(ptr);
}

EMSCRIPTEN_KEEPALIVE
char* test(char* something) {
    string_builder builder = new_string_builder();

    fetch_parse_json_from("test", "./data.json");

    string_builder_add_string(&builder, "<h1>Das hier ist ein Test</h1>", KEEP);
    string_builder_add_string(&builder, "Der Benutzer hat etwas eingegeben...", KEEP);
    string_builder_add_string(&builder, " Er meint: '", KEEP);
    string_builder_add_string(&builder, something, DELETE);
    string_builder_add_string(&builder, "'. Weißt du was <i>er</i> meint?", KEEP);
    string_builder_add_string(&builder, try_read_fetched_value("test"), DELETE);

    char* result = string_builder_to_c_string(&builder);

    clear_string_builder(&builder);
    return result;

}