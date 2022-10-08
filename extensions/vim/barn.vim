" Barn syntax file is taken and edited from 
" https://gitlab.com/tsoding/porth/-/blob/master/editor/porth.vim

" Vim syntax file
" Language: Barn

" Usage Instructions
" Put this file in .vim/syntax/barn.vim
" and add in your .vimrc file the next line:
" autocmd BufRead,BufNewFile *.barn set filetype=barn

if exists("b:current_syntax")
  finish
endif

set iskeyword=a-z,A-Z,-,_,!,@

" Language keywords
syntax keyword barnKeywords if else elif fun @import_c @import let return while break continue for

" Comments
syntax region barnCommentLine start="//" end="$"   contains=barnTodos

" String literals
syntax region barnString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=barnEscapes

" Char literals
syntax region barnChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=barnEscapes

" Escape literals \n, \r, ....
syntax match barnEscapes display contained "\\[nr\"']"

" Number literals
syntax region barnNumber start=/\s\d/ skip=/\d/ end=/\s/

" Type names the compiler recognizes
syntax keyword barnTypeNames char int float string bool

" Set highlights
highlight default link barnKeywords Keyword
highlight default link barnCommentLine Comment
highlight default link barnString String
highlight default link barnNumber Number
highlight default link barnTypeNames Type
highlight default link barnChar Character
highlight default link barnEscapes SpecialChar

let b:current_syntax = "barn"
