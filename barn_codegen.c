/*
 *
 *  Barn Programming Language Organization
 *  All rights reserved. Copyright (C) 2022-2023
 *   
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
*/

#include <barn_codegen.h>

#include <barn_core.h>
#include <barn_parser.h>
#include <barn_string.h>
#include <barn_std.h>

barn_codegen_t*
barn_codegen_create(barn_parser_t* parser)
{
    barn_codegen_t* codegen = (barn_codegen_t*)calloc(1, sizeof(barn_codegen_t));
    BARN_NO_NULL(codegen);

    codegen->parser = parser;
    codegen->tabs   = 0;

    codegen->c_file = fopen("barn.c", "w+");

    return codegen;
}

void
barn_codegen_add_header(barn_codegen_t* codegen, const char* c_header)
{
    fprintf(codegen->c_file, "#include \"");
    fprintf(codegen->c_file, barn_std_get_path());
    fprintf(codegen->c_file, c_header);
    fprintf(codegen->c_file, "\"\n\n");
}

barn_codegen_t* 
barn_codegen_start(barn_parser_t* parser)
{
    barn_codegen_t* codegen = barn_codegen_create(parser);

    // TODO: if flag --no-stdlib is on don't add barn_header.h
    barn_codegen_add_header(codegen, "barn_format.h");
    barn_codegen_add_header(codegen, "barn_header.h");

    

    fclose(codegen->c_file);
}

const char*
barn_codegen_save_output_to_file(barn_codegen_t* codegen, const char* filename)
{
    // printf("nok\n");
    // FILE* f = fopen(filename, "w+");
    // printf("nok\n");

    // fprintf(f, "%s\n%s\n", 
    //     codegen->c_header, codegen->c_code);
    // printf("nok\n");
    // fclose(f);
}

const char* 
barn_codegen_type_convert_to_c(barn_codegen_t* codegen)
{

}