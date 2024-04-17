<p align="center">
  <img src="https://cdn.discordapp.com/icons/1028291656862142466/45ac54896f82c18ddf408e5c35739789.png?size=512" />
</p>

# Barn Programming Language
## About

Barn is a simple and efficient programming language designed for beginners and experienced programmers alike. It is compiled to C, making it fast and portable. Barn has a clean and easy-to-read syntax, making it a great choice for learning programming. It also has a growing community of users and contributors.

This repository includes the whole **BarnC** which is basically Barn compiler written in C, lot's of example programs in Barn and also extensions for well known editors like `Vim, Visual Studio Code, Emacs`
 
<img src="https://cdn.discordapp.com/attachments/858992887706157056/1189564357114339430/image.png?ex=659e9f23&is=658c2a23&hm=a92a458d0f9d4ffeff77693817353905863f84c831b1827ef8bfcc55c8db9e78&">

## Get started with Barn
Welcome to the exciting world of Barn programming! For a seamless introduction to the language, we highly recommend exploring our documentation, which is available online. Dive into the details and unleash your creativity by visiting [our documentation page](https://barn-lang.github.io/barn-docs/documentation.html).

If you don't have time to read our documentation check out everything underneath. To get started with Barn have to install our compiler, to do it correctly put these commands into your (Linux/MacOS) terminal (Windows is not supported right now):

`terminal`
```
git clone https://github.com/barn-lang/barn && cd barn
make CC=gcc # Set the compiler you will use
make install
barn --version
```
After that let's dive into some programming. We will start with the most basic program we can write in any programming language yeah i mean a Hello World.

`main.ba`
```kotlin
@import "std.ba"

fun main() -> i32 {
    println("Hello, World!")
    return 0
}
```
That's all, after compiling this program by using this command `barn ./main.ba` and running it with `./a.out` you should see a beautiful `Hello, World!` pop up in your terminal window! That is the most basic thing in Barn. We would also recommend to check out some examples in Barn [right here](./examples/)
## Finding & reporting bugs
Barn is a dynamic and evolving programming language, your feedback is crucial in ensuring its stability and performance. If you encounter any bugs or unexpected behavior, we encourage you to report them as fast as possible. If you find a bug please create a new github issue on this [repository](https://github.com/barn-lang/barn/issues), please include code and terminal output. 


