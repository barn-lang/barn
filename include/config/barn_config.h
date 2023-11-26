#ifndef __BARN_CONFIG__
#define __BARN_CONFIG__

/* 
 * This will generate prettier code but it will be 
 * more massive and will take more memory. For example
 * without BARN_CODEGEN_PRETTIER_CODE defined generated
 * expression will look like this
 * 
 * With BARN_CODEGEN_PRETTIER_CODE:
 *  Expression: "(1+3*5)/ 2" 
 *  Generated:  "(1 + 3 * 5) / 2"
 * 
 * It will just make generated code look better for reading
 * without it there wouldn't be any spaces 
 * 
 * If you don't want your code look pretty just comment line
 * below
 */
// #define BARN_CODEGEN_PRETTIER_CODE

#endif /* __BARN_CONFIG__ */