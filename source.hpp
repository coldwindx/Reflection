#pragma once
#ifdef __META_PARSER__
#define META __attribute__((annotate("reflect-class")))
#define PROPERTY(...) __attribute__((annotate("reflect-property;" #__VA_ARGS__)))
#define FUNCTION(...) __attribute__((annotate("reflect-function;" #__VA_ARGS__)))
#else
#define META
#define PROPERTY()
#define FUNCTION()
#endif


class META HelloController {
};