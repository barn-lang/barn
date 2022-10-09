package main

import (
	"io/ioutil"
	"os"
	"runtime"
)

func get_file_value(filename string) string {
	content, _ := ioutil.ReadFile(filename)
	return string(content)
}

func get_barn_libs_directory() string {
	if runtime.GOOS == "linux" || runtime.GOOS == "darwin" {
		return os.Getenv("HOME") + "/.barn/libs/"
	} else {
		return "/.barn/libs/"
	}
}
