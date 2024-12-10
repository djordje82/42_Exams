#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function: Skip spaces
void skip_spaces(const char **json) {
    while (isspace(**json)) (*json)++;
}

// Helper function: Parse a string
void parse_string(const char **json, char *buffer) {
    skip_spaces(json);
    if (**json == '"') {
        (*json)++; // Skip the opening quote
        int i = 0;
        while (**json && **json != '"') {
            buffer[i++] = *(*json)++;
        }
        buffer[i] = '\0';
        if (**json == '"') (*json)++; // Skip the closing quote
    }
}

// Helper function: Parse a number
int parse_number(const char **json) {
    skip_spaces(json);
    int num = 0;
    while (isdigit(**json)) {
        num = num * 10 + (**json - '0');
        (*json)++;
    }
    return num;
}

// Function: Parse key-value pairs
void parse_key_value(const char **json) {
    char key[100];
    char value[100];
    skip_spaces(json);
    parse_string(json, key); // Parse the key
    skip_spaces(json);

    if (**json == ':') {
        (*json)++; // Skip the colon
        skip_spaces(json);
        if (**json == '"') { // String value
            parse_string(json, value);
            printf("%s: %s\n", key, value);
        } else if (isdigit(**json)) { // Number value
            int num = parse_number(json);
            printf("%s: %d\n", key, num);
        } else if (**json == '{') { // Nested object
            printf("%s:\n", key);
            parse_object(json);
        } else if (**json == '[') { // Array
            printf("%s:\n", key);
            parse_array(json);
        }
    }
}

// Function: Parse a JSON array
void parse_array(const char **json) {
    skip_spaces(json);
    if (**json == '[') {
        (*json)++; // Skip the opening bracket
        while (**json && **json != ']') {
            char value[100];
            parse_string(json, value);
            printf("Skill: %s\n", value);
            skip_spaces(json);
            if (**json == ',') (*json)++; // Skip the comma
        }
        if (**json == ']') (*json)++; // Skip the closing bracket
    }
}

// Function: Parse a JSON object
void parse_object(const char **json) {
    skip_spaces(json);
    if (**json == '{') {
        (*json)++; // Skip the opening brace
        while (**json && **json != '}') {
            parse_key_value(json);
            skip_spaces(json);
            if (**json == ',') (*json)++; // Skip the comma
        }
        if (**json == '}') (*json)++; // Skip the closing brace
    }
}

// Main function
int main() {
    const char *json_string = "{\"name\": \"Alice\", \"age\": 25, \"skills\": [\"C\", \"Python\", \"JSON\"], \"details\": {\"city\": \"Berlin\", \"country\": \"Germany\"}}";

    printf("Parsing JSON:\n");
    parse_object(&json_string);

    return 0;
}
