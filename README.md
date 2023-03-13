#### Barn is in experimental version (BETA)
# Barn
# About
It's a simple programming language written in Go that is compiled to C.
Syntax is very easy and complex, Barn can call C. Barn is very optimized
and fast, it was written with idea to be the most fastest and the 
most easiest to use.
# Dependencies
- GoLang compiler
- Bash/Bash
- GCC
# Author
[Owner (SolindekDev)](https://github.com/solindekdev/)
# Bugs
If you find a bug, immediately call me about it, you can create [Github Issue](), write to me on Discord Solindek#4773 or on my [website contact page](https://solindek.tech/contact.html)
# Extensions
## Visual Studio Code
[Here!](./extensions/vscode/), copy **barn-lang** folder to $HOME/.vscode/extensions/ for unix or there where you installed visual studio code
## Vim
[Here!](./extensions/vim/) Put file barn.vim to .vim/syntax/barn.vim and add in your .vimrc file this `autocmd BufRead,BufNewFile *.barn set filetype=barn`
# Documentation
[HERE!!](https://barn-lang.github.io/barn-docs/)
# Syntax
## Hello World
[01-hello-world.ba](./examples/01-hello-world.ba)
```kotlin
fun main() {
   println("Hello World\n")
}
```
## Variables
[14-function-arguments-variable.ba](./examples/14-function-arguments-variable.ba)
```kotlin
fun example_function(int i1, string str1, bool b1, char c1, float f1) {
    println(str1)
    println("\n")
}

fun main() {
    let i1: int = 1
    let str1: string = "str"
    let b1: bool = true
    let c1: char = 'A'
    let f1: float = 1.0
    example_function(i1, str1, b1, c1, f1)
}
```
## If statements
[19-if-condition.ba](./examples/19-if-condition.ba)
```kotlin
fun main() {
    let age: int = 13
    if (age == 13) {
        println("Age is 13\n")
    } elif (age == 14) {
        println("Age is 14\n")    
    } else {
        println("Age isn't 13\n")
    }
}
```
## User input
```kotlin
@import "std.ba"

fun main() {
	println("What's your name: ")
	let user_name: string = input()
	println("Welcome ")
	println(user_name)
}
```
