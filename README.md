<p align="center">
  <img src="https://cdn.discordapp.com/icons/1028291656862142466/45ac54896f82c18ddf408e5c35739789.png?size=256" />
</p>

# REWRITE OF BARN!!!
[READ MORE HERE](https://barn-lang.github.io/barn-docs/news/news-rewriting-whole-compiler-and-reactivaiton-of-barn-project.html)
Old Go compiler is in barngo repository

# Barn
# About
#### Barn is in experimental version (BETA)
**Barn** is a simple programming language written in GoLang and compiled to C.
Syntax is very simple, and Barn can call C/C++ functions natively so creating new bindings should be easy. Barn was written with the goal of being as fast and easy to use as possible. Barn is just a little project with which I can chill and just code.
# Documentation
You can find barn documentation at [/documentation](https://barn-lang.github.io/barn-docs/documentation.html)
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
## Fizz Buzz in Barn
[31-fizz-buzz-in-barn.ba](./examples/31-fizz-buzz-in-barn.ba)
```kotlin
@import "std.ba"

fun main() {
    for let i: int = 0; i != 1000; i++ {
        if (i % 15) == 0 {
            println("FizzBuzz\t")
        } elif (i % 3) == 0 {
            println("Fizz\t")
        } elif (i % 5) == 0 {
            println("Buzz\t")
        } else {
            printnum(i)
            println("\t")
        }
    }
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
# Extensions
## Visual Studio Code
[Here!](./extensions/vscode/), copy **barn-lang** folder to where you installed visual studio code (usually $HOME/.vscode/extensions/)
## Vim
[Here!](./extensions/vim/) move barn.vim into .vim/syntax/ and add the following line  to ~/.vimrc `autocmd BufRead,BufNewFile *.barn set filetype=barn`
# Documentation
[HERE!!](https://barn-lang.github.io/barn-docs/)
# Bugs
If you found a bug immediately call me about it, you can create [Github Issue](), write to me on Discord Solindek#9808 or on my [website contact page](https://solindedk.ct8.pl/)


