package main

import (
	"io/ioutil"
)

func get_file_value(filename string) string {
	content, _ := ioutil.ReadFile(filename)
	return string(content)
}
