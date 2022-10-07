package main

import "fmt"

// List of terminal colors
const (
	Black 		= 30		
    Red 		= 31	
    Green 		= 32		
    Yellow 		= 33		
    Blue 		= 34		
    Magenta 	= 35		
    Cyan 		= 36		
    White 		= 37	
    Gray 		= 90	
    BrightGray 	= 97
    Reset	  	= 0	
)

// This function is returning \e[(color)m
func get_color(color int) string {
	return fmt.Sprintf("\x1b[%dm", color);
}