package main

import (
	"syscall"
	"bytes"
	"os/exec"
)

func run_command(name string, args ...string) (stdout string, stderr string, exit_code int) {
    var outbuf, errbuf bytes.Buffer
    cmd := exec.Command(name, args...)
    cmd.Stdout = &outbuf
    cmd.Stderr = &errbuf

    err := cmd.Run()
    stdout = outbuf.String()
    stderr = errbuf.String()

    if err != nil {
        if exitError, ok := err.(*exec.ExitError); ok {
            ws := exitError.Sys().(syscall.WaitStatus)
            exit_code = ws.ExitStatus()
        } else {
            exit_code = 1
            if stderr == "" {
                stderr = err.Error()
            }
        }
    } else {
        ws := cmd.ProcessState.Sys().(syscall.WaitStatus)
        exit_code = ws.ExitStatus()
    }
    return
}