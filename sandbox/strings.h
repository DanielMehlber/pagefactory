

#ifndef bool
#define bool _Bool
#define true 1
#define false 0
#endif

#define KEEP false
#define DELETE true

// get length of string
int string_len(const char* string){
    char c = string[0];
    int index = 0;
    while(c != '\0'){
        index++;
        c = string[index];
    }
    return index;
}


struct s_string_builder_node {
    int length;
    const char* content;
    struct s_string_builder_node* next;
    bool delete_content_on_free;
};

struct s_string_builder_node* new_string_builder_node(char* text, bool mode){
    struct s_string_builder_node* node = (struct s_string_builder_node*) malloc(sizeof(struct s_string_builder_node));
    node->content = text;
    node->length = string_len(text);
    node->next = (struct s_string_builder_node*)0; 
    node->delete_content_on_free = mode;
    return node;
}

void free_string_builder_node(struct s_string_builder_node* node) {
    if(node->next != 0)
        free_string_builder_node(node->next);

    if(node->delete_content_on_free == DELETE)
        free((void*)node->content);

    free(node);
}

typedef struct string_builder{
    int length;
    struct s_string_builder_node* first;
    struct s_string_builder_node* last;
} string_builder;

string_builder new_string_builder(){
    string_builder builder;
    builder.first = (void*)0;
    builder.last = (void*)0;
    builder.length = 0;
    return builder;
}

void string_builder_add_string(string_builder* builder, char* text, bool mode){
    struct s_string_builder_node* new_node = new_string_builder_node(text, mode);
    if(builder->last != (void*)0){
        builder->last->next = new_node;
    } else {
        builder->first = new_node;
    }

    builder->last = new_node;

    builder->length += new_node->length;
}

char* string_builder_to_c_string(string_builder* builder){
    char* string = malloc(sizeof(char) * (builder->length + 1));
    struct s_string_builder_node* current = builder->first;
    int index = 0;
    while(current != (void*)0){
        for(int i = 0; i < current->length; i++){
            string[index] = current->content[i];
            index++;
        }
        current = current->next;
    }

    string[builder->length] = '\0';
    return string;
}

void clear_string_builder(string_builder* builder) {
    if(builder->first != 0) {
        free_string_builder_node(builder->first);
    }
}