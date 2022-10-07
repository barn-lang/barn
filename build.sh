function compile {
	cd ./compiler/
	go build
	cp ./main ../barn
	rm ./main
	echo -e "[  \e[32mOK\e[0m  ] Your compiled program is located in ./barn"
	cd ../
}

function recognize_distro_and_how_to_install {
	distro=$(uname -n)
	if [ $distro == "debian" ]; then
		echo -e "sudo apt-get install golang"
	elif [ $distro == "ubuntu" ]; then
		echo -e "sudo apt-get install golang"
	elif [ $distro == "arch" ]; then
		echo -e "sudo pacman -S go"
	else
		exit
	fi
}

# function compile_std_c {
# 	echo -e "[ \e[35mWAIT\e[0m ] Compiling ./lib/std-c/barn-std.c"
# 	echo -e "[ \e[35mWAIT\e[0m ] gcc -c ./lib/std-c/barn-std.c -o ./lib/std-c/barn-std.o"
# 	gcc -c ./lib/std-c/barn-std.c -o ./lib/std-c/barn-std.o
# 	echo -e "[  \e[32mOK\e[0m  ] Std lib has been compiled"
# }

echo -e "[ \e[35mWAIT\e[0m ] Looking for Go Compiler"
if [ -d ~/go ]; then
	echo -e "[  \e[32mOK\e[0m  ] Go Compiler have been found on your machine"
	echo -e "[ \e[35mWAIT\e[0m ] Start compiling Barn compiler"
	compile
	# compile_std_c
else
	echo -e "[ \e[31mERR\e[0m  ] Go Compiler have not been found on your machine, install it and then run this command"
	recognize_distro_and_how_to_install
	exit
fi
