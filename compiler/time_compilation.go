package main

import (
	"fmt"
	"time"
)

func register_time_compilation(args ArgsParser) (time.Time, bool) {
	if args.is_flag("-time") || args.is_flag("--time") || args.is_flag("-t") || args.is_flag("--t") {
		return time.Now(), true
	} else {
		return time.Now(), false
	}
}

func calculate_time_compiliation(before_time time.Time, is_time_compilation bool) {
	if is_time_compilation {
		time_now := time.Now()
		time_past := time.Unix(0, time_now.UnixNano()-before_time.UnixNano())
		nano_seconds_past := float64(time_past.Nanosecond()) / float64(1000000)
		fmt.Printf("Compilation time taken: %.3f miliseconds!\n", nano_seconds_past)
		if nano_seconds_past < 1.0 {
			fmt.Printf("Not even one milisecond!\n")
		}
	}
}
